/*
 * RegistradorComandos.cpp
 *
 *  Created on: Sep 20, 2017
 *      Author: Rodrigo
 */

#include <RegistradorComandos.h>

RegistradorComandos::RegistradorComandos() {
	// TODO Auto-generated constructor stub

}

RegistradorComandos::~RegistradorComandos() {
	// TODO Auto-generated destructor stub
}

void RegistradorComandos::armazenaComando(int dt_IR){
	comando = dt_IR;
}

int RegistradorComandos::identificaComando(int comando){
	switch(comando){
	case 00110000: return 0;
	break;
	case 01001011: return 1;
	break;
	case 01010101: return 2;
	break;
	case 01010100: return 3;
	break;
	default: return 4;
	}
}

int RegistradorComandos::enviaComando(int wr_RC){
	if(wr_RC == 1){
		return identificaComando(comando);
	}
}

