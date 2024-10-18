#pragma once
#include "utils.h"
#include "persona.h"

using namespace std;

typedef enum{
	PersonaFD, //constructor por defecto
	PersonaFP, //constructor con parametros
	PersonaFDe,//destructor de persona
	setEdadF,
	setNombreF,
	getEdadF,
	getNombreF,
	ackMSG
}personaMSGTypes;

class ClientManager{
	
	public:
		static inline map<int,Persona > clients;
		static inline map<Persona*,connection_t > connections;	
		static inline bool salir=false; 
		static void atiendeCliente(int clientId);

};
