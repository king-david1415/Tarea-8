#define _HAS_STD_BYTE 0
#include <iostream>
#include "ConexionDB.h"
#include "Estudiante.h"
using namespace std;

// ---------FUNCIONES DE VALIDACION---------
//FUNCION PARA VALIDAR CODIGO
bool validarCodigo(string codigo_validar) {
	if (codigo_validar.length() != 4) {
		return false;
	}
	if (codigo_validar[0] != 'E') {
		return false;
	}
	for (int i = 1; i < 4; i++) {
		if (codigo_validar[i] < '0' || codigo_validar[i] > '9') {
			return false;
		}
	}
	return true;
}
// PARA VALIDAR QUE EL CODIGO NO SE REPITA O DUPLIQUE
bool codigo_duplicado(string codigo_a_buscar) {
	ConexionDB cn;
	cn.abrir_conexion();
	if (cn.getConector() != NULL) {

		// consulta para validar el codigo 
		string consulta = "SELECT codigo FROM estudiantes WHERE codigo = '" + codigo_a_buscar + "'";
		const char* c = consulta.c_str();
		int estadoConsulta = mysql_query(cn.getConector(), c);

		if (estadoConsulta == 0) {
			MYSQL_RES* resultado = mysql_store_result(cn.getConector());

			if (mysql_num_rows(resultado) > 0) {
				mysql_free_result(resultado); // Limpiamos la memoria
				cn.cerrar_conexion();         // Cerramos conexion
				return true;                  // Retornamos "verdadero, si existe"
			}
			mysql_free_result(resultado);
		}
	}

	// Si no habia conexion, o hubo un error, o simplemente no lo encontro:
	cn.cerrar_conexion();
	return false; // Retornamos "falso, no existe"
}



//FUNCION PARA VALIDAR NOMBRES Y APELLIDOS
bool validar_nom_ape(string nom_ape_validar) {
	//Validar que no este vacío y no pase de 60 caracteres
	if (nom_ape_validar.length() == 0 || nom_ape_validar.length() > 60) {
		return false;
	}
	// Recorrer letra por letra
	for (int i = 0; i < nom_ape_validar.length(); i++) {
		char letra = nom_ape_validar[i];

		// validar tipo de caracter 
		bool esMayuscula = (letra >= 'A' && letra <= 'Z');
		bool esMinuscula = (letra >= 'a' && letra <= 'z');
		bool esEspacio = (letra == ' ');

		// si la letra no coincide con la validacion se rechaza
		if (!esMayuscula && !esMinuscula && !esEspacio) {
			return false;
		}
	}

	return true; // si todo esta bien
}

//FUNCION PARA VALIDAR DIRECCION
bool validarDireccion(string direccion) {
	// Validar que no esté vacía y no pase de 100 caracteres
	if (direccion.length() == 0 || direccion.length() > 100) {
		return false;
	}

	// Variable para saber si se escribe algo mas que espacios en blanco
	bool contenidoreal = false;

	// Recorrer la direccion ingresada letra por letra
	for (int i = 0; i < direccion.length(); i++) {
		char c = direccion[i];

		// Si se encuentra al menos un caracter que no sea un espacio
		if (c != ' ') {
			contenidoreal = true;
		}

		// Bloquear caracteres peligrosos (', ", ;, \)
		// para revisar una comilla simple o una barra invertida, se le pone un \ antes para que el compilador no se confunda.
		if (c == '\'' || c == '"' || c == ';' || c == '\\' || c == '*' || c == '%' || c == '$' || c == '!' || c == '^' || c == '?') {
			return false; // Rechazar simbolos
		}
	}

	// Si se revisa y todo son espacios en blanco
	if (contenidoreal == false) {
		return false;
	}

	return true; // Si todo esta bien
}

//FUNCION PARA VALIDAR TELEFONO

bool validarTelefono(int telefono) {
	// Si el numero esta entre 10 millones y 99.9 millones, tiene 8 digitos
	if (telefono >= 10000000 && telefono <= 99999999) {
		return true;
	}
	else {
		return false;
	}
}

//FUNCION PARA VALIDAR FECHA
bool validarFecha(string fecha) {
	// longitud exacta (2000-05-20 son 10 caracteres)
	if (fecha.length() != 10) {
		return false;
	}

	// Validar que los guiones esten en las posiciones correctas (posicion 4 y 7)
	if (fecha[4] != '-' || fecha[7] != '-') {
		return false;
	}

	//validar que los caracteres sean unicamente numeros
	for (int i = 0; i < 10; i++) {
		if (i == 4 || i == 7) continue; // no revisar las posiciones de los guiones

		if (fecha[i] < '0' || fecha[i] > '9') {
			return false; // Si hay una letra, rechazar
		}
	}

	// Extraer el Año, Mes y Día substr(posicion_inicial, cantidad_de_caracteres)
	int anio = stoi(fecha.substr(0, 4));
	int mes = stoi(fecha.substr(5, 2));
	int dia = stoi(fecha.substr(8, 2));

	//Validar que el mes y el dia sean reales
	if (mes < 1 || mes > 12) return false;
	if (dia < 1 || dia > 31) return false;

	// validacion de meses que solo tienen 30 dias (Abril, Junio, Septiembre, Noviembre)
	if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
		return false;
	}

	// validar febrero
	if (mes == 2) {
		// si es año bisiesto
		bool esBisiesto = (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0));
		if (esBisiesto && dia > 29) return false;
		if (!esBisiesto && dia > 28) return false;
	}

	// Validar que no sea una fecha futura (2026 como año actual y maximo)
	if (anio > 2026) {
		return false;
	}

	return true; // si todo esta bien, pasamos
}

//VALIDAR TIPO DE SANGRE

//PARA VALIDAR ESTE CAMPO ES NECESARIO SABER QUE LOS ID DE CADA TIPO DE SANGRE SON:
//1 A+
//2 A-
//3 B+
//4 B-
//5 AB+
//6 AB-
//7 O+
//8 O-

bool validarTipoSangre(int id_sangre) {
	// Verificar que el numero este dentro del rango de la tabla
	if (id_sangre >= 1 && id_sangre <= 8) {
		return true;
	}

	return false; // Si ingresa otro numero se rechaza
}
//----------------------------------------EMPIEZA MAIN --------------------------------------------------
int main()
{
	//agregar nuevo
	string codigo, nombres, apellidos, direccion, fecha_nacimiento;
	int telefono = 0, id_tipo_sangre = 0, id_estudiante = 0;
	bool telefonoValido = false;//variable para validar el numero de telefono 
	bool sangreValida = false; //variable para validar tipo de sangre
	bool codigoValido = false; //variable para validar duplicados en el codigo

	
	do {
		codigoValido = false;
		cout << "Ingrese codigo (Ej. E001): ";
		cin >> codigo;

		// validar el formato E001
		if (validarCodigo(codigo) == false) {
			cout << "Error: El codigo debe ser 'E' seguido de 3 numeros.\n";
		}
		// validacion en MYSQL
		else if (codigo_duplicado(codigo) == true) {
			cout << "Error: El codigo " << codigo << " ya esta registrado. Ingrese uno distinto.\n";
		}
		//  si Todo está perfecto
		else {
			codigoValido = true;
		}

	} while (codigoValido == false);
	cin.ignore();

	do{
		cout << "Ingrese Nombres: ";
		getline(cin, nombres);
		if (validar_nom_ape(nombres)==false) {
			cout << "Error, no se permiten caracteres especiales, numeros o mas de 60 caracteres" << endl;
		}
	} while (validar_nom_ape(nombres) == false);
	
	do {
		cout << "Ingrese Apellidos: ";
		getline(cin, apellidos);
		if (validar_nom_ape(apellidos) == false) {
			cout << "Error, no se permiten caracteres especiales, numeros o mas de 60 caracteres" << endl;
		}
	} while (validar_nom_ape(apellidos) == false);

	do {
		cout << "Ingrese Direccion: ";
		getline(cin, direccion);
		if (validarDireccion(direccion) == false) {
			cout << "Error, no se permiten simbolos especiales (comillas simples, comillas, punto y coma, dos puntos o barras invertidas) o mas de 100 caracteres " << endl;
		}
	} while(validarDireccion(direccion)==false);

	do {
		// iniciar en falso por que aun no se han ingresado datos
		telefonoValido = false;

		cout << "Ingrese el numero de telefono: ";
		cin >> telefono;

		// validar si hay letras al final o enmedio del numero
		if (cin.fail() || cin.peek() != '\n') {
			cout << "Error: Solo se permiten numeros. No ingrese letras.\n";
			cin.clear();
			cin.ignore(10000, '\n');
		}
		//validar si hay 8 numeros
		else if (validarTelefono(telefono) == false) {
			cout << "Error: El telefono debe tener exactamente 8 digitos.\n";
			cin.ignore(10000, '\n');
		}
		//si todo esta bien
		else {
			cin.ignore(10000, '\n'); //limpiar el buffer

			telefonoValido = true; //si todo esta bien, continuar
		}
	} while (telefonoValido == false);


	do {
		cout << "Ingrese Fecha de nacimiento (Formato AAAA-MM-DD): ";
		getline(cin, fecha_nacimiento);
		if (validarFecha(fecha_nacimiento) == false) {
			cout << "Error: Fecha invalida. Use el formato AAAA-MM-DD, asegurese de que la fecha sea real y no futura." << endl;
		}
	} while (validarFecha(fecha_nacimiento) == false);


	do {
		sangreValida = false;

		cout << "| ID TIPO|" << endl;
		cout << "| 1   A+ |" << endl;
		cout << "| 2   A- |" << endl;
		cout << "| 3   B+ |" << endl;
		cout << "| 4   B- |" << endl;
		cout << "| 5  AB+ |" << endl;
		cout << "| 6  AB- |" << endl;
		cout << "| 7   O+ |" << endl;
		cout << "| 8   O- |" << endl;
		cout << "Ingrese el ID del Tipo de sangre: ";
		cin >> id_tipo_sangre;

		// validar si se ingresan letras se rechaza
		if (cin.fail() || cin.peek() != '\n') {
			cout << "Error: Solo se permiten numeros. No ingrese letras.\n";
			cin.clear();
			cin.ignore(10000, '\n');
		}
		// validar si el numero existe en la tabla
		else if (validarTipoSangre(id_tipo_sangre) == false) {
			cout << "Error: Ese ID no existe. Ingrese un numero del 1 al 8.\n";
			cin.ignore(10000, '\n');
		}
		//si todo esta bien
		else {
			cin.ignore(10000, '\n');
			sangreValida = true;
		}

	} while (sangreValida == false);

	Estudiante e = Estudiante(nombres, apellidos, direccion, telefono, fecha_nacimiento, id_tipo_sangre, codigo, id_estudiante);
	e.crear();
	e.leer();

	//actualizar
	cout << "Ingrese el ID a actualizar";
	cin >> id_estudiante;
	do {
		codigoValido = false;
		cout << "Ingrese codigo (Ej. E001): ";
		cin >> codigo;

		// validar el formato E001
		if (validarCodigo(codigo) == false) {
			cout << "Error: El codigo debe ser 'E' seguido de 3 numeros.\n";
		}
		// validacion en MYSQL
		else if (codigo_duplicado(codigo) == true) {
			cout << "Error: El codigo " << codigo << " ya esta registrado. Ingrese uno distinto.\n";
		}
		//  si Todo está perfecto
		else {
			codigoValido = true;
		}

	} while (codigoValido == false);
	cin.ignore();

	do {
		cout << "Ingrese Nombres: ";
		getline(cin, nombres);
		if (validar_nom_ape(nombres) == false) {
			cout << "Error, no se permiten caracteres especiales, numeros o mas de 60 caracteres" << endl;
		}
	} while (validar_nom_ape(nombres) == false);

	do {
		cout << "Ingrese Apellidos: ";
		getline(cin, apellidos);
		if (validar_nom_ape(apellidos) == false) {
			cout << "Error, no se permiten caracteres especiales, numeros o mas de 60 caracteres" << endl;
		}
	} while (validar_nom_ape(apellidos) == false);

	do {
		cout << "Ingrese Direccion: ";
		getline(cin, direccion);
		if (validarDireccion(direccion) == false) {
			cout << "Error, no se permiten simbolos especiales (comillas simples, comillas, punto y coma, dos puntos o barras invertidas) o mas de 100 caracteres " << endl;
		}
	} while (validarDireccion(direccion) == false);

	do {
		// iniciar en falso por que aun no se han ingresado datos
		telefonoValido = false;

		cout << "Ingrese el numero de telefono: ";
		cin >> telefono;

		// validar si hay letras al final o enmedio del numero
		if (cin.fail() || cin.peek() != '\n') {
			cout << "Error: Solo se permiten numeros. No ingrese letras.\n";
			cin.clear();
			cin.ignore(10000, '\n');
		}
		//validar si hay 8 numeros
		else if (validarTelefono(telefono) == false) {
			cout << "Error: El telefono debe tener exactamente 8 digitos.\n";
			cin.ignore(10000, '\n');
		}
		//si todo esta bien
		else {
			cin.ignore(10000, '\n'); //limpiar el buffer

			telefonoValido = true; //si todo esta bien, continuar
		}
	} while (telefonoValido == false);


	do {
		cout << "Ingrese Fecha de nacimiento (Formato AAAA-MM-DD): ";
		getline(cin, fecha_nacimiento);
		if (validarFecha(fecha_nacimiento) == false) {
			cout << "Error: Fecha invalida. Use el formato AAAA-MM-DD, asegurese de que la fecha sea real y no futura." << endl;
		}
	} while (validarFecha(fecha_nacimiento) == false);


	do {
		sangreValida = false;

		cout << "| ID TIPO|" << endl;
		cout << "| 1   A+ |" << endl;
		cout << "| 2   A- |" << endl;
		cout << "| 3   B+ |" << endl;
		cout << "| 4   B- |" << endl;
		cout << "| 5  AB+ |" << endl;
		cout << "| 6  AB- |" << endl;
		cout << "| 7   O+ |" << endl;
		cout << "| 8   O- |" << endl;
		cout << "Ingrese el ID del Tipo de sangre: ";
		cin >> id_tipo_sangre;

		// validar si se ingresan letras se rechaza
		if (cin.fail() || cin.peek() != '\n') {
			cout << "Error: Solo se permiten numeros. No ingrese letras.\n";
			cin.clear();
			cin.ignore(10000, '\n');
		}
		// validar si el numero existe en la tabla
		else if (validarTipoSangre(id_tipo_sangre) == false) {
			cout << "Error: Ese ID no existe. Ingrese un numero del 1 al 8.\n";
			cin.ignore(10000, '\n');
		}
		//si todo esta bien
		else {
			cin.ignore(10000, '\n');
			sangreValida = true;
		}

	} while (sangreValida == false);

	
	e.setId_estudiante(id_estudiante);
	e.setNombres(nombres);
	e.setApellidos(apellidos);
	e.setDireccion(direccion);
	e.setTelefono(telefono);
	e.setFecha_Nacimiento(fecha_nacimiento);
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

