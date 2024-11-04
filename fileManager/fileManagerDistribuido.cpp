//Guillermo Casaus

#include "filemanager.h"
#include "clientManager.h"

FileManager::FileManager(){
    
	auto conn=initClient("127.0.0.1",5000);

	vector<unsigned char> buffer;

    pack(buffer,FileManagerFD);
	
	sendMSG(conn.serverId, buffer);
	
	buffer.clear();
	recvMSG(conn.serverId,buffer);
	
	if(buffer.size()==0){
        cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<< "Tamaño de mensaje inválido"<<endl;							/
	}
	else{	
		fileManagerMSGTypes ack=unpack<fileManagerMSGTypes>(buffer);
		
        if(ack!=ackMSG)
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tipo de mensaje inválido"<<endl;	
		else{
			ClientManager::connections[this]=conn;
		}
	}		
}

FileManager::FileManager(string path){
    auto conn = initClient("127.0.0.1", 5000);

    vector<unsigned char> buffer;
    pack(buffer, FileManagerFP);  
    pack(buffer, (int)path.size());
    packv(buffer, (char*)path.data(), (int)path.size());

    sendMSG(conn.serverId, buffer);

    buffer.clear();
    recvMSG(conn.serverId, buffer);

    if (buffer.size() == 0) {
        cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " Tamaño de mensaje inválido" << endl;
    } else {
        fileManagerMSGTypes ack = unpack<fileManagerMSGTypes>(buffer);
        if (ack != ackMSG) {
            cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " Tipo de mensaje inválido" << endl;
        } else {
            ClientManager::connections[this] = conn;
        }
    }
}

FileManager::~FileManager(){
    
    auto conn = ClientManager::connections[this];
    
    vector<unsigned char> buffer;
    pack(buffer, FileManagerFDe);  

    sendMSG(conn.serverId, buffer);

    buffer.clear();
    recvMSG(conn.serverId, buffer);

    if (buffer.size() == 0) {
        cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " Tamaño de mensaje inválido" << endl;
    } else {
        fileManagerMSGTypes ack = unpack<fileManagerMSGTypes>(buffer);
        if (ack != ackMSG) {
            cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " Tipo de mensaje inválido" << endl;
        } else {
            closeConnection(conn.serverId);
            ClientManager::connections.erase(this);
        }
    }
}

FileManager::listFiles(){ 
    auto conn = ClientManager::connections[this];
    vector<unsigned char> buffer;

    pack(buffer, listFilesF);
    sendMSG(conn.serverId, buffer);

    buffer.clear();
    recvMSG(conn.serverId, buffer);

    vector<string> result;
    if (buffer.size() == 0) {
        cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " Tamaño de mensaje inválido" << endl;
    } else {
        fileManagerMSGTypes ack = unpack<fileManagerMSGTypes>(buffer);
        if (ack != ackMSG) {
            cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " Tipo de mensaje inválido" << endl;
        } else {
            int fileCount = unpack<int>(buffer);
            for (int i = 0; i < fileCount; i++) {
                int fileNameLength = unpack<int>(buffer);
                string fileName(fileNameLength, '\0');
                unpackv<char>(buffer, &fileName[0], fileNameLength);
                result.push_back(fileName);
            }
        }
    }
    return result;
}

FileManager::readFile(string fileName, vector<unsigned char> &data){
    auto conn = ClientManager::connections[this];
    vector<unsigned char> buffer;

    pack(buffer, readFileF);
    pack(buffer, (int)fileName.size());
    packv(buffer, (char*)fileName.data(), (int)fileName.size());
    
    sendMSG(conn.serverId, buffer);

    buffer.clear();
    recvMSG(conn.serverId, buffer);

    if (buffer.size() == 0) {
        cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " Tamaño de mensaje inválido" << endl;
    } else {
        fileManagerMSGTypes ack = unpack<fileManagerMSGTypes>(buffer);
        if (ack != ackMSG) {
            cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " Tipo de mensaje inválido" << endl;
        } else {
            int tamData = unpack<int>(buffer);
            data.resize(tamData);
            unpackv<unsigned char>(buffer, data.data(), tamData);
        }
    }
}

FileManager::writeFile(string fileName, vector<unsigned char> &data){ 

    auto conn = ClientManager::connections[this];
    vector<unsigned char> buffer;

    pack(buffer, writeFileF);
    pack(buffer, (int)fileName.size());
    packv(buffer, (char*)fileName.data(), (int)fileName.size());
    pack(buffer, (int)data.size());
    packv(buffer, (unsigned char*)data.data(), (int)data.size());
    
    sendMSG(conn.serverId, buffer);

    buffer.clear();
    recvMSG(conn.serverId, buffer);

    if (buffer.size() == 0) {
        cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " Tamaño de mensaje inválido" << endl;
    } else {
        fileManagerMSGTypes ack = unpack<fileManagerMSGTypes>(buffer);
        if (ack != ackMSG) {
            cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " Tipo de mensaje inválido" << endl;
        }
    }

}