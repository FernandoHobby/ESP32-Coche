// ----------
void motores(int rpmDI, int rpmDD, int rpmTI, int rpmTD) {

  Serial.printf("rpmDI= %4d rpmDD= %4d rpmTI= %4d rpmTD= %4d\n", rpmDI, rpmDD, rpmTI, rpmTD);
  
  int arpmDI = abs(rpmDI);
  int arpmDD = abs(rpmDD);
  int arpmTI = abs(rpmTI);
  int arpmTD = abs(rpmTD);

  // variables con el ancho del pulso PWM (duty cycle). para 8 bits de 0 a 255 
  int pwmDI, pwmDD, pwmTI, pwmTD;

  pwmDI = (arpmDI == 0)?0:arpmDI;
  pwmDD = (arpmDD == 0)?0:arpmDD;
  pwmTI = (arpmTI == 0)?0:arpmTI;
  pwmTD = (arpmTD == 0)?0:arpmTD;

  Serial.printf("pwmDI= %4d pwmDD= %4d pwmTI= %4d pwmTD= %4d \n",pwmDI,pwmDD,pwmTI,pwmTD);

  if (rpmDI > 0) {
    digitalWrite(DI_IN1, HIGH);
    digitalWrite(DI_IN2, LOW);
    ledcWrite(0, pwmDI);
  } else if (rpmDI < 0) {
    digitalWrite(DI_IN1, LOW);
    digitalWrite(DI_IN2, HIGH);
    ledcWrite(0, pwmDI);
  } else {
    digitalWrite(DI_IN1, LOW);
    digitalWrite(DI_IN2, LOW);
    ledcWrite(0, 0); 
  }

  if (rpmDD > 0) {
    digitalWrite(DD_IN3, HIGH);
    digitalWrite(DD_IN4, LOW);
    ledcWrite(1, pwmDD);
  } else if (rpmDD < 0) {
    digitalWrite(DD_IN3, LOW);
    digitalWrite(DD_IN4, HIGH);
    ledcWrite(1, pwmDD);
  } else {
    digitalWrite(DD_IN3, LOW);
    digitalWrite(DD_IN4, LOW);
    ledcWrite(1, 0); 
  }

  if (rpmTI > 0) {
    digitalWrite(TI_IN1, HIGH);
    digitalWrite(TI_IN2, LOW);
    ledcWrite(2, pwmTI);
  } else if (rpmTI < 0) {
    digitalWrite(TI_IN1, LOW);
    digitalWrite(TI_IN2, HIGH);
    ledcWrite(2, pwmTI);
  } else {
    digitalWrite(TI_IN1, LOW);
    digitalWrite(TI_IN2, LOW);
    ledcWrite(2, 0); 
  }

  if (rpmTD > 0) {
    digitalWrite(TD_IN3, HIGH);
    digitalWrite(TD_IN4, LOW);
    ledcWrite(3, pwmTD);
  } else if (rpmTD < 0) {
    digitalWrite(TD_IN3, LOW);
    digitalWrite(TD_IN4, HIGH);
    ledcWrite(3, pwmTD);
  } else {
    digitalWrite(TD_IN3, LOW);
    digitalWrite(TD_IN4, LOW);
    ledcWrite(3, 0); 
  }

}
// ----------