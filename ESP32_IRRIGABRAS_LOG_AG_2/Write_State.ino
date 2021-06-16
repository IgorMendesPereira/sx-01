//Escreve na memoria o historico
void WriteState() {
  id++;
  String SaidaMEM = String(String(EstadoAtual[0]) + "-" + String(EstadoAtual[1]) + "-" + String(EstadoAtual[2]) + "-" + String(perc) + "-" + angulo + "-" + epoch + "#");
  SaidaMEM = SaidaMEM.substring(0, SaidaMEM.indexOf('#'));
  values = String(String(id) + ":" + SaidaMEM);
  while (values.length() < 30) {
    values += " ";
  }
  //Serial2.println(values);
  if (values != "" && !ObjFS.writeFile(values, &errorMsg))
    Serial2.println(errorMsg);

  //showFile();
  values = "";
}
