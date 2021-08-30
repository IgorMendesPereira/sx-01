void showFile()
{
  dados = "";
  int count = 0;
  String linha = "";
  registros = 0;
  // Exibe na Serial2.e no display o início do arquivo
  //Serial.println("# Begin of file 1 #");
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

  // Se existir mensagem de erro exibe na Serial2.e no display
  if (errorMsg != "")
  {
    //Serial.println(errorMsg);
  }

  // Exibe na Serial2.e no display o fim do arquivo
  //Serial.println("# End of file 1#");
  registros = count;
}

void showFile2()
{
  dados = "";
  int count = 0;
  String linha = "";
  registros2 = 0;
  // Exibe na Serial2.e no display o início do arquivo
  //Serial.println("# Begin of file 2 #");
  errorMsg = "";

  // Posiciona o ponteiro do arquivo no início
  AgFS.rewind();

  // Exibe todos os registros até o fim
  while (AgFS.readFileNextRecord(&linha, &errorMsg) && linha != "")
  {
    dados += linha;
    //Serial.println(linha);
    count++;

  }

  // Se existir mensagem de erro exibe na Serial2.e no display
  if (errorMsg != "")
  {
    //Serial.println(errorMsg);
  }

  // Exibe na Serial2.e no display o fim do arquivo
  //Serial.println("# End of file 2#");
  registros2 = count;
}


void showFile3()
{
  dados = "";
  int count = 0;
  String linha = "";
  registros3 = 0;
  // Exibe na Serial2.e no display o início do arquivo
  Serial.println("# Begin of file 3 #");
  errorMsg = "";

  // Posiciona o ponteiro do arquivo no início
  PosFS.rewind();

  // Exibe todos os registros até o fim
  while (PosFS.readFileNextRecord(&linha, &errorMsg) && linha != "")
  {
    dados += linha;
    Serial.println(linha);
    count++;

  }

  // Se existir mensagem de erro exibe na Serial2.e no display
  if (errorMsg != "")
  {
    Serial.println(errorMsg);
  }

  // Exibe na Serial2.e no display o fim do arquivo
  Serial.println("# End of file 3#");
  registros3 = count;
}
