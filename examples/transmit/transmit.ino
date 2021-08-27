/*
 * Sheller: transmit message
 * 
 * Every second send the message via Serial
 * For work needs to be installed ShellerTerminal application: https://github.com/VNovytskyi/ShellerTerminal
 *
 * @author: vladyslavN
 * @data: 27.08.2021
 */
#include <Sheller.h>
#define DATA_LENGTH 8
Sheller shell(0x23, DATA_LENGTH, 128);
uint8_t transmitData[DATA_LENGTH] = { 0, 1, 2, 3, 4, 5, 6, 7 };
uint8_t wraperedData[DATA_LENGTH + Sheller::serviceBytesCount];

void setup() {
  Serial.begin(9600);
}

void loop() {
  shell.wrap(transmitData, 8, wraperedData);
  Serial.write(wraperedData, DATA_LENGTH + Sheller::serviceBytesCount);
  delay(1000);
}
