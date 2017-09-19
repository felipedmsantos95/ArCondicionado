/*!
 * @copyright   � 2017 Universidade Federal do Amazonas.
 *
 * @brief       Interface de programa��o de aplica��es em C++ para Controle Remoto.
 *
 * @file        mkl_RemoteControl.h
 * @version     1.0
 * @date        6 Julho 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+
 *              +compiler     Kinetis� Design Studio IDE
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012
 *              +revisions    Vers�o (data): Descri��o breve.
 *                             ++ 1.0 (12 agosto 2017): Vers�o inicial.
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas
 *              +courses      Engenharia da Computa��o / Engenharia El�trica
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Vers�o inicial:
 *                             ++ Jose Luis da Silva <joseluis70123@gmail.com>
 *
 * @section     LICENSE
 *
 *              GNU General Public License (GNU GPL)
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
#ifndef C__USERS_JOSEL_DESKTOP_CPPLINT_REMOTECONTROL_H_
#define C__USERS_JOSEL_DESKTOP_CPPLINT_REMOTECONTROL_H_
#include <MKL25Z4.h>
#include "mkl_GPIOInterrupt.h"
#include "mkl_TPMMeasure.h"
#include "mkl_TPM.h"
/*!
 * Enum de defini��o da exce��o.
 */
typedef enum {
  mkl_ok = 0,
  mkl_overWrite,
  mkl_parityError
} Exception_t;
/*!
 *  @class    mkl_RemoteControl
 *
 *  @brief    Classe de implementa��o do controlador do controle remoto para a placa KL25Z128.
 *
 *  @details  Esta classe � usada para leitura e decodifica��o dos sinais
 *  		  do receptor IR, para aplica��o do controle remoto.
 *
 *  @section  EXAMPLES USAGE
 *             +fn setupPeripheral(TPM_t::TPMNumber_t tpmName,GPIO_t::mkl_GPIO gpioName,TPM_t::Pin_t pin)
 *	           +fn enableInterrupt();
 *             +fn commandAvailable();
 *             +fn waitCommandAvailable;
 *             +fn readCommand()
 */
class mkl_RemoteControl {
 public:
    /*!
     * M�todo construtor padr�o da classe.
     */
    mkl_RemoteControl(gpio_Name gpioName, tpm_Pin pin);
    /*!
     * M�todo de configura��o do perif�rico para operar..
     */
    void setupPeripheral(gpio_Name gpioName, tpm_Pin pin);
    /*!
     * M�todo de tratar e ler os dados do receptor IR.
     */
    uint8_t readCommand();
    uint8_t readAddress();
    bool commandAvailable();
    Exception_t waitCommandAvailable();
    void setStartCommand();
    /*!
     * M�todos que tratam da interrup��o do GPIO
     */
    void clearInterruptFlag();
    void enableInterrupt();
    void disableInterrupt();

 private:
    mkl_GPIOInterrupt gpio;
    mkl_TPMMeasure tpm0;
    bool flagRead, flagStartCommand, flagEndData, flagInterrupt;
    bool  flagStart;
    uint16_t cont = 0;
    uint8_t i, j, f = 0;
    uint8_t dado[40], adress[8], command[8], Address_Ir, Command_Ir;
    uint8_t parity[8] = {0, 0, 0, 0, 0, 0, 0, 0};
};
#endif  /* C__USERS_JOSEL_DESKTOP_CPPLINT_REMOTECONTROL_H_*/
