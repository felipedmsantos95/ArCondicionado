/*!
 * @copyright   © 2017 Universidade Federal do Amazonas.
 *
 * @brief       Implementação da classe em C++ do periférico DHT11.
 * @file        mkl_DHT11Sensor.cpp
 * @version     1.0
 * @date        06 Julho 2017
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

#include <stdint.h>
#include "mkl_DHT11Sensor.h"

/*!
 *  @fn       mkl_DHT11Sensor
 *
 *  @brief    Configura o periférico TPM e o GPIO para uso com o
 *            periférico off-chip.
 *
 *  @param    TPMNumber - periférico TPM desejado para uso com o sensor.
 *            pin - pino do GPIO desejado para uso com o sensor.
 */
mkl_DHT11Sensor::mkl_DHT11Sensor(tpm_TPMNumberMask TPMNumber,
                                         gpio_Pin pin)
                                        :tpm(TPMNumber),
                                         gpio(pin) {
  /*!
   * Ajusta a frequência de operação do TPM.
   */
  tpm.setFrequency(tpm_div1);
}

/*!
 *  @fn       doAcquisition
 *
 *  @brief    Realiza uma aquisição de dados de temperatura e
 *            umidade no periférico do sensor DHT11 e armazena
 *            os dados nos atributos correspondentes
 *            no objeto de software.
 *
 *  @return   dht11_ok           - se a aquisição for realizada com sucesso.
 *            dht11_timeoutError - se não houver resposta do sensor.
 *            dht11_parityError  - se os dados enviados pelo sensor.
 *                                 não corresponderem ao byte de paridade.
 */
dht11_Exception mkl_DHT11Sensor::doAcquisition() {
  /*!
   * Envia o comando de start ao sensor de temperatura.
   */
  startCommunication();

  /*!
   * Configura o pino para operar como entrada com resistor de Pull-Up.
   */
  gpio.setPortMode(gpio_input);
  gpio.setPullResistor(gpio_pullUpResistor);

  /*!
   * Aguarda a resposta do sensor ao comando enviado.
   */
  if (!waitResponse()) {
    return dht11_timeoutError;
  }

  /*!
   * Realiza a aquisição de dados de temperatura e umidade.
   */
  return readSensorData();
}

/*!
 *  @fn       readTemperature
 *
 *  @brief    Lê a temperatura armazenada no atributo do objeto sensor.
 *
 *  @param    temp - variável em que o valor de temperatura
 *            obtido será armzenado.
 */
void mkl_DHT11Sensor::readTemperature(uint8_t *temp) {
  *temp = temperature;
}

/*!
 *  @fn       readUmidity
 *
 *  @brief    Lê a umidade armazenada no atributo do objeto sensor.
 *
 *  @param    umidity - variável em que o valor de umidade
 *            obtido será armzenado.
 */
void mkl_DHT11Sensor::readUmidity(uint8_t *umid) {
  *umid = umidity;
}

/*!
 *  @fn       readSensorData
 *
 *  @brief    Armazena o tempo que o barramento ficou em nível lógico alto
 *            em um vetor de inteiros.
 *
 *  @return   dht11_ok           - se as leituras forem realizadas com sucesso
 *            dht11_timeoutError - se não houver resposta do sensor.
 */
dht11_Exception mkl_DHT11Sensor::readSensorData() {
  int i;
  uint16_t time[40];

  /*!
   * Realiza a medição de 40 tempos (pois são 40 bits) em que o sinal
   * permaneceu em nível lógico alto.
   */
  for (i = 0; i < 40; i++) {
    if (!wait(1)) {
      /*!
       * Houve transbordamento de tempo.
       */
      return dht11_timeoutError;
    }

    /*!
     * Lê o tempo correspondente no periférico TPM.
     */
    tpm.getCounter(&time[i]);

    /*!
     * Aguarda enquanto o barramento permanece em '0'.
     */
    if (!wait(0)) {
      /*!
       * Houve transbordamento de tempo.
       */
      return dht11_timeoutError;
    }
  }
  return readPackage(time);
}

/*!
 *  @fn       wait
 *
 *  @brief    Mede o tempo em que o barramento ficou no nível lógico passado
 *            por parâmetro.
 *
 *  @param    bit - o bit que se deseja medir o tempo.
 *
 *  @return   0 - se houver transbordamento do fundo de escala
 *            do temporizador.
 *            1 - se não houver transbordamento do fundo de escala
 *            do temporizador.
 */
int mkl_DHT11Sensor::wait(int bit) {
  uint8_t data;

  /*!
   * Inicia uma temporização.
   */
  tpm.startDelay(0xFFFF);

  /*!
   * Prende até que o dado lido seja diferente daquele especificado
   * no parâmetro ou até que haja transbordamento do fundo de escala
   * do periférico TPM.
   */
  do {
    data = gpio.readBit();
  }while (!tpm.timeoutDelay() && data == bit);

  /*!
   * Verifica se ocorreu o transbordamento do tempo.
   */
  if (tpm.timeoutDelay() == 1) {
    return 0;
  }
  return 1;
}

/*!
 *  @fn       waitResponse
 *
 *  @brief    Aguarda a resposta do sensor ao comando inicial enviado.
 *
 *  @return   0 - se não resposta do sensor ao comando enviado.
 *            1 - se o sensor responder ao comando enviado.
 */
int mkl_DHT11Sensor::waitResponse() {
  if (!wait(1)) {
    return 0;
  }
  if (!wait(0)) {
    return 0;
  }
  if (!wait(1)) {
    return 0;
  }
  if (!wait(0)) {
    return 0;
  }
  return 1;
}

/*!
 *  @fn       startCommunication
 *
 *  @brief    Inicia a comunicação com o sensor, escrevendo
 *            '0' no barramento por 20 ms.
 */
void mkl_DHT11Sensor::startCommunication() {
  int i;

  /*!
   * Configura o pino do GPIO para atuar como saída.
   */
  gpio.setPortMode(gpio_output);

  /*!
   * Escreve '0' na saída do GPIO e aguarda 20 ms.
   */
  gpio.writeBit(0);
  for (i = 0; i < 7; i++) {
    tpm.waitDelay(0xFFFF);
  }

  /*!
   * Escreve '1' na saída do GPIO e aguarda 1 ms.
   */
  gpio.writeBit(1);
  tpm.waitDelay(1000);
}

/*!
 *  @fn       checkSum
 *
 *  @brief    Verifica se a soma dos bytes de temperatura e umidade
 *            corresponde ao byte de paridade enviado.
 *
 *  @return   0 - se a soma dos bytes de temperatura e umidade
 *            corresponder ao byte de paridade enviado.
 *            1 - se a soma dos bytes de temperatura e umidade
 *            não corresponder ao byte de paridade enviado.
 */
int mkl_DHT11Sensor::checkSum() {
  if (temperature + umidity != verifyByte) {
    return 0;
  }
  return 1;
}
/*!
 *  @fn       readPackage
 *
 *  @brief    Lê os tempos medidos, atribui o bit equivalente a cada tempo
 *            e armazena o valor de temperatura e umidade correspondente.
 *
 *  @return   dht11_ok           - se a paridade dos bytes de umidade e
 *                               temperatura corresponderem ao byte de
 *                               paridade enviado.
 *            dht11_parityError  - se a paridade dos bytes de umidade e
 *                               temperatura não corresponderem ao byte de
 *                               paridade enviado.
 */
dht11_Exception mkl_DHT11Sensor::readPackage(uint16_t time[40]) {
  int i;

  /*!
   * Inicializa as variáveis de temperatura, umidade e de paridade.
   */
  umidity = 0;
  temperature = 0;
  verifyByte = 0;
  /*!
   * Verifica se o tempo armazenado é superior a 30 us.
   * Caso seja, o bit é assinalado como '1' e é realizado o deslocamento
   * para a esquerda na variável de umidade e o bit '1' é inserido na último
   * bit da variável.
   */
  for (i = 0; i < 8; i++) {
    umidity = umidity << 1;
    if (time[i] >= 1000) {
      umidity |= 1;
    }
  }
  /*!
   * Verifica se o tempo armazenado é superior a 30 us.
   * Caso seja, o bit é assinalado como '1' e é realizado o deslocamento
   * para a esquerda na variável de temperatura e o bit '1' é inserido na último
   * bit da variável.
   */
  for (i = 16; i < 24; i++) {
    temperature = temperature << 1;
    if (time[i] >= 1000) {
      temperature |= 1;
    }
  }

  /*!
   * Verifica se o tempo armazenado é superior a 30 us.
   * Caso seja, o bit é assinalado como '1' e é realizado o deslocamento
   * para a esquerda na variável de paridade e o bit '1' é inserido na último
   * bit da variável.
   */
  for (i = 32; i < 40; i++) {
    verifyByte = verifyByte << 1;
    if (time[i] >= 1000) {
      verifyByte |= 1;
    }
  }

  /*!
   * Realiza a verificação da paridade dos dados enviados pelo sensor.
   */
  if (!checkSum()) {
	/*!
	 * A paridade dos dados enviados não corresponde ao byte de paridade.
	 */
    return dht11_parityError;
  }
  return dht11_ok;
}
