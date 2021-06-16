//Conta a quatidade de registros no historico

void countFile() {
  dados = "";
  int count = 0;
  String linha = "";
  registros = 0;
  errorMsg = "";

  // Posiciona o ponteiro do arquivo no início
  ObjFS.rewind();

  // Exibe todos os registros até o fim
  while (ObjFS.readFileNextRecord(&linha, &errorMsg) && linha != "")
  {
    dados += linha;
    //Serial2.println(linha);
    count++;

  }

  // Se existir mensagem de erro exibe na Serial2. e no display
  if (errorMsg != "")
  {
    Serial2.println(errorMsg);
  }

  // Exibe na Serial2. e no display o fim do arquivo
  //Serial2.println("# End of file #");
  registros = count;
}
