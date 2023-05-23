void Kaskade() {
  outerFlag = 0; //vi er ikke i ydre regulator
  current = omega_s * KK1 + omega_r * KK2 + theta_r * KK3;

  //saturation
  if (current >= maxCurrent) {
    current = maxCurrent - 0.01;
  }
  else if (current <= -maxCurrent) {
    current = -maxCurrent + 0.01;
  }
  duty = interpolate(current, -7.5, 7.5, 90, 10);//strøm til duty
  ECMotor(duty);
  secondLastCurrent = 0;
  lastCurrent = 0;
  secondLastError = 0;
  lastError = 0;
}

void StateSpace() {
  outerFlag = 0; //vi er ikke i ydre regulator
  current = SSK1 * theta_r + SSK2 * omega_r + SSK3 * omega_s;//Controller

  //saturation
  if (current >= maxCurrent) {
    current = maxCurrent - 0.01;
  }
  else if (current <= -maxCurrent) {
    current = -maxCurrent + 0.01;
  }
  duty = interpolate(current, -7.5, 7.5, 90, 10);//strøm til duty
  ECMotor(duty);
  secondLastCurrent = 0;
  lastCurrent = 0;
  secondLastError = 0;
  lastError = 0;
}

void Outer() {
  outerFlag = 1; //vi er i ydre regulator

  current = (theta_r * theta_r_K + senesteFejl_K * lastError + naestSenesteFejl_K * secondLastError + lastCurrent_K * lastCurrent + secondLastCurrent_K * secondLastCurrent); //controller

  //saturation
  if (current >= maxCurrent) {
    current = maxCurrent - 0.01;
  }
  else if (current <= -maxCurrent) {
    current = -maxCurrent + 0.01;
  }


  duty = interpolate(current, -7.5, 7.5, 90, 10); //strøm til duty
  ECMotor(100 - duty);

  //opdater parametre
  secondLastCurrent = lastCurrent;
  lastCurrent = current;
  secondLastError = lastError;
  lastError = theta_r;
}


/*
  void Outer() {
  current = (theta_r * theta_r_K + senesteFejl_K * lastError + naestSenesteFejl_K * secondLastError + lastCurrent_K * lastCurrent + secondLastCurrent_K * secondLastCurrent); //controller

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
  lastError = theta_maalt;
  }
*/
