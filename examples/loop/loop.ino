/*
 * Sheller: serial loop message
 * 
 * Receive message from Serial and sends it back.
 * For work needs to be installed ShellerTerminal application: https://github.com/VNovytskyi/ShellerTerminal
 *
 * @author: vladyslavN
 * @data: 27.08.2021
 */
#include <Sheller.h>
#define DATA_LENGTH 8
Sheller shell(0x23, DATA_LENGTH, 128);

uint8_t receivedMessage[DATA_LENGTH + Sheller::serviceBytesCount];
uint8_t wraperedData[DATA_LENGTH + Sheller::serviceBytesCount];

void setup() {
  Serial.begin(9600);
}

void loop() {
    while(Serial.available()) {
    shell.push(Serial.read());
  }

  if (shell.read(receivedMessage)) {
    shell.wrap(receivedMessage, DATA_LENGTH, wraperedData);
    Serial.write(wraperedData, DATA_LENGTH + Sheller::serviceBytesCount);
  }
}
