#include <math.h>
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

float potOffset = -0.01020324;

float theta_r = 0, omega_r = 0, omega_s = 0;

int lastMil=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  IMU_Setup(MPU_addr1);
  IMU_Setup(MPU_addr2);
}

float GYRO_Read(int address) {//læs vinkelhastighed fra gyro
  int16_t GyZ;
  float GyrZ;

  //Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(0x47);                       //Specify address for gyroscope (0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L))
  Wire.endTransmission();
  Wire.requestFrom(address, 2);           // request a total of 2 registers
  if (Wire.available())  GyZ = Wire.read() << 8 | Wire.read();

  GyrZ = (GyZ / gyroScale) * M_PI / 180 ;
  //GyrZ=GyrZ-GyroOffset;
  return GyrZ;
}

void IMU_Setup(int address) { //setup IMUer
  Wire.beginTransmission(address);
  Wire.write(0x6B); Wire.write(0);
  Wire.endTransmission();
  //Gyro config
  Wire.beginTransmission(address);        // Contact IMU for setup
  Wire.write(0x1B);                       // GYRO_CONFIG register
  Wire.write(0b00001000);                 // Register bits set to b'00010000 (500dps full scale)
  Wire.endTransmission();                 // End transmission for gyro
  //Acc config
  Wire.beginTransmission(address);        // Contact IMU for setup
  Wire.write(0x1C);                       // ACCEL_CONFIG register
  Wire.write(0b00000000);                 // Setting the accel to +/- 2g
  Wire.endTransmission();
}

float ACC_Read(int address) { //find vinklen ud fra acceleromteret
  int16_t AcX, AcY;
  float AccX, AccY;
  float Accang;

  //Wire.begin();
  Wire.beginTransmission(address);  //I2C address of the MPU
  Wire.write(0x3B);                 //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(address, 4);     //Request 4 Accel Registers

  if (Wire.available()) {
    AcX = Wire.read() << 8 | Wire.read();
    AcY = Wire.read() << 8 | Wire.read();
  }
  //if (!Wire.available()) Wire.end();

  AccX = AcX / scaleacc;
  AccY = AcY / scaleacc;

  if (address == MPU_addr1) {
    Accang = (atan2(AccX, AccY) - M_PI / 4);
  }
  else if (address == MPU_addr2) {
    Accang = (atan2(AccY, - AccX) - M_PI / 4);
  }
  return Accang;
}

float potmeter() {                      //returnerer omega_r vha potmeter
  float volt = (float)(analogRead(A1)) * 5 / 4096;
  float ang =  18.11594203 * volt - 46.01449275;
  return ang * deg2Rad;
}

float komplementaerFilter() { //find vinklen ud fra sensorfusion mellem gyro og accelerometer
  float acc = 0;

  omega_r = (GYRO_Read(MPU_addr1) - GyroOffset1 + GYRO_Read(MPU_addr2) - GyroOffset2) / 2;

  acc = ACC_Read(MPU_addr1) - AccOffset1;
  acc = acc + ACC_Read(MPU_addr2) - AccOffset2;
  acc = acc / 2;

  float ang = 0;

  ang = 0.98 * (lastang + (omega_r * 10/1000)) + 0.02 * acc;
  lastang = ang;

  return ang;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - lastMil >= 10) {
    Serial.print(komplementaerFilter()); Serial.print(","); Serial.println(potmeter() - potOffset);
    Serial.println(millis());
    lastMil=millis();
  }
}
