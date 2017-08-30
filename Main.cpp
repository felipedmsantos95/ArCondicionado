/*@copyright   ï¿½ 2017 UFAM - Universidade Federal do Amazonas.
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
 *              +compiler     Kinetisï¿½ Design Studio IDE
 *              +peripherals  GPIO, PIT e Mï¿½dulo 74HC595 com Display 4 Dï¿½gitos.
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012
 *              +revisions    Versï¿½o (data): Descriï¿½ï¿½o breve.
 *                             ++ 0.9 (17 Julho 2017): Versï¿½o inicial.
 *                             ++ 1.0 (2 Agosto 2017): Generalizaï¿½ï¿½o dos perifï¿½ricos
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas
 *              +courses      Engenharia Elï¿½trica / Engenharia da Computaï¿½ï¿½o.
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Versï¿½o inicial:
 *                             ++ Magno Aguiar de Carvalho <carvalhodemagno@gmail.com>
 *                             ++ Phillipp de Souza Gama Lavor <lavorphillipp@gmail.com>
 *                             ++ Jadir Campos Barbosa Junior <jadirjr96@gmail.com>
 *              +student      Revisï¿½es - Nome revisor <e-mail do revisor>
 *                             ++v1.0 - Jose Luis da Silva e Silva <joseluis70123@gmail.com>
 *
 * @section     LICENSE
 *
 *              GNU General Public License (GNU GPL).
 *
 *              Este programa ï¿½ um software livre; Vocï¿½ pode redistribuï¿½-lo
 *              e/ou modificï¿½-lo de acordo com os termos do "GNU General Public
 *              License" como publicado pela Free Software Foundation; Seja a
 *              versï¿½o 3 da licenï¿½a, ou qualquer versï¿½o posterior.
 *
 *              Este programa ï¿½ distribuï¿½do na esperanï¿½a de que seja ï¿½til,
 *              mas SEM QUALQUER GARANTIA; Sem a garantia implï¿½cita de
 *              COMERCIALIZAï¿½ï¿½O OU USO PARA UM DETERMINADO PROPï¿½SITO.
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

//Include da Rotina de Interrupção
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
 *  Definição do objeto led a ser usado.
 */
Gpio led;											//PIT


/*!
 *  Configuração do PIT para gerar interrupições periodicas.
 */
  int setupPIT() {
	 pit.enablePeripheralModule();
	 pit.setPeriod(0x4e20);
}

  /*!
   *  Configuração dos objetos PIT e Led
   */
  void setup() {
    led.init();
    pit.enablePeripheralModule();
    pit.setPeriod(0x1406F3F);
  }


/*!
 *  Rotina de Serviço de Interrupição (ISR) do PIT.
 *  Atualiza as informaçoes dos displays
 */
extern "C" {
  void PIT_IRQHandler(void) {
    disp.updateDisplays();

  }
}

//Usado na Classe PIT, tem que fazer um merge??

/*!
 *  Definição da rotina de interrupção do PIT

extern "C" {
  void PIT_IRQHandler(void) {
    pit.clearInterruptFlag();
    led.invertCurrentState();
  }
}*/


int main() {
  /*!
   *  Configura PIT para gerar interrupiçoes a cada 10 ms.
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

  //Função Main do PIT (para testar funcionalidades separadas)
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
