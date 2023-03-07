//--------------
// https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi
#include <WiFi.h>
// https://github.com/me-no-dev/ESPAsyncWebServer
#include <ESPAsyncWebServer.h>
// https://github.com/espressif/arduino-esp32/tree/master/libraries/SPIFFS
#include <SPIFFS.h>
// https://github.com/bblanchon/ArduinoJson
#include <ArduinoJson.h>
// https://github.com/espressif/arduino-esp32/tree/master/libraries/Wire
#include <Wire.h>
// https://github.com/flav1972/ArduinoINA219
#include <INA219.h>


#include "configurar.hpp"
#include "actuarMotores.hpp"

const char *ssid = "Coche";
const char *password = "12345678";

AsyncWebServer server(80);        // Iniciamos el servidor web en el ESP32. 
AsyncWebSocket ws("/ws");         // Iniciamos el pluging para poder establecer conexiones mediante websocket
INA219 ina219;                    // Iniciamos el modulo INA219 para medir voltaje e intensidad.

float voltaje = 0;
float corriente = 0;
float capacidad = 0;
float voltajeCorte = 6.5;         // No descargar bateria por debajo 3 V. En este caso el voltaje de corte sera de 2 x 3,25 = 6,5 voltios
bool parar = false;               // Si voltaje bateria < Voltaje corte parar = true y ya no funcionan los motores y es necesario reiniciar 

unsigned long tiempo;

//==============
void setup() {

  Serial.begin(115200);
  delay(2000);
  Serial.println("\nIniciando coche v8");

  iniciarSPIFFS();
  iniciarSoftAP();
  iniciarServer();
  iniciarWebSockets();
  iniciarIna219();
  iniciarPines();

  Serial.println("Estado inicial realizarAccion(0,0)");
  realizarAccion(0,0);            // Coche parado como estado inicial

  digitalWrite(LedRojo, LOW);     // Led rojo indicador de voltaje bateria < 6,5 voltios apagado
  digitalWrite(LedVerde, LOW);    // Led verde indicador de conexion websocket establecida apagado

  tiempo = millis();              // Variable tiempo para establecer el lapso entre envios del voltaje y corriente al movil (o PC).
}

//==============
void loop() {

  if ( millis() > (tiempo + 200)) {     // Envio del voltaje y corriente al movil aproximadamente cada 200 mseg. 

    if (ws.count() > 0) {

      voltaje = ina219.busVoltage();

      if (ina219.ready()) {

        ws.cleanupClients(); 
                     
        corriente = ina219.shuntCurrent() * 1000;           // mA
        capacidad += corriente * (millis()-tiempo)/3600000; // mAh

        if (voltaje < voltajeCorte) {                       // Voltaje de corte es de 2 x 3,25 = 6,5 voltios
          parar = true;
          realizarAccion(0, 0);
          Serial.printf("Atencion, voltaje menor de %4.2f voltios. ...\n", voltajeCorte);
          ledcWrite(LedRojo_canal, 128);                     // Led rojo se enciende intermitente con frecuencia 2 Hz.
        }

        String envioControl;                     
        StaticJsonDocument<200> jsonDoc;
        jsonDoc["voltaje"] = voltaje;
        jsonDoc["corriente"] = corriente;
        jsonDoc["capacidad"] = capacidad;
        jsonDoc["parar"] = (parar) ? "Parado por voltaje < " + String(voltajeCorte,1) + " V" : "";
        serializeJson(jsonDoc, envioControl);      // Serializamos en formato JSON la informacion de voltaje e intensidad a enviar al movil 
        ws.textAll(envioControl); 
        
        tiempo = millis();
        
      }
    } 
  }
}

//==============
void iniciarSPIFFS() {

  SPIFFS.begin();

  if (!SPIFFS.exists("/menuCoche.html")) {     // Solo compruebo que esiste un fichero aunque son necesarios todos los indicados en el directorio data
    Serial.println("error SPIFFS");
  } else {
    Serial.println("Ok SPIFFS");
  }
}

//--------------
void iniciarSoftAP() {

  WiFi.softAP(ssid, password,1,0,1);      // Reduzco de 4 a 1 el maximo numero de conexiones al SoftAP para evitar varios controles simultaneos
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Ok softAP - direccion IP: ");
  Serial.println(IP);                     // La direccion IP del punto de acceso del ESP32 es 192.168.4.1
}

//--------------
void iniciarServer() {

  // Establecemos los ruteos en el web server para atender las peticiones del navegador del movil
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("menuCoche.html");
  server.serveStatic("/favicon.png", SPIFFS, "/favicon.png");
  server.onNotFound(noEncontrado);
  server.begin();
  Serial.print("Ok AsincWebServer\n");
}

//--------------
void noEncontrado(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/404.html", "text/html");
}

//--------------
void iniciarWebSockets() {

  ws.onEvent(onEvent);              // Establecemos la funcion de callback que atiende los eventos y datos recibidos por websockets
  server.addHandler(&ws);
  Serial.print("Ok WebSocket\n");
}

//--------------
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {

  switch (type) {

    case WS_EVT_CONNECT:
      Serial.printf("WebSocket cliente #%u conectado desde %s\n", client->id(), client->remoteIP().toString().c_str());
      digitalWrite(LedVerde, HIGH);      
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("Cliente WebSocket #%u desconectado\n", client->id());
      digitalWrite(LedVerde, LOW);
      realizarAccion(0,0);        // Parar coche por desconexion
      break;

    case WS_EVT_DATA: {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {

          String recibido = "";
          for(size_t i=0; i < info->len; i++) {
            recibido += (char) data[i];
          }
          
          Serial.println(recibido);

          StaticJsonDocument<200> jsonDoc;
          DeserializationError error = deserializeJson(jsonDoc, recibido);    // deserializamos de formato JSON el mensaje recibido del movil 
          if (error) {
             Serial.println("Error al deserializar mensaje recibido"); 
          }
          realizarAccion(jsonDoc["accion"],jsonDoc["pwm"]);   
        } 
      }
      break;

    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

// ----------
void realizarAccion(int accion, int pwm) {
 
  if (parar) {    

    motores(0,0,0,0);

  } else {
    
    switch (accion) {
      case 0: // parar
        motores(0,0,0,0);
        break;
      case 1: //giro adel. izq.
        motores(0,pwm,0,pwm);
        break;
      case 2: // adelante
        motores(pwm,pwm,pwm,pwm);
        break;
      case 3: // giro adel. der.
        motores(pwm,0,pwm,0);
        break;
      case 4:// desplaza izq.
        motores(-pwm,pwm,pwm,-pwm);
        break;
      case 6: // desplaza der.
        motores(pwm,-pwm,-pwm,pwm);
        break;
      case 7: // giro atras izq.
        motores(0,-pwm,0,-pwm);
        break;
      case 8:// atras
        motores(-pwm,-pwm,-pwm,-pwm);
        break;
      case 9: // giro atras der.
        motores(-pwm,0,-pwm,0);
        break;
      case 10: // rotar anti horario
        motores(-pwm,pwm,-pwm,pwm);
        break;
      case 12: // rotar horario
        motores(pwm,-pwm,pwm,-pwm);
        break;
      default: 
        Serial.printf("accion desconocida = %d\n",accion);  
    } 
  }
}

//----------
void iniciarIna219() {

  // Comunicacion I2C
  if (ina219.begin() != 0) {
      Serial.println("error Ina219");
  } else {
    Serial.println("Ok Ina219");
  }

  // configure INA219 for averaging at 64 samples, aprox 60 ms por ciclo de 64 medidas
  ina219.configure(INA219::RANGE_16V, INA219::GAIN_4_160MV, INA219::ADC_64SAMP, INA219::ADC_64SAMP, INA219::CONT_SH_BUS);

  // ina219.calibrate(R_SHUNT, V_SHUNT_MAX, V_BUS_MAX, I_MAX_EXPECTED);
  ina219.calibrate(0.1, 0.2, 16, 2);
}

//----------