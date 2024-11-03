//Guillermo Casaus

#include "utils.h"
#include "filemanager.h"  
#include "clientManager.h"  
#include <iostream>
#include <string>
#include <thread>
#include <list>
#include <fstream>
#include <vector>


using namespace std;

void mostrarMenu() {
    cout << "Comandos disponibles:" << endl;
    cout << "1. ls - Listar archivos locales al programa" << endl;
    cout << "2. lls - Listar archivos en el servidor (directorio del FileManager)" << endl;
    cout << "3. upload <filename> - Subir archivo local al servidor" << endl;
    cout << "4. download <filename> - Descargar archivo del servidor al directorio local" << endl;
    cout << "5. exit - Salir del programa" << endl;
    cout << "Ingrese el comando: ";
}

int main() {

    // Conectar al broker para obtener la IP y el puerto de un servidor `FileManager` disponible
    auto brokerConnection = initClient("127.0.0.1", 6000);
    vector<unsigned char> buffer;

    // Enviar tipo de conexión al broker (2 = cliente)
    pack(buffer, 2);  // 2 indica que es un cliente
    sendMSG(brokerConnection.serverId, buffer);

    // Recibir IP y puerto del servidor asignado
    buffer.clear();
    recvMSG(brokerConnection.serverId, buffer);
    int ipSize = unpack<int>(buffer);  // Desempaquetar el tamaño del string IP
                   // Crear el string IP con el tamaño exacto y desempaquetar la IP
    string serverIp;
    serverIp.resize(ipSize);  // Cambiamos el tamaño del string a ipSize

    for (int i = 0; i < ipSize; ++i) {  // Desempaquetar carácter por carácter
      serverIp[i] = unpack<char>(buffer);
    }
    int serverPort = unpack<int>(buffer);

    // Cerrar la conexión al broker
    closeConnection(brokerConnection.serverId);

    // Conectar al servidor
    auto serverId=initClient(serverIp,serverPort);
    cout<<"Cliente conectado al servidor: "<<serverIp<<" "<<serverPort<<endl;

    
    string comando;
    while (true) {
        mostrarMenu();
        getline(cin, comando);

        vector<unsigned char> buffer;

        if (comando == "ls") {
  		    cout<<"Listing files in local path"<<endl;
			experimental::filesystem::path directorypath="./";

			for (const auto& entry : 
					 directory_iterator(directorypath)) { 
					 if(is_regular_file(entry))
						cout<<entry.path()<<endl; 
				} 
			cout<<endl;
        } 
        else if (comando == "lls") {
            // Solicitar lista de archivos en el servidor
            pack(buffer, fileManagerMSGTypes::listFilesF);
            sendMSG(serverId.serverId, buffer);

            // Recibir respuesta del servidor
            buffer.clear();
            recvMSG(serverId.serverId, buffer);
            
            // Desempaquetar y mostrar los nombres de archivos
            int ack = unpack<int>(buffer);
            int numFiles = unpack<int>(buffer);

            cout << "en el cliente de nuevo: " << ack << endl;
            cout << "Archivos en el servidor: " << numFiles << endl;

            // Verificar si hay archivos antes de intentar leerlos
            if (numFiles > 0) {
                for (int i = 0; i < numFiles; ++i) {
                    int nameSize = unpack<int>(buffer);
                    string filename(nameSize, '\0');
                    unpackv<char>(buffer, (char *) filename.data(), nameSize);
                    cout << filename << endl;
                }
            } else {
                cout << "No hay archivos en el servidor." << endl;
            }
        }

        else if (comando.rfind("upload", 0) == 0) {
            string filename = comando.substr(7);
            ifstream archivoLocal(filename, ios::binary);
            if (!archivoLocal) {
                cout << "No se pudo abrir el archivo " << filename << " para subir." << endl;
                continue;
            }

            vector<unsigned char> data((istreambuf_iterator<char>(archivoLocal)), istreambuf_iterator<char>());
            archivoLocal.close();

            // Empaquetar y enviar datos al servidor
            pack(buffer, fileManagerMSGTypes::writeFileF);
            pack(buffer, (int)filename.size());
            packv(buffer, (char*)filename.data(), filename.size());
            pack(buffer, (int)data.size());
            packv(buffer, data.data(), data.size());

            sendMSG(serverId.serverId, buffer);

            // Recibir confirmación
            buffer.clear();
            recvMSG(serverId.serverId, buffer);
            auto ack = unpack<fileManagerMSGTypes>(buffer);

            if (ack == fileManagerMSGTypes::ackMSG) {
                cout << "Archivo " << filename << " subido al servidor." << endl;
            } else {
                cout << "Error al subir el archivo " << filename << endl;
            }
        } 
        else if (comando.rfind("download", 0) == 0) {
            string filename = comando.substr(9);

            pack(buffer, fileManagerMSGTypes::readFileF);
            pack(buffer, (int)filename.size());
            packv(buffer, (char*)filename.data(), filename.size());

            sendMSG(serverId.serverId, buffer);

            // Recibir datos del archivo
            buffer.clear();
            recvMSG(serverId.serverId, buffer);
            auto ack = unpack<fileManagerMSGTypes>(buffer);

            if (ack == fileManagerMSGTypes::ackMSG) {
                int dataSize = unpack<int>(buffer);
                vector<unsigned char> data(dataSize);
                unpackv(buffer, data.data(), dataSize);

                // Guardar archivo en el directorio local
                ofstream archivoLocal(filename, ios::binary);
                if (archivoLocal) {
                    archivoLocal.write((char*)data.data(), data.size());
                    cout << "Archivo " << filename << " descargado del servidor." << endl;
                } else {
                    cout << "Error al crear el archivo " << filename << " localmente." << endl;
                }
            } else {
                cout << "Error al descargar el archivo " << filename << endl;
            }
        } 
        else if (comando == "exit") {
            // Enviar comando de salida al servidor y salir del programa
            pack(buffer, fileManagerMSGTypes::FileManagerFDe);
            sendMSG(serverId.serverId, buffer);
            break;
        } 
        else {
            cout << "Comando no reconocido. Inténtelo de nuevo." << endl;
        }
    }

    closeConnection(serverId.serverId);  // Cierra la conexión al servidor
    return 0;
}
