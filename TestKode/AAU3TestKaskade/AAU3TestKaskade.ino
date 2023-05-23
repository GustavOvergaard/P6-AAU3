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
void Jump( void *pvParameters );
void printTask( void *pvParameters );
void IMUTest( void *pvParameters );
void LQRTest( void *pvParameters );

//Sampling
#define Samplingstid 10
float avg_theta_rf = 0;
float theta_meas = 0;
int avgOver = 1000;
int recovery = 0;

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
float lastError_K = -225.9;
float secondLastError_K = 111;
float lastCurrent_K = 1.915;
float secondLastCurrent_K = - 0.9139;
float secondLastCurrent = 0, lastCurrent = 0, secondLastError = 0, lastError = 0;

//Alle controllere
float maxCurrent = 7.5;
float theta_rf = 0, omega_rf = 0, omega_sf = 0;
float current = 0, duty = 0;
float ang_filt_old = 0;
float lastFilt = 0;

//state
int state = 0;
int kont = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  analogReadResolution(12);
  pinMode(A5, INPUT);         // Hjul hastighed
  pinMode(1, OUTPUT);         // Driver enable
  digitalWrite(1, LOW);       // Sikrer at driveren er slukket

  // Kontakt
  pinMode(3, INPUT);
  pinMode(4, INPUT);

  // Opsætning af IMU
  Wire.begin();
  IMU_Setup(MPU_addr1);
  IMU_Setup(MPU_addr2);
  //GyroOffset1 = offsetGyro(100, MPU_addr1); //find offset - kræver at rammen står stille
  //GyroOffset2 = offsetGyro(100, MPU_addr2); //find offset - kræver at rammen står stille
  //AccOffset=offsetAcc(100, MPU_addr2);

  //potentiometer
  pinMode(A1, INPUT);         // Potentiometer
  //potOffset = potmeterAvg(100);


  //opsætning af Tasks
  xTaskCreate(Regulatorer, "Regulatorer", 1000, NULL, 2, NULL);
  xTaskCreate(Sampling, "Sampling", 1000, NULL, 3, NULL);
  //xTaskCreate(Jump, "Jump", 1000, NULL, 3, NULL);
  //xTaskCreate(printTask, "printTask", 1000, NULL, 3, NULL);
  //xTaskCreate(IMUTest, "Test", 1000, NULL, 3, NULL);
  //xTaskCreate(LQRTest, "LQRTest", 1000, NULL, , NULL);

  //Opsætning af Servo
  brake.attach(breakPin);             // Initialiserer servo
  brake.write(go);                    // Skriver vinklen for "go"

  // Opsætning af PWM til driveren
  FPGA.begin();
  FPGA.analogWriteResolution(10, 4700);
  FPGA.pinMode(45, 3);
  FPGA.analogWrite(45, map(50, 0, 100, pow(2, 10), 0));
  digitalWrite(1, HIGH);              // Tænder for driveren


  //offsetMellemIMUOgPotmeter(10000);
  Serial.println("Setup Done");

  vTaskStartScheduler();// starter skeduleringen
}

void loop() {
}

void Regulatorer(void *pvParameters ) {

  TickType_t xLastWakeTime;
  const TickType_t xPeriod = Samplingstid; //goes every 10ms
  int temp = 0;
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    kont = kontakt();
    if (kont == 3) {
      ECMotor(50);
      avg_theta_rf = 0;
    }
    else if (state == 0 ) {
      Kaskade();
    }
    else if (state == 1) {
      if (temp == 0) {
        if (abs(omega_sf) < 1) {     //tjek om svinghjulet står stille
          temp = 1;
        }
        else {                                      //ellers bring svinghjulet til stilstand
          if (omega_sf > 0) {
            ECMotor(42);
          }
          else {
            ECMotor(58);
          }
        }
      }
      if (temp == 1) {                            //efter hjulet har stået stille sætter vi fart på i den ene eller anden retning
        if (theta_rf > 0) {
          ECMotor(38);
          if (abs(omega_sf) > 180) {  //Vent på at vi når den ønskede hastighed
            ECMotor(45);
            brake.write(brk);
            vTaskDelay(300);
            brake.write(go);
            temp = 0;
            avg_theta_rf = 0;
            recovery = 1;
          }
        }
        else {
          ECMotor(62);
          if (abs(omega_sf) > 175) {  //Vent på at vi når den ønskede hastighed
            ECMotor(45);
            brake.write(brk);
            vTaskDelay(300);
            brake.write(go);
            temp = 0;
            avg_theta_rf = 0;
            recovery = 1;
          }
        }

      }
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
    if (abs(theta_rf) <= 0.35) {
      state = 0;
    }
    else {
      state = 1;
    }
    if (state == 0 and recovery == 0) {
      theta_meas = complementaryFilter2();
      //Rammens vinkelhastighed bliver opdateret i complementaryFilter()!
      Serial.print(theta_meas, 5); Serial.print(",");; Serial.print(current, 5); Serial.print(","); Serial.println(kont);

      omega_sf = hastighedHjul() * RPM2Rad;

      avg_theta_rf = avg_theta_rf + theta_meas / avgOver - avg_theta_rf / avgOver;
      theta_rf = theta_meas - avg_theta_rf;
    }
    else if (state == 1 or recovery != 0) {
      theta_rf  = potmeter() - potOffset;
      omega_sf = hastighedHjul() * RPM2Rad;
      gyr = GYRO_Read(MPU_addr1) - GyroOffset1;
      gyr = gyr + GYRO_Read(MPU_addr2) - GyroOffset2;
      omega_rf = gyr / 2;
      avg_theta_rf = 0;
      Serial.print(theta_rf, 5); Serial.print(",");; Serial.print(current, 5); Serial.print(","); Serial.println(kont);

      if (recovery != 0) {
        recovery++;

        if (recovery > 1000 / Samplingstid) {
          recovery = 0;
          lastang = theta_rf;
        }
      }

    }
    else {
      //Serial.println("state fejl");
    }
  }
}

void printTask(void *pvParameters ) {

  TickType_t xLastWakeTime;
  const TickType_t xPeriod = 30; //samples 10ms
  xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
    if (state == 0 and recovery == 0) {
      Serial.print(theta_meas, 5); Serial.print(",");; Serial.print(current, 5); Serial.print(","); Serial.println(kont);
    }
    else if (state == 1 or recovery != 0) {
      Serial.print(theta_rf, 5); Serial.print(",");; Serial.print(current, 5); Serial.print(","); Serial.println(kont);
    }
  }
}
