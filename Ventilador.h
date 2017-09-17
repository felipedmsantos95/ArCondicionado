/*
 * Ventilador.h
 *
 *  Created on: 17/09/2017
 *      Author: felipedmsantos
 */


#ifndef SOURCES_VENTILADOR_H_
#define SOURCES_VENTILADOR_H_

#include "mkl_TPMPulseWidthModulation.h"

typedef enum {
  vel1 = 200,
  vel2 = 500,
  vel3 = 800

}vel;



class Ventilador {
public:
	Ventilador();
	virtual ~Ventilador();
	void selecionaVel(vel v, mkl_TPMPulseWidthModulation pwm);


};

#endif /* SOURCES_VENTILADOR_H_ */
