#ifndef _TEMPORIZADOR_
#define _TEMPORIZADOR_

#include "MKL25Z4.h"
#include <stdint.h>

/*!
 *  @class    Temporizador
 *
 *  @brief    A classe Temporizador representa o bloco Temporizador de Minutos do Ar Condicionado Digital
 *
 *  @details  Esta classe realiza as diversas funções de incrementação e decrementação do tempo que o Ar Condicionado fica ligado
 *
 *  @section  EXAMPLES USAGE
 *
 *      Uso dos metodos para escrita
 *        +fn void decrementa();
 *	      +fn void reset();
 *        +fn void sleep();
 *	      +fn int minutos()
 *	      +fn int disable();
 *	      +fn int ledTmrOn();
 */

class Temporizador {
public:
	void decrementa();
	void reset();
	void sleep();
	int minutos();
	int disable();
	int ledTmrOn();
	int cont;
	int off;
};

 #endif





