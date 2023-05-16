void faldet() {
  if (abs(omega_s) > 1) {     //tjek om svinghjulet stadig drejer rundt
    if (omega_s > 0) {
      ECMotor(42);            //modsatrettet kraft
    }
    else {
      ECMotor(58);            //modsatrettet kraft
    }
  } else {                    //hvis svinghjulet står stille er vi klar til hop
    klarTilHop = 1;
  }
}

void hop() {
  if (theta_r > 0) {          //bring svinghjulet op i hastighed
    ECMotor(38);
  }
  else {
    ECMotor(62);
  }
  if (abs(omega_s) > 180) {   //Vent på at vi når den ønskede hastighed og klods bremsen
    ECMotor(45);
    brake.write(brk);
    vTaskDelay(300);
    brake.write(go);
    gns_theta_r = 0;
    klarTilHop = 0;
    gns_theta_r = 0;
    genopretning = 1;
  }
}

void imuSampling() {              //omega_r findes vha IMUer
  theta_maalt = komplementaerFilter(0.98);
  //Rammens vinkelhastighed bliver opdateret i complementaryFilter()!
  omega_s = hastighedHjul() * RPM2Rad;
  gns_theta_r = gns_theta_r + theta_maalt / antalGnsSamples - gns_theta_r / antalGnsSamples;
  theta_r = theta_maalt - gns_theta_r;
}

void potSampling() {              //omega_r findes vha potmeter
  theta_maalt  = potmeter() - potOffset;
  omega_s = hastighedHjul() * RPM2Rad;
  omega_r = (GYRO_Read(MPU_addr1) - GyroOffset1 + GYRO_Read(MPU_addr2) - GyroOffset2) / 2;
  gns_theta_r = gns_theta_r + theta_maalt / antalGnsSamples - gns_theta_r / antalGnsSamples;
  theta_r = theta_maalt - gns_theta_r;
  lastang = theta_r;
}

void potSamplingUdenMovAvg() {    //omega_r findes vha potmeter - referencevinklen ændres ikke, da vi er langt fra vinkel 0
  theta_r  = potmeter() - potOffset;
  omega_s = hastighedHjul() * RPM2Rad;
  omega_r = (GYRO_Read(MPU_addr1) - GyroOffset1 + GYRO_Read(MPU_addr2) - GyroOffset2) / 2;
  lastang = theta_r;
  gns_theta_r = 0;
}

float gyroVinkelSamp(){
  omega_r = (GYRO_Read(MPU_addr1) - GyroOffset1 + GYRO_Read(MPU_addr2) - GyroOffset2) / 2;
  lastang=theta_r;
  theta_r=lastang+omega_r*Samplingstid/1000;
  omega_s = hastighedHjul() * RPM2Rad;
}
