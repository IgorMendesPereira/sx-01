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
    //Serial.println("mudou");
    WriteState();
    flagmem = 0;
  } else {
    EstadoAnterior[0] = EstadoAtual[0];
    EstadoAnterior[1] = EstadoAtual[1];
    EstadoAnterior[2] = EstadoAtual[2];
    flagmem = 1;
  }
}
