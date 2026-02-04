# SIM7600 with GSM

Arduino library and firmware for SIM7600 GSM modem communication with SMS, web server, and cellular connectivity features.

## Overview

This project provides a comprehensive Arduino implementation for interfacing with the SIM7600 GSM/LTE modem. It includes support for SMS communication, web server functionality, and cellular connectivity features, making it ideal for IoT and embedded systems applications.

## Features

- **GSM/SMS Communication**: Send and receive SMS messages
- **Web Server**: Built-in web server capabilities for remote access and control
- **Cellular Connectivity**: Full cellular network support via SIM7600 modem
- **Camera Integration**: Arducam SPI camera interface support
- **UART Communication**: Reliable serial communication with the modem
- **Arduino Compatible**: Works with Arduino and Arduino-compatible boards

## Hardware Requirements

- **Microcontroller**: Arduino or Arduino-compatible board (e.g., Arduino Uno, Mega, etc.)
- **Modem**: SIM7600 GSM/LTE module
- **Camera** (Optional): Arducam OV2640 or compatible SPI camera
- **SIM Card**: Active SIM card with cellular plan
- **Power Supply**: Appropriate power supply (check modem specifications)

## Pin Configuration

The default pin configuration for the SIM7600 modem UART interface:
- **RX**: Digital pin (configurable)
- **TX**: Digital pin (configurable)
- **GND**: Ground
- **VCC**: Power supply

For Arducam camera (if used):
- **CS**: Chip select pin
- **MOSI**: SPI data out
- **MISO**: SPI data in
- **SCK**: SPI clock

## Project Files

### Core Library Files
- `ArducamLink.cpp` - Arducam camera driver implementation
- `ArducamLink.h` - Arducam camera driver header
- `ArducamUart.h` - UART configuration for Arducam communication

### Firmware Examples
- `SIM7000G_SMS_REC.ino` - SMS receiving and transmission example
- `webserver.ino` - Web server implementation example

## Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/AerioJobin/SIM7600-withGSM.git
   ```

2. **Add to Arduino Library** (Optional):
   - Copy the header files to your Arduino libraries folder:
     ```
     ~/Documents/Arduino/libraries/SIM7600withGSM/
     ```

3. **Include Headers in Your Sketch**:
   ```cpp
   #include "ArducamLink.h"
   #include "ArducamUart.h"
   ```

## Usage Examples

### SMS Communication Example

```cpp
#include "ArducamLink.h"
#include "ArducamUart.h"

// Initialize UART for modem communication
void setup() {
  Serial.begin(9600);      // Debug output
  Serial1.begin(115200);   // SIM7600 modem communication
  
  // Initialize your modem here
}

void loop() {
  // Send SMS, receive data, etc.
  // Implementation based on SIM7000G_SMS_REC.ino
}
```

### Web Server Example

Refer to `webserver.ino` for a complete web server implementation that allows remote control and monitoring via HTTP.

## Configuration

Key configuration parameters:
- **Baud Rate**: 115200 bps (standard for SIM7600)
- **UART Pins**: Configurable via hardware definitions
- **SMS Format**: Text mode or PDU mode
- **APN Settings**: Customize for your carrier

## Troubleshooting

### No Connection to Modem
- Verify UART connections (RX/TX pins)
- Check baud rate (default 115200)
- Ensure power supply is adequate
- Check SIM card is active and inserted properly

### SMS Not Sending
- Verify network signal quality
- Check modem is registered on network
- Verify SIM card has active SMS plan
- Check AT command responses in serial monitor

### Camera Issues (If Using Arducam)
- Verify SPI pin connections
- Check chip select (CS) pin is properly configured
- Ensure camera is properly initialized in code

## Dependencies

- Arduino IDE 1.8.0 or higher
- Arduino core libraries (Serial, SPI)
- Optional: Arducam libraries if using camera module

## License

This project includes code from Arducam Technology. Please refer to the source files for specific license information.

## Contributing

Contributions are welcome! If you have improvements or bug fixes:
1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## Support

For issues, questions, or suggestions, please open an issue on the GitHub repository.

## References

- SIM7600 Datasheet and AT Command Manual
- Arduino Official Documentation: https://www.arduino.cc/
- Arducam Documentation and Examples

## Author

**Aerio Jobin** - Initial implementation and maintenance

---

**Last Updated**: 2026
**Status**: Active Development
