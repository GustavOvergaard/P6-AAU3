void Kaskade() {
  current = omega_sf * KK1 + omega_rf * KK2 + theta_rf * KK3;

  //saturation
  if (current >= maxCurrent) {
    current = maxCurrent - 0.01;
  }
  else if (current <= -maxCurrent) {
    current = -maxCurrent + 0.01;
  }

  duty = interpolate(current, -7.5, 7.5, 90, 10); //strøm til duty cycle
  ECMotor(duty);//duty cycle til pwm til motor
}

void StateSpace() {
  current = SSK1 * theta_rf + SSK2 * omega_rf + SSK3 * omega_sf;//Controller

  //saturation
  if (current >= maxCurrent) {
    current = maxCurrent - 0.01;
  }
  else if (current <= -maxCurrent) {
    current = -maxCurrent + 0.01;
  }

  duty = interpolate(current, -7.5, 7.5, 90, 10);//strøm til duty cycle
  ECMotor(duty);//duty cycle til pwm til motor
}

void Outer() {
  
  current = (theta_rf * theta_r_K + lastError_K * lastError + secondLastError_K * secondLastError + lastCurrent_K * lastCurrent + secondLastCurrent_K * secondLastCurrent); //controller

  //saturation
  if (current >= maxCurrent) {
    current = maxCurrent - 0.01;
  }
  else if (current <= -maxCurrent) {
    current = -maxCurrent + 0.01;
  }
  duty = interpolate(current, -7.5, 7.5, 90, 10); //strøm til duty cycle
  ECMotor(100 - duty); //duty cycle til pwm til motor

  //opdater parametre
  secondLastCurrent = lastCurrent;
  lastCurrent = current;
  secondLastError = lastError;
  lastError = theta_rf;
}


/*
  void Outer() {
  current = (theta_rf * theta_r_K + lastError_K * lastError + secondLastError_K * secondLastError + lastCurrent_K * lastCurrent + secondLastCurrent_K * secondLastCurrent); //controller

  //saturation
  if (current >= maxCurrent) {
    current = maxCurrent - 0.01;
  }
  else if (current <= -maxCurrent) {
    current = -maxCurrent + 0.01;
  }
  duty = interpolate(current, -7.5, 7.5, 90, 10); //strøm til duty cycle
  ECMotor(100-duty); //duty cycle til pwm til motor

  //opdater parametre
  secondLastCurrent = lastCurrent;
  lastCurrent = current;
  secondLastError = lastError;
  lastError = theta_meas;
  }
*/
