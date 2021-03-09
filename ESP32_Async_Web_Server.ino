#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#define AVREAL            23            //16  avanço lido
#define RTREAL            19              //RETORNO LIDO
#define PRESS             18              //PRESSOSTATO
#define PERC              5             //PERCENTIMETRO


// Replace with your network credentials
const char* ssid = "Wifi ed";
const char* password = "pilantra";
const char* PARAM_INPUT_1 = "percentimetro";

String inputMessage = "100";
String valueString = String(5); 
int pos1 = 0; 
int pos2 = 0;

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

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


void LeEntrada(){
    if(digitalRead(PRESS)==HIGH){     
    EstadoAtual[1]='5';
    seco = "seco";
  }
  if(digitalRead(PRESS)==LOW){
    EstadoAtual[1]='6';
    seco = "molhado";
    
  }
  if(digitalRead(AVREAL)==HIGH && digitalRead(RTREAL)==HIGH){       //CONTATORA NC >>>> AQ LOW E RESTO HIGH
    EstadoAtual[2]='2';
    ligado = "Desligado";
    sentido = "";
    seco = "";
    
   return;
    }
    if(digitalRead(AVREAL)==LOW){
    EstadoAtual[0]='3';
    sentido = "Avanco";
  }
  if(digitalRead(RTREAL)==LOW){
    EstadoAtual[0]='4';
    sentido = "Reverso";
  }
  
  if(digitalRead(AVREAL)==LOW || digitalRead(RTREAL)==LOW){
   EstadoAtual[2]='1';
   ligado = "Ligado";
  }

}

// Replaces placeholder with LED state value
String processor(const String& var){
  //Serial.println(var);
  if(var == "SENTIDO"){
    if(sentido =="Avanco"){
      sentidow = "Avanco";
    }else if(sentido =="Reverso"){
      sentidow = "Reverso";
    }else {
      sentidow = "";
    }
    //Serial.print(ledState);
    return sentidow;
  }
    if(var == "SECO"){
    if(seco =="seco"){
      secow = "Seco";
    }else if(seco =="molhado"){
      secow = "Molhado";
    }
    if(EstadoAtual[2]=='2') {
      secow = "";
    }

    return secow;
  }
  if(var == "LIGADO"){
   if( EstadoAtual[2]=='2'){
    ligadow="Desligado";
   }else{
    ligadow="Ligado";
   }
  return ligadow;
  }   
  if(var == "PERCENTIMETRO"){
    return inputMessage;
  }
  return String();
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(AVREAL, INPUT_PULLUP);
  pinMode(RTREAL, INPUT_PULLUP);
  pinMode(PERC, INPUT_PULLUP);
  pinMode(PRESS, INPUT_PULLUP);
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
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
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on( "/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
server.on("/logo_irrigabras.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logo_irrigabras.png", "img/png");
  });
server.on("logo_soil.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "logo_soil.png", "img/png");
  });


  
 //percentimetro
 server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();    
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  // Avanço
  server.on("/av", HTTP_GET, [](AsyncWebServerRequest *request){
    INWEB[0] = '3';    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Reverso
  server.on("/rev", HTTP_GET, [](AsyncWebServerRequest *request){
    INWEB[0] = '4';     
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Seco
  server.on("/seco", HTTP_GET, [](AsyncWebServerRequest *request){
    INWEB[1] = '5';    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  // Molhado
  server.on("/molhado", HTTP_GET, [](AsyncWebServerRequest *request){
    INWEB[1] = '6';   
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  // Confirma
  server.on("/confirma", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);
    if(sentido == "3" || sentido =="4"){
      INWEB[2] = '1'; 
    }else{
      INWEB[2] = '2';
      }
      
    Serial.print(sentido);
    Serial.print(seco);
    Serial.print(ligado);
    Serial.println(inputMessage); 
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
// Desliga
  server.on("/desliga", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);
      INWEB[0] = '0';
      INWEB[1] = '5';
      INWEB[2] = '2';
      inputMessage = "00";
   
    Serial.print(INWEB);
    Serial.println(inputMessage);      
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  server.begin();
}
 
void loop(){
LeEntrada();
}
