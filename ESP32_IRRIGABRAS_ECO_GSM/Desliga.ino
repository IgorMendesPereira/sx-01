void Desliga() {
  digitalWrite(DESLIGA, LOW);
  digitalWrite(RAUX, HIGH);
  digitalWrite(LIGA, HIGH);
  digitalWrite(AVANCO, HIGH);
  digitalWrite(REVERSO, HIGH);
  digitalWrite(MOLHADO, HIGH);
  digitalWrite(RAUXP, HIGH);
  digitalWrite(PERCAT, HIGH);
  delay(espera);
  //epoch = epoch +espera/1000;
  digitalWrite(DESLIGA, HIGH);
  perc = 0;
  auxP = 0;
  num = 0;
  numw = 0;
  aux2 = 0;
  writeFile(SPIFFS, "/estadoanterior.bin", String(String(0) + "-" +  String(0) + "-" + String(0) + "-" + String(perc)).c_str());
}

void DesligaEco() {
  writeFile(SPIFFS, "/estadoanterior.bin", String(String(EstadoAtual[0]) + "-" +  String(EstadoAtual[1]) + "-" + String(EstadoAtual[2]) + "-" + String(perc)).c_str());
  digitalWrite(DESLIGA, LOW);
  digitalWrite(RAUX, HIGH);
  digitalWrite(LIGA, HIGH);
  digitalWrite(AVANCO, HIGH);
  digitalWrite(REVERSO, HIGH);
  digitalWrite(MOLHADO, HIGH);
  digitalWrite(RAUXP, HIGH);
  digitalWrite(PERCAT, HIGH);
  delay(espera);
  //epoch = epoch +espera/1000;
  digitalWrite(DESLIGA, HIGH);
  perc = 0;
  auxP = 0;
  num = 0;
  numw = 0;
  aux2 = 0;
  }
