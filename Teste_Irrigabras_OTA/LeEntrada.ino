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
