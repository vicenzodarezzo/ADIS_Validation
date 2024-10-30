#ifndef SPI_ESP_INTERFACE_H
#define SPI_ESP_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

/* Defining pins for ESP32 which uses MISO, MOSI, CS, SCLK */
#define ESP_HOST     VSPI_HOST // Using VSPI Hardware interface
#define PIN_NUM_MISO GPIO_NUM_19 // RX0                         
#define PIN_NUM_MOSI GPIO_NUM_23  // TX0                       
#define PIN_NUM_CLK  GPIO_NUM_18 // D23                       
#define PIN_NUM_CS   GPIO_NUM_5  // D22
#define QUEUE_SIZE   3 // Queue size for the IQR SPI Transactions Buffer 

#define SPI_TAG "spi_protocol" // Tag for ESP_LOG

void spi_init(bool non_blocking_flag);
uint16_t sync_spi_read_data(uint8_t addr);
void sync_spi_write_data(uint8_t addr, uint8_t data);
void spi_read_multiple_data_sync(uint8_t mult_read_addrs, uint8_t n_data, uint16_t * rx_buffer);
void spi_read_multiple_data_assync(uint8_t mult_read_addrs, uint8_t n_data, uint16_t * rx_buffer);

#endif // SPI_ESP_INTERFACE_H