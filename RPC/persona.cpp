#include "persona.h"
		
		

Persona::Persona():nombre(""),edad(0){}
Persona::Persona(string nombre,int edad):nombre(nombre),edad(edad){}
Persona::~Persona(){}
void Persona::setEdad(int edad){this->edad=edad;}	
void Persona::setNombre(string nombre){this->nombre=nombre;}
int  Persona::getEdad(){return edad;}	
string Persona::getNombre(){return nombre;}