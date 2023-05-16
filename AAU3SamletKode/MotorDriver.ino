float hastighedHjul() {
  return ((analogRead(A5) - 2048.0) * (12000.0 / 4096.0)); //RPM
}

void ECMotor(int duty) {      //kør svinghjulet
  FPGA.analogWrite(45, map(duty, 0, 100, pow(2, 10), 0)); //skriver pwm signalet til fpga pinen, som kører i baggrunden - det skal aktivt ændres
}

float interpolate(float x, float in_min, float in_max, float out_min, float out_max) { //tager x's position ift in_min og in_max og returnerer hvad x ville være hvis in_min=out_min og in_max=out_max
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
