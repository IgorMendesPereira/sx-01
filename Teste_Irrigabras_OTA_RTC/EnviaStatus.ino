void EnviaStatus() {

  Serial.print(EstadoAtual[0]);                  //imprime a saida "3-5-1-"
  Serial.print("-");
  Serial.print(EstadoAtual[1]);
  Serial.print("-");
  Serial.print(EstadoAtual[2]);
  Serial.print("-");
  Serial.print(perc);
  Serial.print("-");
  Serial.print(angulo);
  Serial.print("-");
  Serial.print(hora);
//  Serial.print(hora);

  //Serial.print(disp);
  EstadoAtual[0] = '0';
  EstadoAtual[1] = '0';
  EstadoAtual[2] = '0';
  //  ESP.wdtFeed();//Alimenta o Watchdog.

}
