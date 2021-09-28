void LeEntrada() {

  if (TIPO == "NF") {

    if (digitalRead(PRESS) == HIGH) {
      EstadoAtual[1] = '5';
      seco = "Seco";
    }
    if (digitalRead(PRESS) == LOW) {
      EstadoAtual[1] = '6';
      seco = "Molhado";

    }
    if (digitalRead(AVREAL) == LOW && digitalRead(RTREAL) == LOW) { //CONTATORA NC >>>> AQ LOW E RESTO HIGH
      EstadoAtual[2] = '2';
      EstadoAtual[0] = '0';
      ligado = "Desligado";
      sentido = "";
      seco = "";
      //return;
    }
    if (digitalRead(AVREAL) == HIGH) {
      EstadoAtual[0] = '3';
      sentido = "Avanco";
    }
    if (digitalRead(RTREAL) == HIGH) {
      EstadoAtual[0] = '4';
      sentido = "Reverso";
    }

    if (digitalRead(AVREAL) == HIGH || digitalRead(RTREAL) == HIGH) {
      EstadoAtual[2] = '1';
      ligado = "Ligado";
    }
  }

  //---------------------------------------------------------------------------- CONTATORA NA (defaut) -----------------------------------------------------------------------------------------

  else {
    if (digitalRead(PRESS) == HIGH) {
      EstadoAtual[1] = '5';
      seco = "Seco";
    }
    if (digitalRead(PRESS) == LOW) {
      EstadoAtual[1] = '6';
      seco = "Molhado";

    }
    if (digitalRead(AVREAL) == HIGH && digitalRead(RTREAL) == HIGH) { //CONTATORA NC >>>> AQ LOW E RESTO HIGH
      EstadoAtual[2] = '2';
      EstadoAtual[0] = '0';
      ligado = "Desligado";
      sentido = "";
      seco = "";
      //return;
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
  if ((EstadoAnterior[0] != EstadoAtual[0] || EstadoAnterior[1] != EstadoAtual[1] || EstadoAnterior[2] != EstadoAtual[2]) && (flagmem != 0)) {

    String EstadoWS = "#";
    if (EstadoAtual[0] == '3' || EstadoAtual[0] == '4') {
      if (EstadoAtual[0] == '3') {
        EstadoWS += "Avanco ";
      }
      if (EstadoAtual[0] == '4') {
        EstadoWS += "Reverso ";
      }

    }
    if (EstadoAtual[1] == '5' || EstadoAtual[1] == '6') {
      if (EstadoAtual[1] == '5') {
        EstadoWS += "Seco ";
      }
      if (EstadoAtual[1] == '6') {
        EstadoWS += "Molhado ";
      }

    }
    if (EstadoAtual[2] == '1' || EstadoAtual[2] == '2') {
      if (EstadoAtual[2] == '1') {
        EstadoWS += "Ligado ";
      }
      if (EstadoAtual[2] == '2') {
        EstadoWS = "#Desligado ";
      }
    }

    notifyClients(EstadoWS);
    WriteState();
    writeFile(SPIFFS, "/estadoanteriorAUTO.bin", String(String(EstadoAtual[0]) + "-" +  String(EstadoAtual[1]) + "-" + String(EstadoAtual[2]) + "-" + String(num)).c_str());
    StaticJsonDocument<200> doc;
    doc["type"] = "status_pivos";
    doc["pivo_id"] = PIVO_ID;
    doc["on_off"] = String(EstadoAtual[2]);
    doc["front_back"] = String(EstadoAtual[0]);
    doc["water"] = String(EstadoAtual[1]);
    doc["percent"] = perc;
    doc["angle"] = angulo.toInt();
    doc["timestamp"] = tempo;
    doc["fail"] = 0;
    char out[200];
    int b = serializeJson(doc, out);
//    Serial.print("bytes = ");
//    Serial.println(b, DEC);
    mqtt.publish("ESP/OUT", out);
    //mqtt.publish("ESP/OUT", String(String(chipId) + "/" +  String(EstadoAtual[0]) + "-" +  String(EstadoAtual[1]) + "-" + String(EstadoAtual[2]) + "-" + String(num) + "-" + String(angulo.toInt()) + "-" + String(tempo)).c_str());
    flagmem = 0;
    PublishFlag = 1;
  } else {
    EstadoAnterior[0] = EstadoAtual[0];
    EstadoAnterior[1] = EstadoAtual[1];
    EstadoAnterior[2] = EstadoAtual[2];
    flagmem = 1;
    PublishFlag = 0;
  }
}
