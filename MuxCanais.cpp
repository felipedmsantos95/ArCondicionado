/*
 * MuxCanais.cpp
 *
 *  Created on: Aug 31, 2017
 *      Author: Rodrigo
 */

#include <MuxCanais.h>

MuxCanais::MuxCanais() {
	// TODO Auto-generated constructor stub

}

MuxCanais::~MuxCanais() {
	// TODO Auto-generated destructor stub
}

void MuxCanais::mux(){
	this->minutos = minutos;
	this->tp = tp;

}

int MuxCanais::select(int minutos, int tp){
	if(minutos != 0)
		return minutos;
	else
		return tp;
}

