//Guillermo Casaus

#include "utils.h"
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>

using namespace std;

struct ServerInfo {
    string ip;
    int port;
    int connections;
};

map<int, ServerInfo> serverList; 
mutex serverListMutex;

void handleConnection(int clientId) {
    vector<unsigned char> buffer;

    recvMSG(clientId, buffer);
    int connectionType = unpack<int>(buffer);

    if (connectionType == 1) {  
        
       int ipSize = unpack<int>(buffer);  // Desempaquetar el tamaño del string IP
                   // Crear el string IP con el tamaño exacto y desempaquetar la IP
          string ip;
          ip.resize(ipSize);  // Cambiamos el tamaño del string a ipSize

            for (int i = 0; i < ipSize; ++i) {  // Desempaquetar carácter por carácter
              ip[i] = unpack<char>(buffer);
         }
    
    // Imprimir IP leída para ver si es correcta
    cout << "IP recibida del buffer: " << ip << endl;

         // Desempaquetar el puerto
         int port = unpack<int>(buffer);

        lock_guard<mutex> lock(serverListMutex);
        serverList[clientId] = {ip, port, 0};
        cout << "Servidor registrado con IP: " << ip << " y puerto: " << port << endl;

    } else if (connectionType == 2) {  
        lock_guard<mutex> lock(serverListMutex);

        if (serverList.empty()) {
            cerr << "No hay servidores disponibles." << endl;
            closeConnection(clientId);
            return;
        }

        int selectedServerId = -1;
        int minConnections = 5;
        for (const auto& entry : serverList) {
            if (entry.second.connections < minConnections) {
                selectedServerId = entry.first;
                minConnections = entry.second.connections;
            }
        }

        if (selectedServerId != -1) {
            buffer.clear();
            // Empaquetar tamaño de la IP
            pack(buffer, static_cast<int>(serverList[selectedServerId].ip.size()));

            // Empaquetar cada carácter de la IP uno por uno
            for (char c : serverList[selectedServerId].ip) {
                pack(buffer, c);
            }

            // Empaquetar el puerto
            pack(buffer, serverList[selectedServerId].port);
            
            // Enviar el buffer al cliente
            sendMSG(clientId, buffer);

            serverList[selectedServerId].connections++;
            cout << "Cliente asignado al servidor con IP: " << serverList[selectedServerId].ip
                << " y puerto: " << serverList[selectedServerId].port << endl;
        }
    }

    closeConnection(clientId);
}

int main() {
    int brokerSocket = initServer(6000);  
    cout << "Broker iniciado y esperando conexiones..." << endl;

    while (true) {
        while (!checkClient()) usleep(100);
        int clientId = getLastClientID();

        thread(handleConnection, clientId).detach();
    }

    close(brokerSocket);
    return 0;
}
