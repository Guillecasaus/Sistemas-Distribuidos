//Guillermo Casaus

#include "clientManager.h"
#include "filemanager.h"

void ClientManager::atiendeCliente(int clientId) {
    bool salir = false;
    vector<unsigned char> buffer;

    FileManager fileManager("./");

    while (!salir) {

        recvMSG(clientId, buffer);
		
        auto tipo = unpack<fileManagerMSGTypes>(buffer);

        switch (tipo) {
            case FileManagerFD: {

                fileManager = FileManager();


                buffer.clear();
                pack(buffer, ackMSG);
            } break;

            case FileManagerFP: {

                string path;
                path.resize(unpack<int>(buffer));
                unpackv<char>(buffer, (char*)path.data(), path.size());

                fileManager = FileManager(path);

                buffer.clear();
                pack(buffer, ackMSG);
            } break;

            case FileManagerFDe: {
                salir = true;  

                buffer.clear();
                pack(buffer, ackMSG);
            } break;

            case listFilesF: {
                auto fileList = fileManager.listFiles();
                cout<<"ya leidos los ficheros"<<" "<<(int)fileList.size()<<" "<<endl;
                buffer.clear();
                pack(buffer, ackMSG);
                pack(buffer, (int)fileList.size());
                cout<<"1: "<<" "<<ackMSG<<" "<<endl;
                cout<<"2: "<<" "<<(int)fileList.size()<<" "<<endl;
                for (const auto &fileName : fileList) {
                    pack(buffer, (int)fileName.size());
                    packv(buffer, (char*)fileName.data(), (int)fileName.size());
                }
                // Imprimir el buffer recibido como bytes
                cout << "Contenido del buffer recibido: ";
                for (unsigned char byte : buffer) {
                    printf("%02x ", byte);
                }
                cout << endl;



            } break;

            case readFileF: {
                string fileName;
                fileName.resize(unpack<int>(buffer));
                unpackv<char>(buffer, (char*)fileName.data(), fileName.size());

                vector<unsigned char> data;
                fileManager.readFile(fileName, data);

                buffer.clear();
                pack(buffer, ackMSG);
                pack(buffer, (int)data.size());
                packv(buffer, data.data(), (int)data.size());
            } break;

            case writeFileF: {
                string fileName;
                fileName.resize(unpack<int>(buffer));
                unpackv<char>(buffer, (char*)fileName.data(), fileName.size());

                vector<unsigned char> data;
                int dataSize = unpack<int>(buffer);
                data.resize(dataSize);
                unpackv<unsigned char>(buffer, data.data(), dataSize);

                fileManager.writeFile(fileName, data);

                buffer.clear();
                pack(buffer, ackMSG);
            } break;

            case ackMSG:
            default: {
                cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " Tipo de mensaje inválido" << endl;
            } break;
        }

        sendMSG(clientId, buffer);
    }

    closeConnection(clientId);
}
