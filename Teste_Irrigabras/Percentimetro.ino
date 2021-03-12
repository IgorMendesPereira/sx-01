void Percentimetro() {
  if (digitalRead(PERC) == LOW && auxP == 0) {
    while (auxP == 0) {
      t2 = millis();
      delay(100);
      auxP = 1;
    }

  } else if (auxP == 1 && digitalRead(PERC) == HIGH) {
    t3 = millis();
    perc = (t3 - t2) / 600;
    if (perc >= 100) {
      perc = 99;
    }
    delay(100);
    auxP = 0;
  } else if ((auxP == 1) && (tatual >= (t2 + 60000)) && (digitalRead(PERC) == LOW)) {
    perc = 100;
    auxP = 0;
  }
  STRperc = String(perc);
}
