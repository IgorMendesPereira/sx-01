void EnviaEstado() {   //ok
  
  
  if (stats[0] == '3' && stats[1] == '5' && stats[2] == '1') {
    digitalWrite(LIGA, LOW);
    digitalWrite(RAUX, LOW);
    digitalWrite(AVANCO, LOW);
    digitalWrite(REVERSO, HIGH);
    digitalWrite(MOLHADO, HIGH);
    delay(espera);
    //epoch = epoch +espera/1000; 
    digitalWrite(LIGA, HIGH);

  }
  if (stats[0] == '4' && stats[1] == '5' && stats[2] == '1') {
    digitalWrite(LIGA, LOW);
    digitalWrite(RAUX, LOW);
    digitalWrite(REVERSO, LOW);
    digitalWrite(AVANCO, HIGH);
    digitalWrite(MOLHADO, HIGH);
    delay(espera);
    //epoch = epoch +espera/1000;
    digitalWrite(LIGA, HIGH);
  }
  if (stats[0] == '3' && stats[1] == '6' && stats[2] == '1') {
    digitalWrite(LIGA, LOW);
    digitalWrite(RAUX, LOW);
    digitalWrite(AVANCO, LOW);
    digitalWrite(REVERSO, HIGH);
    digitalWrite(MOLHADO, LOW);
    delay(espera);
    //epoch = epoch +espera/1000;
    digitalWrite(LIGA, HIGH);
  }
  if (stats[0] == '4' && stats[1] == '6' && stats[2] == '1') {
    digitalWrite(LIGA, LOW);
    digitalWrite(RAUX, LOW);
    digitalWrite(REVERSO, LOW);
    digitalWrite(AVANCO, HIGH);
    digitalWrite(MOLHADO, LOW);
    delay(espera);
    //epoch = epoch +espera/1000;
    digitalWrite(LIGA, HIGH);
  }
  if (stats[0] == '0' && stats[1] == '0' && stats[2] == '2') {
    Desliga();
  }


  if (webflag == 1 && num != numw) {
    num = numw;
    aux2 = 0;
  }
  INWEB[0]=' ';
  INWEB[1]=' ';
  INWEB[2]=' ';
}
