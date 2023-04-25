#include <math.h>
#include <Servo.h>
#include <Wire.h>
#define RPM2Rad 2 * M_PI / 60
#include <VidorFPGA.h>

float hastighedHjul() {
  return ((analogRead(A5) - 2048.0) * (12000.0 / 4096.0)); //RPM
}

void ECMotor(int duty){
  FPGA.analogWrite(45, map(duty, 0, 100, pow(2, 10), 0));
}

void setup() {
  Serial.begin(115200);
   analogReadResolution(12);
  pinMode(A5, INPUT);         // Hjul hastighed
  pinMode(1, OUTPUT);         // Driver enable
  digitalWrite(1, LOW);       // Sikrer at driveren er slukket

  // Opsætning af PWM til driveren
  FPGA.begin();
  FPGA.analogWriteResolution(10, 4700);
  FPGA.pinMode(45, 3);
  FPGA.analogWrite(45, map(50, 0, 100, pow(2, 10), 0));
  digitalWrite(1, HIGH);      // Tænder for driveren

  Serial.println("Setup Done");


}



void loop() {
ECMotor(50);
Serial.println(hastighedHjul()*RPM2Rad);
delay(10);
}
