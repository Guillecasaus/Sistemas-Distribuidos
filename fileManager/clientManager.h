//Guillermo Casaus

#pragma once
#include "utils.h"
#include "filemanager.h"

using namespace std;

typedef enum {
    FileManagerFD,
    FileManagerFP,
    FileManagerFDe,
    listFilesF,
    readFileF,
    writeFileF,
    ackMSG          
} fileManagerMSGTypes;

class ClientManager {
public:
    static void atiendeCliente(int clientId);
};