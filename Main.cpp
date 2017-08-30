/*@copyright   � 2017 UFAM - Universidade Federal do Amazonas.
 *
 * @brief       Exemplo de uso da classe dsf_SerialDisplays.
 *
 * @file        main.cpp
 * @version     1.0
 * @date        2 Agosto 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+
 *              +compiler     Kinetis� Design Studio IDE
 *              +peripherals  GPIO, PIT e M�dulo 74HC595 com Display 4 D�gitos.
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012
 *              +revisions    Vers�o (data): Descri��o breve.
 *                             ++ 0.9 (17 Julho 2017): Vers�o inicial.
 *                             ++ 1.0 (2 Agosto 2017): Generaliza��o dos perif�ricos
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas
 *              +courses      Engenharia El�trica / Engenharia da Computa��o.
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Vers�o inicial:
 *                             ++ Magno Aguiar de Carvalho <carvalhodemagno@gmail.com>
 *                             ++ Phillipp de Souza Gama Lavor <lavorphillipp@gmail.com>
 *                             ++ Jadir Campos Barbosa Junior <jadirjr96@gmail.com>
 *              +student      Revis�es - Nome revisor <e-mail do revisor>
 *                             ++v1.0 - Jose Luis da Silva e Silva <joseluis70123@gmail.com>
 *
 * @section     LICENSE
 *
 *              GNU General Public License (GNU GPL).
 *
 *              Este programa � um software livre; Voc� pode redistribu�-lo
 *              e/ou modific�-lo de acordo com os termos do "GNU General Public
 *              License" como publicado pela Free Software Foundation; Seja a
 *              vers�o 3 da licen�a, ou qualquer vers�o posterior.
 *
 *              Este programa � distribu�do na esperan�a de que seja �til,
 *              mas SEM QUALQUER GARANTIA; Sem a garantia impl�cita de
 *              COMERCIALIZA��O OU USO PARA UM DETERMINADO PROP�SITO.
 *              Veja o site da "GNU General Public License" para mais detalhes.
 *
 * @htmlonly    http://www.gnu.org/copyleft/gpl.html
 */

//Include do Serial Display
#include <dsf_SerialDisplays.h>
#include <mkl_PIT.h>
#include <mkl_PITPeriodicInterrupt.h>
#include <mkl_GPIOPort.h>
#include <stdint.h>

//Include da Rotina de Interrup��o
#ifndef C__USERS_DIOGO_CPPLINT_MASTER_MAIN_CPP_
#define C__USERS_DIOGO_CPPLINT_MASTER_MAIN_CPP_
#include "MKL25Z4.h"
#include <gpio.h>
#include <mkl_PITDelay.h>


/*!
 *  Metodos construtores das classes usadas.
 */
mkl_PITInterruptInterrupt pit(PIT_Ch0);
dsf_SerialDisplays disp(gpio_PTA1, gpio_PTA2,gpio_PTA4);


//mkl_GPIOPort a(gpio_PTB19);

/*!
 *  Defini��o do objeto led a ser usado.
 */
Gpio led;											//PIT


/*!
 *  Configura��o do PIT para gerar interrupi��es periodicas.
 */
  int setupPIT() {
	 pit.enablePeripheralModule();
	 pit.setPeriod(0x4e20);
}

  /*!
   *  Configura��o dos objetos PIT e Led
   */
  void setup() {
    led.init();
    pit.enablePeripheralModule();
    pit.setPeriod(0x1406F3F);
  }


/*!
 *  Rotina de Servi�o de Interrupi��o (ISR) do PIT.
 *  Atualiza as informa�oes dos displays
 */
extern "C" {
  void PIT_IRQHandler(void) {
    disp.updateDisplays();

  }
}

//Usado na Classe PIT, tem que fazer um merge??

/*!
 *  Defini��o da rotina de interrup��o do PIT

extern "C" {
  void PIT_IRQHandler(void) {
    pit.clearInterruptFlag();
    led.invertCurrentState();
  }
}*/


int main() {
  /*!
   *  Configura PIT para gerar interrupi�oes a cada 10 ms.
   */
   setupPIT();
  pit.resetCounter();
  pit.enableTimer();
  pit.enableInterruptRequests();
  /*!
   *  Escreve no display o numero "2019".
   */
  disp.clearDisplays();
  disp.writeWord(2300);
  disp.hideZerosRight();

//PitDesable??

  //Fun��o Main do PIT (para testar funcionalidades separadas)
  setup();
  pit.resetCounter();
  pit.enableTimer();
  led.invertCurrentState();
  pit.enableInterruptRequests();
  while (1) {
    pit.waitInterruptFlag();
  }

  pit.disableInterruptRequests();

  return 0;
}

#endif  // C__USERS_DIOGO_CPPLINT_MASTER_MAIN_CPP_
