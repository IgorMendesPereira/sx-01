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
    //    events.send(sentido.c_str(), "sentido", millis());
    //    events.send(String(seco).c_str(), "seco", millis());
    //    events.send(String(ligado).c_str(), "ligado", millis());
    //    events.send(String(perc).c_str(), "STRperc", millis());
    //notifyClients(String(String(EstadoAtual[0]) + "-" +  String(EstadoAtual[1]) + "-" + String(EstadoAtual[2]) + "-" + String(num)));
    String EstadoWS ="#";
//    Serial2.println(EstadoAtual[0]);
//    Serial2.println(EstadoAtual[1]);
//    Serial2.println(EstadoAtual[2]);
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

    //Serial2.println(EstadoWS);
    notifyClients(EstadoWS);
    WriteState();
    writeFile(SPIFFS, "/estadoanteriorAUTO.bin", String(String(EstadoAtual[0]) + "-" +  String(EstadoAtual[1]) + "-" + String(EstadoAtual[2]) + "-" + String(num)).c_str());
    flagmem = 0;
  } else {
    EstadoAnterior[0] = EstadoAtual[0];
    EstadoAnterior[1] = EstadoAtual[1];
    EstadoAnterior[2] = EstadoAtual[2];
    flagmem = 1;
  }
}
