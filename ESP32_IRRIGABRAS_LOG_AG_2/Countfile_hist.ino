void countFile() {
  dados = "";
  int count = 0;
  String linha = "";
  registros = 0;
  // Exibe na serial e no display o início do arquivo
  //Serial.println("# Begin of file #");
  errorMsg = "";

  // Posiciona o ponteiro do arquivo no início
  ObjFS.rewind();

  // Exibe todos os registros até o fim
  while (ObjFS.readFileNextRecord(&linha, &errorMsg) && linha != "")
  {
    dados += linha;
    //Serial.println(linha);
    count++;

  }

  // Se existir mensagem de erro exibe na serial e no display
  if (errorMsg != "")
  {
    Serial.println(errorMsg);
  }

  // Exibe na serial e no display o fim do arquivo
  //Serial.println("# End of file #");
  registros = count;
}
