/*
 * LigaDesliga.h
 *
 *  Created on: 31/08/2017
 *      Author: leona
 */

#ifndef _LIGADESLIGA_
#define _LIGADESLIGA_

#include <mkl_GPIOPort.h>
#include <Temporizador.h>
#include <mkl_TPMDelay.h>
#include <gpio.h>

/*!
 *  @class    LigaDesliga
 *
 *  @brief    A classe LigaDesliga representa o bloco LigaDesliga do Ar Condicionado Digital
 *
 *  @details  Esta classe realiza as funções de debounce dos botoes sleep, reset e funções do bloco OnOff
 *
 *  @section  EXAMPLES USAGE
 *
 *      Uso dos metodos para escrita
 *        +fn LigaDesliga(gpio_Pin Led_TmrOn, gpio_Pin Led_OnOff);
 *		  +fn void Liga(mkl_TPMDelay tpm, Temporizador temp, mkl_GPIOPort rst_T, mkl_GPIOPort sleep_T);
 *	      +fn void Desliga(Temporizador temp);
 */

class LigaDesliga {
public:
	LigaDesliga(gpio_Pin Led_TmrOn, gpio_Pin Led_OnOff);
	virtual ~LigaDesliga();
	void Liga(int temp, uint8_t led1, int led2);
	void Desliga();

	mkl_GPIOPort tmrOn_T;
	mkl_GPIOPort onoff_T;

	int cont;
	int tempo;
	int min;
	int tem;

};

#endif /* LIGADESLIGA_H */
