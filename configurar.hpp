// ----------
// Establecer los pines conectados al L298N en cada motor y los led de informacion

// Rueda delantera izquierda
const uint8_t DI_ENA = 25;
const uint8_t DI_IN1 = 33;
const uint8_t DI_IN2 = 32;
const uint8_t DI_canal = 0;

// Rueda delantera derecha
const uint8_t DD_IN3 = 19;
const uint8_t DD_IN4 = 18;
const uint8_t DD_ENB = 5;
const uint8_t DD_canal = 1;

// Rueda trasrera izquierda
const uint8_t TI_ENA = 14;
const uint8_t TI_IN1 = 27;
const uint8_t TI_IN2 = 26;
const uint8_t TI_canal = 2;

// Rueda trasera derecha
const uint8_t TD_IN3 = 17;
const uint8_t TD_IN4 = 16;
const uint8_t TD_ENB = 4;
const uint8_t TD_canal = 3;

// Led rojo parada por bajo voltaje bateria
const uint8_t LedRojo = 12;
const uint8_t LedRojo_canal = 4;

// Led verde conexion WS
const uint8_t LedVerde = 13;

// ----------
void iniciarPines() {

  // Establecer los canales, frecuenciacuencia y resolucion de cada señal PWM 
  double frecuencia = 1000;   // frecuenciacuencia señal PWM
  uint8_t res = 8;            // resolucion ancho de pulso (de 0 a 255 que equivale a un ancho del 100%)

  // ledcSetup(uint8_t channel, double frecuenciaq, uint8_t resolution_bits)
  ledcSetup(DI_canal, frecuencia, res);  // rueda delantera izquierda
  ledcSetup(DD_canal, frecuencia, res);  // rueda delantera derecha
  ledcSetup(TI_canal, frecuencia, res);  // rueda trasera izquierda
  ledcSetup(TD_canal, frecuencia, res);  // rueda trasera derecha

  // Establecer el modo de cada uno de los pines conectados al L298N
  // Rueda delantera izquierda
  pinMode(DI_IN1, OUTPUT);
  pinMode(DI_IN2, OUTPUT);
  ledcAttachPin(DI_ENA, DI_canal);

  // Rueda delantera derecha
  pinMode(DD_IN3, OUTPUT);
  pinMode(DD_IN4, OUTPUT);
  ledcAttachPin(DD_ENB, DD_canal);

  // Rueda trasera izquierda
  pinMode(TI_IN1, OUTPUT);
  pinMode(TI_IN2, OUTPUT);
  ledcAttachPin(TI_ENA, TI_canal);

  // Rueda trasera derecha
  pinMode(TD_IN3, OUTPUT);
  pinMode(TD_IN4, OUTPUT);
  ledcAttachPin(TD_ENB, TD_canal);

  // Led rojo informa parada por bajo voltaje bateria
  double LedRojo_frecuencia = 2;  // frecuenciacuencia señal PWM
  uint8_t LedRojo_resolucion = 8;
  pinMode(LedRojo, OUTPUT);
  ledcSetup(LedRojo_canal, LedRojo_resolucion, LedRojo_frecuencia); 
  ledcAttachPin(LedRojo, LedRojo_canal);
  ledcWrite(LedRojo_canal, 0); 
  
  // Led verde informa conexion WS establecida
  pinMode(LedVerde, OUTPUT);

}
// ----------