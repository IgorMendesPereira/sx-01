#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "ESPmDNS.h"
#include "WiFiUdp.h"
#include "ArduinoOTA.h"


#define LIGA              13              //mesmo que 1En5
#define DESLIGA           12             //RUIM
#define AVANCO            14           //RUIM
#define REVERSO           27            //mesmo que 0
#define MOLHADO           26              //mesmo que 14
#define RAUX              25          //RUIM
#define RAUXP              33
#define PERCAT             32
#define AVREAL            23            //16  avanço lido
#define RTREAL            19              //RETORNO LIDO
#define PRESS             18              //PRESSOSTATO
#define PERC              5             //PERCENTIMETRO


const char* ssid = "ESP32teste";
const char* password = "senhasenha";
const char* PARAM_INPUT_1 = "percentimetro";

String inputMessage = "100";

char INWEB[3];
char EstadoAtual[3];

// Set LED GPIO
const int ledPin = 2;

// Estado apresentado na pagina web
String sentidow;
String secow;
String ligadow;

String sentido = "";
String seco = "";
String ligado = "";

int webflag = 0;
String percs;
//--------------------------------------------------------------------------------------------------------------------------------
char stats[5];  // para entrada Serial2

int auxP = 0;
int perc = 0;

String STRperc;

int auxPa = 0;

int num;
int aux2 = 0;

unsigned long tatual = 0;
unsigned long t2 = 0;
unsigned long t3 = 0;
unsigned long t4 = 0;
unsigned long t5 = 0;
unsigned long t6 = 0;


int cont = 0;
int aux;

int lixo = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



//---------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(LIGA, OUTPUT);
  pinMode(DESLIGA, OUTPUT);
  pinMode(AVANCO, OUTPUT);
  pinMode(REVERSO, OUTPUT);
  pinMode(MOLHADO, OUTPUT);
  pinMode(RAUX, OUTPUT);
  pinMode(RAUXP, OUTPUT);
  pinMode(PERCAT, OUTPUT);

  pinMode(AVREAL, INPUT_PULLUP);
  pinMode(RTREAL, INPUT_PULLUP);
  pinMode(PERC, INPUT_PULLUP);
  pinMode(PRESS, INPUT_PULLUP);

  digitalWrite(LIGA, HIGH);
  digitalWrite(DESLIGA, HIGH);
  digitalWrite(AVANCO, HIGH);
  digitalWrite(REVERSO, HIGH);
  digitalWrite(MOLHADO, HIGH);
  digitalWrite(RAUX, HIGH);
  digitalWrite(RAUXP, HIGH);
  digitalWrite(PERCAT, HIGH);
  
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  
  // Initialize SPIFFS

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  //    while (WiFi.status() != WL_CONNECTED) {
  //      delay(1000);
  //      Serial.println("Connecting to WiFi..");
  //  }

  // Print ESP32 Local IP Address
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  
  // Route for root / web page
  server.on( "/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/logo_irrigabras.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/logo_irrigabras.png");
  });
  server.on("/logo_soil.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/logo_soil.png");
  });
  server.on("/reload.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/reload.png");
  });

  //percentimetro

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Avanço

  server.on("/av", HTTP_GET, [](AsyncWebServerRequest * request) {
    INWEB[0] = '3';
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Reverso

  server.on("/rev", HTTP_GET, [](AsyncWebServerRequest * request) {
    INWEB[0] = '4';
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Seco

  server.on("/seco", HTTP_GET, [](AsyncWebServerRequest * request) {
    INWEB[1] = '5';
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  // Molhado

  server.on("/molhado", HTTP_GET, [](AsyncWebServerRequest * request) {
    INWEB[1] = '6';
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  // Confirma

  server.on("/confirma", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(ledPin, HIGH);
    if (INWEB[0] == '3' || INWEB[0] == '4') {
      INWEB[2] = '1';
    } else {
      INWEB[2] = '2';
    }
    Serial.print(INWEB);
    Serial.println(inputMessage);
    percs = inputMessage;
    num = percs.toInt();
    webflag = 1;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Desliga

  server.on("/desliga", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(ledPin, LOW);
    INWEB[0] = '0';
    INWEB[1] = '0';
    INWEB[2] = '2';
    inputMessage = "000";

    Serial.print(INWEB);
    Serial.println(inputMessage);
    webflag = 1;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  server.begin();
}

void loop() {
  ArduinoOTA.handle();
  //Serial.println("ok");
  LeEntrada();
  tatual = millis();
  if (Serial2.available() == 5 || webflag == 1) { //se o software Serial2 receber uma mensagem de 3 bytes
    digitalWrite(2, !digitalRead(2));
    Leitura();
    Serial.println("ok1");

    if (stats[0] == '0' && stats[1] == '0' && stats[2] == '0') {
      LeEntrada();
      EnviaStatus();
    }

    if ( stats[0] != '0' || stats[1] != '0' || stats[2] != '0') {
      EnviaEstado();
      LeEntrada();
      VerificaEstado();
    }
    webflag = 0;

  } else
  {
    Serial2.read();
    delay(50);
  }

  if (digitalRead(AVREAL) == HIGH && digitalRead(RTREAL) == HIGH) {
    cont++;
    delay(1000);
    if (cont > 5) {
      digitalWrite(RAUX, HIGH);
      digitalWrite(LIGA, HIGH);
      digitalWrite(AVANCO, HIGH);
      digitalWrite(REVERSO, HIGH);
      digitalWrite(MOLHADO, HIGH);
      //  digitalWrite(SECO,HIGH);
      cont = 0;
      perc = 0;
      auxP = 0;
      num = 0;
      aux2 = 0;
      cont = 0;
    }
  } else {
    cont = 0;
  }
  Percentimetro();
  AtuaPercentimetro();
}
