/******** UFAM - UNIVERSIDADE FEDERAL DO AMAZONAS ********
 ******** ARQUITETURA DE SISTEMAS DIGITAIS ********
* Equipe: Elaine Soares
 * 		  Felipe Santos
 * 		  Leonam Bernardo
 * 		  Rodrigo Santos
 */

//Include do Serial Display
#include <dsf_SerialDisplays.h>
#include <mkl_PIT.h>
#include <mkl_PITPeriodicInterrupt.h>
#include <mkl_GPIOPort.h>
#include <stdint.h>

//Include da Rotina de Interrup��o
#ifndef C__USERS_DIOGO_CPPLINT_MASTER_MAIN_CPP_
#define C__USERS_DIOGO_CPPLINT_MASTER_MAIN_CPP_
#include "MKL25Z4.h"
#include <gpio.h>
#include <mkl_PITDelay.h>

//Include tpm_Delay
#include <mkl_TPMDelay.h>

//Include Liga Desliga
#include <LigaDesliga.h>

//Include Temporizador
#include <Temporizador.h>

//Include MUX
#include <MuxCanais.h>

//Defini��o dos objetos GPIO obs.:

//Botoes
mkl_GPIOPort sleep_T(gpio_PTA1);
mkl_GPIOPort rst_T(gpio_PTA2);
mkl_GPIOPort b_onoff(gpio_PTD4);

// Leds

//dsf_SerialDisplays disp(gpio_PTC7, gpio_PTC0,gpio_PTC3);

//Variaveis

Temporizador temp;
MuxCanais mux;


/*!
 *  Metodos construtores das classes usadas.
 */
mkl_PITInterruptInterrupt pit(PIT_Ch0);
dsf_SerialDisplays disp(gpio_PTA13, gpio_PTD5, gpio_PTD0);
mkl_TPMDelay tpm(tpm_TPM0);
LigaDesliga ld(gpio_PTB19, gpio_PTD1);


//mkl_GPIOPort a(gpio_PTB19);

/*!
 *  Defini��o do objeto led a ser usado.
 */
Gpio led;											//PIT


/*!
 *  Configura��o do PIT para gerar interrupi��es periodicas.
 */
void setup_PIT() {
	pit.enablePeripheralModule();
	pit.setPeriod(0x4E20); 			//10 milisegundos
	pit.resetCounter();
	pit.enableTimer();
	pit.enableInterruptRequests();
}

// Configura��o dos objetos de GPIO
void setup_GPIO() {
	rst_T.setPortMode(gpio_input);
	rst_T.setPullResistor(gpio_pullUpResistor);
	sleep_T.setPortMode(gpio_input);
	sleep_T.setPullResistor(gpio_pullUpResistor);
	b_onoff.setPortMode(gpio_input);
	b_onoff.setPullResistor(gpio_pullUpResistor);
}

void DebounceReset(){
	tpm.waitDelay(0x1332);
	temp.reset();
}

void DebounceSleep(){
	tpm.waitDelay(0x1332);
	NVIC_DisableIRQ(PIT_IRQn); 		// desabilita interrup��o do PIT
	temp.sleep();
	NVIC_EnableIRQ(PIT_IRQn); 		// habilita interrup��o do PIT
	ld.cont = 0;
}
/*!
 *  Rotina de Servi�o de Interrupi��o (ISR) do PIT.
 */
extern "C" {
  void PIT_IRQHandler(void) {
	  disp.updateDisplays();
	  disp.hideZerosRight();
	  disp.hideZerosLeft();
	  pit.clearInterruptFlag();
	  ld.cont++;
	  if(ld.cont >= 500){
		  temp.decrementa();
		  ld.cont = 0;
	  }
  }
}


void startTemporizador()
{
		tpm.setFrequency(tpm_div128);
		disp.clearDisplays();
		temp.reset();
		ld.min = temp.minutos();
		ld.tem = 0;
}

void rotinaTemporizador(LigaDesliga ld, mkl_TPMDelay tpm, dsf_SerialDisplays disp)
{
	uint8_t flag = 0;
	if(!b_onoff.readBit()){
		while(!b_onoff.readBit()){}
		tpm.waitDelay(0x1332);
		flag = ~flag;
	};

	if(flag){
		if(!rst_T.readBit()){
			while(!rst_T.readBit()){}
			DebounceReset();
		}
		if(!sleep_T.readBit()){
			while(!sleep_T.readBit()){}
			DebounceSleep();
		}
		//novafuncao();
		ld.Liga(temp.minutos(), flag, temp.ledTmrOn());
		disp.writeWord(ld.tempo);
		if(temp.disable()){
			flag = false;
		}
	} else {
		ld.Desliga();
		temp.reset();
		disp.clearDisplays();
	}
}


int main() {
	setup_PIT();
	setup_GPIO();
	startTemporizador();

	while(1){
		rotinaTemporizador(ld, tmp, disp);


	}


	return 0;

}

#endif  // C__USERS_DIOGO_CPPLINT_MASTER_MAIN_CPP_
