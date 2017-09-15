/*
 * MuxCanais.h
 *
 *  Created on: Aug 31, 2017
 *      Author: Rodrigo
 */

#ifndef MUXCANAIS_H
#define MUXCANAIS_H

#include <stdint.h>

class MuxCanais {
public:
	MuxCanais();
	virtual ~MuxCanais();

	int minutos;
	int tp;

	void mux();
	int select(int minutos, int tp);

};

#endif /* MUXCANAIS_H */
