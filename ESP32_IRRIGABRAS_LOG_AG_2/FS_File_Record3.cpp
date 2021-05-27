// Importamos o header referente a lib FS_File_Record
#include "FS_File_Record3.h"

String fileName3; // Nome do arquivo
int sizeOfRecord3 = 25; // Default 3 (somando \r\n o tamanho será 5)
File pFile3; // Ponteiro do arquivo

// Construtor que seta somente o nome do arquivo, deixando o tamanho de registro default 3
FS_File_Record3::FS_File_Record3(String _fileName3)
{
  fileName3 = _fileName3;
}

// Construtor que seta nome do arquivo e tamanho de registro +2 (\r\n)
FS_File_Record3::FS_File_Record3(String _fileName3, int _sizeOfRecord3)
{
  fileName3 = _fileName3;
  sizeOfRecord3 = _sizeOfRecord3 + 2;
}

// Inicializa SPIFFS
bool FS_File_Record3::init()
{
  return SPIFFS.begin(true);
}

// Posiciona o ponteiro do arquivo no início
bool FS_File_Record3::rewind()
{
  if (pFile3)
    return pFile3.seek(0);

  return false;
}

// Lê a próxima linha do arquivo
bool FS_File_Record3::readFileNextRecord(String *line, String *errorMsg)
{
  *errorMsg = "";
  *line = "";
  // Se o ponteiro estiver nulo
  if (!pFile3)
  {
    // Abre arquivo para leitura
    pFile3 = SPIFFS.open(fileName3.c_str(), FILE_READ);

    // Se aconteceu algum erro
    if (!pFile3)
    {
      // Guarda msg de erro
      *errorMsg = "Failed to open the file";
      // Retorna falso
      return false;
    }
  }

  // Se for possível ler o arquivo
  if (pFile3.available())
  {
    // Lê arquivo
    *line = pFile3.readStringUntil('\n');
    // Retorna true
    return true;
  }

  // Se o arquivo estiver vazio retorna true
  if (pFile3.size() <= 0)
    return true;

  // Se não for possível ler o arquivo retorna falso
  return false;
}

//Posiciona ponteiro do arquivo na posição "pos"
bool FS_File_Record3::seekFile(int pos)
{
  // Se o ponteiro estiver nulo
  if (pFile3)
    pFile3.close();

  pFile3 = SPIFFS.open(fileName3.c_str(), FILE_READ); // Abre o arquivo para leitura

  // Posiciona o ponteiro na posição multiplicando pelo tamanho do registro
  return pFile3.seek(sizeOfRecord3 * pos);
}

// Escreve no arquivo
bool FS_File_Record3::writeFile(String line, String *errorMsg)
{
  if (pFile3)
    pFile3.close();

  pFile3 = SPIFFS.open(fileName3.c_str(), FILE_APPEND);

  // Se foi possível abrir
  if (pFile3)
  {
    // Escreve registro
    pFile3.println(line);
    // Fecha arquivo
    pFile3.close();
    // Retorna true
    return true;
  }

  // Se não foi possível abrir guarda mensagem de erro e retorna false
  *errorMsg = "Failed to open the file: " + String(fileName3);
  return false;
}

// Lê o último registro do arquivo
bool FS_File_Record3::readFileLastRecord(String *line, String *errorMsg)
{
  // Variável que guardará o tamanho do arquivo
  int sizeArq;

  // Limpa string
  *errorMsg = "";

  // Se o arquivo está aberto, fecha
  if (pFile3)
    pFile3.close();

  // Abre o arquivo para leitura
  pFile3 = SPIFFS.open(fileName3.c_str(), FILE_READ);

  // Se não foi possível abrir o arquivo
  if (!pFile3)
  {
    // Guarda mensagem de erro e retorna false
    *errorMsg = "Failed to open the file: " + String(fileName3);
    return false;
  }

  // Obtém o tamanho do arquivo
  sizeArq = pFile3.size();
  Serial.println("Size: " + String(sizeArq));

  // Se existe ao menos um registro
  if (sizeArq >= sizeOfRecord3)
  {
    pFile3.seek(sizeArq - sizeOfRecord3); // Posiciona o ponteiro no final do arquivo menos o tamanho de um registro (sizeOfRecord)
    *line =  pFile3.readStringUntil('\n');
    pFile3.close();
  }

  return true;
}

// Exclui arquivo
bool FS_File_Record3::destroyFile()
{
  // Se o arquivo estiver aberto, fecha
  if (pFile3)
    pFile3.close();

  // Exclui arquivo e retorna o resultado da função "remove"
  return SPIFFS.remove((char*)fileName3.c_str());
}

// Função que busca um registro
// "pos" é a posição referente ao registro buscado
String FS_File_Record3::findRecord(int pos)
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
bool FS_File_Record3::fileExists()
{
  return SPIFFS.exists((char*)fileName3.c_str());
}

// Cria um novo arquivo, se já existir um arquivo de mesmo nome, ele será removido antes
void FS_File_Record3::newFile()
{
  if (pFile3)
    pFile3.close();

  SPIFFS.remove((char*)fileName3.c_str());
  pFile3 = SPIFFS.open(fileName3.c_str(), FILE_WRITE);
  pFile3.close();
}

// Obtém o nome do arquivo


// Obtém o tamanho do registro
int FS_File_Record3::getSizeRecord()
{
  return sizeOfRecord3 - 2;
}

// Seta o tamanho do registro
void FS_File_Record3::setSizeRecord(int _sizeOfRecord3)
{
  sizeOfRecord3 = _sizeOfRecord3 + 2;
}

// Verifica se existe espaço suficiente na memória flash
bool FS_File_Record3::availableSpace()
{
  return getUsedSpace() + sizeOfRecord3 <= getTotalSpace();
  // ou também pode ser:
  //return getUsedSpace()+(sizeof(char)*sizeOfRecord3) <= getTotalSpace();  // sizeof(char) = 1
}

// Retorna o tamanho em bytes total da memória flash
int FS_File_Record3::getTotalSpace()
{
  return SPIFFS.totalBytes();
}

// Retorna a quantidade usada de memória flash
int FS_File_Record3::getUsedSpace()
{
  return SPIFFS.usedBytes();
}
