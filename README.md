# Sheller integrity guarantee 
Special version of Sheller for using with Arduino. [Original Sheller](https://github.com/VNovytskyi/Sheller)<br>
To run tests with your device, use [ShellerTerminal](https://github.com/VNovytskyi/ShellerTerminal)
<p align="center">
  <img src="logo.PNG">
</p>

## Functions description
```c
/*!
 * \brief Sheller::Sheller - constructor.
 * \param[in] startByte - value of start byte. This value starts the definition of the package.
 * \param[in] usefullDataLength - the number of bytes which user can transmit or receive via sheller.
 * \param[in] rxBuffLength - the size of the circular receive buffer.
 * \details parameter ratio (usefullDataLength, rxBuffLength): (8, 128), (16, 256)
 */
Sheller(uint8_t startByte, uint8_t usefullDataLength, uint16_t rxBuffLength);
```

```c
/*!
 * \brief Sheller::push - Add received byte to the buffer.
 * \param[in] receivedByte - current receive byte from the system or peripheral.
 * \return the result of insert byte to the internal circular buffer.
 */
bool Sheller::push(const uint8_t receivedByte);

/*!
 * \brief Sheller::push - Add received bytes to the buffer.
 * \param[in] data - the pointer to the received data.
 * \param[in] dataLength - the length of received data.
 * \return the result of insert bytes to the internal circular buffer.
 */
bool Sheller::push(const uint8_t *data, const uint8_t dataLength);
```

```c
/*!
 * \brief Sheller::read - Tries to allocate a message from the internal buffer.
 * \param[out] dest - the pointer to the buffer for received message.
 * \return the result of reading message.
 */
 bool Sheller::read(uint8_t *dest);
```

```c
/*!
 * \brief Sheller::wrap - Create package from message: add start byte and CRC-16.
 * \param[in] data - the pointer to user message.
 * \param[in] dataLength - the size of user message.
 * \param[out] dest - the pointer to the buffer for package.
 * \return the result of creating package.
 */
bool Sheller::wrap(uint8_t *data, const uint8_t dataLength, uint8_t *dest);
```

```c
/*!
 * \brief Sheller::getPackageLength.
 * \return the package length.
 */
uint8_t Sheller::getPackageLength();
```

```c
/*!
 * \brief Sheller::getUsefullDataLength.
 * \return the number of bytes which user can transmit or receive via sheller.
 */
uint8_t Sheller::getUsefullDataLength()
```
