void EnviaStatus() {

  Serial.print(EstadoAtual[0]);
  Serial.print("-");
  Serial.print(EstadoAtual[1]);
  Serial.print("-");
  Serial.print(EstadoAtual[2]);
  Serial.print("-");
  Serial.print(perc);
  Serial.print("-");
  int anguloint = angulo.toInt();
  Serial.print(anguloint);
  Serial.print("-");
  Serial.print(tempo);
  Serial.print("#");

  EstadoAtual[0] = '0';
  EstadoAtual[1] = '0';
  EstadoAtual[2] = '0';

}
