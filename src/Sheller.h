
/*
    @file       sheller.h
    @author     vladyslavN
*/
#ifndef SHELLER_H
#define SHELLER_H

#include "Arduino.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

class Sheller
{
    uint8_t  startByte = 0;
    uint8_t  usefullDataLength = 0;   
    uint8_t  packageLength = 0;
    uint16_t rxBuffLength = 0;

    bool rxBuffEmptyFlag = true;
    uint8_t  *rxBuff = NULL;
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
    static const uint8_t serviceBytesCount = 3;
    
    Sheller(uint8_t startByte, uint8_t usefullDataLength, uint16_t rxBuffLength);

    bool push(const uint8_t receivedByte);
    bool push(const uint8_t *data, const uint8_t dataLength);

    bool read(uint8_t *dest);
    bool wrap(uint8_t *data, const uint8_t dataLength, uint8_t *dest);

    uint8_t getPackageLength();
    uint8_t getUsefullDataLength();
};

#endif // SHELLER_H
