// ----------
void motores(int DI_pwm, int DD_pwm, int TI_pwm, int TD_pwm) {


  bool homogeneizar = true;

  if (homogeneizar) {

    int indice;

    // Valores posibles que recibe el coche del control movil/PC = [75,90,105,120,135,150,165,180,195,210,225,240,255];   
    // Ancho pwm que igualan las rpm de cada motor
    int DI_pwm_h[13] = { 95, 102, 110, 118, 128, 138, 148, 161, 175, 195, 219, 242, 247 };
    int DD_pwm_h[13] = { 76, 83, 91, 98, 106, 114, 123, 134, 147, 163, 183, 212, 233 };
    int TI_pwm_h[13] = { 97, 104, 112, 120, 131, 142, 154, 170, 189, 210, 240, 250, 255 };
    int TD_pwm_h[13] = { 82, 89, 96, 103, 111, 120, 130, 141, 155, 173, 194, 222, 240 };

    DI_pwm = ((DI_pwm == 0) ? 0 : (DI_pwm > 0) ? 1 : -1 ) * DI_pwm_h[ (abs(DI_pwm) - 75) / 15 ];
    DD_pwm = ((DD_pwm == 0) ? 0 : (DD_pwm > 0) ? 1 : -1 ) * DD_pwm_h[ (abs(DD_pwm) - 75) / 15 ];
    TI_pwm = ((TI_pwm == 0) ? 0 : (TI_pwm > 0) ? 1 : -1 ) * TI_pwm_h[ (abs(TI_pwm) - 75) / 15 ];
    TD_pwm = ((TD_pwm == 0) ? 0 : (TD_pwm > 0) ? 1 : -1 ) * TD_pwm_h[ (abs(TD_pwm) - 75) / 15 ];

  }

  Serial.printf("DI_pwm= %4d DD_pwm= %4d TI_pwm= %4d TD_pwm= %4d\n", DI_pwm, DD_pwm, TI_pwm, TD_pwm);

  if (DI_pwm > 0) {
    digitalWrite(DI_IN1, HIGH);
    digitalWrite(DI_IN2, LOW);
    ledcWrite(DI_canal, DI_pwm);
  } else if (DI_pwm < 0) {
    digitalWrite(DI_IN1, LOW);
    digitalWrite(DI_IN2, HIGH);
    ledcWrite(DI_canal, abs(DI_pwm));
  } else {
    digitalWrite(DI_IN1, LOW);
    digitalWrite(DI_IN2, LOW);
    ledcWrite(DI_canal, 0);
  }

  if (DD_pwm > 0) {
    digitalWrite(DD_IN3, HIGH);
    digitalWrite(DD_IN4, LOW);
    ledcWrite(DD_canal, DD_pwm);
  } else if (DD_pwm < 0) {
    digitalWrite(DD_IN3, LOW);
    digitalWrite(DD_IN4, HIGH);
    ledcWrite(DD_canal, abs(DD_pwm));
  } else {
    digitalWrite(DD_IN3, LOW);
    digitalWrite(DD_IN4, LOW);
    ledcWrite(DD_canal, 0);
  }

  if (TI_pwm > 0) {
    digitalWrite(TI_IN1, HIGH);
    digitalWrite(TI_IN2, LOW);
    ledcWrite(TI_canal, TI_pwm);
  } else if (TI_pwm < 0) {
    digitalWrite(TI_IN1, LOW);
    digitalWrite(TI_IN2, HIGH);
    ledcWrite(TI_canal, abs(TI_pwm));
  } else {
    digitalWrite(TI_IN1, LOW);
    digitalWrite(TI_IN2, LOW);
    ledcWrite(TI_canal, 0);
  }

  if (TD_pwm > 0) {
    digitalWrite(TD_IN3, HIGH);
    digitalWrite(TD_IN4, LOW);
    ledcWrite(TD_canal, TD_pwm);
  } else if (TD_pwm < 0) {
    digitalWrite(TD_IN3, LOW);
    digitalWrite(TD_IN4, HIGH);
    ledcWrite(TD_canal, abs(TD_pwm));
  } else {
    digitalWrite(TD_IN3, LOW);
    digitalWrite(TD_IN4, LOW);
    ledcWrite(TD_canal, 0);
  }
}
// ----------