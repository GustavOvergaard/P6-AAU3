int kontakt() {
  int temp11, temp22;
  temp11 = digitalRead(3);
  temp22 = digitalRead(4);
  if (temp11 == 1 and temp22 == 0) {
    return 1;
  }
  else if (temp11 == 1 and temp22 == 1) {
    return 2;
  }
  else if (temp11 == 0 and temp22 == 1) {
    return 3;
  }
  else {
    Serial.println("Fejl på kontakten");
    return 0;
  }
}
