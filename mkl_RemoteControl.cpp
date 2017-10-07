/*!
 * @copyright   � 2017 Universidade Federal do Amazonas.
 *
 * @brief       Uma API em C++ para Controle Remoto.
 *
 * @file        mkl_RemoteControl.cpp
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
#include <MKL25Z4.h>
#include "mkl_GPIOInterrupt.h"
#include "mkl_RemoteControl.h"
#include "mkl_TPMMeasure.h"


/*!
 *   @fn       mkl_RemoteControl
 *
 *   @brief    M�todo construtor da classe.
 *
 *   Este � o m�todo construtor padr�o da classe e realiza a associa��o
 *   entre o objeto de software e os perif�ricos de hardware usados pelo controle.
 *
 *   @param[in]  gpioName - GPIO;
 *               pin - pino do TPM Measure
 */
mkl_RemoteControl::mkl_RemoteControl(gpio_Name gpioName, tpm_Pin pin) {
  setupPeripheral(gpioName, pin);
}


/*!
 * @fn		    setupPeripheral
 *
 * @brief		Configura o perif�rico on-chip TPM e GPIO para aquisi��o
 * 				de dados do sensor.
 * @details		Utiliza os m�todos das classe measure e GPIO para inicializar
 * 				estes perif�ricos
 *
 * @param[in]    gpioName - GPIO;
 *               pin - pino do TPM Measure
 */
void mkl_RemoteControl::setupPeripheral(gpio_Name gpioName, tpm_Pin pin) {
  __disable_irq();

  gpio = mkl_GPIOInterrupt(gpio_GPIOA, gpio_PTA1);
  gpio.setPortMode(gpio_input);
  tpm0 = mkl_TPMMeasure(pin);
  tpm0.setFrequency(tpm_div128);
  tpm0.setEdge(tpm_falling);

  flagRead = true;
}


/*!
 * @fn			readComamnd
 *
 * @brief		Retorna o comando digitado no controle remoto
 *
 * @details		Converte a sequ�ncia de bits armazenados, que correspondem
 * 				aos pulsos, no valor hexadecimal equivalente
 *
 * @return		Retorna o valor hexadecimal do comando digitado no controle
 */
uint8_t mkl_RemoteControl::readCommand() {

  if (f == 0) {
    Command_Ir = command[0]*128 + command[1]*64 + command[2]*32
	             + command[3]*16 + command[4]*8 + command[5]*4
				 + command[6]*2 + command[7]*1;
  } else {
    Command_Ir = command[1]*128 + command[2]*64 + command[3]*32
	             + command[4]*16 + command[5]*8 + command[6]*4
				 + command[7]*2 + 0;
  }

  flagRead = true;

  return Command_Ir;
}


/*!
 * @fn			readAddress
 *
 * @brief		Retorna o endere�o do controle
 *
 * @details		Converte a sequ�ncia de bits armazenados, que correspondem
 * 				aos pulsos, no valor hexadecimal equivalente
 *
 * @return		Retorna o valor hexadecimal do endere�o do controle
 */
uint8_t mkl_RemoteControl::readAddress() {
  if (f == 0) {
    Address_Ir = adress[0]*128 + adress[1]*64 + adress[2]*32
	             + adress[3]*16 + adress[4]*8 + adress[5]*4
				 + adress[6]*2 + adress[7]*1;
  } else {
    Address_Ir = adress[1]*128 + adress[2]*64 + adress[3]*32
	             + adress[4]*16 + adress[5]*8 + adress[6]*4 + adress[7]*2;
  }
  return Address_Ir;
}


/*!
 * @fn			commandAvailable
 *
 * @brief		Retorna true se ocorreu o in�cio da recep��o de um comando
 * 				(start bit) e false caso contr�rio
 *
 * @return		Retorna verdadeiro em caso de in�cio da recep��o e falso em
 * 				caso contr�rio
 */
bool mkl_RemoteControl::commandAvailable() {
  if (flagStart == true) {
    return true;
  }
  return false;
}


/*!
 * @fn			waitCommandAvailable
 *
 * @brief		Espera o comando chegar(t�rmino do start bit, dados e o
 * 				stop bit) e se ocorrer erro, retorna mkl_parityError,
 * 				mkl_overwriteError
 *
 * @details		Mede o tempo que o sinal passa em baixo mais o tempo que o
 * 				sinal passa em alto e define se os pulsos analisados
 * 				correspondem a um bit 1, ou bit 0. Al�m de identificar erros
 * 				retornando para um tratamento de exce��es
 *
 * @return		Retorna o erro encontrado,mkl_parityError ou mkl_overwriteError
 */
Exception_t mkl_RemoteControl::waitCommandAvailable() {
  flagEndData = false;
  flagStart = false;
  commandAvailable();
  //__disable_irq();
  f = f;
  tpm0.resetMeasure();

  for (i=0; i <= 31; i++) {
    tpm0.waitMeasure();
    cont = tpm0.readData();

      if (cont > 275) {
        dado[i] = 1;
      } else {
        dado[i] = 0;
      }
      tpm0.resetMeasure();
  }
  flagEndData = true;

  for (j = 0; j < 8; j++) {
    adress[j] = dado[j];//dado[j]
  }

  for (j = 0; j < 8; j++) {
    command[j] = dado[j+16];
  }

  if (adress[0] != 0 && adress[1] != 0 &&adress[2] != 0 &&adress[3] != 0 &&adress[4] != 0 &&adress[5] != 0 &&adress[6] != 0 &&adress[7] != 0) {
    return mkl_parityError;
  }
  if (flagEndData && flagRead == false) {
    return mkl_overWrite;
  }
  return mkl_ok;
}


/*!
 * @fn			setStartCommand
 *
 * @brief		Indica no objeto de software que est� chegando um start bit
 */
void mkl_RemoteControl::setStartCommand() {
  flagStart = true;
  commandAvailable();
  tpm0.waitMeasure();
  cont = tpm0.readData();
}


/*!
 * @fn			clearInterruptFlag
 *
 * @brief		Habilita a recep��o do controle remoto para operar
 */
void mkl_RemoteControl::clearInterruptFlag() {
  flagInterrupt = true;
  gpio.clearInterruptFlag();
}


/*!
 * @fn			enableInterrupt
 *
 * @brief		Habilita a recep��o do controle remoto via interrup��o
 */
void mkl_RemoteControl::enableInterrupt() {
  gpio.enableInterrupt();
}


/*!
 * @fn			disableInterrupt
 *
 * @brief		Desabilita a recep��o do controle remoto via interrup��o
 */
void mkl_RemoteControl::disableInterrupt() {
  gpio.disableInterrupt();
}
