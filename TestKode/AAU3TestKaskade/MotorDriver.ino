float hastighedHjul() {
  return ((analogRead(A5) - 2048.0) * (12000.0 / 4096.0)); //RPM
}

void ECMotor(int duty) {
  FPGA.analogWrite(45, map(duty, 0, 100, pow(2, 10), 0));
}

float interpolate(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
