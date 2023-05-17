#include <FreeRTOS_SAMD21.h>
#include <VidorFPGA.h>
#include <math.h>
#include <Servo.h>
#include <Wire.h>

//  IMU
#define gyroScale 65.5      //Scale for gyroscope (From datasheet)
#define scaleacc 16384.0    //Scale for accelerometer (From datasheet)
#define MPU_addr1 0x69      //I2C address for 1st IMU. (CODE WILL BE STUCK IF NOT CORRECT!)
#define MPU_addr2 0x68      //I2C address for 2nd IMU  (AGAIN CODE WILL BE STUCK IF NOT CORRECT!)
#define RPM2Rad 2 * M_PI / 60
#define deg2Rad 1/360*2 * M_PI
float GyroOffset1 = -0.0031161464285105;   //offset på vinkelhastigheden
float GyroOffset2 = -0.0258060935884714;   //offset på vinkelhastigheden
float AccOffset1 = 0;
float AccOffset2 = 0;
float lastComp3 = 0;
float gyr = 0;
float lastang = 0;

//Servo
Servo brake;
#define brk 127  //Servo position when braking0
#define go 100   //Servo position when running
#define breakPin 2 //Servo pin

//Potentiometer
float potOffset = -0.01020324;
float potImuOffset = 0.0672322958707809;

//Tasks
void Regulatorer( void *pvParameters );
void Sampling( void *pvParameters );
void State( void *pvParameters );

//Sampling
#define Samplingstid 10
float gns_theta_r = 0;
float theta_maalt = 0;
int antalGnsSamples = 1000;
int genopretning = 0;
int klarTilHop = 0;

//tilbagekolbingsparametre
//float omega_s, omega_r, theta_r = 0;

//State Space Controller
float SSK1 = -74.9033;
float SSK2 = -7.6768;
float SSK3 = -0.0596;

//Kaskade Controller
float KK3 = -70;
float KK2 = -10;
float KK1 = -0.1;

//Outer Controller
float theta_r_K = 114.9;
float senesteFejl_K = -225.9;
float naestSenesteFejl_K = 111;
float senesteStroem_K = 1.915;
float naestSenesteStroem_K = - 0.9139;
float naestSenesteStroem = 0, senesteStroem = 0, naestSenesteFejl = 0, senesteFejl = 0;
int outerFlag = 0;

//Alle controllere
float maksStroem = 7.5;
float theta_r = 0, omega_r = 0, omega_s = 0;
float stroem = 0, duty = 0;

//state
int state = 0;
int kont = 0;
int startMicros = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //MotorDriver
  analogReadResolution(12);
  pinMode(A5, INPUT);         // Hjul hastighed
  pinMode(1, OUTPUT);         // Driver enable
  digitalWrite(1, LOW);       // Sikrer at driveren er slukket

  // Kontakt
  pinMode(3, INPUT); //kontaktpin1
  pinMode(4, INPUT); //kontaktpin2

  // Opsætning af IMU
  Wire.begin();
  IMU_Setup(MPU_addr1);
  IMU_Setup(MPU_addr2);

  //potentiometer
  pinMode(A1, INPUT); //potmeterpin


  //opsætning af Tasks
  xTaskCreate(Regulatorer, "Regulatorer", 1000, NULL, 3, NULL);
  xTaskCreate(Sampling, "Sampling", 1000, NULL, 2, NULL);
  xTaskCreate(State, "State", 1000, NULL, 2, NULL);
  //prioritet - højt talt = høj prioritet

  //Opsætning af Servo
  brake.attach(breakPin);             // Initialiserer servo
  brake.write(go);                    // Skriver vinklen for "go"

  // Opsætning af PWM til driveren
  FPGA.begin();
  FPGA.analogWriteResolution(10, 4700); //skal være mellem 10-5kHz (datasheet)
  FPGA.pinMode(45, 3); //fpga pin
  FPGA.analogWrite(45, map(50, 0, 100, pow(2, 10), 0));//sæt pwm signalet til 50%duty = 0 amp til motor
  digitalWrite(1, HIGH);              // Tænder for driveren

  Serial.println("Setup Done");
  vTaskStartScheduler();// starter skeduleringen
}

void loop() {
}

void Regulatorer(void *pvParameters ) {

  TickType_t xLastWakeTime;
  const TickType_t xPeriod = Samplingstid; //samples 10ms
  xLastWakeTime = xTaskGetTickCount();

  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    kont = kontakt();
    if (state == 0 or state == 3) {
      //Regulatorer
      if (kont == 1) {
        Kaskade();
      }
      else if (kont == 2) {
        StateSpace();
      }
      else if (kont == 3) {
        Outer();
      }
    }
    else if (state == 1) {
      faldet();
    }
    else if (state == 2) {
      hop();
    }
    else {
      Serial.println("statefejl!");
    }
  }
}

void Sampling(void *pvParameters ) {

  TickType_t xLastWakeTime;
  const TickType_t xPeriod = Samplingstid; //samples 10ms
  xLastWakeTime = xTaskGetTickCount();

  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);

    if (state == 0 and outerFlag == 0) {
      imuSampling();
    }
    else if (state == 0 and outerFlag != 0) {
      potSampling();
    }
    else if (state == 3) {
      gyroVinkelSamp();
    }
    else {
      potSamplingUdenVinkKori();
    }
  }
}

void State(void *pvParameters ) {

  TickType_t xLastWakeTime;
  const TickType_t xPeriod = Samplingstid; //samples 10ms
  xLastWakeTime = xTaskGetTickCount();

  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);

    if (abs(theta_r) < 0.261 or genopretning != 0) {
      if (genopretning == 0) {
        state = 0;
      }
      else {
        state = 3;
        if (genopretning >= 500 / Samplingstid) {
          genopretning = 0;
        }
        else {
          genopretning++;
        }
      }
    }
    else {
      if (klarTilHop == 0) {
        state = 1;
      }
      else {
        state = 2;
      }
    }
  }
}
