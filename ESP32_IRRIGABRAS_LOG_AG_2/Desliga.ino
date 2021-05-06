void Desliga() {
  digitalWrite(DESLIGA, LOW);
  digitalWrite(RAUX, HIGH);
  digitalWrite(LIGA, HIGH);
  digitalWrite(AVANCO, HIGH);
  digitalWrite(REVERSO, HIGH);
  digitalWrite(MOLHADO, HIGH);
  digitalWrite(RAUXP, HIGH);
  digitalWrite(PERCAT, HIGH);
  delay(1000);
  digitalWrite(DESLIGA, HIGH);
  perc = 0;
  auxP = 0;
  num = 0;
  numw = 0;
  aux2 = 0;
}
