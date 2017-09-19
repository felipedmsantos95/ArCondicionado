/*!
 * @copyright   © 2017 Universidade Federal do Amazonas.
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
 *              +compiler     Kinetis® Design Studio IDE
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012
 *              +revisions    Versão (data): Descrição breve.
 *                             ++ 1.0 (12 agosto 2017): Versão inicial.
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas
 *              +courses      Engenharia da Computação / Engenharia Elétrica
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Versão inicial:
 *                             ++ Jose Luis da Silva <joseluis70123@gmail.com>
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
#include <MKL25Z4.h>
#include "mkl_GPIOInterrupt.h"
#include "mkl_RemoteControl.h"
#include "mkl_TPMMeasure.h"


/*!
 *   @fn       mkl_RemoteControl
 *
 *   @brief    Método construtor da classe.
 *
 *   Este é o método construtor padrão da classe e realiza a associação
 *   entre o objeto de software e os periféricos de hardware usados pelo controle.
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
 * @brief		Configura o periférico on-chip TPM e GPIO para aquisição
 * 				de dados do sensor.
 * @details		Utiliza os métodos das classe measure e GPIO para inicializar
 * 				estes periféricos
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
 * @details		Converte a sequência de bits armazenados, que correspondem
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
 * @brief		Retorna o endereço do controle
 *
 * @details		Converte a sequência de bits armazenados, que correspondem
 * 				aos pulsos, no valor hexadecimal equivalente
 *
 * @return		Retorna o valor hexadecimal do endereço do controle
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
 * @brief		Retorna true se ocorreu o início da recepção de um comando
 * 				(start bit) e false caso contrário
 *
 * @return		Retorna verdadeiro em caso de início da recepção e falso em
 * 				caso contrário
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
 * @brief		Espera o comando chegar(término do start bit, dados e o
 * 				stop bit) e se ocorrer erro, retorna mkl_parityError,
 * 				mkl_overwriteError
 *
 * @details		Mede o tempo que o sinal passa em baixo mais o tempo que o
 * 				sinal passa em alto e define se os pulsos analisados
 * 				correspondem a um bit 1, ou bit 0. Além de identificar erros
 * 				retornando para um tratamento de exceções
 *
 * @return		Retorna o erro encontrado,mkl_parityError ou mkl_overwriteError
 */
Exception_t mkl_RemoteControl::waitCommandAvailable() {
  flagEndData = false;
  flagStart = false;
  commandAvailable();
  __disable_irq();
  f = f+1;
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
    adress[j] = dado[j];
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
 * @brief		Indica no objeto de software que está chegando um start bit
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
 * @brief		Habilita a recepção do controle remoto para operar
 */
void mkl_RemoteControl::clearInterruptFlag() {
  flagInterrupt = true;
  gpio.clearInterruptFlag();
}


/*!
 * @fn			enableInterrupt
 *
 * @brief		Habilita a recepção do controle remoto via interrupção
 */
void mkl_RemoteControl::enableInterrupt() {
  gpio.enableInterrupt();
}


/*!
 * @fn			disableInterrupt
 *
 * @brief		Desabilita a recepção do controle remoto via interrupção
 */
void mkl_RemoteControl::disableInterrupt() {
  gpio.disableInterrupt();
}
