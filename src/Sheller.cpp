#include "sheller.h"

static const uint16_t crc16_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

uint16_t Sheller::getCrc(const uint8_t *data, const uint16_t length)
{
    uint16_t crc = 0xFFFF;
    for(uint8_t i = 0; i < length; ++i){
        crc = (crc << 8) ^ crc16_table[(crc >> 8) ^ *data++];
    }

    return crc;
}

void Sheller::getCrcByteByByte(uint16_t &crc, const uint8_t byte)
{
    crc = (crc << 8) ^ crc16_table[(crc >> 8) ^ byte];
}

void Sheller::incCircVal(uint16_t &val, const uint16_t amountIncrease)
{
    for (uint16_t i = 0; i < amountIncrease; ++i) {
        val = (val + 1) % rxBuffLength;
    }
}

bool Sheller::foundStartByte()
{
    if ((rxBuffBegin  == rxBuffEnd) && rxBuffEmptyFlag  == 0) {
        for (uint16_t i = 0; i < rxBuffLength ; ++i) {
            if(rxBuff[rxBuffBegin ] == startByte) {
                startBytePos = rxBuffBegin;
                return true;
            }
            incCircVal(rxBuffBegin, 1);
        }
    } else {
        while((rxBuff[rxBuffBegin ] != startByte) && (rxBuffBegin  != rxBuffEnd)) {
            incCircVal(rxBuffBegin, 1);
        }

        if (rxBuff[rxBuffBegin ] == startByte) {
            startBytePos = rxBuffBegin;
            return true;
        }
    }

    return false;
}

bool Sheller::tryReadData()
{
    uint16_t received_crc_position = rxBuffBegin;
    incCircVal(received_crc_position, usefullDataLength + 1);
    uint8_t received_crc_l = rxBuff[received_crc_position];
    incCircVal(received_crc_position, 1);
    uint8_t received_crc_h = rxBuff[received_crc_position];
    uint16_t received_crc = received_crc_l | ((uint16_t)received_crc_h << 8);

    uint16_t calculate_crc = 0xFFFF;
    uint16_t begin = rxBuffBegin;
    incCircVal(begin, 1);
    for (uint8_t i = 0; i < usefullDataLength; ++i) {
        getCrcByteByByte(calculate_crc, rxBuff[begin]);
        incCircVal(begin, 1);
    }

    if (calculate_crc == received_crc) {
        return true;
    }

    return false;
}


void Sheller::writeReceivedPackage(uint8_t *dest)
{
    for (uint8_t i = 0; i < usefullDataLength; ++i) {
        incCircVal(rxBuffBegin, 1);
        dest[i] = rxBuff[rxBuffBegin];
    }

    if (rxBuffBegin == rxBuffEnd) {
        rxBuffEmptyFlag  = 1;
    }
}


uint16_t Sheller::getCircularBufferLength()
{
    if (rxBuffEnd  > rxBuffBegin ) {
        return (rxBuffEnd  - rxBuffBegin);
    } else if (rxBuffEnd  < rxBuffBegin) {
        return ((rxBuffLength - rxBuffBegin) + rxBuffEnd);
    } else if (rxBuffEmptyFlag == false) {
        return (rxBuffLength);
    }

    return 0;
}


/*!
 * \brief Sheller::Sheller
 * \param[in] startByte - value of start byte. This value starts the definition of the package.
 * \param[in] usefullDataLength - the number of bytes which user can transmit or receive via sheller.
 * \param[in] rxBuffLength - the size of the circular receive buffer. The size of this buffer should be much more than usefullDataLength.
 * \details parameter ratio (usefullDataLength, rxBuffLength): (8, 128), (16, 256)
 */
Sheller::Sheller(uint8_t startByte, uint8_t usefullDataLength, uint16_t rxBuffLength)
{
    this->startByte = startByte;
    this->usefullDataLength = usefullDataLength;
    this->rxBuffLength = rxBuffLength;
    rxBuff = new uint8_t[rxBuffLength];
    packageLength = usefullDataLength + serviceBytesCount;
}

/*!
 * \brief Sheller::push - Add received byte to the buffer.
 * \param[in] receivedByte - current receive byte from the system or peripheral.
 * \return the result of insert byte to the internal circular buffer.
 */
bool Sheller::push(const uint8_t receivedByte)
{
    if (rxBuffEnd != rxBuffBegin || rxBuffEmptyFlag == true) {
        rxBuffEmptyFlag = false;
        rxBuff[rxBuffEnd] = receivedByte;
        rxBuffEnd = (rxBuffEnd + 1) % rxBuffLength;
        return true;
    }

    return false;
}

/*!
 * \brief Sheller::push - Add received bytes to the buffer.
 * \param[in] data - the pointer to the received data.
 * \param[in] dataLength - the length of received data.
 * \return the result of insert bytes to the internal circular buffer.
 */
bool Sheller::push(const uint8_t *data, const uint8_t dataLength)
{
    for (uint8_t i = 0; i < dataLength; ++i) {
        if (!push(data[i])) {
            return false;
        }
    }
    return true;
}

/*!
 * \brief Sheller::read - Tries to allocate a message from the internal buffer.
 * \param[out] dest - the pointer to the buffer for received message.
 * \return the result of reading message.
 */
bool Sheller::read(uint8_t *dest)
{
    if (dest != NULL) {
        if (getCircularBufferLength() >= packageLength ) {
            if (foundStartByte()) {
                if (getCircularBufferLength() >= packageLength ) {
                    if (tryReadData()) {
                        writeReceivedPackage(dest);
                        return true;
                    } else {
                        if (rxBuffBegin != rxBuffEnd) {
                            incCircVal(rxBuffBegin, 1);
                        }
                    }

                    if (rxBuffBegin == rxBuffEnd) {
                        rxBuffEmptyFlag  = 1;
                    }
                }
            }
        }
    }

    return false;
}

/*!
 * \brief Sheller::wrap - Create package from message: add start byte and CRC-16.
 * \param[in] data - the pointer to user message.
 * \param[in] dataLength - the size of user message.
 * \param[out] dest - the pointer to the buffer for package.
 * \return the result of creating package.
 */
bool Sheller::wrap(uint8_t *data, const uint8_t dataLength, uint8_t *dest)
{
    if ((dest != NULL) && (data != NULL)) {
        if ((dataLength <= usefullDataLength ) && (dataLength > 0)) {
            memset(dest, 0, packageLength);
            dest[0] = startByte ;
            memcpy((dest + 1), data, dataLength);

            uint16_t crc = getCrc((dest + 1), usefullDataLength);
            dest[usefullDataLength  + 1] = crc & 0xFF;
            dest[usefullDataLength  + 2] = (crc >> 8) & 0xFF;

            return true;
        }
    }

    return false;
}

/*!
 * \brief Sheller::getPackageLength.
 * \return the package length.
 */
uint8_t Sheller::getPackageLength()
{
    return packageLength;
}

/*!
 * \brief Sheller::getUsefullDataLength.
 * \return return the number of bytes which user can transmit or receive via sheller.
 */
uint8_t Sheller::getUsefullDataLength()
{
    return usefullDataLength;
}
