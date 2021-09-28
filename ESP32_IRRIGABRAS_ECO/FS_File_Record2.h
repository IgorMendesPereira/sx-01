// ifndef evita erros de duplicação ao chamar esta lib mais de uma vez
#ifndef FS_File_Record2_h
#define FS_File_Record2_h

#include <Arduino.h> //Biblioteca Arduino (opcional)

#include <SPIFFS.h>
#include <FS.h>

// Classe FS_File_Record e suas funções
class FS_File_Record2
{
    // Todas as funções desta lib são publicas, mais detalhes em FS_File_Record.cpp
  public:
    FS_File_Record2(String, int);
    FS_File_Record2(String);
    bool init();
    bool readFileLastRecord(String *, String *);
    bool destroyFile();
    String findRecord(int);
    bool rewind();
    bool writeFile(String, String *);
    bool seekFile(int);
    bool readFileNextRecord(String *, String *);

    int getSizeRecord();
    void setSizeRecord(int);
    void newFile();
    bool fileExists();
    bool availableSpace();
    int getTotalSpace();
    int getUsedSpace();
};

#endif
