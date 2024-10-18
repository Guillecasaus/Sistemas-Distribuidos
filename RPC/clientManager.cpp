#include "clientManager.h"

void ClientManager::atiendeCliente(int clientId){
	bool salir=false;
	vector<unsigned char> buffer;
 
	while(!salir)
	{
		//recibir mensaje
		recvMSG(clientId,buffer);
		//tratar mensaje
			//desempaquetar tipo
			auto tipo=unpack<personaMSGTypes>(buffer);
				//swith tipo
			
			switch(tipo){
				case PersonaFD:{
					
						Persona p;
						
						clients[clientId]=p;
					
						//devolver resultado AckMSG
						buffer.clear();
						pack(buffer,ackMSG);
							
					}break;
				case PersonaFP:{
						string nombre;
						int edad;
						nombre.resize(unpack<int>(buffer));
						unpackv<char>(buffer,(char*)nombre.data(),nombre.size());
						edad=unpack<int>(buffer);
						Persona p(nombre,edad);
						
						clients[clientId]=p;
					
						//devolver resultado AckMSG
						buffer.clear();
						pack(buffer,ackMSG);
					
					}break;
				case PersonaFDe:{
						clients.erase(clientId);
						//devolver resultado AckMSG
						buffer.clear();
						pack(buffer,ackMSG);
						salir=true;
					
					}break;
				case setEdadF:{
						clients[clientId].setEdad(unpack<int>(buffer));
					
						//devolver resultado AckMSG
						buffer.clear();
						pack(buffer,ackMSG);
					
					}break;
				case setNombreF:{
						string nombre;
						nombre.resize(unpack<int>(buffer));
						unpackv<char>(buffer,(char*)nombre.data(),nombre.size());
						clients[clientId].setNombre(nombre);
						//devolver resultado AckMSG
						buffer.clear();
						pack(buffer,ackMSG);
					
					}break;
				case getEdadF:{
						
						//devolver resultado AckMSG
						buffer.clear();
						pack(buffer,ackMSG);
						pack(buffer,(int)clients[clientId].getEdad());
					}break;
				case getNombreF:{
						//devolver resultado AckMSG
						buffer.clear();
						pack(buffer,ackMSG);
						
						string nombre=clients[clientId].getNombre();
						pack(buffer,(int)nombre.size());
						packv(buffer,(char*)nombre.data(),nombre.size());

					
					}break;
				case ackMSG:
				default:
				{
					
				//default
					//mensaje error
					//recuperacion exit
					cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
							"Tipo de mensaje inválido"<<endl; 		
				}break;			
			};
			sendMSG(clientId,buffer);
	}
	//cerrar conexion
	closeConnection(clientId);	
}





