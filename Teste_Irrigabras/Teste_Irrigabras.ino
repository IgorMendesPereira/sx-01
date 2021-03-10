#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

#define LIGA              13              //mesmo que 15
#define DESLIGA           12             //RUIM
#define AVANCO            14           //RUIM
#define REVERSO           27            //mesmo que 0
//#define SECO              PA8           //mesmo que 2
#define MOLHADO           26              //mesmo que 14
#define RAUX              25          //RUIM
#define RAUXP              33
#define PERCAT             32
#define AVREAL            23            //16  avanço lido
#define RTREAL            19              //RETORNO LIDO
#define PRESS             18              //PRESSOSTATO
#define PERC              5             //PERCENTIMETRO


// Replace with your network credentials
const char* ssid = "Wifi ed";
const char* password = "pilantra";
const char* PARAM_INPUT_1 = "percentimetro";

unsigned long previousMillis = 0;
const long interval = 5000;

String inputMessage = "100";
String valueString = String(5);

char INWEB[3];
char EstadoAtual[3];

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
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
int perc;

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

//---------------------------------------------------------------------------------------------------------------------------------------------------


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


void LeEntrada() {
  if (digitalRead(PRESS) == HIGH) {
    EstadoAtual[1] = '5';
    seco = "seco";
  }
  if (digitalRead(PRESS) == LOW) {
    EstadoAtual[1] = '6';
    seco = "molhado";

  }
  if (digitalRead(AVREAL) == HIGH && digitalRead(RTREAL) == HIGH) { //CONTATORA NC >>>> AQ LOW E RESTO HIGH
    EstadoAtual[2] = '2';
    ligado = "Desligado";
    sentido = "";
    seco = "";

    return;
  }
  if (digitalRead(AVREAL) == LOW) {
    EstadoAtual[0] = '3';
    sentido = "Avanco";
  }
  if (digitalRead(RTREAL) == LOW) {
    EstadoAtual[0] = '4';
    sentido = "Reverso";
  }

  if (digitalRead(AVREAL) == LOW || digitalRead(RTREAL) == LOW) {
    EstadoAtual[2] = '1';
    ligado = "Ligado";
  }

}

void Leitura() {
  for (int i = 0; i <= 5; i++) {
    stats[i] = Serial2.read();                //le a entrada Serial2, byte por byte
  }
  if(webflag == 1){
    stats[0] = INWEB[0];
    stats[1] = INWEB[1];
    stats[2] = INWEB[2];
    stats[3] = INWEB[3];
    stats[4] = INWEB[4];
    stats[5] = INWEB[5];
  }
}


void EnviaEstado() {   //ok
  if (stats[0] == '0' && stats[1] == '0' && stats[2] == '2') {
    digitalWrite(AVANCO, HIGH);
    digitalWrite(REVERSO, HIGH);
    Desliga();
  } else {
    if (stats[0] == '3') {
      digitalWrite(AVANCO, LOW);
      digitalWrite(REVERSO, HIGH);
    }
    if (stats[0] == '4') {
      digitalWrite(AVANCO, HIGH);
      digitalWrite(REVERSO, LOW);
    }

    if (stats[1] == '5') {
      // digitalWrite(SECO, LOW);
      digitalWrite(MOLHADO, HIGH);
    }
    if (stats[1] == '6') {
      digitalWrite(MOLHADO, LOW);
      // digitalWrite(SECO, HIGH);
    } else {
      //  digitalWrite(SECO, HIGH);
      digitalWrite(MOLHADO, HIGH);
    }
    if (stats[2] == '1') {
      num = (stats[3] - '0') * 10 + (stats[4] - '0');
      digitalWrite(LIGA, LOW);
      digitalWrite(RAUX, LOW);
      digitalWrite(DESLIGA, HIGH);
      delay(4000);
      digitalWrite(LIGA, HIGH);
      
    }
    else {
      digitalWrite(LIGA, HIGH);
      digitalWrite(DESLIGA, HIGH);
    }
    delay(500);
  }
}

void VerificaEstado() {
  aux = 0;
  if (EstadoAtual[2] == '2') {
    delay(150);
    EnviaStatus();
    aux = 1;

    return;
  }
  if (stats[0] == EstadoAtual[0] && stats[2] == EstadoAtual[2]) {

    if (aux == 0) {
      EnviaStatus();
      aux = 1;
    }

  } else if (stats[0] == '0' && stats[1] == '0' && stats[2] == '0') {
    if (aux == 0) {
      EnviaStatus();
      aux = 1;
    }

  }
  else {
    LeEntrada();
    EnviaStatus();
  }
  aux = 0;
}

void EnviaStatus() {

  Serial2.print(EstadoAtual[0]);                  //imprime a saida "3-5-1-"
  Serial2.print("-");
  Serial2.print(EstadoAtual[1]);
  Serial2.print("-");
  Serial2.print(EstadoAtual[2]);
  Serial2.print("-");
  Serial2.print(perc);
  // Serial2.print("-");


  //Serial2.print(disp);
  EstadoAtual[0] = '0';
  EstadoAtual[1] = '0';
  EstadoAtual[2] = '0';
  //  ESP.wdtFeed();//Alimenta o Watchdog.

}

void Desliga() {
  digitalWrite(DESLIGA, LOW);
  digitalWrite(RAUX, HIGH);
  digitalWrite(LIGA, HIGH);
  digitalWrite(AVANCO, HIGH);
  digitalWrite(REVERSO, HIGH);
  digitalWrite(MOLHADO, HIGH);
  digitalWrite(RAUXP, HIGH);
  digitalWrite(PERCAT, HIGH);
  //  digitalWrite(SECO,HIGH);
  delay(4000);
  perc = 0;
  auxP = 0;
  num = 0;
  aux2 = 0;
  digitalWrite(DESLIGA, HIGH);
}
/*
  void Serial2flush(){
  while(Serial2.available()>0){
  Serial2.read();
  }
  }
*/
void Percentimetro() {
  if (digitalRead(PERC) == LOW && auxP == 0) {
    while (auxP == 0) {
      t2 = millis();
      delay(100);
      auxP = 1;
    }

  } else if (auxP == 1 && digitalRead(PERC) == HIGH) {
    t3 = millis();
    perc = (t3 - t2) / 600;
    if (perc >= 100) {
      perc = 99;
    }
    delay(100);
    auxP = 0;
  } else if ((auxP == 1) && (tatual >= (t2 + 60000)) && (digitalRead(PERC) == LOW)) {
    perc = 100;
    auxP = 0;
  }
}

void AtuaPercentimetro() {

  if (num == 0) {
    aux2 = 0;
    t4 = 0;
    t5 = 0;
    t6 = 0;
    digitalWrite(RAUXP, HIGH);
    digitalWrite(PERCAT, HIGH);
  }
  if (num > 0 && aux2 == 0) {
    digitalWrite(RAUXP, LOW);
    digitalWrite(PERCAT, LOW);
    t4 = millis();
    t5 = t4 + num * 600; //ton
    t6 = t4 + 60000;
    aux2 = 1;
  }

  if (tatual >= t5 && aux2 == 1) {
    digitalWrite(PERCAT, HIGH);
    aux2 = 2;
  }
  if (tatual >= t6 && aux2 == 2) {
    aux2 = 0;

  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------

// Replaces placeholder with LED state value
String processor(const String& var) {
  //Serial.println(var);
  if (var == "SENTIDO") {
    if (sentido == "Avanco") {
      sentidow = "Avanco";
    } else if (sentido == "Reverso") {
      sentidow = "Reverso";
    } else {
      sentidow = "";
    }
    //Serial.print(ledState);
    return sentidow;
  }
  if (var == "SECO") {
    if (seco == "seco") {
      secow = "Seco";
    } else if (seco == "molhado") {
      secow = "Molhado";
    }
    if (EstadoAtual[2] == '2') {
      secow = "";
    }

    return secow;
  }
  if (var == "LIGADO") {
    if ( EstadoAtual[2] == '2') {
      ligadow = "Desligado";
    } else {
      ligadow = "Ligado";
    }
    return ligadow;
  }
  if (var == "PERCENTIMETRO") {
    return inputMessage;
  }
  return String();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(LIGA, OUTPUT);
  pinMode(DESLIGA, OUTPUT);
  pinMode(AVANCO, OUTPUT);
  pinMode(REVERSO, OUTPUT);
  pinMode(MOLHADO, OUTPUT);
  pinMode(RAUX, OUTPUT);
  pinMode(RAUXP,OUTPUT);
  pinMode(PERCAT,OUTPUT);

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
  //Desliga();
  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  //WiFi.softAP(ssid, password);
 // IPAddress IP = WiFi.softAPIP();
//  Serial.print("AP IP address: ");
//  Serial.println(IP);

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
    //perc = atoi(percs);
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
  LeEntrada();
  tatual = millis();
  if (Serial2.available() == 5 || webflag == 1) { //se o software Serial2 receber uma mensagem de 3 bytes
    Leitura();


    if (stats[0] == '0' && stats[1] == '0' && stats[2] == '0') {
      LeEntrada();
      EnviaStatus();
    }

    if ( stats[0] != '0' || stats[1] != '0' || stats[2] != '0') {
      EnviaEstado();
      LeEntrada();
      VerificaEstado();   
    }
    webflag=0;
    
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
