/*!
 * @copyright   © 2017 Universidade Federal do Amazonas.
 *
 * @brief       Implementação da API de interrupção em C++ para GPIO.
 *
 * @file        mkl_GPIOInterrupt.cpp
 * @version     1.0
 * @date        6 Julho 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+
 *              +compiler     Kinetis® Design Studio IDE
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012
 *              +revisions    Versão (data): Descrição breve.
 *                             ++ 1.0 (30 Julho 2017): Versão inicial.
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas
 *              +courses      Engenharia da Computação / Engenharia Elétrica
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Versão inicial:
 *                             ++ José Luis da Silva <joseluis70123@gmail.com>
 *
 * @section     LICENSE
 *
 *              GNU General Public License (GNU GPL)
 *
 *              Este programa é um software livre; Você pode redistribuí-lo
 *              e/ou modificá-lo de acordo com os termos do "GNU General Public
 *              License" como publicado pela Free Software Foundation; Seja a
 *              versão 3 da licença, ou qualquer versão posterior.
 *
 *              Este programa é distribuído na esperança de que seja útil,
 *              mas SEM QUALQUER GARANTIA; Sem a garantia implícita de
 *              COMERCIALIZAÇÃO OU USO PARA UM DETERMINADO PROPÓSITO.
 *              Veja o site da "GNU General Public License" para mais detalhes.
 *
 * @htmlonly    http://www.gnu.org/copyleft/gpl.html
 */

#include "mkl_GPIOInterrupt.h"
/*!
 *   @fn       mkl_GPIOInterrupt
 *
 *   @brief    Construtor padrão da classe.
 *
 *   O construtor padrãa classe obtém o número do pino e do GPIO a partir
 *   do pino desejado, associa fisicamento o objeto de software ao periféco
 *   de hardware, habilita o clock do GPIO e seleciona o modo GPIO de operação
 *   do pino.
 *
 *   @remarks  Siglas e páginas do Manual de Referêia KL25:
 *             - PortxPCRn: Pin Control Register.Pág. 183 (Mux) and 185 (Pull).
 */

mkl_GPIOInterrupt::mkl_GPIOInterrupt(gpio_Name GPIOName, gpio_Pin pin){
  pinPort = 1 << pin;
  bindPeripheral(GPIOName, pin);
  enableModuleClock(GPIOName);
  selectMuxAlternative();
}
/*!
 *   @fn         clearInterruptFlag
 *
 *   @brief      Limpa a flag de interrupção
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *                - PortxPCRn: Pin Control Register.Pág. 183(Mux) and 185(Pull).
 */
void mkl_GPIOInterrupt::clearInterruptFlag(){
  *port_pcr_isfr = 0x00000002;
}
/*!
 *   @fn         enableInterrupt
 *
 *   @brief      habilita as interrupções das GPIOS para operar.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - PortxPCRn: Pin Control Register.Pág. 183 (Mux) and 185 (Pull).
 */
void mkl_GPIOInterrupt::enableInterrupt(){
  *addressPortxPCRn &= ~0xF0000;
  *addressPortxPCRn |= 0xA0000;
}
/*!
 *   @fn         disableInterrupt
 *
 *   @brief      Desabilita as interrupções das GPIOS para pararem de funcionar.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - PortxPCRn: Pin Control Register.Pág. 183 (Mux) and 185 (Pull).
 */
void mkl_GPIOInterrupt::disableInterrupt(){
  *addressPortxPCRn |= 0x00000;
}
