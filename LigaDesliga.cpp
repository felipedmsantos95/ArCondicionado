/*
 * LigaDesliga.cpp
 *
 *  Created on: 31/08/2017
 *      Author: leona
 */

#include "LigaDesliga.h"
#include <mkl_GPIOPort.h>
#include <gpio.h>


LigaDesliga::LigaDesliga(gpio_Pin Led_TmrOn, gpio_Pin Led_OnOff) {
	// TODO Auto-generated constructor stub
	tmrOn_T = mkl_GPIOPort(Led_TmrOn);
	onoff_T = mkl_GPIOPort(Led_OnOff);

	tmrOn_T.setPortMode(gpio_output); //Led indicador do funcionamento do Temporizador
	onoff_T.setPortMode(gpio_output); //Led de standby
}

LigaDesliga::~LigaDesliga() {
	// TODO Auto-generated destructor stub
}

/*
 * Realiza debounce dos botoes reset e sleep
 * Habilita/desabilita leds de status
 * Atualiza tempo de contagem
 */
//void LigaDesliga::Liga(mkl_TPMDelay tpm, Temporizador temp, mkl_GPIOPort rst_T, mkl_GPIOPort sleep_T){
void LigaDesliga::Liga(int t, uint8_t led1, int led2){

	tmrOn_T.writeBit(led1); 	//Aciona ou não o led temporizador: depende da variavel temp.cont
	onoff_T.writeBit(!led2);				//Aciona o led on_off
	min = t; 				//Atualiza a variavel min com o tempo do Temporizador

	tempo = 100*min + tem;
}

/*
 * Coloca o circuito no modo desligado
 */
void LigaDesliga::Desliga(){
	tmrOn_T.writeBit(0); 	//Aciona ou não o led temporizador: modo standby
	onoff_T.writeBit(1); 	//Atualização do led on_off: "o Ar Condicionado está desligado"
//	temp.reset();			//Reseta o contador do Temporizador
}

