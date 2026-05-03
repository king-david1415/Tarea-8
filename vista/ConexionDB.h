#define _HAS_STD_BYTE 0
#pragma once
#include <mysql.h>
#include <iostream>
using namespace std;
class ConexionDB{

private: MYSQL* conector;
public: 
	void abrir_conexion() {
		conector = mysql_init(0);
		conector = mysql_real_connect(conector, "localhost", "root", "D@vid0rellana", "db_escuela", 3306, NULL, 0);
	}

	MYSQL* getConector() {
		return conector;
	}

	void cerrar_conexion(){
		mysql_close(conector);
	}
};

