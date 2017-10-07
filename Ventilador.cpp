/*
 * Ventilador.cpp
 *
 *  Created on: 17/09/2017
 *      Author: felipedmsantos
 */

#include <Ventilador.h>
#include "mkl_TPM.h"
#include "mkl_TPMPulseWidthModulation.h"



Ventilador::Ventilador(tpm_Pin pin) {
	this->ventPwm = mkl_TPMPulseWidthModulation(pin);
}

Ventilador::~Ventilador() {
	// TODO Auto-generated destructor stub
}

void Ventilador::desligaVel(){
	selecionaVel(vel0);
	selVel = 0;
}

void Ventilador::mantemVel(){
	switch(selVel){ //O resto da divisão por 3 de selVel irá determinar qual velocidade
		case 1:
			this->selecionaVel(vel1);
			break;
		case 2:
			this->selecionaVel(vel2);
			break;
		case 3:
			this->selecionaVel(vel3);
			break;
	}
}

void Ventilador::selecionaVel(vel v) {

	ventPwm.setFrequency(tpm_div16, 999);

	switch(v) {
	    case vel0:
	    	ventPwm.setDutyCycle(vel0);
	    	ventPwm.enableOperation();
	    	break;
		case vel1:
			ventPwm.setDutyCycle(vel1);
			ventPwm.enableOperation();
			break;
		case vel2:
			ventPwm.setDutyCycle(vel2);
			ventPwm.enableOperation();
			break;
		case vel3:
			ventPwm.setDutyCycle(vel3);
			ventPwm.enableOperation();
			break;

	}


}

//Essa funçao se colocada no loop da main irá sempre buscar se a tecla de aumento de velocidade foi pressionada para mudar a velocidade do controlador
void Ventilador::aumentaVel(){//, mkl_RemoteControl rc){
		selVel++;
		if(selVel == 4) selVel = 1;
		this->mantemVel();

}

void Ventilador::diminuiVel(){//, mkl_RemoteControl rc){

	uint8_t cmd = 0x00;

	if(selVel == 0)
		this->selecionaVel(vel1);

		switch(selVel%3){ //O resto da divisão por 3 de selVel irá determinar qual velocidade
			case 0:
				this->selecionaVel(vel1);
				break;
			case 1:
				this->selecionaVel(vel2);
				break;
			case 2:
				this->selecionaVel(vel3);
				break;
		}

}

