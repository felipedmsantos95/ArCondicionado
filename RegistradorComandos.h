/*
 * RegistradorComandos.h
 *
 *  Created on: Sep 20, 2017
 *      Author: Rodrigo
 */

#ifndef SOURCES_REGISTRADORCOMANDOS_H_
#define SOURCES_REGISTRADORCOMANDOS_H_

class RegistradorComandos {
public:
	RegistradorComandos();
	virtual ~RegistradorComandos();

	int comando;
	void armazenaComando(int dt_IR);
	int identificaComando(int comando);
	int enviaComando(int wr_RC);

};

#endif /* SOURCES_REGISTRADORCOMANDOS_H_ */
