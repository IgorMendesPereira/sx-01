void EnviaStatus() {

  Serial2.print(EstadoAtual[0]);                  //imprime a saida "3-5-1-"
  Serial2.print("-");
  Serial2.print(EstadoAtual[1]);
  Serial2.print("-");
  Serial2.print(EstadoAtual[2]);
  Serial2.print("-");
  Serial2.print(perc);
  // Serial2.print("-");


  //Serial2.print(disp);
  EstadoAtual[0] = '0';
  EstadoAtual[1] = '0';
  EstadoAtual[2] = '0';
  //  ESP.wdtFeed();//Alimenta o Watchdog.

}
