#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>

#include <time.h>
#include <WiFi.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

String readFile(fs::FS &fs, const char * path) {
  //Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    Serial.println("- empty file or failed to open file");
    return String();
  }
  //Serial.println("- read from file:");
  String fileContent;
  while (file.available()) {
    fileContent += String((char)file.read());
  }
  file.close();
  //Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  //Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    //Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}


const char* ssid = "Poligamia";
const char* password = "0034731858";

AsyncWebServer server(80);

const char* PARAM_LAT = "inputLat";
const char* PARAM_LON = "inputLon";
const char* PARAM_OFFSET = "inputOffset";
const char* PARAM_SW = "inputSW";

String processor(const String& var) {
  //Serial.println(var);
  if (var == "inputLat") {
    return readFile(SPIFFS, "/inputLat.txt");
  }
  else if (var == "inputLon") {
    return readFile(SPIFFS, "/inputLon.txt");
  }
  else if (var == "inputOffset") {
    return readFile(SPIFFS, "/inputOffset.txt");
  } else if (var == "SW") {
    return readFile(SPIFFS, "/sw.txt");
  }
  return String();
}

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;


TinyGPSPlus gps;
HardwareSerial GPS(1);

double LATC = 0;
//-22.255920;
double LONC = 0;
//-45.695277;

double angulo2;

double LAT;
double LON;
String LATs;
String LONs;
String syncword;

double VALx;
double VALy;
String Saida;

int  iano;
int imes;
int  idia;
int  ihora;
int  iminuto;
int  isegundo;

String seg;
String minuto;
String horas;

int comeca = 0;

RTC_DATA_ATTR int hora;

RTC_DATA_ATTR double angulo;
String ang;

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 915E6
#define txPower 20
#define spreadingFactor 12


void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}
//
//int unix() {
//
//  long cyear = ((gps.date.year()) - 1970) * 31556926; // Year - 1900
//  long cmon = (gps.date.month() - 1 ) * 2629743;         // Month, where 0 = jan
//  long cday = gps.date.day() * 86400;        // Day of the month
//  long chour = hora * 3600;
//  long cmin = gps.time.minute() * 60;
//  long csec = gps.time.second();
//  long t_of_day = cyear + cmon + cday + chour + cmin + csec;
//
//  return (t_of_day - 2762484);
//}
//

int TESTE() {
    struct tm t;
    time_t t_of_day;

    t.tm_year = gps.date.year()-1900;  // Year - 1900
    t.tm_mon = (gps.date.month() - 1 );           // Month, where 0 = jan
    t.tm_mday = gps.date.day();          // Day of the month
    t.tm_hour = hora;
    t.tm_min = gps.time.minute();
    t.tm_sec = gps.time.second();
    t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
    t_of_day = mktime(&t);

    //Serial.println(t_of_day);
    return t_of_day;
}



void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Sender Test");
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //LoRa.setSyncWord(0x96);
  //  LoRa.setSpreadingFactor(6);
  Serial.println("LoRa Initializing OK!");
  //  Wire.begin(21, 22);
  //  if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) {
  //    Serial.println("AXP192 Begin PASS");
  //  } else {
  //    Serial.println("AXP192 Begin FAIL");
  //  }
  //  axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
  //  axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
  //  axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
  //  axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
  //  axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
  GPS.begin(9600, SERIAL_8N1, 12, 15);   //17-TX 18-RX
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  //Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
    if (request->hasParam(PARAM_LAT)) {
      inputMessage = request->getParam(PARAM_LAT)->value();
      writeFile(SPIFFS, "/inputLat.txt", inputMessage.c_str());
    }
    // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(PARAM_LON)) {
      inputMessage = request->getParam(PARAM_LON)->value();
      writeFile(SPIFFS, "/inputLon.txt", inputMessage.c_str());
    }
    // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
    else if (request->hasParam(PARAM_OFFSET)) {
      inputMessage = request->getParam(PARAM_OFFSET)->value();
      writeFile(SPIFFS, "/inputOffset.txt", inputMessage.c_str());
    }
    else {
      inputMessage = "No message sent";
    }
    if (request->hasParam(PARAM_SW)) {

      syncword = request->getParam(PARAM_SW)->value();
      writeFile(SPIFFS, "/sw.txt", syncword.c_str());
      //LoRa.setSyncWord(syncword.toInt());
      Serial.println(byte(syncword.toInt()));
    }
    Serial.println(inputMessage);
  });
  server.begin();
  LATC = readFile(SPIFFS, "/inputLat.txt").toDouble();
  LONC = readFile(SPIFFS, "/inputLon.txt").toDouble();
  syncword = readFile(SPIFFS, "/sw.txt").toInt();
  //LoRa.setSyncWord(syncword.toInt());
  Serial.println(syncword);
}

void loop() {
  bootCount++;
  LAT = gps.location.lat();
  LON = gps.location.lng();
  LATC = readFile(SPIFFS, "/inputLat.txt").toDouble();
  LONC = readFile(SPIFFS, "/inputLon.txt").toDouble();
  //LoRa.setSyncWord(syncword.toInt());
  LATs = String(LAT);
  LONs = String(LON);
  Angulo();

  
  horas = gps.time.hour();
  hora = horas.toInt() - 3;
  if (hora < 0) {
    if (hora == -3) {
      hora = 0;
    }
    if (hora == -2) {
      hora = 1;
    }
    if (hora == -1) {
      hora = 2;
    }
  }
  horas = String(hora);
  minuto = gps.time.minute();
  seg = gps.time.second();

  
  Serial.println(gps.satellites.value());
  //TESTE();
  if (comeca == 1) {
    Saida = String(ang + "-" + horas + ":" + minuto + ":" + seg + "#" + TESTE());
  Serial.println(Saida);
    LoRa.beginPacket();
    LoRa.write(byte(syncword.toInt()));
    LoRa.print(Saida);
    LoRa.print("#");
    LoRa.endPacket();
  }

  smartDelay(1000);
  if (bootCount >= 5) {
    bootCount = 0;
    //Serial.println("to indo");
    Serial.flush();
    //esp_deep_sleep_start();
  }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPS.available())
      gps.encode(GPS.read());
  } while (millis() - start < ms);
  comeca = 1;
}


void Angulo() {
  VALx = LON - LONC;
  VALy = LAT - LATC;
  double offset = (readFile(SPIFFS, "/inputOffset.txt").toInt());
  if (VALx > 0 && VALy > 0) {
    angulo = atan(VALy / VALx);
    angulo = (angulo * 180) / PI;
    angulo = angulo + offset;
    if (angulo >= 360) {
      angulo = angulo - 360;
    }
    ang = String(angulo);
  }
  if (VALx < 0 && VALy > 0) {
    angulo = PI + atan(VALy / VALx);
    angulo = (angulo * 180) / PI;
    angulo = angulo + offset;
    if (angulo >= 360) {
      angulo = angulo - 360;
    }
    ang = String(angulo);
  }
  if (VALx < 0 && VALy < 0) {
    angulo = PI + atan(VALy / VALx);
    angulo = (angulo * 180) / PI;
    angulo = angulo + offset;
    if (angulo >= 360) {
      angulo = angulo - 360;
    }
    ang = String(angulo);
  }
  if (VALx > 0 && VALy < 0) {
    angulo = TWO_PI + atan(VALy / VALx);
    angulo = (angulo * 180) / PI;
    angulo = angulo + offset;
    if (angulo >= 360) {
      angulo = angulo - 360;
    }
    ang = String(angulo);
  }

}
