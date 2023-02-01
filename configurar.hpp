// ----------
// establecer los pines conectados al L298N en cada motor y los led de informacion

// rueda delantera izquierda
const uint8_t DI_ENA = 25;
const uint8_t DI_IN1 = 33;
const uint8_t DI_IN2 = 32;

// rueda delantera derecha
const uint8_t DD_IN3 = 19;
const uint8_t DD_IN4 = 18;
const uint8_t DD_ENB = 5;

// rueda trasrera izquierda
const uint8_t TI_ENA = 14;
const uint8_t TI_IN1 = 27;
const uint8_t TI_IN2 = 26;

// rueda trasera derecha
const uint8_t TD_IN3 = 17;
const uint8_t TD_IN4 = 16;
const uint8_t TD_ENB = 4;

// led rojo parada por bajo voltaje bateria
const uint8_t LVOL = 12;

// led verde conexion WS
const uint8_t LCON = 13;

// ----------
void iniciarPines() {

  // establecer los canales, frecuencia y resolucion de cada señal PWM 

  double fre = 1000;  // frecuencia señal PWM
  uint8_t res = 8;    // resolucion ancho de pulso (de 0 a 255 que equivale a un ancho del 100%)

  // ledcSetup(uint8_t channel, double freq, uint8_t resolution_bits)
  ledcSetup(0, fre, res);  // rueda delantera izquierda
  ledcSetup(1, fre, res);  // rueda delantera derecha
  ledcSetup(2, fre, res);  // rueda trasera izquierda
  ledcSetup(3, fre, res);  // rueda trasera derecha

  // establecer el modo de cada uno de los pines conectados al L298N

  // rueda delantera izquierda
  pinMode(DI_IN1, OUTPUT);
  pinMode(DI_IN2, OUTPUT);
  ledcAttachPin(DI_ENA, 0);

  // rueda delantera derecha
  pinMode(DD_IN3, OUTPUT);
  pinMode(DD_IN4, OUTPUT);
  ledcAttachPin(DD_ENB, 1);

  // rueda trasera izquierda
  pinMode(TI_IN1, OUTPUT);
  pinMode(TI_IN2, OUTPUT);
  ledcAttachPin(TI_ENA, 0);

  // rueda trasera derecha
  pinMode(TD_IN3, OUTPUT);
  pinMode(TD_IN4, OUTPUT);
  ledcAttachPin(TD_ENB, 1);

  // led rojo parada por bajo voltaje bateria
  ledcSetup(4, 2, 8); 
  ledcAttachPin(LVOL, 4);
  ledcWrite(4, 0); 

  // led rojo parada por bajo voltaje bateria
  pinMode(LVOL, OUTPUT);
  
  // led verde conexion WS
  pinMode(LCON, OUTPUT);

}
// ----------