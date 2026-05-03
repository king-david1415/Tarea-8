#define _HAS_STD_BYTE 0
#include <iostream>
#include "ConexionDB.h"
#include "Estudiante.h"
using namespace std;

int main()
{
	//agregar nuevo
	string codigo, nombres, apellidos, direccion, fecha_nacimiento;
	int telefono = 0, id_tipo_sangre = 0, id_estudiante = 0;
	cout << "Ingrese codigo: ";
	cin >> codigo;
	cin.ignore();
	cout << "Ingrese Nombres: ";
	getline(cin, nombres);
	cout << "Ingrese Apellidos: ";
	getline(cin, apellidos);
	cout << "Ingrese Direccion: ";
	getline(cin, direccion);
	cout << "Ingrese Telefono: ";
	cin >> telefono;
	cin.ignore();
	cout << "Ingrese Fecha de nacimiento: ";
	getline(cin, fecha_nacimiento);
	cout << "Ingrese Tipo de sangre: ";
	cin >> id_tipo_sangre;

	Estudiante e = Estudiante(nombres, apellidos, direccion, telefono, fecha_nacimiento, id_tipo_sangre, codigo, id_estudiante);
	e.crear();
	e.leer();

	//actualizar
	cout << "Ingrese el ID a actualizar";
	cin >> id_estudiante;
	cout << "Ingrese codigo: ";
	cin >> codigo;
	cin.ignore();
	cout << "Ingrese Nombres: ";
	getline(cin, nombres);
	cout << "Ingrese Apellidos: ";
	getline(cin, apellidos);
	cout << "Ingrese Direccion: ";
	getline(cin, direccion);
	cout << "Ingrese Telefono: ";
	cin >> telefono;
	cin.ignore();
	cout << "Ingrese Fecha de nacimiento: ";
	getline(cin, fecha_nacimiento);
	cout << "Ingrese Tipo de sangre: ";
	cin >> id_tipo_sangre;

	
	e.setId_estudiante(id_estudiante);
	e.setNombres(nombres);
	e.setApellidos(apellidos);
	e.setDireccion(direccion);
	e.setTelefono(telefono);
	//e.setFecha_Nacimiento(fecha_nacimiento);
	e.setId_Tipo_Sangre(id_tipo_sangre);
	e.actualizar();
	e.leer();

	//borrar
	cout << "Ingrese el ID a borrar: ";
	cin >> id_estudiante;
	e.setId_estudiante(id_estudiante);
	e.borrar();
	e.leer();

}

