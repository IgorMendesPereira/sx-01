void EnviaStatus() {

  Serial2.print(EstadoAtual[0]);
  Serial2.print("-");
  Serial2.print(EstadoAtual[1]);
  Serial2.print("-");
  Serial2.print(EstadoAtual[2]);
  Serial2.print("-");
  Serial2.print(perc);
  Serial2.print("-");
  int anguloint = angulo.toInt();
  Serial2.print(anguloint);
  Serial2.print("-");
  Serial2.print(tempo);
  Serial2.print("#");

  EstadoAtual[0] = '0';
  EstadoAtual[1] = '0';
  EstadoAtual[2] = '0';

}
