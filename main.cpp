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
//Include Ventilador
#include "Ventilador.h"
//Include PWM
#include "mkl_TPMPulseWidthModulation.h"
#include "mkl_DHT11Sensor.h"


mkl_GPIOPort sleep_T(gpio_PTA1);
mkl_GPIOPort rst_T(gpio_PTA2);
mkl_GPIOPort b_onoff(gpio_PTD4);
mkl_GPIOPort fan_T(gpio_PTA12);


Temporizador temp;
MuxCanais mux;
Ventilador vent(tpm_PTD3);

mkl_DHT11Sensor dht11(tpm_TPM1, gpio_PTC1);
int temperatura;
uint8_t umidade;
dht11_Exception excecao;

mkl_PITInterruptInterrupt pit(PIT_Ch0);
dsf_SerialDisplays disp(gpio_PTA13, gpio_PTD5, gpio_PTD0);
mkl_TPMDelay tpm(tpm_TPM0);
LigaDesliga ld(gpio_PTB19, gpio_PTD1);

void setup_PIT() {
	pit.enablePeripheralModule();
	pit.setPeriod(0x4E20); 			//10 milisegundos
	pit.resetCounter();
	pit.enableTimer();
	pit.enableInterruptRequests();
}

void setup_GPIO() {
	rst_T.setPortMode(gpio_input);
	rst_T.setPullResistor(gpio_pullUpResistor);
	sleep_T.setPortMode(gpio_input);
	sleep_T.setPullResistor(gpio_pullUpResistor);
	b_onoff.setPortMode(gpio_input);
	b_onoff.setPullResistor(gpio_pullUpResistor);
	fan_T.setPortMode(gpio_input);
	fan_T.setPullResistor(gpio_pullUpResistor);
}

void DebounceReset(){
	tpm.waitDelay(0x1332);//(0x1332);
	temp.reset();
}

void DebounceSleep(){
	tpm.waitDelay(0x1332);//(0x1332);
	NVIC_DisableIRQ(PIT_IRQn);
	temp.sleep();
	NVIC_EnableIRQ(PIT_IRQn);
	ld.cont = 0;
}

void DebounceFan(){
	tpm.waitDelay(0x1332);//(0x1332);
	vent.aumentaVel();
}

extern "C" {
  void PIT_IRQHandler(void) {
	  disp.updateDisplays();
	 // disp.hideZerosRight();
	  disp.hideZerosLeft();
	  pit.clearInterruptFlag();
	  ld.cont++;
	  if(ld.cont >= 500){
		  temp.decrementa();
		  ld.cont = 0;
	  }
  }
}

int main() {
	setup_PIT();
	setup_GPIO();
	mkl_DHT11Sensor dht11(tpm_TPM1, gpio_PTC1);

	excecao = dht11.doAcquisition();
	uint8_t flag = 0;
	tpm.setFrequency(tpm_div128);
	disp.clearDisplays();
	temp.reset();
	ld.min = temp.minutos();
	ld.tem = 0;
	while (1) {
		excecao = dht11.doAcquisition();
		dht11.readTemperature(&temperatura);
		if(!b_onoff.readBit()){
			while(!b_onoff.readBit()){}
			tpm.waitDelay(0x1332);//(0x1332);
			flag = ~flag;
			//vent.selVel = 0;
			vent.aumentaVel();
		}

		if(flag){
			if(!rst_T.readBit()){
				while(!rst_T.readBit()){}
				DebounceReset();
			}
			if(!sleep_T.readBit()){
				while(!sleep_T.readBit()){}
				DebounceSleep();
			}
			if(!fan_T.readBit()){
				while(!fan_T.readBit()){}
				DebounceFan();
			}
			//novafuncao();
			vent.mantemVel();
			ld.Liga(temp.minutos(), flag, temp.ledTmrOn());
			int mostra = ld.tempo + temperatura;
			disp.writeWord(mostra);
			if(temp.disable()){
				flag = false;
			}
		} else {
			ld.Desliga();
			temp.reset();
			vent.desligaVel();
			disp.clearDisplays();
		}

	}

	return 0;

}

#endif
