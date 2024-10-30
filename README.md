# VALIDATION OF ANALOG DEVICES IMU SENSORS

## GOAL
The main idea is to implement a library to use the commands provided by the ADIS16407 and ADIS16365 IMUs.

## CONTEXT
This project consists of a C implementation for the ESP32 ESP-IDF platform, designed to monitor data through the Serial Monitor feature.

## METHOD
The implementation consists of a simple C module structure, `adis_protocol`, which provides an interface for interacting with the sensor. The module is responsible for handling low-level communication through the **SPI Master ESP Driver** feature, encapsulated by the `spi_esp_interface` included in `adis_protocol`.

This project aims to maintain high hardware responsibility. Therefore, the low-level communication interface must remain abstracted from the main functionalities of the `adis_protocol` module, defining the following semantic division:
- **`spi_protocol`** – Handles the use and implementation of ADIS sensor features.
- **`hardware_communication_interface`**: `spi_esp_interface` – Manages low-level communication through the full-duplex SPI protocol between hardware devices (ESP32 x ADIS).

## References:
- [SPI Master Esp Driver](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/spi_master.html)
- [Introduction Article](https://krutarthpurohit.medium.com/implementing-spi-protocol-on-esp32-idf-5-1-version-6f2383af1c22)
- [ESP PINOUT](https://www.upesy.com/blogs/tutorials/esp32-pinout-reference-gpio-pins-ultimate-guide)
- [ADIS16365 Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/ADIS16365.pdf)
- [ADIS16407 Datasgeet](https://www.analog.com/media/en/technical-documentation/data-sheets/ADIS16407.pdf)
- [SPI Protocol](https://www.analog.com/en/resources/analog-dialogue/articles/introduction-to-spi-interface.html)

## How to run?
- connect ESP32 Devkit and select the board in the ESP IDF IDE (or vscode extension)
- run SDK Configuration (menuconfig) in the ESP IDF IDE (or vscode extension), generating sdkconfig file
- select the usb port in the ESP IDF IDE (or vscode extension)
- run build (Cmake)
- run flash (Ninja)
- run monitor