/*!
 * @copyright   © 2017 Universidade Federal do Amazonas.
 *
 * @brief       Implementação da classe mãe "mkl_TPMMeasure".
 *
 * @file        mkl_TPMMeasure.cpp
 * @version     1.0
 * @date        06 Agosto 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+.
 *              +compiler     Kinetis® Design Studio IDE.
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012.
 *              +revisions    Versão (data): Descrição breve.
 *                             ++ 1.0 (06 Agosto 2017): Versão inicial.
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas.
 *              +courses      Engenharia da Computação / Engenharia Elétrica.
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Versão inicial:
 *                             ++ Hamilton Nascimento <hdan_neto@hotmail.com>
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

#include <stdint.h>
#include <MKL25Z4.h>
#include "mkl_TPMMeasure.h"

/*!
 *   @fn         mkl_TPMMeasure.
 *
 *   @brief      Construtor da classe mkl_TPMMeasure.
 *
 *   Este método é o construtor padrão da classe, que realiza a associação
 *   física do periférico ao objeto de software, inicializa o periférico e os
 *   pinos associados.
 */


mkl_TPMMeasure::mkl_TPMMeasure(tpm_Pin pin) {
  uint8_t muxAltMask;
  uint8_t pinNumber;
  uint8_t GPIONumber;
  uint8_t chnNumber;
  uint8_t TPMNumber;
  uint8_t *baseAddress;

  setTPMParameters(pin, pinNumber, GPIONumber, chnNumber,
                   TPMNumber, muxAltMask);
  setBaseAddress(TPMNumber, &baseAddress);
  bindPeripheral(baseAddress);
  bindChannel(baseAddress, chnNumber);
  bindPin(GPIONumber, pinNumber);
  enablePeripheralClock(TPMNumber);
  enableGPIOClock(GPIONumber);
  selectMuxAlternative(muxAltMask);
}
mkl_TPMMeasure::mkl_TPMMeasure(){

}

/*!
 *   @fn         setFrequency.
 *
 *   @brief      Ajusta a frequência de operação.
 *
 *   Ajusta a frequência de operação para valores 1/2, 1/4, 1/8, 1/16,
 *   1/32, 1/64, 1/128 da frequência base (20,94 MHz).
 *
 *   @param[in]  divBase - fator de divisão.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. Pág.552.
 */
void mkl_TPMMeasure::setFrequency(tpm_Div divBase) {
  /*!
   * Desabilita medição (recomendação do fabricante).
   */
  disableMeasure();
  /*!
   * Ajusta a frequência.
   */
  *addressTPMxSC = divBase;
}

/*!
 *   @fn         setEdge.
 *
 *   @brief      Ajusta a borda de operação.
 *
 *   Ao detectar a borda indicada no parâmetro, para a contagem.
 *
 *   @param[in]  edge - borda de operação.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxMOD: Modulo Register. Pág. 554.
 *               - TPMxCnSC: Channel Status and Control Register. Pág.555.
 */
void mkl_TPMMeasure::setEdge(tpm_Edge edge) {
  /*!
   * Desabilita medição (recomendação do fabricante).
   */
  disableMeasure();
  /*!
   * Ajusta o fundo de escala do contador para 0xFFFF.
   */
  *addressTPMxMOD = 0xFFFF;
  /*!
   * Ajusta a borda de detecção no registrador CnSC.
   */
  *addressTPMxCnSC = edge << 2;
}

/*!
 *   @fn         enableMeasure.
 *
 *   @brief      Habilita a operação de medição.
 *
 *   Este método inicia a medição do tempo em que o sinal de entrada permanece
 *   em nível alto ou baixo, conforme o parâmetro "edge".
 *   Note que este método não "prende", i.e., o programa não fica preso neste
 *   método até a identificação da borda indicada.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. Pág.552.
 */
void mkl_TPMMeasure::enableMeasure() {
  /*!
   * Habilita a contagem escrevendo "10" no campo CMOD (Clock Mode Selection)
   * do registrador SC.
   */
  *addressTPMxSC |= 0x08;
}

/*!
 *   @fn         waitMeasure.
 *
 *   @brief      Habilita a operação de medição e aguarda a transição indicada.
 *
 *   Este método inicia a medição do tempo em que o sinal de entrada permanece
 *   em nível alto ou baixo, conforme o parâmetro "edge".
 *   Note que este método "prende", i.e., o programa fica preso neste
 *   método até a identificação da borda indicada.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. Pág.552.
 *               - TPMxCnSC: Channel Status and Control Register. Pág.555.
 */
void mkl_TPMMeasure::waitMeasure() {
  enableMeasure();
  /*!
   * Aguarda até que a flag CHF de detecção de borda seja sinalizada.
   */
  while (!(*addressTPMxCnSC & 0x80)) {}
  /*!
   * Limpa a flag CHF.
   */
  *addressTPMxCnSC |= 0x80;
  /*!
   * Armazena o valor medido.
   */
  measure = *addressTPMxCnV;
}

/*!
 *   @fn         disableMeasure.
 *
 *   @brief      Desabilita a operação de medição.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. Pág.552.
 */
void mkl_TPMMeasure::disableMeasure() {
  *addressTPMxSC &= ~0x08;
}

/*!
 *   @fn         resetMeasure.
 *
 *   @brief      Reseta a contagem.
 *
 *   Este método desabilita a operação de medição e após reseta o contador
 *   de medição.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. Pág.552.
 *               - TPMxCNT: Counter Register. Pág.553.
 *
 *   @warning    Este método desabilita a operação de medição.
 */
void mkl_TPMMeasure::resetMeasure() {
  /*!
   * Desabilita a operação de contagem.
   */
  *addressTPMxSC &= ~0x08;
  /*!
   * Limpa o contador.
   */
  *addressTPMxCNT = 0;
}

/*!
 *   @fn         readData.
 *
 *   @brief      Lê o valor da medição iniciada no método "waitMeasure".
 *
 *   Este método retorna o valor da medição que o usuário iniciou no método
 *   "waitMeasure".
 *
 *   @return     measure - valor da medição.
 */
int mkl_TPMMeasure::readData() {
  return measure;
}

/*!
 *   @fn         getCounter.
 *
 *   @brief      Lê o valor da medição iniciada no método "enableMeasure".
 *
 *   Este método retorna o valor da medição que o usuário iniciou no método
 *   "enableMeasure".
 *
 *   @return     addressTPMxCNT - valor de contagem do contador CNT.
 */
int mkl_TPMMeasure::getCounter() {
  return *addressTPMxCNT;
}
