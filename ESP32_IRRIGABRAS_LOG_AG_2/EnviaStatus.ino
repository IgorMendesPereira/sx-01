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
  Serial.print(epoch);

  EstadoAtual[0] = '0';
  EstadoAtual[1] = '0';
  EstadoAtual[2] = '0';
//   digitalWrite(16, HIGH);
//  delay(50);
//  digitalWrite(16, LOW);

}
