void EnviaEstado() {   //ok
  if (stats[0] == '3' && stats[1] == '5' && stats[2] == '1') {
    digitalWrite(LIGA, LOW);
    digitalWrite(RAUX, LOW);
    digitalWrite(AVANCO, LOW);
    digitalWrite(REVERSO, HIGH);
    digitalWrite(MOLHADO, HIGH);
    delay(4000);
    epoch = epoch +4; 
    digitalWrite(LIGA, HIGH);

  }
  if (stats[0] == '4' && stats[1] == '5' && stats[2] == '1') {
    digitalWrite(LIGA, LOW);
    digitalWrite(RAUX, LOW);
    digitalWrite(REVERSO, LOW);
    digitalWrite(AVANCO, HIGH);
    digitalWrite(MOLHADO, HIGH);
    delay(4000);
    epoch = epoch +4;
    digitalWrite(LIGA, HIGH);
  }
  if (stats[0] == '3' && stats[1] == '6' && stats[2] == '1') {
    digitalWrite(LIGA, LOW);
    digitalWrite(RAUX, LOW);
    digitalWrite(AVANCO, LOW);
    digitalWrite(REVERSO, HIGH);
    digitalWrite(MOLHADO, LOW);
    delay(4000);
    epoch = epoch +4;
    digitalWrite(LIGA, HIGH);
  }
  if (stats[0] == '4' && stats[1] == '6' && stats[2] == '1') {
    digitalWrite(LIGA, LOW);
    digitalWrite(RAUX, LOW);
    digitalWrite(REVERSO, LOW);
    digitalWrite(AVANCO, HIGH);
    digitalWrite(MOLHADO, LOW);
    delay(4000);
    epoch = epoch +4;
    digitalWrite(LIGA, HIGH);
  }
  if (stats[0] == '0' && stats[1] == '0' && stats[2] == '2') {
    digitalWrite(DESLIGA, LOW);
    digitalWrite(RAUX, HIGH);
    digitalWrite(LIGA, HIGH);
    digitalWrite(AVANCO, HIGH);
    digitalWrite(REVERSO, HIGH);
    digitalWrite(MOLHADO, HIGH);
    digitalWrite(RAUXP, HIGH);
    digitalWrite(PERCAT, HIGH);
    delay(4000);
    epoch = epoch +4;
    digitalWrite(DESLIGA, HIGH);
    perc = 0;
    auxP = 0;
    num = 0;
    numw = 0;
    aux2 = 0;
  }


  if (webflag == 1 && num != numw) {
    num = numw;
  }
  INWEB[0]=' ';
  INWEB[1]=' ';
  INWEB[2]=' ';
}