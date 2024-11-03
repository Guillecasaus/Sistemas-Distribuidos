//Guillermo Casaus

#include "utils.h"
#include <iostream>
#include <string>
#include <thread>
#include <list>
#include "clientManager.h"
using namespace std;

int main(int argc, char** argv)
{
	 // Conectar al broker para registrarse como servidor
    auto brokerConnection = initClient("127.0.0.1", 6000);  // IP y puerto del broker

    if (brokerConnection.serverId < 0) {
        cerr << "No se pudo conectar al broker para registrar el servidor." << endl;
        return -1;
    }

    vector<unsigned char> buffer;

    // Enviar tipo de conexión al broker (1 = servidor)
    pack(buffer, 1);  // 1 indica que es un servidor
    string serverIp = "127.0.0.1"; // Cambia esta IP si es necesario
    int serverPort = 5001;         // Puerto en el que este servidor escuchará a los clientes

    // Enviar la IP y puerto del servidor al broker
    // Empaquetar tamaño de la IP, luego la IP como string y el puerto del servidor
    pack(buffer, static_cast<int>(serverIp.size()));
    for (char c : serverIp) {
        pack(buffer, c);  // Empaqueta cada carácter individual de la IP
    }
  

    pack(buffer, serverPort);
	 // Verificar contenido del buffer antes de enviarlo
    
    sendMSG(brokerConnection.serverId, buffer);

    // Cerrar la conexión al broker
    closeConnection(brokerConnection.serverId);

	
	//iniciar servidor
	int serverSocket=initServer(5001);
	cout<<"Server iniciado\n";
	//loop
	while(1){
		//esperar conexion
		while(!checkClient()){usleep(100);}
		//conexion recibida
		//sacar clientId
		int clientId=getLastClientID();
		cout<<"Conexion establecida con cliente:"<<clientId<<"\n";

		//crear thread atiendeCliente
		thread *th=new thread(ClientManager::atiendeCliente,clientId);
	//till ctrl-C
	}
	//cerrar socket
	close(serverSocket);
	
	
 return 0;
}
