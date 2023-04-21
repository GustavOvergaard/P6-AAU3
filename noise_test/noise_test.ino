
#include <math.h>
#include <Servo.h>
#include <Wire.h>

//  IMU
#define gyroScale 65.5        //Scale for gyroscope (From datasheet)
#define scaleacc 16384.0  //Scale for accelerometer (From datasheet)

#define MPU_addr 0x68     //I2C address for 2nd IMU  (AGAIN CODE WILL BE STUCK IF NOT CORRECT!)
#define MPU_addr2 0x77

#define RPM2Rad 2 * M_PI / 60
#define deg2Rad 1/360*2 * M_PI
float GyroAng = 0;
float lastAngTime = 0;
float GyroOffset = -0.03217750;

float GYRO_Read(int address) {
  int16_t GyZ;
  float GyrZ;

  //Wire.begin();
  Wire.beginTransmission(address);
  Wire.write(0x47);                       //Specify address for gyroscope (0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L))
  Wire.endTransmission();
  Wire.requestFrom(address, 2);           // request a total of 2 registers
  if (Wire.available())  GyZ = Wire.read() << 8 | Wire.read();

  GyrZ = (GyZ / gyroScale)* M_PI / 180 ;
  //GyrZ=GyrZ-GyroOffset;
  return GyrZ;
}

void IMU_Setup(int address) {
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

void setup() {
  Serial.begin(115200);
  // Opsætning af IMU
  Serial.print("test");
  Wire.begin();
  IMU_Setup(MPU_addr);
  IMU_Setup(MPU_addr2);
  //GyroOffset = offsetGyro(100, MPU_addr2); //find offset - kræver at rammen står stille
  Serial.println("Setup Done");

}



void loop() {
Serial.println(GYRO_Read(MPU_addr) - GyroOffset,5);
Serial.println(GYRO_Read(MPU_addr2) - GyroOffset,5);
Serial.println();
delay(10);
}
