#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "ESPmDNS.h"
#include "ArduinoOTA.h"
#include <SPI.h>
#include "LoRa.h"
#include <Arduino.h>
#include "FS_File_Record.h"
#include "FS_File_Record2.h"
#include "FS_File_Record3.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"


//ATUAÇÃO

#define LIGA              3
#define DESLIGA           1
#define AVANCO            0
#define REVERSO           22
#define MOLHADO           23
#define RAUX              15
#define RAUXP             2
#define PERCAT            4

//LEITURA

#define AVREAL            36
#define RTREAL            37
#define PRESS             39
#define PERC              38

//LoRA SPI

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define BAND 915E6

#define PABOOST true

void taskDES( void *pvParameters ); //DEFINE TASK

//hw_timer_t *timer = NULL;

String LoRaData;
String JSONhora;

int rssi;

String TIPO;
String nome;
String eco;
int auxeco = 0;
int inicioH = 0;
int fimH = 0;
int inicioM = 0;
int fimM = 0;

int PROGMEM horaag[4];
int PROGMEM percag[4];
String PROGMEM atuaag[4];
int PROGMEM horaag2[4];
int PROGMEM percag2[4];
String PROGMEM atuaag2[4];
String atuaP[10];
int pos[10];
String atuaP2[10];
int pos2[10];
int PROGMEM contag;
int PROGMEM contpos;

const char* ssid = "Poligamia";
const char* password = "0034731858";

const char* ssidap = "Painel Soil";
const char* passwordap = "soil2021";

// INPUTS PAGINA WEB

const char* PARAM_INPUT_0 = "Estado";
const char* PARAM_INPUT_0_2 = "sentido";
const char* PARAM_INPUT_0_3 = "seco";
const char* PARAM_INPUT_1 = "percentimetro";
const char* PARAM_INPUT_2_1 = "Estado1";
const char* PARAM_INPUT_2_2 = "sentido1";
const char* PARAM_INPUT_2_3 = "seco1";
const char* PARAM_INPUT_2_4 = "horario1";
const char* PARAM_INPUT_2_5 = "horario2";
const char* PARAM_INPUT_3_1 = "Estado2";
const char* PARAM_INPUT_3_3 = "angulo1";
const char* PARAM_INPUT_3_4 = "angulo2";
const char* PARAM_CONTATORA = "inputTipo";
const char* PARAM_ESPERA = "inputDelay";
const char* PARAM_NOME = "inputNome";
const char* PARAM_SW = "inputSW";
const char* PARAM_PERC_H = "percentimetroAH";
String inputMessage = "100";

int LoRaAdress;

char INWEB[3];
char EstadoAtual[3];
char EstadoAnterior[3];

int flagmem = 0;

long epoch;
long tempo = 0;
int endloop;
int espera;
int error = 0;
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
String horaH;
String horaM;
String horaS;
int horaTS;


int webflag = 0;
String percs;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

const char botao_on[] PROGMEM = R"rawliteral(<a href="/get?modoeco=off"><button class="bt_eco_on">Eco</button></a>)rawliteral";
const char botao_off[] PROGMEM = R"rawliteral(<a href="/get?modoeco=on"><button class="bt_eco_off">Eco</button></a>)rawliteral";
const char eco_on[] PROGMEM = R"rawliteral(<div class="eco_on"></div>)rawliteral";
const char eco_off[] PROGMEM = R"rawliteral(<div class="eco_off"></div>)rawliteral";


char stats[60];  // para entrada Serial2.

int auxP = 0;
int perc = 0;
int entraperc = 0;

int RETflag = 0;

String STRperc;

int auxPa = 0;

int numw;
int num;
int aux2 = 0;


unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
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
AsyncWebSocket ws("/ws");

AsyncEventSource events("/events");


const int sizeOfRecord = 30;
int id;
String values;
int registros;
int registros2;
int registros3;

// Objeto da nossa lib que recebe o nome do arquivo e tamanho fixo de registro
FS_File_Record ObjFS("/teste.bin", sizeOfRecord);
FS_File_Record2 AgFS("/agenda.bin", sizeOfRecord);
FS_File_Record3 PosFS("/agendaPOS.bin", 10);


String dados;


// String que recebe as mensagens de erro
String errorMsg;

// Variável que guarda o último registro obtido
String lastRecord = "";


// Função reset WD
//void IRAM_ATTR resetModule() {
//  ets_printf("(watchdog) reiniciar\n"); //imprime no log
//  esp_restart(); //reinicia o chip
//}

//void configureWatchdog()
//{
//  timer = timerBegin(0, 80, true); //timerID 0, div 80
//  //timer, callback, interrupção de borda
//  timerAttachInterrupt(timer, &resetModule, true);
//  //timer, tempo (us), repetição
//  timerAlarmWrite(timer, 5 * 1000000, true);
//  timerAlarmEnable(timer); //habilita a interrupção //enable interrupt
//}


void notifyClients(String text) {
  ws.textAll(text);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "status") == 0) {
      LeEntrada();
      String EstadoWS = "#";
//      Serial2.println(EstadoAtual[0]);
//      Serial2.println(EstadoAtual[1]);
//      Serial2.println(EstadoAtual[2]);
      if(EstadoAtual[0]=='3' || EstadoAtual[0]=='4'){
        if(EstadoAtual[0]=='3'){
          EstadoWS += "Avanco ";
        }
        if(EstadoAtual[0]=='4'){
          EstadoWS += "Reverso ";
        }
        
      }
      if(EstadoAtual[1]=='5' || EstadoAtual[1]=='6'){
        if(EstadoAtual[1]=='5'){
          EstadoWS += "Seco ";
        }
        if(EstadoAtual[1]=='6'){
          EstadoWS += "Molhado ";
        }
        
      }
      if(EstadoAtual[2]=='1' || EstadoAtual[2]=='2'){
        if(EstadoAtual[2]=='1'){
          EstadoWS += "Ligado ";
        }
        if(EstadoAtual[2]=='2'){
          EstadoWS = "#Desligado ";
        }
      }
      
      //Serial2.println(EstadoWS);
      notifyClients(EstadoWS);
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial2.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial2.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

//Listagem dos Diretorios
void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial2.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial2.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial2.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial2.print("  DIR : ");
      Serial2.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial2.print("  FILE: ");
      Serial2.print(file.name());
      Serial2.print("  SIZE: ");
      Serial2.println(file.size());
    }
    file = root.openNextFile();
  }
}

//task para desarmar os reles caso o painel seja desligado manualmente

void TaskDES( void *pvParameters) {




  long lastTime0 = 0;
  long timer = 1000;
  for (;;) {
    // timerWrite(timer, 0); //ALIMENTA WD

    if ((millis() - lastTime0) > timer) {
      tempo = tempo + (millis() - lastTime0) / 1000;
      lastTime0 = millis();

      //Serial2.println(tempo);

    }

    time_t rawtime = tempo;
    struct tm  ts;
    char       bufT[80];

    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&rawtime);
    strftime(bufT, sizeof(bufT), "%H:%M-%S", &ts);
    horaH = String(bufT).substring(0, String(bufT).indexOf(':'));
    horaM = String(bufT).substring(String(bufT).indexOf(':') + 1, String(bufT).indexOf('-'));
    horaS = String(bufT).substring(String(bufT).indexOf('-') + 1, String(bufT).indexOf('\n'));
    horaTS = horaH.toInt() * 3600 + horaM.toInt() * 60 + horaS.toInt();



    delay(1);
  }
}

void setup()
{
  Serial2.begin(9600);   //INICIALIZA SERIAL2, PARA ESP32 LORAV2 NUNCA UTILIZAR A SERIAL1
  // Serial2.begin(9600);
  //configureWatchdog();

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

  EstadoAnterior[0] = EstadoAtual[0];
  EstadoAnterior[1] = EstadoAtual[1];
  EstadoAnterior[2] = EstadoAtual[2];
  // Exibe na Serial2."Starting..." para debug
  Serial2.print("Starting...");

  // Se não foi possível iniciar o File System, exibimos erro e reiniciamos o ESP
  if (!ObjFS.init())
  {
    Serial2.println("File system error");
    delay(1000);
    //ESP.restart();
  }

  // Exibimos mensagem
  Serial2.println("File system ok");

  // Se o arquivo não existe, criamos o arquivo
  if (!ObjFS.fileExists())
  {
    Serial2.println("New file ObjFS");
    ObjFS.newFile(); // Cria o arquivo
  }

  if (!AgFS.fileExists())
  {
    Serial2.println("New file AgFS");
    AgFS.newFile(); // Cria o arquivo
  }
  if (!PosFS.fileExists())
  {
    Serial2.println("New file PosFS");
    PosFS.newFile(); // Cria o arquivo
  }

  //EXECUTA A FUNCAO AGENDAMENTO TODA VEZ QUE INICIA PARA GRAVAR OS VALORES NAS STRINGS DE AGENDAMENTO SALVAS NA FLASH
  Agendamento();

  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  //INICIALIZA LORA

  if (!LoRa.begin(BAND)) {
    Serial2.println("Starting LoRa failed!");
    while (1);
  }

  //SYNC WORD DEFAUT 0X99, CASO NENHUMA TENHA SIDO SETADA NAS CONFIGURACOES

  //LoRa.setSyncWord(0x99);
  //LoRa.setSpreadingFactor(8);

  //WIFI em modo hibrido STA para desenvolvimento e AP para campo

  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.softAP(ssidap, passwordap);
  WiFi.begin(ssid, password);
  int contw = 0;
  while (WiFi.status() != WL_CONNECTED && contw != 8) {

    delay(500);
    Serial2.println("Connecting to WiFi..");
    contw++;
  }
  initWebSocket();

  //inicializa as variaveis de configuração salvas na memoria
  inicioH = String(readFile(SPIFFS, "/inicioeco.bin")).substring(0, String(String(readFile(SPIFFS, "/inicioeco.bin"))).indexOf(':')).toInt();
  fimH =    String(readFile(SPIFFS, "/fimeco.bin")).substring(0, String(String(readFile(SPIFFS, "/fimeco.bin"))).indexOf(':')).toInt();
  inicioM = String(readFile(SPIFFS, "/inicioeco.bin")).substring(String(String(readFile(SPIFFS, "/inicioeco.bin"))).indexOf(':') + 1 , String(String(readFile(SPIFFS, "/inicioeco.bin"))).indexOf('\n')).toInt();
  fimM =    String(readFile(SPIFFS, "/fimeco.bin")).substring(String(String(readFile(SPIFFS, "/fimeco.bin"))).indexOf(':') + 1 , String(String(readFile(SPIFFS, "/fimeco.bin"))).indexOf('\n')).toInt();


  TIPO = readFile(SPIFFS, "/contator.txt");
  espera = readFile(SPIFFS, "/delay.txt").toInt() * 1000;
  nome = readFile(SPIFFS, "/nome.txt");
  LoRaAdress = readFile(SPIFFS, "/sw.txt").toInt();
  if (readFile(SPIFFS, "/modoeco.bin") == "on") {
    eco = "on";
  } else {
    eco = " ";
  }
  //caso não haja nada na memoria será setado o defaut (NA;4s;Pivo 1;sw:0x99)
  if (error == 1) {
    TIPO = "NA";
    writeFile(SPIFFS, "/contator.txt", TIPO.c_str());
    espera = 4000;
    String indelay = String((espera / 1000));
    writeFile(SPIFFS, "/delay.txt", indelay.c_str());
    nome = "Pivo 1";
    writeFile(SPIFFS, "/nome.txt", nome.c_str());
    LoRaAdress = 153;
    writeFile(SPIFFS, "/sw.txt", String(LoRaAdress).c_str());
  }
  String RA = readFile(SPIFFS, "/estadoanteriorAUTO.bin");
  stats[0] = RA[0];
  stats[1] = RA[2];
  stats[2] = RA[4];
  numw = RA.substring(6, RA.indexOf('\n')).toInt();
  EnviaEstado();

  //LoRa.setSyncWord(syncword.toInt());

  Serial2.print("ESP32 IP as soft AP: ");
  Serial2.println(WiFi.softAPIP());

  Serial2.print("ESP32 IP on the WiFi network: ");
  Serial2.println(WiFi.localIP());

  //CODIGO DE GRAVACAO DE ARQUIVOS VIA OTA

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial2.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial2.println("nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial2.printf("Progress: %u%%r", (progress / (total / 100)));
    // timerWrite(timer, 0);
    //digitalWrite(2, !digitalRead(2));
  })
  .onError([](ota_error_t error) {
    Serial2.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial2.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial2.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial2.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial2.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial2.println("End Failed");
  });
  ArduinoOTA.begin();


  //PARAMETRIZACAO DA TASK

  xTaskCreatePinnedToCore(
    TaskDES,
    "TaskDES",
    10000,  // Stack size
    NULL,
    1, // Priority
    NULL,
    0); //CORE 0

  //-------------------------------------------------------------------- WEB SERVER ---------------------------------------------------------------------------------------

  server.on( "/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/config", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/config.html", String(), false, processor);
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
  server.on("/play.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/play.png");
  });
  server.on("/rotate_left_small.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/rotate_left_small.png");
  });
  server.on("/rotate_right_small.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/rotate_right_small.png");
  });
  server.on("/stop.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/stop.png");
  });
  server.on("/water_off.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/water_off.png");
  });
  server.on("/water_on.png", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/water_on.png");
  });


  //AQUISIÇÃO DOS PARAMETROS NA PAGINA WEB

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    //------------------------------------------CONFIG------------------------------------------------------------------------

    if (request->hasParam(PARAM_CONTATORA)) {
      TIPO = request->getParam(PARAM_CONTATORA)->value();
      writeFile(SPIFFS, "/contator.txt", TIPO.c_str());
    }
    if (request->hasParam(PARAM_ESPERA)) {

      String indelay = (request->getParam(PARAM_ESPERA)->value());
      espera = indelay.toInt() * 1000;
      writeFile(SPIFFS, "/delay.txt", indelay.c_str());
    }
    if (request->hasParam(PARAM_NOME)) {

      nome = request->getParam(PARAM_NOME)->value();
      writeFile(SPIFFS, "/nome.txt", nome.c_str());
    }
    if (request->hasParam(PARAM_SW)) {

      LoRaAdress = (request->getParam(PARAM_SW)->value()).toInt();
      writeFile(SPIFFS, "/sw.txt", String(LoRaAdress).c_str());
      //LoRa.setSyncWord(syncword.toInt());
      Serial2.println(byte(LoRaAdress));
    }
    //-----------------------------------------------------------------------------------------------------------------------
    if (request->hasParam("modoeco")) {
      if (request->getParam("modoeco")->value() == "on") {
        eco = request->getParam("modoeco")->value();
      } else {
        eco = " ";
      }
      writeFile(SPIFFS, "/modoeco.bin", eco.c_str());
      request->send(SPIFFS, "/index.html", String(), false, processor);
    }
    if (request->hasParam("inicioeco") && request->hasParam("fimeco")) {
      String inicio = request->getParam("inicioeco")->value();
      String fim = request->getParam("fimeco")->value();
      if (request->hasParam("switch")) {
        eco = request->getParam("switch")->value();
      } else {
        eco = " ";
      }
      writeFile(SPIFFS, "/modoeco.bin", eco.c_str());

      writeFile(SPIFFS, "/inicioeco.bin", inicio.c_str());
      writeFile(SPIFFS, "/fimeco.bin", fim.c_str());
      inicioH = String(readFile(SPIFFS, "/inicioeco.bin")).substring(0, String(String(readFile(SPIFFS, "/inicioeco.bin"))).indexOf(':')).toInt();
      fimH =    String(readFile(SPIFFS, "/fimeco.bin")).substring(0, String(String(readFile(SPIFFS, "/fimeco.bin"))).indexOf(':')).toInt();
      inicioM = String(readFile(SPIFFS, "/inicioeco.bin")).substring(String(String(readFile(SPIFFS, "/inicioeco.bin"))).indexOf(':') + 1 , String(String(readFile(SPIFFS, "/inicioeco.bin"))).indexOf('\n')).toInt();
      fimM =    String(readFile(SPIFFS, "/fimeco.bin")).substring(String(String(readFile(SPIFFS, "/fimeco.bin"))).indexOf(':') + 1 , String(String(readFile(SPIFFS, "/fimeco.bin"))).indexOf('\n')).toInt();



      //Serial2.println(readFile(SPIFFS, "/inicioeco.bin"));

      //Serial2.println(readFile(SPIFFS, "/fimeco.bin"));
      request->send(SPIFFS, "/eco.html", String(), false, processor);
    }

    //---------------------------------------AGENDAMENTO POR HORARIO---------------------------------------------------------

    if (request->hasParam(PARAM_INPUT_2_1)) {

      //String Sai = String(request->getParam(PARAM_INPUT_2_1)->value() + " " + request->getParam(PARAM_INPUT_2_2)->value() + " " + request->getParam(PARAM_INPUT_2_3)->value() + " " + request->getParam(PARAM_INPUT_2_4)->value());
      if (request->getParam(PARAM_INPUT_2_1)->value() == "2") {
        String horarioag = request->getParam(PARAM_INPUT_2_4)->value();
        String ano = (horarioag.substring(0, 4));
        String mes = (horarioag.substring(5, 7));
        String dia = (horarioag.substring(8, 10));
        String hora = (horarioag.substring(11, 16));
        horarioag = String(hora + " - " + dia + "/" + mes + "/" + ano);
        String Sai = String("002/0-" + horarioag);

        values = Sai;

        while (values.length() < 30) {
          values += " ";
        }

        if (registros2 < 4) {
          if (values != "" && !AgFS.writeFile(values, &errorMsg))
            Serial2.println(errorMsg);
        }
        

        values = "";
        Agendamento();


      }
      if (request->getParam(PARAM_INPUT_2_1)->value() == "1") {
        String perc_a = request->getParam(PARAM_PERC_H)->value();
        //num = perc_a.toInt();
        //Serial2.println(num);
        String horarioag = request->getParam(PARAM_INPUT_2_4)->value();
        String ano = (horarioag.substring(0, 4));
        String mes = (horarioag.substring(5, 7));
        String dia = (horarioag.substring(8, 10));
        String hora = (horarioag.substring(11, 16));
        horarioag = String(hora + " - " + dia + "/" + mes + "/" + ano);
        String Sai = String(request->getParam(PARAM_INPUT_2_2)->value() + request->getParam(PARAM_INPUT_2_3)->value() + request->getParam(PARAM_INPUT_2_1)->value() + "/" + perc_a + "-" + horarioag);
        Serial2.println(Sai);
        values = Sai;

        while (values.length() < 30) {
          values += " ";
        }

        if (registros2 < 4) {
          if (values != "" && !AgFS.writeFile(values, &errorMsg))
            Serial2.println(errorMsg);
        }

        values = "";
        Agendamento();
        if (request->getParam(PARAM_INPUT_2_5)->value() != "") {
          String horarioag = request->getParam(PARAM_INPUT_2_5)->value();
          String ano = (horarioag.substring(0, 4));
          String mes = (horarioag.substring(5, 7));
          String dia = (horarioag.substring(8, 10));
          String hora = (horarioag.substring(11, 16));
          horarioag = String(hora + " - " + dia + "/" + mes + "/" + ano);
          String Sai = String("002/0-" + horarioag);
          Serial2.println(Sai);
          values = Sai;

          while (values.length() < 30) {
            values += " ";
          }
          //Serial2.println(values);
          if (registros2 < 4) {
            if (values != "" && !AgFS.writeFile(values, &errorMsg))
              Serial2.println(errorMsg);
          }
          //showFile2();
          values = "";
          Agendamento();
        }
      }
      ws.textAll("%");
      request->send(SPIFFS, "/agendaH.html", String(), false, processor);
    }

    //-----------------------------------------------------------------------------------------------------------------------

    //---------------------------------------AGENDAMENTO POR POSIÇÃO---------------------------------------------------------

    if (request->hasParam(PARAM_INPUT_3_1)) {
      if (request->getParam(PARAM_INPUT_3_1)->value() == "1") {
        String angW = request->getParam(PARAM_INPUT_3_3)->value();
        String acaoPOS;
        acaoPOS = "002";
        Serial2.println(acaoPOS + "-" + angW);
        values = String(acaoPOS + "-" + angW);

        while (values.length() < 10) {
          values += " ";
        }

        if (values != "" && !PosFS.writeFile(values, &errorMsg))
          Serial2.println(errorMsg);

        values = "";
        RETflag = 1;
        AgendaPOS();

      }
      if (request->getParam(PARAM_INPUT_3_1)->value() == "2") {
        String angW = request->getParam(PARAM_INPUT_3_3)->value();
        String acaoPOS;
        acaoPOS = "RET";

        values = String(acaoPOS + "-" + angW);

        while (values.length() < 10) {
          values += " ";
        }

        if (values != "" && !PosFS.writeFile(values, &errorMsg))
          Serial2.println(errorMsg);

        values = "";
        angW = request->getParam(PARAM_INPUT_3_4)->value();
        //Serial2.println("RET-" + angW);
        values = String("002-" + angW);

        while (values.length() < 10) {
          values += " ";
        }

        if (values != "" && !PosFS.writeFile(values, &errorMsg))
          Serial2.println(errorMsg);

        values = "";
        AgendaPOS();
      }
      ws.textAll("%");
      request->send(SPIFFS, "/AgendaP.html", String(), false, processor);
    }
    //-----------------------------------------------------------------------------------------------------------------------

    //------------------------------------------------ ATUAÇÃO --------------------------------------------------------------

    if (request->hasParam(PARAM_INPUT_0)) {
      if (request->getParam(PARAM_INPUT_0)->value() == "1") {
        INWEB[2] = '1';

        if (request->hasParam(PARAM_INPUT_0_2)) {
          if (request->getParam(PARAM_INPUT_0_2)->value() == "3") {
            INWEB[0] = '3';
          }
          if (request->getParam(PARAM_INPUT_0_2)->value() == "4") {
            INWEB[0] = '4';
          }
        }
        if (request->hasParam(PARAM_INPUT_0_3)) {
          if (request->getParam(PARAM_INPUT_0_3)->value() == "5") {
            INWEB[1] = '5';
          }
          if (request->getParam(PARAM_INPUT_0_3)->value() == "6") {
            INWEB[1] = '6';
          }

        }
        if (request->hasParam(PARAM_INPUT_1)) {
          inputMessage = request->getParam(PARAM_INPUT_1)->value();
          if (percs != inputMessage) {
            percs = inputMessage;
            numw = percs.toInt();
            aux2 = 0;
          }
        }

        webflag = 1;
      } else {
        if (request->getParam(PARAM_INPUT_0)->value() == "2") {
          INWEB[0] = '0';
          INWEB[1] = '0';
          INWEB[2] = '2';
          inputMessage = "000";

          webflag = 1;
        }
      }
      request->send(SPIFFS, "/index.html", String(), false, processor);
    }




  });

  //---------------------------------------ROTAS---------------------------------------------------------


  server.on("/agen", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/agenda.html", String(), false, processor);
  });
  server.on("/eco", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/eco.html", String(), false, processor);
  });
  server.on("/xama", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", String(String(EstadoAtual[0]) + "-" +  String(EstadoAtual[1]) + "-" + String(EstadoAtual[2]) + "-" + String(perc) + "-" + String(angulo.toInt()) + "-" + String(tempo)));
  });
  server.on("/aghora", HTTP_GET, [](AsyncWebServerRequest * request) {
    //    showFile2();
    //    //request->send(200, "text/plain", String(dados));
    //    request->send(SPIFFS, "/agenda.bin" , "text/plain");
    StaticJsonDocument<400> doc;
    JsonArray data = doc.createNestedArray("Atua");
    JsonArray data1 = doc.createNestedArray("Hora");
    JsonArray data2 = doc.createNestedArray("Percent");
    for (int i = 0; i < registros2; i++) {
      data.add(atuaag[i]);
      time_t rawtime = horaag[i];
      struct tm  ts;
      char       buf[80];

      // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
      ts = *localtime(&rawtime);
      strftime(buf, sizeof(buf), "%H:%M:%S %d-%m-%Y", &ts);
      data1.add(buf);
      data2.add(percag[i]);
    }
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);

  });
  server.on("/agpos", HTTP_GET, [](AsyncWebServerRequest * request) {
    StaticJsonDocument<200> doc;
    JsonArray data = doc.createNestedArray("Atua");
    JsonArray data1 = doc.createNestedArray("POS");

    for (int i = 0; i < registros3; i++) {
      data.add(atuaP[i]);
      data1.add(pos[i]);

    }
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });
  server.on("/ecodata", HTTP_GET, [](AsyncWebServerRequest * request) {
    StaticJsonDocument<200> doc;
    JsonArray data = doc.createNestedArray("Eco");
    JsonArray data1 = doc.createNestedArray("Inicio");
    JsonArray data2 = doc.createNestedArray("Fim");

    data.add(eco);
    data1.add(readFile(SPIFFS, "/inicioeco.bin"));
    data2.add(readFile(SPIFFS, "/fimeco.bin"));
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });
  server.on("/hist", HTTP_GET, [](AsyncWebServerRequest * request) {
    showFile();
    //request->send(200, "text/plain", String(dados));
    request->send(SPIFFS, "/teste.bin" , "text/plain");
  });
  server.on("/agendaP", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/AgendaP.html", String(), false, processor);
  });
  server.on("/agendaH", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/agendaH.html", String(), false, processor);
  });

  //  server.on("/get-message", HTTP_GET, [](AsyncWebServerRequest * request) {
  //    StaticJsonDocument<400> doc;
  //    JsonArray data = doc.createNestedArray("Radios");
  //    data.add(1);
  //    data.add(2);
  //
  //    JsonArray data1 = doc.createNestedArray("rssi");
  //    data1.add(-50);
  //    data1.add(-78);
  //
  //    JsonArray data2 = doc.createNestedArray("rota");
  //    data2.add("1>C");
  //    data2.add("2>1>C");
  //    String response;
  //    serializeJson(doc, response);
  //    request->send(200, "application/json", response);
  //  });

  //BOTAO PARA APAGAR AGENDAMENTOS

  server.on("/clearag", HTTP_GET, [](AsyncWebServerRequest * request) {
    AgFS.destroyFile();
    PosFS.destroyFile();
    ws.textAll("%");
    for (int i = 0; i < 4; i++) {
      horaag[i] = 0;
      atuaag[i] = " ";
    }
    Serial2.println("Delete ag OK");
    request->send(SPIFFS, "/agenda.html", String(), false, processor);
  });


  // Start server
  events.onConnect([](AsyncEventSourceClient * client) {
    if (client->lastId()) {
      Serial2.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();

  countFile();
  String reg = ObjFS.findRecord(registros - 1);
  int idc = reg.indexOf(':');
  String ids = reg.substring(0, idc);
  id = ids.toInt();

  LeEntrada();
}

// Exibe o espaço total, usado e disponível na memoria
void showAvailableSpace()
{
  Serial2.println("Space: " + String(ObjFS.getTotalSpace()) + " Bytes");
  Serial2.println("Used: " + String(ObjFS.getUsedSpace()) + " Bytes");

}

void loop()
{


  int iniTS = inicioH * 3600 + inicioM * 60;
  int fimTS = fimH * 3600 + fimM * 60;
  //int horaTS = horaH.toInt() * 3600 + horaM.toInt() * 60 + horaS.toInt();


  if (eco == "on" && (iniTS <= horaTS && horaTS < fimTS + 15)) {

    if (auxeco == 0) {
      //atuacao do modo economia aqui    \/
      //writeFile(SPIFFS, "/estadoanterior.bin", String(String(EstadoAtual[0]) + "-" +  String(EstadoAtual[1]) + "-" + String(EstadoAtual[2]) + "-" + String(perc)).c_str());
      //Serial2.println(readFile(SPIFFS, "/estadoanterior.bin"));
      if (TIPO == "NA" && (digitalRead(AVREAL) == LOW || digitalRead(RTREAL) == LOW)) {
        DesligaEco();
      }
      if (TIPO == "NF" && (digitalRead(AVREAL) == HIGH || digitalRead(RTREAL) == HIGH)) {
        DesligaEco();
      }
      //----------------------------------
      auxeco = 1;
      Serial2.println("MODO ECO");
    }

  } else {
    if (auxeco == 1) {
      Serial2.println("Sai do Eco");
      auxeco = 0;
      String estadoanterior = readFile(SPIFFS, "/estadoanterior.bin");
      //Serial2.println(estadoanterior);
      INWEB[0] = estadoanterior[0];
      INWEB[1] = estadoanterior[2];
      INWEB[2] = estadoanterior[4];
      numw = estadoanterior.substring(6, estadoanterior.indexOf('\n')).toInt();
      webflag = 1;
      //Serial2.println(numw);
      delay(2000);

    }

  }
  tatual = millis();


  //timerWrite(timer, 0);
  ArduinoOTA.handle();

  // RECEBE E TRATA O PACOTE LORA

  int packetSize = LoRa.parsePacket();
  if (packetSize) {

    while (LoRa.available()) {
      int LoRaAdressR = LoRa.read();
      //Serial2.println(LoRaAdressR);
      if (LoRaAdressR == LoRaAdress) {
        LoRaData = LoRa.readString();
        rssi = LoRa.packetRssi();
        int separa = LoRaData.indexOf('-');
        int fim = LoRaData.indexOf('#');
        if (separa >= 0) {
          angulo = LoRaData.substring(0, separa);
          hora = LoRaData.substring((separa + 1), (fim));
          if ((LoRaData.substring((LoRaData.indexOf('#') + 1), LoRaData.indexOf('\n')).toInt()) > 1631533539) {
            tempo = (LoRaData.substring((LoRaData.indexOf('#') + 1), LoRaData.indexOf('\n'))).toInt();
          }
        }
      } else {
        String msgerrada = LoRa.readString();
        //Serial2.println("MSG ERRADA");
      }
    }
  }

  if (TIPO == "NF") {
    if (digitalRead(AVREAL) == LOW && digitalRead(RTREAL) == LOW) {
      //      timerWrite(timer, 0);
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
  }

  if (TIPO == "NA") {
    if (digitalRead(AVREAL) == HIGH && digitalRead(RTREAL) == HIGH) {
      //      timerWrite(timer, 0);
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
  }

  if ( Serial2.available() > 0 || webflag == 1) { //se o Serial2.receber uma mensagem de 6 caracteres ou receber uma msg do WebServer
    int buffersize =  Serial2.available();
    //Serial2.println("COASda");

    delay(1000);
    //epoch = epoch +1;
    for (int i = 0; i < 50; i++) {
      if ( Serial2.available()) {
        stats[i] =  Serial2.read();
      } else {
        stats[i] = '#';
        i = 51;
      }
    }
    //Serial2.print(stats);
    if ((stats[0] == '3' || stats[0] == '4')  && webflag == 0) {
      Snum = String((stats[3] - '0') * 100 + (stats[4] - '0') * 10 + (stats[5] - '0'));
      num = Snum.toInt();
      aux2 = 0;
      buffersize = 6;
    }
    if (stats[2] == '2') {
      buffersize = 6;
    }
    if (webflag == 1) {
      stats[0] = INWEB[0];
      stats[1] = INWEB[1];
      stats[2] = INWEB[2];
    }


    //Serial2.println("OI");
    //Serial2.println(stats);
    if (buffersize == 6 || webflag == 1) {

      if (stats[0] == '0' && stats[1] == '0' && stats[2] == '0') {
        LeEntrada();
        EnviaStatus();
        for (int i = 0; i <= buffersize; i++) {
          stats[i] = ' ';
        }
      }

      if ( stats[0] != '0' || stats[1] != '0' || stats[2] != '0') {

        EnviaEstado();
        LeEntrada();
        EnviaStatus();
        for (int i = 0; i <= buffersize; i++) {
          stats[i] = ' ';
        }
      }
      webflag = 0;
      return;
    } else
    {
      if (stats[0] == '0' && stats[1] == '0' && stats[2] == '0') {
        LeEntrada();
        EnviaStatus();
      }

      //DESLIGA

      if (stats[0] == '0' && stats[1] == '0' && stats[2] == '2') {
        Desliga();
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
        for (int i = (busca); i < id; i++) {
          int busca = statss.toInt() - 1;
          String reg = ObjFS.findRecord(i);
          Serial2.println(reg);
          //delay(100);
        }
        return;
      }
      if (stats[0] == 'S' && stats[1] == 'T' && stats[2] == 'R') {
        showFile();
        Serial2.println("Historico:");
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
        showAvailableSpace();
        return;
      }

      //******************************************************************agendamento*****************************************************************************************

      if (stats[0] == 'A' && stats[1] == 'G') {
        Serial2.print(String(stats));
        values = String(stats).substring((String(stats).indexOf('>') + 1), (String(stats).indexOf('#')));

        while (values.length() < 30) {
          values += " ";
        }
        //Serial2.println(values);
        if (values != "" && !AgFS.writeFile(values, &errorMsg))
          Serial2.println(errorMsg);
        values = "";
        Agendamento();

      }
      if (stats[0] == 'S' && stats[1] == 'A') {
        showFile2();
        //        Serial2.print("Numero de Registros: ");
        //        Serial2.println(registros2);
        //        showAvailableSpace();
        Serial2.println("Agendamentos por horario:");
        Serial2.print(dados);
        return;
      }
      if (stats[0] == 'S' && stats[1] == 'P') {
        showFile3();
        Serial2.print("Numero de Registros: ");
        Serial2.println(registros3);
        showAvailableSpace();
        return;
      }
      if (stats[0] == 'A' && stats[1] == 'D' && stats[2] == 'E' && stats[3] == 'L') {
        AgFS.destroyFile();
        PosFS.destroyFile();
        Serial2.println("Delete ag OK");
        //id = 0;
        return;
      }
      /*  Lista de comandos:
           SF: exibe a lista inteira
           RN(numero): exibe a partir de um registro especifico
           LST: exibe o ultimo registro
           DEL: deleta a lista inteira
           ADEL: deleta os agendamentos
           STR: Salva a lista inteira em uma string "dados"
           RST: Reset via serial
           SA: exibe agendamentos por horario
           SP: exibe agendamentos por posição

           NENHUM COMANDO PODE TER 6 CARACTERES
      */
    }

  } else {
    Serial2.read();
    delay(10);
  }



  //Envia informações para pagina web via /events
  if ((millis() - lastTime) > timerDelay) {
    // epoch = epoch + 5;
    LeEntrada();
    //notifyClients();
    ws.cleanupClients();
//    String eventoEstado = String(String(sentido) + " " + String(seco) + " " + String(ligado));
//    events.send(String(sentido).c_str(), "sentido", millis());
//    events.send(String(seco).c_str(), "seco", millis());
//    events.send(String(ligado).c_str(), "ligado", millis());
    events.send(String(perc).c_str(), "STRperc", millis());
    Agendamento2();
    StaticJsonDocument<400> doc;
    JsonArray data = doc.createNestedArray("Atua");
    JsonArray data1 = doc.createNestedArray("Hora");
    JsonArray data2 = doc.createNestedArray("Percent");
    for (int i = 0; i < registros2; i++) {
      String intencao;
      if (atuaag2[i] == "351") {
        intencao = "Avanço Seco Ligado";
      }
      if (atuaag2[i] == "451") {
        intencao = "Reverso Seco Ligado";
      }
      if (atuaag2[i] == "361") {
        intencao = "Avanço Molhado Ligado";
      }
      if (atuaag2[i] == "461") {
        intencao = "Reverso Molhado Ligado";
      }
      if (atuaag2[i] == "002") { 
        intencao = "Desligado";
      }
      data.add(intencao);
      time_t rawtime = horaag2[i];
      struct tm  ts;
      char       buf[80];

      // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
      ts = *localtime(&rawtime);
      strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
      data1.add(buf);
      data2.add(percag[i]);
    }
    String JSONhora;
    serializeJson(doc, JSONhora);
    events.send(String(JSONhora).c_str(), "JSONhora", millis());

    AgendaPOS2();
    StaticJsonDocument<200> doc2;
    JsonArray datap = doc2.createNestedArray("Atua");
    JsonArray data1p = doc2.createNestedArray("POS");

    for (int i = 0; i < registros3; i++) {
      String intencao;
      if (atuaP2[i] == "RET") {
        intencao = "Retorno";
      }
      if (atuaP2[i] == "002") {
        intencao = "Desliga";
      }
      datap.add(intencao);
      data1p.add(pos2[i]);

    }
    String JSpos;
    serializeJson(doc2, JSpos);
    events.send(String(JSpos).c_str(), "JSpos", millis());


    events.send(String(angulo.toInt()).c_str(), "angulo", millis());
    //events.send(String(rssi).c_str(), "rssi", millis());
    lastTime = millis();
    //
    //    Serial2.println(iniTS);
    //    Serial2.println(fimTS);
    //    Serial2.println(horaTS);
  }
  endloop = millis() - tatual;
  Percentimetro();
  AtuaPercentimetro();
  AtuaAg();
  AtuaPOS();


}

//Função para leitura de arquivos de configuração

String readFile(fs::FS &fs, const char * path) {
  //Serial2.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    Serial2.println("- empty file or failed to open file");
    error = 1;
    return String();
  }
  //Serial2.println("- read from file:");
  String fileContent;
  while (file.available()) {
    fileContent += String((char)file.read());
  }
  file.close();
  error = 0;
  //Serial2.println(fileContent);
  return fileContent;
}

//Função para gravação de arquivos de configuração

void writeFile(fs::FS &fs, const char * path, const char * message) {
  //Serial2.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file) {
    Serial2.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    //Serial2.println("- file written");
  } else {
    Serial2.println("- write failed");
  }
  file.close();
}
