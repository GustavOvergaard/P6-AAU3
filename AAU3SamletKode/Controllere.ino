void Kaskade() {
  outerFlag=0; //vi er ikke i ydre regulator
  stroem = omega_s * KK1 + omega_r * KK2 + theta_r * KK3;

  //saturation
  if (stroem >= maksStroem) {
    stroem = maksStroem - 0.01;
  }
  else if (stroem <= -maksStroem) {
    stroem = -maksStroem + 0.01;
  }
  duty = interpolate(stroem, -7.5, 7.5, 90, 10);//strøm til duty
  ECMotor(duty);
}

void StateSpace() {
  outerFlag=0;//vi er ikke i ydre regulator
  stroem = SSK1 * theta_r + SSK2 * omega_r + SSK3 * omega_s;//Controller

  //saturation
  if (stroem >= maksStroem) {
    stroem = maksStroem - 0.01;
  }
  else if (stroem <= -maksStroem) {
    stroem = -maksStroem + 0.01;
  }
  duty = interpolate(stroem, -7.5, 7.5, 90, 10);//strøm til duty
  ECMotor(duty);
}

void Outer() {
  outerFlag=1;//vi er i ydre regulator
  
  stroem = (theta_r * theta_r_K + senesteFejl_K * senesteFejl + naestSenesteFejl_K * naestSenesteFejl + senesteStroem_K * senesteStroem + naestSenesteStroem_K * naestSenesteStroem); //controller

  //saturation
  if (stroem >= maksStroem) {
    stroem = maksStroem - 0.01;
  }
  else if (stroem <= -maksStroem) {
    stroem = -maksStroem + 0.01;
  }

  
  duty = interpolate(stroem, -7.5, 7.5, 90, 10); //strøm til duty
  ECMotor(100-duty); 

  //opdater parametre
  naestSenesteStroem = senesteStroem;
  senesteStroem = stroem;
  naestSenesteFejl = senesteFejl;
  senesteFejl = theta_r;
}
