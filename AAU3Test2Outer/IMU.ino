float GYRO_Read(int address) {
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

float ACC_Read(int address) {
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

float offsetGyro(int n, int address) { //er klasse
  Serial.println("Calculating offset");
  float avg = 0;

  for (int b = 0; b < n; b++) {
    float temp = 0;
    for (int i = 0; i < 100; i++) {
      temp = temp + GYRO_Read(address);
    }
    avg = avg + (temp / 100);
    temp = 0;
    Serial.print(100 / n * b); Serial.println("%");
  }

  avg = avg / n;
  Serial.print("Offset gyroscope adress: "); Serial.print(address); Serial.print(" calculated to: "); Serial.println(avg, 16);
  return avg;
}

float offsetMellemIMUOgPotmeter(int n) { //er klasse
  Serial.println("Calculating offset");
  float avg = 0;

  for (int i = 0; i < n; i++) {
    avg = avg + (complementaryFilter2() - potmeter() - potOffset);
    delay(10);
  }
  avg = avg/n;
  Serial.print("Offset mellem potmeter og imu måling"); Serial.print(" calculated to: "); Serial.println(avg, 16);
  return avg;
}

float offsetAcc(int n, int address) { //er klasse
  Serial.println("Calculating offset");
  float avg = 0;

  for (int b = 0; b < n; b++) {
    float temp = 0;
    for (int i = 0; i < 100; i++) {
      temp = temp + ACC_Read(address);
    }
    avg = avg + (temp / 100);
    temp = 0;
    Serial.print(100 / n * b); Serial.println("%");
  }

  avg = avg / n;
  Serial.print("Offset accellerometer adress: "); Serial.print(address); Serial.print(" calculated to: "); Serial.println(avg, 16);
  return avg;
}


float complementaryFilter() {
  float tau = 0.5;
  float samplingperiod_s = Samplingstid / 1000.0;
  float c1 = samplingperiod_s / (samplingperiod_s + 2 * tau);
  float speedFrame = 0;
  omega_rf = GYRO_Read(MPU_addr2);
  float acc_data = ACC_Read(MPU_addr2);

  float ang_filt = c1 * ( tau * omega_rf + acc_data) + ang_filt_old;
  ang_filt_old = c1 * ( tau * omega_rf + acc_data) - (samplingperiod_s - 2 * tau) / (samplingperiod_s + 2 * tau) * ang_filt;

  return ang_filt;
}


float complementaryFilter2() {
  float gyr = 0;
  float acc = 0;

  gyr = GYRO_Read(MPU_addr1); // - GyroOffset1
  gyr = gyr + GYRO_Read(MPU_addr2);//- GyroOffset2
  omega_rf = gyr / 2;

  acc = ACC_Read(MPU_addr1) - AccOffset1;
  acc = acc + ACC_Read(MPU_addr2) - AccOffset2;
  acc = acc / 2;

  float ang = 0;
  float samplingperiod_s = Samplingstid / 1000.0;

  ang = 0.98 * (lastang + (omega_rf * samplingperiod_s)) + 0.02 * acc;
  lastang = ang;

  return ang;
}

float complementaryFilter3() {
  float acc = 0;

  omega_rf = GYRO_Read(MPU_addr2);

  acc = ACC_Read(MPU_addr2);

  float ang = 0;
  float samplingperiod_s = Samplingstid / 1000.0;

  ang = 0.98 * (lastComp3 + (omega_rf * samplingperiod_s)) + 0.02 * acc;
  lastComp3 = ang;
  return ang;
}
