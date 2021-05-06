#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "ESPmDNS.h"
#include "WiFiUdp.h"
#include "ArduinoOTA.h"
#include <SPI.h>
#include "LoRa.h"
#include <Wire.h>  //responsável pela comunicação i2c
#include "SSD1306.h" //responsável pela comunicação com o display
#include <Arduino.h>
#include "FS_File_Record.h"
#include "FS_File_Record2.h"
#include "logo.h"

#define LIGA              32
#define DESLIGA           33             //RUIM
#define AVANCO            25           //RUIM
#define REVERSO           12           //mesmo que 0
#define MOLHADO           13              //mesmo que 14
#define RAUX              21       //RUIM
#define RAUXP             22
#define PERCAT            23

#define AVREAL            36            //16  avanço lido
#define RTREAL            38              //RETORNO LIDO
#define PRESS             39             //PRESSOSTATO
#define PERC              37             //PERCENTIMETRO

//LoRA SPI

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define BAND 915E6

#define PABOOST true
SSD1306 display(0x3c, 4, 15);

//void TaskAtuaAg( void *pvParameters );

hw_timer_t *timer = NULL; 

String LoRaData;

int rssi;

int PROGMEM horaag[10];
String PROGMEM atuaag[10];
int PROGMEM contag;


const char* ssid = "Soil";
const char* password = "soil2021";

const char* ssidap = "Painel Soil";
const char* passwordap = "soil2021";

const char* PARAM_INPUT_1 = "percentimetro";
const char* PARAM_INPUT_2_1 = "Estado1";
const char* PARAM_INPUT_2_2 = "sentido1";
const char* PARAM_INPUT_2_3 = "seco1";
const char* PARAM_INPUT_2_4 = "horario1";
const char* PARAM_INPUT_2_5 = "horario2";
String inputMessage = "100";

char INWEB[3];
char EstadoAtual[3];
char EstadoAnterior[3];

int flagmem = 0;

long epoch;

// Estado apresentado na pagina web

String sentidow;
String secow;
String ligadow;
String Snum;

String sentido = "";
String seco = "";
String ligado = "";

String angulo;
String hora;

int webflag = 0;
String percs;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

char stats[10];  // para entrada Serial

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


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

AsyncEventSource events("/events");


const int sizeOfRecord = 30;
int id;
String values;
int registros;
int registros2;

// Objeto da nossa lib que recebe o nome do arquivo e tamanho fixo de registro
FS_File_Record ObjFS("/teste.bin", sizeOfRecord);
FS_File_Record2 AgFS("/agenda.bin", sizeOfRecord);


String dados;


// String que recebe as mensagens de erro
String errorMsg;

// Variável que guarda o último registro obtido
String lastRecord = "";


void IRAM_ATTR resetModule(){
ets_printf("(watchdog) reiniciar\n"); //imprime no log
esp_restart(); //reinicia o chip
}

void configureWatchdog()
{
timer = timerBegin(0, 80, true); //timerID 0, div 80
//timer, callback, interrupção de borda
timerAttachInterrupt(timer, &resetModule, true);
//timer, tempo (us), repetição
timerAlarmWrite(timer, 5*1000000, true);
timerAlarmEnable(timer); //habilita a interrupção //enable interrupt
}

// Replaces placeholder with LED state value

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

//void TaskAtuaAg( void *pvParameters) {
//
//  for (;;) {
//
//    delay(10);
//  }
//}

void setup()
{
  Serial2.begin(9600);
  Serial.begin(115200);
  configureWatchdog();
  pinMode(16, OUTPUT); //RST do oled
  digitalWrite(16, HIGH);
  delay(50);
  digitalWrite(16, LOW);    // reseta o OLED
  delay(50);
  digitalWrite(16, HIGH); // enquanto o OLED estiver ligado, GPIO16 deve estar HIGH
  display.init();
  display.flipScreenVertically();

  display.clear();


  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawXbm(0, 0, logo_width, logo_height, logo_bits);
  display.display();
  
  //Serial.begin(9600);
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

  // Iniciamos a Serial com velocidade de 115200
  EstadoAnterior[0] = EstadoAtual[0];
  EstadoAnterior[1] = EstadoAtual[1];
  EstadoAnterior[2] = EstadoAtual[2];
  // Exibe na Serial "Starting..." para debug
  Serial.print("Starting...");

  // Se não foi possível iniciar o File System, exibimos erro e reiniciamos o ESP
  if (!ObjFS.init())
  {
    Serial.println("File system error");
    delay(1000);
    ESP.restart();
  }

  // Exibimos mensagem
  Serial.println("File system ok");

  // Se o arquivo não existe, criamos o arquivo
  if (!ObjFS.fileExists())
  {
    Serial.println("New file ObjFS");
    ObjFS.newFile(); // Cria o arquivo
  }

  if (!AgFS.fileExists())
  {
    Serial.println("New file AgFS");
    AgFS.newFile(); // Cria o arquivo
  }

  Agendamento();
  SPI.begin(SCK, MISO, MOSI, SS);
  //  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSyncWord(0x99);
  LoRa.setSpreadingFactor(8);
  // Connect to Wi-Fi
  //  WiFi.begin(ssid, password);
  //  while (WiFi.status() != WL_CONNECTED) {
  //    delay(500);
  //    Serial.print(".");
  //  }
  // Print ESP32 Local IP Address
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.softAP(ssidap, passwordap);
  WiFi.begin(ssid, password);
  int contw = 0;
  while (WiFi.status() != WL_CONNECTED && contw!= 8) {
    
    delay(500);
    Serial.println("Connecting to WiFi..");
    contw++;
  }
  
  Serial.print("ESP32 IP as soft AP: ");
  Serial.println(WiFi.softAPIP());

  Serial.print("ESP32 IP on the WiFi network: ");
  Serial.println(WiFi.localIP());

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
    //digitalWrite(2, !digitalRead(2));
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

  //  xTaskCreatePinnedToCore(
  //    TaskAtuaAg,
  //    "AtuaAg",
  //    10000,  // Stack size
  //    NULL,
  //    1, // Priority
  //    NULL,
  //    0);

  server.on( "/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on( "/agenda", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/agenda.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/styles.css", "text/css");
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
      request->send(SPIFFS, "/index.html", String(), false, processor);
    }
    if (request->hasParam(PARAM_INPUT_2_1)) {

      //String Sai = String(request->getParam(PARAM_INPUT_2_1)->value() + " " + request->getParam(PARAM_INPUT_2_2)->value() + " " + request->getParam(PARAM_INPUT_2_3)->value() + " " + request->getParam(PARAM_INPUT_2_4)->value());
      if (request->getParam(PARAM_INPUT_2_1)->value() == "2") {
        String horarioag = request->getParam(PARAM_INPUT_2_4)->value();
        String ano = (horarioag.substring(0, 4));
        String mes = (horarioag.substring(5, 7));
        String dia = (horarioag.substring(8, 10));
        String hora = (horarioag.substring(11, 16));
        horarioag = String(hora + " - " + dia + "/" + mes + "/" + ano);
        String Sai = String("002-" + horarioag);

        values = Sai;

        while (values.length() < 30) {
          values += " ";
        }
        //Serial.println(values);
        if (values != "" && !AgFS.writeFile(values, &errorMsg))
          Serial.println(errorMsg);

        //showFile2();
        values = "";
        Agendamento();


      }
      if (request->getParam(PARAM_INPUT_2_1)->value() == "1") {
        String horarioag = request->getParam(PARAM_INPUT_2_4)->value();
        String ano = (horarioag.substring(0, 4));
        String mes = (horarioag.substring(5, 7));
        String dia = (horarioag.substring(8, 10));
        String hora = (horarioag.substring(11, 16));
        horarioag = String(hora + " - " + dia + "/" + mes + "/" + ano);
        String Sai = String(request->getParam(PARAM_INPUT_2_2)->value() + request->getParam(PARAM_INPUT_2_3)->value() + request->getParam(PARAM_INPUT_2_1)->value() + "-" + horarioag);
        Serial.println(Sai);
        values = Sai;

        while (values.length() < 30) {
          values += " ";
        }
        //Serial.println(values);
        if (values != "" && !AgFS.writeFile(values, &errorMsg))
          Serial.println(errorMsg);

        //showFile2();
        values = "";
        Agendamento();
        if (request->getParam(PARAM_INPUT_2_5)->value() != "") {
          //Serial2.println(request->getParam(PARAM_INPUT_2_5)->value());
          String horarioag = request->getParam(PARAM_INPUT_2_5)->value();
          String ano = (horarioag.substring(0, 4));
          String mes = (horarioag.substring(5, 7));
          String dia = (horarioag.substring(8, 10));
          String hora = (horarioag.substring(11, 16));
          horarioag = String(hora + " - " + dia + "/" + mes + "/" + ano);
          String Sai = String("002-" + horarioag);
          Serial.println(Sai);
          values = Sai;

          while (values.length() < 30) {
            values += " ";
          }
          //Serial.println(values);
          if (values != "" && !AgFS.writeFile(values, &errorMsg))
            Serial.println(errorMsg);

          //showFile2();
          values = "";
          Agendamento();
        }
      }
      request->send(SPIFFS, "/agenda.html", String(), false, processor);
    }
  });
  server.on("/agen", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/agenda.html", String(), false, processor);
  });
  server.on("/xama", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String(String(EstadoAtual[0]) + "-" +  String(EstadoAtual[1]) + "-" + String(EstadoAtual[2]) + "-" + String(perc) + "-" + String(angulo.toInt())));
  });
  server.on("/clearag", HTTP_GET, [](AsyncWebServerRequest * request) {
    AgFS.destroyFile();
    for(int i=0; i<10;i++){
    horaag[i]=0;
    atuaag[i]=" ";
    }
    Serial2.println("Delete ag OK");
    request->send(SPIFFS, "/agenda.html", String(), false, processor);
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
     
    INWEB[0] = '0';
    INWEB[1] = '0';
    INWEB[2] = '2';
    inputMessage = "000";

    //Serial.print(INWEB);
    // Serial.println(inputMessage);
    webflag = 1;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  events.onConnect([](AsyncEventSourceClient * client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();

  // EXEMPLO DE BUSCA (FIND)
  // Obs: O primeiro registro se posiciona na pos 0 (zero)
  // String reg = ObjFS.findRecord(10);
  // showDisplay(reg);

  // Exibimos o arquivo
  //showFile();
  countFile();
  String reg = ObjFS.findRecord(registros - 1);
  int idc = reg.indexOf(':');
  String ids = reg.substring(0, idc);
  id = ids.toInt();
  //Serial.println(horaag[1]);
  //Serial.println(atuaag);

}

// Exibe o espaço total, usado e disponível no display
void showAvailableSpace()
{
  Serial.println("Space: " + String(ObjFS.getTotalSpace()) + " Bytes");
  Serial.println("Used: " + String(ObjFS.getUsedSpace()) + " Bytes");

}

void loop()
{
  timerWrite(timer, 0); 
  ArduinoOTA.handle();
  int packetSize = LoRa.parsePacket();
  if (packetSize) {

    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      rssi = LoRa.packetRssi();
      //Serial.println("oi");
      //Serial.print(rssi);
      //Serial.print(" SNR: ");
      //float snr = LoRa.packetSnr();
      //Serial.println(snr);
      int separa = LoRaData.indexOf('-');
      int fim = LoRaData.indexOf('#');
      if (separa >= 0) {
        angulo = LoRaData.substring(0, separa);
        hora = LoRaData.substring((separa + 1), (fim));
        epoch = (LoRaData.substring((LoRaData.indexOf('#') + 1), LoRaData.indexOf('\n'))).toInt();
        //Serial.println(epoch);

      }


    }
  }


  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  String disprssi = String("RSSI = " + String(rssi));
  //Serial.println(disprssi);
  display.drawString(0, 16, disprssi);
  display.drawString(0 , 0, sentido + " " + seco + " " + ligado);
  display.drawString(0, 32, hora);
  display.drawString(0, 54, String(epoch));
  display.drawString(80, 54, "Soil Tech");
  display.display();

  // Se não houver memória disponível, exibe e reinicia o ESP
  //  if (!ObjFS.availableSpace())
  //  {
  //    Serial.println("Memory is full!");
  //    delay(10000);
  //    return;
  //  }
  tatual = millis();

  if (Serial2.available() > 0 || webflag == 1) { //se o Serial receber uma mensagem de 6 caracteres ou receber uma msg do WebServer
    //Serial.println("OK1");
    int buffersize = Serial2.available();
    Leitura(buffersize);
    if (buffersize == 6 || webflag == 1) {
      //Serial.println("OK2");

      if (stats[0] == '0' && stats[1] == '0' && stats[2] == '0') {
        LeEntrada();
        EnviaStatus();
      }

      if ( stats[0] != '0' || stats[1] != '0' || stats[2] != '0') {
        EnviaEstado();
        LeEntrada();
        EnviaStatus();
      }
      webflag = 0;
      return;
    } else
    {
      if (stats[0] == '0' && stats[1] == '0' && stats[2] == '0') {
        LeEntrada();
        EnviaStatus();
      }
      if (stats[0] == 'R' && stats[1] == 'S' && stats[2] == 'T') {
        Serial2.println("Reiniciando");
        delay(500);
        ESP.restart();
      }
      if (stats[0] == 'D' && stats[1] == 'E' && stats[2] == 'L') {
        ObjFS.destroyFile();
        Serial2.println("Delete OK");
        id = 0;
        return;
      }
      if (stats[0] == 'S' && stats[1] == 'F') {
        showFile();
        Serial2.print("Numero de Registros: ");
        Serial2.println(registros);
        showAvailableSpace();
        return;
      }
      if (stats[0] == 'R' && stats[1] == 'N') {
        String statss = String(stats);
        statss = statss.substring((statss.indexOf('N') + 1), statss.indexOf('#'));
        int busca = statss.toInt() - 1;
        //        Serial.print("Buscando Registro ");
        //        Serial.println((busca + 1));
        for (int i = (busca); i < id; i++) {
          int busca = statss.toInt() - 1;
          //          Serial2.print("Buscando Registro ");
          //          Serial2.println((i + 1));
          String reg = ObjFS.findRecord(i);
          Serial2.println(reg);
        }
        return;
      }
      if (stats[0] == 'S' && stats[1] == 'T' && stats[2] == 'R') {
        Serial2.println("teste str");
        Serial2.println(dados);
        return;
      }
      if (stats[0] == 'L' && stats[1] == 'S' && stats[2] == 'T') {
        Serial2.println("teste LST");
        String reg = ObjFS.findRecord(id - 1);
        Serial2.println(reg);
        return;
      }
      if (stats[0] == 'D' && stats[1] == 'I' && stats[2] == 'R') {
        listDir(SPIFFS, "/", 0);
        return;
      }

      //******************************************************************agendamento*****************************************************************************************

      if (stats[0] == 'A' && stats[1] == 'G') {
        Serial.print(String(stats));
        values = String(stats).substring((String(stats).indexOf('>') + 1), (String(stats).indexOf('#')));

        while (values.length() < 30) {
          values += " ";
        }
        //Serial.println(values);
        if (values != "" && !AgFS.writeFile(values, &errorMsg))
          Serial.println(errorMsg);

        //showFile2();
        values = "";
        Agendamento();

        //return;
      }
      if (stats[0] == 'S' && stats[1] == 'A') {
        showFile2();
        Serial2.print("Numero de Registros: ");
        Serial2.println(registros2);
        showAvailableSpace();
        return;
      }
      if (stats[0] == 'A' && stats[1] == 'D' && stats[2] == 'E' && stats[3] == 'L') {
        AgFS.destroyFile();
        Serial2.println("Delete ag OK");
        //id = 0;
        return;
      }
      /*  Lista de comandos:
           SF: exibe a lista inteira
           RN(numero): exibe um registro especifico
           LST: exibe o ultimo registro
           DEL: deleta a lista inteira
           STR: Salva a lista inteira em uma string "dados"

           NENHUM COMANDO PODE TER 6 CARACTERES
      */
    }

  } else {
    Serial2.read();
    delay(5);
  }
  if (digitalRead(AVREAL) == LOW && digitalRead(RTREAL) == LOW) {
    cont++;
    delay(100);
    if (cont > 50) {
      digitalWrite(LIGA, HIGH);
      digitalWrite(DESLIGA, HIGH);
      digitalWrite(AVANCO, HIGH);
      digitalWrite(REVERSO, HIGH);
      digitalWrite(MOLHADO, HIGH);
      digitalWrite(RAUX, HIGH);
      digitalWrite(RAUXP, HIGH);
      digitalWrite(PERCAT, HIGH);
      cont = 0;
      perc = 0;
      auxP = 0;
      num = 0;
      aux2 = 0;
    }
  } else {
    cont = 0;
  }


  if ((millis() - lastTime) > timerDelay) {
    //events.send("ping", NULL, millis());
    LeEntrada();
    events.send(String(sentido).c_str(), "sentido", millis());
    events.send(String(seco).c_str(), "seco", millis());
    events.send(String(ligado).c_str(), "ligado", millis());
    events.send(String(STRperc).c_str(), "STRperc", millis());
    events.send(String(hora).c_str(), "hora", millis());
    events.send(String(angulo.toInt()).c_str(), "angulo", millis());
    //events.send(String(rssi).c_str(), "rssi", millis());
    lastTime = millis();
  }
  Percentimetro();
  AtuaPercentimetro();
  AtuaAg();
  
}
