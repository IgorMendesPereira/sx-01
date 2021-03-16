void Desliga() {
  digitalWrite(DESLIGA, LOW);
  digitalWrite(RAUX, HIGH);
  digitalWrite(LIGA, HIGH);
  digitalWrite(AVANCO, HIGH);
  digitalWrite(REVERSO, HIGH);
  digitalWrite(MOLHADO, HIGH);
  digitalWrite(RAUXP, HIGH);
  digitalWrite(PERCAT, HIGH);
  //  digitalWrite(SECO,HIGH);
  delay(4000);
  perc = 0;
  auxP = 0;
  num = 0;
  numw = 0;
  aux2 = 0;
  digitalWrite(DESLIGA, HIGH);
}
