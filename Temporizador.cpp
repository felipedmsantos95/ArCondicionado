#include "Temporizador.h"
#include <stdint.h>

/*!
 * Incrementa o contador (cont) em 10, caso cont seja multiplo de 10
 * Caso contrario, variavel é incrementada para o proximo multiplo de 10
 * Se o contador for igual a 90, a variavel volta para 0
 */
void Temporizador::sleep(){  // dec = 0 crescente
	int div;
	div = cont%10;

	if(cont == 90)
		cont = 0;
	else{
		if(div == 0)
			cont = cont + 10;
		else
			cont = cont - div + 10;
	}
}

/*!
 * Verifica se o contador foi para o valor zero pela decrementação da variavel
 */
int Temporizador::disable(){
	if(cont == 1){
		off = 1;
	}
	if(cont == 0 and off == 1){
		off = 0;
		return 1;
	}
	else{
		return 0;
	}

}

/*!
 * Liga ou desliga o led que indica que o temporizador eestá ligado
 */
int Temporizador::ledTmrOn(){
	if(cont == 0){
			return 0;
		}
	else {
			return 1;
	}
}

/*!
 * Retorna o valor do contador
 */
int Temporizador::minutos(){
	return cont;
}

/*!
 * Decrementa a variável cont
 */
void Temporizador::decrementa(){
	if(cont > 0){
		cont = cont - 1;
	}
	else{
		cont = 0;
	}
}

/*!
 * Reseta a variavel cont
 */
void Temporizador::reset(){
	cont = 0;
}

