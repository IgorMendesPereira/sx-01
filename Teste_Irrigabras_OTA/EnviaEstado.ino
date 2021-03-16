void EnviaEstado() {   //ok
  if (stats[0] == '0' && stats[1] == '0' && stats[2] == '2') {
    digitalWrite(AVANCO, HIGH);
    digitalWrite(REVERSO, HIGH);
    Desliga();
  } else {
    if (stats[0] == '3') {
      digitalWrite(AVANCO, LOW);
      digitalWrite(REVERSO, HIGH);
    }
    if (stats[0] == '4') {
      digitalWrite(AVANCO, HIGH);
      digitalWrite(REVERSO, LOW);
    }

    if (stats[1] == '5') {
      // digitalWrite(SECO, LOW);
      digitalWrite(MOLHADO, HIGH);
    }
    if (stats[1] == '6') {
      digitalWrite(MOLHADO, LOW);
      // digitalWrite(SECO, HIGH);
    } else {
      //  digitalWrite(SECO, HIGH);
      digitalWrite(MOLHADO, HIGH);
    }
    if (stats[2] == '1') {
      
      num = (stats[3] - '0') * 10 + (stats[4] - '0');
      if(webflag == 1 && num!= numw){
      num = numw;
      }else{
        if(stats[3] == '1' && stats[4] == '0' && stats[5] == '0'){
          
        }else{
        num = (stats[3] - '0') * 10 + (stats[4] - '0');
        }
      }
      digitalWrite(LIGA, LOW);
      digitalWrite(RAUX, LOW);
      digitalWrite(DESLIGA, HIGH);
      delay(4000);
      digitalWrite(LIGA, HIGH);
      
    }
    else {
      digitalWrite(LIGA, HIGH);
      digitalWrite(DESLIGA, HIGH);
    }
    delay(500);
  }
}
