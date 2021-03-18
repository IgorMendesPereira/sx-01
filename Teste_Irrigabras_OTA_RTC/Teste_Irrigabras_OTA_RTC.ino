#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "ESPmDNS.h"
#include "WiFiUdp.h"
#include "ArduinoOTA.h"
#include "Wire.h"
#include "RTClib.h"

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

RTC_DS3231 rtc;

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

String seg;
String minuto;
String horas;
String dia;
String mes;
String ano;
String hora;


String sentido = "";
String seco = "";
String ligado = "";

int webflag = 0;
String percs;
//--------------------------------------------------------------------------------------------------------------------------------
char stats[5];  // para entrada Serial

int auxP = 0;
int perc = 0;

String STRperc;

int auxPa = 0;

int numw;
int num;
int aux2 = 0;


unsigned long lastTime = 0;  
unsigned long timerDelay = 1000;
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

AsyncEventSource events("/events");


//---------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.begin(115200);
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
  delay(100);
  digitalWrite(ledPin, HIGH);

  // Initialize SPIFFS

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  if (! rtc.begin()) {
    Serial.println("Não foi possível encontrar RTC");
    while (1);
  }

//******************************************IMPORTANTISSIMO*******************************************************************************************
//Apos gravar a primeira vez, comentar a lina rtc.adjust

//rtc.adjust(DateTime(__DATE__, __TIME__));
  
//******************************************IMPORTANTISSIMO*******************************************************************************************

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
    digitalWrite(2, !digitalRead(2));
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
    if (percs != inputMessage) {
      percs = inputMessage;
      numw = percs.toInt();
    }
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
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();
}

void loop() {
  ArduinoOTA.handle();
  DateTime now = rtc.now();
  horas = now.hour();
  minuto = now.minute();
  seg = now.second();
  dia = now.day();
  mes = now.month();
  ano = now.year();
  hora = String(horas + ":" + minuto + ":" + seg + " - " + dia + "/" + mes + "/" + ano);
  LeEntrada();
  tatual = millis();
  if (Serial.available() == 5 || webflag == 1) { //se o software Serial receber uma mensagem de 3 bytes
    digitalWrite(2, !digitalRead(2));
    Leitura();
    //Serial.println("ok1");
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
    Serial.read();
    delay(5);
  }

  if (digitalRead(AVREAL) == HIGH && digitalRead(RTREAL) == HIGH) {
    cont++;
    delay(10);
    if (cont > 500) {
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
  if ((millis() - lastTime) > timerDelay) {
    events.send("ping",NULL,millis());
    events.send(String(sentido).c_str(),"sentido",millis());
    events.send(String(seco).c_str(),"seco",millis());
    events.send(String(ligado).c_str(),"ligado",millis());
    events.send(String(STRperc).c_str(),"STRperc",millis());
    events.send(String(hora).c_str(),"hora",millis());
    lastTime = millis();
  }
  Percentimetro();
  AtuaPercentimetro();
}
