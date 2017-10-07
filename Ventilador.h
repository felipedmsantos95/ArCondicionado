/*
 * Ventilador.h
 *
 *  Created on: 17/09/2017
 *      Author: felipedmsantos
 */


#ifndef SOURCES_VENTILADOR_H_
#define SOURCES_VENTILADOR_H_

#include "mkl_TPMPulseWidthModulation.h"
#include "mkl_RemoteControl.h"
#include "mkl_TPM.h"

typedef enum {
  vel0 = 0,
  vel1 = 400,
  vel2 = 650,
  vel3 = 999

}vel;



class Ventilador {
public:
	Ventilador(tpm_Pin pin);
	virtual ~Ventilador();
	void desligaVel();
	void mantemVel();
	void selecionaVel(vel v);
	void aumentaVel();//, mkl_RemoteControl rc);
	void diminuiVel();//, mkl_RemoteControl rc);
	int selVel = 0;
	mkl_TPMPulseWidthModulation ventPwm;

};

#endif /* SOURCES_VENTILADOR_H_ */
