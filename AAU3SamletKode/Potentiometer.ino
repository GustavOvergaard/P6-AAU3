float potmeter() {                      //returnerer omega_r vha potmeter
  float volt = (float)(analogRead(A1)) * 5 / 4096;
  float ang =  18.11594203 * volt - 46.01449275;
  return ang * deg2Rad;
}



float potmeterAvg(int tusinder) {       //find offset fra den vinkel rammen står i i øjeblikket
  float avg, temp = 0;

  for (int i = 0; i < tusinder; i++) {
    for (int m = 0; m < 1000; m++) {
      temp = temp + potmeter();
    }
    temp = temp / 1000;
    avg = avg + temp;
    Serial.print(100 / tusinder * i); Serial.println("%");
    temp = 0;
  }
  avg = avg / tusinder;
  Serial.print("Potentiometer offset calculated to: "); Serial.println(avg, 8);
  return avg;
}
