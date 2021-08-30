// Importamos o header referente a lib FS_File_Record
#include "FS_File_Record2.h"

String fileName2; // Nome do arquivo
int sizeOfRecord2 = 25; // Default 3 (somando \r\n o tamanho será 5)
File pFile2; // Ponteiro do arquivo

// Construtor que seta somente o nome do arquivo, deixando o tamanho de registro default 3
FS_File_Record2::FS_File_Record2(String _fileName2)
{
  fileName2 = _fileName2;
}

// Construtor que seta nome do arquivo e tamanho de registro +2 (\r\n)
FS_File_Record2::FS_File_Record2(String _fileName2, int _sizeOfRecord2)
{
  fileName2 = _fileName2;
  sizeOfRecord2 = _sizeOfRecord2 + 2;
}

// Inicializa SPIFFS
bool FS_File_Record2::init()
{
  return SPIFFS.begin(true);
}

// Posiciona o ponteiro do arquivo no início
bool FS_File_Record2::rewind()
{
  if (pFile2)
    return pFile2.seek(0);

  return false;
}

// Lê a próxima linha do arquivo
bool FS_File_Record2::readFileNextRecord(String *line, String *errorMsg)
{
  *errorMsg = "";
  *line = "";
  // Se o ponteiro estiver nulo
  if (!pFile2)
  {
    // Abre arquivo para leitura
    pFile2 = SPIFFS.open(fileName2.c_str(), FILE_READ);

    // Se aconteceu algum erro
    if (!pFile2)
    {
      // Guarda msg de erro
      *errorMsg = "Failed to open the file";
      // Retorna falso
      return false;
    }
  }

  // Se for possível ler o arquivo
  if (pFile2.available())
  {
    // Lê arquivo
    *line = pFile2.readStringUntil('\n');
    // Retorna true
    return true;
  }

  // Se o arquivo estiver vazio retorna true
  if (pFile2.size() <= 0)
    return true;

  // Se não for possível ler o arquivo retorna falso
  return false;
}

//Posiciona ponteiro do arquivo na posição "pos"
bool FS_File_Record2::seekFile(int pos)
{
  // Se o ponteiro estiver nulo
  if (pFile2)
    pFile2.close();

  pFile2 = SPIFFS.open(fileName2.c_str(), FILE_READ); // Abre o arquivo para leitura

  // Posiciona o ponteiro na posição multiplicando pelo tamanho do registro
  return pFile2.seek(sizeOfRecord2 * pos);
}

// Escreve no arquivo
bool FS_File_Record2::writeFile(String line, String *errorMsg)
{
  if (pFile2)
    pFile2.close();

  pFile2 = SPIFFS.open(fileName2.c_str(), FILE_APPEND);

  // Se foi possível abrir
  if (pFile2)
  {
    // Escreve registro
    pFile2.println(line);
    // Fecha arquivo
    pFile2.close();
    // Retorna true
    return true;
  }

  // Se não foi possível abrir guarda mensagem de erro e retorna false
  *errorMsg = "Failed to open the file: " + String(fileName2);
  return false;
}

// Lê o último registro do arquivo
bool FS_File_Record2::readFileLastRecord(String *line, String *errorMsg)
{
  // Variável que guardará o tamanho do arquivo
  int sizeArq;

  // Limpa string
  *errorMsg = "";

  // Se o arquivo está aberto, fecha
  if (pFile2)
    pFile2.close();

  // Abre o arquivo para leitura
  pFile2 = SPIFFS.open(fileName2.c_str(), FILE_READ);

  // Se não foi possível abrir o arquivo
  if (!pFile2)
  {
    // Guarda mensagem de erro e retorna false
    *errorMsg = "Failed to open the file: " + String(fileName2);
    return false;
  }

  // Obtém o tamanho do arquivo
  sizeArq = pFile2.size();
  Serial.println("Size: " + String(sizeArq));

  // Se existe ao menos um registro
  if (sizeArq >= sizeOfRecord2)
  {
    pFile2.seek(sizeArq - sizeOfRecord2); // Posiciona o ponteiro no final do arquivo menos o tamanho de um registro (sizeOfRecord)
    *line =  pFile2.readStringUntil('\n');
    pFile2.close();
  }

  return true;
}

// Exclui arquivo
bool FS_File_Record2::destroyFile()
{
  // Se o arquivo estiver aberto, fecha
  if (pFile2)
    pFile2.close();

  // Exclui arquivo e retorna o resultado da função "remove"
  return SPIFFS.remove((char*)fileName2.c_str());
}

// Função que busca um registro
// "pos" é a posição referente ao registro buscado
String FS_File_Record2::findRecord(int pos)
{
  // Linha que receberá o valor do registro buscado
  String line = "", errorMsg = "";

  // Posiciona na posição desejada
  // Obs. A posição se inicia com zero "0"
  if (!seekFile(pos))
    return "Seek error";

  // Lê o registro
  if (!readFileNextRecord(&line, &errorMsg))
    return errorMsg;

  return line;
}

// Verifica se o arquivo existe
bool FS_File_Record2::fileExists()
{
  return SPIFFS.exists((char*)fileName2.c_str());
}

// Cria um novo arquivo, se já existir um arquivo de mesmo nome, ele será removido antes
void FS_File_Record2::newFile()
{
  if (pFile2)
    pFile2.close();

  SPIFFS.remove((char*)fileName2.c_str());
  pFile2 = SPIFFS.open(fileName2.c_str(), FILE_WRITE);
  pFile2.close();
}

// Obtém o nome do arquivo


// Obtém o tamanho do registro
int FS_File_Record2::getSizeRecord()
{
  return sizeOfRecord2 - 2;
}

// Seta o tamanho do registro
void FS_File_Record2::setSizeRecord(int _sizeOfRecord2)
{
  sizeOfRecord2 = _sizeOfRecord2 + 2;
}

// Verifica se existe espaço suficiente na memória flash
bool FS_File_Record2::availableSpace()
{
  return getUsedSpace() + sizeOfRecord2 <= getTotalSpace();
  // ou também pode ser:
  //return getUsedSpace()+(sizeof(char)*sizeOfRecord2) <= getTotalSpace();  // sizeof(char) = 1
}

// Retorna o tamanho em bytes total da memória flash
int FS_File_Record2::getTotalSpace()
{
  return SPIFFS.totalBytes();
}

// Retorna a quantidade usada de memória flash
int FS_File_Record2::getUsedSpace()
{
  return SPIFFS.usedBytes();
}
