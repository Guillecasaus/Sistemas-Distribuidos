#include "persona.h"
#include "clientManager.h"
		
		

Persona::Persona(){
	//iniciar conexion
	auto conn=initClient("127.0.0.1",5000);

	//invocar constructor en server:
		//buffer
	vector<unsigned char> buffer;
		//empaquetar tipo de llamada constructor por defecto de persona
	pack(buffer,PersonaFD);
		//mandar mensaje
	sendMSG(conn.serverId, buffer);
		//esperar confirmación de ejecución
	buffer.clear();
	recvMSG(conn.serverId,buffer);
	
	if(buffer.size()==0)
	{
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tamaño de mensaje inválido"<<endl;							//error
	}
	else{	
		personaMSGTypes ack=unpack<personaMSGTypes>(buffer);
				//si no recibo confirmación
		if(ack!=ackMSG)
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tipo de mensaje inválido"<<endl;	
		//error
		else{
			ClientManager::connections[this]=conn;
		}
	}		
}
Persona::Persona(string nombre,int edad){
		//iniciar conexion
	auto conn=initClient("127.0.0.1",5000);

	//invocar constructor en server:
		//buffer
	vector<unsigned char> buffer;
		//empaquetar tipo de llamada constructor por defecto de persona
	pack(buffer,PersonaFP);
	pack(buffer,(int)nombre.size());
	packv(buffer,(char*)nombre.data(),(int)nombre.size());
	pack(buffer,(int)edad);
	
		//mandar mensaje
	sendMSG(conn.serverId, buffer);
		//esperar confirmación de ejecución
	buffer.clear();
	recvMSG(conn.serverId,buffer);
	
	if(buffer.size()==0)
	{
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<"Tamaño de mensaje inválido"<<endl;							//error
	}
	else{	
		personaMSGTypes ack=unpack<personaMSGTypes>(buffer);
		if(ack!=ackMSG)
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<	"Tipo de mensaje inválido"<<endl;	
		else{
			ClientManager::connections[this]=conn;
		}
	}	
	
	
}
Persona::~Persona(){
	auto conn=ClientManager::connections[this];
	vector<unsigned char> buffer;
	pack(buffer,PersonaFDe);
	sendMSG(conn.serverId, buffer);
	buffer.clear();
	recvMSG(conn.serverId,buffer);
	
	if(buffer.size()==0)
	{
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tamaño de mensaje inválido"<<endl;							//error
	}
	else{	
		personaMSGTypes ack=unpack<personaMSGTypes>(buffer);
		if(ack!=ackMSG)
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tipo de mensaje inválido"<<endl;
		else{
			closeConnection(conn.serverId);
			ClientManager::connections.erase(this);
		}
	}	
}

void Persona::setEdad(int edad){
	auto conn=ClientManager::connections[this];
	vector<unsigned char> buffer;
	
	pack(buffer,setEdadF);
	pack(buffer,edad);
	
	sendMSG(conn.serverId, buffer);
	buffer.clear();
	recvMSG(conn.serverId,buffer);
	
	if(buffer.size()==0)
	{
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tamaño de mensaje inválido"<<endl;							//error
	}
	else{	
		personaMSGTypes ack=unpack<personaMSGTypes>(buffer);
		if(ack!=ackMSG)
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tipo de mensaje inválido"<<endl;
	}	
	

}

void Persona::setNombre(string nombre){
	auto conn=ClientManager::connections[this];
	vector<unsigned char> buffer;
	
	pack(buffer,setNombreF);
	pack(buffer,(int)nombre.size());
	packv(buffer,(char*)nombre.data(),(int)nombre.size());

	sendMSG(conn.serverId, buffer);
	buffer.clear();
	recvMSG(conn.serverId,buffer);
	
	if(buffer.size()==0)
	{
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tamaño de mensaje inválido"<<endl;							//error
	}
	else{	
		personaMSGTypes ack=unpack<personaMSGTypes>(buffer);
		if(ack!=ackMSG)
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tipo de mensaje inválido"<<endl;
	}	
	
}

int  Persona::getEdad(){
	int result;
	//crear mensaje peticion de ejecución
	auto conn=ClientManager::connections[this];
	vector<unsigned char> buffer;
	
	//empaquetar tipo
	pack(buffer,getEdadF);
	sendMSG(conn.serverId,buffer);
	buffer.clear();
	recvMSG(conn.serverId,buffer);
	
	if(buffer.size()==0)
	{
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tamaño de mensaje inválido"<<endl;							//error
	}
	else{	
		personaMSGTypes ack=unpack<personaMSGTypes>(buffer);
		if(ack!=ackMSG)
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tipo de mensaje inválido"<<endl;
		else{
			result=unpack<int>(buffer);
		}
	}	
	return result;	
}



string Persona::getNombre(){
	string result;
	//crear mensaje peticion de ejecución
	auto conn=ClientManager::connections[this];
	vector<unsigned char> buffer;
	
	//empaquetar tipo
	pack(buffer,getNombreF);
	sendMSG(conn.serverId,buffer);
	buffer.clear();
	recvMSG(conn.serverId,buffer);
	
	if(buffer.size()==0)
	{
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tamaño de mensaje inválido"<<endl;							//error
	}
	else{	
		personaMSGTypes ack=unpack<personaMSGTypes>(buffer);
		if(ack!=ackMSG)
			cout<<"ERROR: "<<__FILE__<<":"<<__LINE__<<
						"Tipo de mensaje inválido"<<endl;
		else{
			result.resize(unpack<int>(buffer));
			unpackv<char>(buffer,(char*)result.data(),result.size());
		}
	}	
	return result;
}












