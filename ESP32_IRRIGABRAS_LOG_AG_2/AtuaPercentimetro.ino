void AtuaPercentimetro() {

  if (num == 0) {
    aux2 = 0;
    t4 = 0;
    t5 = 0;
    t6 = 0;
    digitalWrite(RAUXP, HIGH);
    digitalWrite(PERCAT, HIGH);
  }
  if (num > 0 && aux2 == 0) {
    digitalWrite(RAUXP, LOW);
    digitalWrite(PERCAT, LOW);
    t4 = millis();
    t5 = t4 + num * 600; //ton
    t6 = t4 + 60000;
    aux2 = 1;

  }

  if (tatual >= t5 && aux2 == 1) {
    digitalWrite(PERCAT, HIGH);
    aux2 = 2;

  }
  if (tatual >= t6 && aux2 == 2) {
    aux2 = 0;

  }

}
