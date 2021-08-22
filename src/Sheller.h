#ifndef SHELLER_H
#define SHELLER_H

#include "Arduino.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

class Sheller
{
    static const uint8_t  startByte = 0x23;
    static const uint8_t  usefullDataLength = 8;
    static const uint8_t  serviceBytesCount = 3;
    static const uint8_t  packageLength = usefullDataLength + serviceBytesCount;
    static const uint16_t rxBuffLength = 128;

    bool rxBuffEmptyFlag = true;
    uint8_t  rxBuff[rxBuffLength] = { 0 };
    uint16_t rxBuffBegin = 0;
    uint16_t rxBuffEnd = 0;
    uint16_t startBytePos = 0;

    bool tryReadData();
    bool foundStartByte();
    uint16_t getCircularBufferLength();
    uint16_t getCrc(const uint8_t *data, const uint16_t length);
    void getCrcByteByByte(uint16_t &crc, const uint8_t byte);
    void incCircVal(uint16_t &val, const uint16_t amountIncrease);
    void writeReceivedPackage(uint8_t *dest);

public:
    Sheller();
    bool push(const uint8_t receivedByte);
    bool read(uint8_t *dest);
    bool wrap(uint8_t *data, const uint8_t dataLength, uint8_t *dest);
};

#endif // SHELLER_H
