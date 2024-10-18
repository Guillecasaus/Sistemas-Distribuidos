#include <iostream>
#include "persona.h"

int main(int argc, char** argv)
{
	Persona p1;
	string nombre;
	int edad;
	cout<<"Introduzca nombre\n";
	cin>>nombre;
	cout<<"Introduzca edad\n";
	cin>> edad;
	
	p1.setNombre(nombre);
	p1.setEdad(edad);
	
	cout<<"Datos almacenados:\n"
		<<p1.getNombre()<<"\n"<<p1.getEdad()<<endl;
	
	return 0;
}