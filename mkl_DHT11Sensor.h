/*!
 * @copyright   © 2017 Universidade Federal do Amazonas.
 *
 * @brief       Interface da API em C++ do periférico sensor DHT11.
 * @file        mkl_DHT11Sensor.h
 * @version     1.0
 * @date        16 Julho 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+.
 *              +compiler     Kinetis® Design Studio IDE.
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012.
 *              +revisions    Versão (data): Descrição breve.
 *                             ++ 1.0 (03 Setembro 2017): Versão inicial.
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
 *              GNU General Public License (GNU GPL).
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

#include "mkl_GPIOPort.h"
#include "mkl_TPMDelay.h"
#include <stdint.h>

#ifndef MKL_DHT11Sensor_H_
#define MKL_DHT11Sensor_H_

typedef enum {
  dht11_parityError,
  dht11_timeoutError,
  dht11_ok
}dht11_Exception;

/*!
 *  @class    mkl_DHT11Sensor
 *
 *  @brief    A classe mkl_DHT11Sensor representa o periférico off-chip
 *            Sensor de Temperatura DHT11.
 *
 *  @details  Esta classe é usada a aquisição de dados de temperatura e umidade
 *            proveniente do sensor.
 *
 *  @section  EXAMPLES USAGE
 *
 *            Uso dos métodos como porta de entrada
 *	           +fn doAcquisition();
 *	           +fn readUmidity(&umidade);
 *             +fn readTemperature(&temperatura);
 */
class mkl_DHT11Sensor {
 public:
  /*!
   * Método construtor padrão da classe.
   */
  mkl_DHT11Sensor(tpm_TPMNumberMask TPMNumber = tpm_TPM0,
                  gpio_Pin pin = gpio_PTA1);
  /*!
   * Método de aquisição de dados.
   */
  dht11_Exception doAcquisition();
 /*!
  * Métodos de leitura de dados já adquiridos.
  */
  void readTemperature(uint8_t *temp);
  void readUmidity(uint8_t *umid);

 private:
  /*!
   * Objeto de software da classe dsf_Delay_OCP.
   */
  mkl_TPMDelay tpm;

  /*!
   * Objeto de software da classe mkl_GPIOPort.
   */
  mkl_GPIOPort gpio;

  /*!
   * Atributos de umidade, temperatura e byte de paridade.
   */
  uint8_t umidity, temperature, verifyByte;

  /*!
   * Método de leitura do sensor.
   */
  dht11_Exception readSensorData();

  /*!
   * Métodos de aguardar a resposta do sensor.
   */
  int wait(int bit);
  int waitResponse();

  /*!
   * Método de iniciar a comunicação com o sensor.
   */
  void startCommunication();

  /*!
   * Método de verificar a paridade dos dados enviados pelo sensor.
   */
  int checkSum();
  /*!
   * Método de associação do tempo em que o sinal permaneceu em nível alto
   * ao bit correspondente.
   */
  dht11_Exception readPackage(uint16_t time[40]);
};

#endif  // MKL_DHT11Sensor_H_
