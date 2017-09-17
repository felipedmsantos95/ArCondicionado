/*
 * Ventilador.cpp
 *
 *  Created on: 17/09/2017
 *      Author: felipedmsantos
 */

#include <Ventilador.h>

#include "mkl_TPMPulseWidthModulation.h"



Ventilador::Ventilador() {
	// TODO Auto-generated constructor stub

}

Ventilador::~Ventilador() {
	// TODO Auto-generated destructor stub
}

void Ventilador::selecionaVel(vel v, mkl_TPMPulseWidthModulation pwm) {

	pwm.setFrequency(tpm_div16, 999);


	switch(v) {
		case vel1:
			pwm.setDutyCycle(vel1);
			pwm.enableOperation();
			break;
		case vel2:
			pwm.setDutyCycle(vel2);
			pwm.enableOperation();
			break;
		case vel3:
			pwm.setDutyCycle(vel3);
			pwm.enableOperation();
			break;

	}
}

