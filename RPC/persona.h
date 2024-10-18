#pragma once
#include <string>

using namespace std;

class Persona{
	private:
		string nombre;
		int edad;
		
	public:
		Persona();
		Persona(string nombre,int edad);
		~Persona();
		void setEdad(int edad);	
		void setNombre(string nombre);
		int  getEdad();	
		string getNombre();
	
	
};