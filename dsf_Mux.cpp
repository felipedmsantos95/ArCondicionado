#include "dsf_Mux.h"


void dsf_Mux::mux(){
	this->minutos = minutos;
	this->tp = tp;
	this->selecionar;

}

uint8_t dsf_Mux::select(uint8_t minutos, uint8_t tp, bool selecionar){
	if(selecionar)
		return minutos;
	else
		return tp;
}

