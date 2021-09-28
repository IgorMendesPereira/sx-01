void EnviaStatus() {

  Serial.print(EstadoAtual[0]);
  Serial.print("-");
  Serial.print(EstadoAtual[1]);
  Serial.print("-");
  Serial.print(EstadoAtual[2]);
  Serial.print("-");
  Serial.print(perc);
  Serial.print("-");
  int anguloint = angulo.toInt();
  Serial.print(anguloint);
  Serial.print("-");
  Serial.print(tempo);
  Serial.println("#");
  //String time = modem.getGSMDateTime(DATE_FULL);
//  int   year3    = 0;
//  int   month3   = 0;
//  int   day3     = 0;
//  int   hour3    = 0;
//  int   min3     = 0;
//  int   sec3     = 0;
//  float timezone = 0;
//  modem.getNetworkTime(&year3, &month3, &day3, &hour3, &min3, &sec3, &timezone);
//  Serial.print("Current Network Time:");
//  Serial.print(year3);
//  Serial.print("/");
// Serial.print(month3);
//  Serial.print("/");
//  Serial.print(day3);
//  Serial.print("-");
//  Serial.print(hour3);
//  Serial.print(":");32764282
//  Serial.print(min3);
//  Serial.print(":");
//  Serial.print(sec3);

  if (PublishFlag == 0) {
    StaticJsonDocument<200> doc;
    doc["type"] = "status_pivos";
    doc["pivo_id"] = PIVO_ID;
    doc["on_off"] = String(EstadoAtual[2]);
    doc["front_back"] = String(EstadoAtual[0]);
    doc["water"] = String(EstadoAtual[1]);
    doc["percent"] = perc;
    doc["angle"] = anguloint;
    doc["timestamp"] = tempo;
    doc["fail"] = 0;
    char out[200];
    int b = serializeJson(doc, out);
//    Serial.print("bytes = ");
//    Serial.println(b, DEC);
    mqtt.publish("ESP/OUT", out);
   
    PublishFlag = 1;
  }
  EstadoAtual[0] = '0';
  EstadoAtual[1] = '0';
  EstadoAtual[2] = '0';

}
