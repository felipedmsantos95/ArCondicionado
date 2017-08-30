/*!
 * @copyright   © 2017 UFAM - Universidade Federal do Amazonas.
 *
 * @brief    Interface de programação de aplicações em C++ para GPIO
 * @file     gpio.h
 * @version  1.0
 * @date     6 Julho 2017
 *
 * @section	 HARDWARES & SOFTWARES
 *            +processor MKL25Z128VLK4 -ARM Cortex-M0+.
 *            +compile   Kinetis® Design Studio IDE.
 *            +manual    L25P80M48SF0RM, Rev.3, September2012.
 *            +revisions
 *              ++ 1.0 (6 Julho 2017): Versão inicial.
 *
 * @section  AUTHORS & DEVELOPERS
 *            +institution Universidade Federal do Amazonas.
 *            +courses     Eng. Elétrica
 *            +teacher     Miguel Grimm <miguelgrimm@gmail.com>
 *            +student     Versão inicial:
 * 	            ++ Jose Luis da Silva <joseluis70123@gmail.com>
 *
 * @section  LICENSE
 *
 *           GNU General Public License (GNU GPL).
 *
 *           Este programa é um software livre; Você pode redistribuí-lo
 *           e/ou modificá-lo de acordo com os termos do "GNU General Public
 *           License" como publicado pela Free Software Foundation; Seja a
 *           versão 3 da licença, ou qualquer versão posterior.
 *
 *           Este programa é distribuído na esperança de que seja útil,
 *           mas SEM QUALQUER GARANTIA; Sem a garantia implícita de
 *           COMERCIALIZAÇÃO OU USO PARA UM DETERMINADO PROPÓSITO.
 *           Veja o site da "GNU General Public License" para mais detalhes.
 *
 * @htmlonly http://www.gnu.org/copyleft/gpl.html
 */
 
#ifndef C__USERS_DIOGO_CPPLINT_MASTER_GPIO_H_
#define C__USERS_DIOGO_CPPLINT_MASTER_GPIO_H_


#define sim_scgc5 (*((volatile unsigned int *)0x40048038))

#define portb_pcr18 (*((volatile unsigned int *)0x4004A048))
#define gpiob_pddr (*((volatile unsigned int *)0x400FF054))
#define gpiob_pdor (*((volatile unsigned int *)0x400FF040))
#define gpiob_ptor (*((volatile unsigned int *)0x400FF04C))


class Gpio {
 public:
	  /*!
	             * @fn			init
	             *
	             * @brief		inicia o led RGB de uma determinada cor para uso.
	             * @details		habiita os registradores de habilitação da classe GPIO.
	             */
  void init() {
    sim_scgc5 |= 0x400;
    portb_pcr18 = 0x100;
    gpiob_pddr |= 0x40000;
    gpiob_ptor |= 0x40000;
}
  /*!
  	             * @fn			invertCurrentState
  	             *
  	             * @brief		inverte o estado atual do led habilitado
  	             * @details		habiilita o registrador que atruibui valor ao terminal do led RGB
  	             */

  void invertCurrentState() {
    gpiob_ptor |= 0x40000;
}
};
#endif  // C__USERS_DIOGO_CPPLINT_MASTER_GPIO_H_

