#ifndef dsf_Mux_H
#define dsf_Mux_H

#include <stdint.h>


class dsf_Mux {

public:
	uint8_t minutos;
	uint8_t tp;
	bool selecionar;

	void mux();
	uint8_t select(uint8_t minutos, uint8_t tp, bool selecionar);

};





#endif
