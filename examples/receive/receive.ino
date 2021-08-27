/*
 * Sheller: receive message
 * 
 * Receive message from Serial and execute the command in message.
 * For work needs to be installed ShellerTerminal application: https://github.com/VNovytskyi/ShellerTerminal
 *
 * @author: vladyslavN
 * @data: 26.08.2021
 */
#include <Sheller.h>
#define DATA_LENGTH 8
Sheller shell(0x23, DATA_LENGTH, 128);
uint8_t receivedMessage[DATA_LENGTH];

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  while(Serial.available()) {
    shell.push(Serial.read());
  }

  if (shell.read(receivedMessage)) {
    handleMessage(receivedMessage, DATA_LENGTH);
  }
}

void handleMessage(uint8_t *data, uint8_t dataLength) {
  if (data[0] == 0x0C) {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (data[0] == 0x20) {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
