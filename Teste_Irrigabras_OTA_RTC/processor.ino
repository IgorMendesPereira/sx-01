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
    if (seco == "Seco") {
      secow = "Seco";
    } else if (seco == "Molhado") {
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
  if (var == "PERC") {
    return STRperc;
  }
  if (var == "HORA") {
    return String(hora);
  }
  return String(); 
}
