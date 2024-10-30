#include "../inc/spi_esp_interface.h"

esp_err_t ret;
spi_device_handle_t spi;

#define LOG(str, ...) ESP_LOGI(SPI_TAG, str, ##__VA_ARGS__)
#define ERROR(str, ...) ESP_LOGE(SPI_TAG, str, ##__VA_ARGS__)

/**
 * Global Callback Function for Non-Blocking SPI Transactions
 *  
 */ 
void IRAM_ATTR spi_transaction_print_callback(spi_transaction_t *trans) {
    LOG("### SPI TRANSACTION CALLBACK ###");

    uint16_t *received_data = (uint16_t *) trans->rx_buffer;
    uint8_t buffer_size =  (uint8_t) (trans->rxlength / sizeof(uint16_t));

    for (uint8_t i = 0; i < buffer_size; i++) {
        LOG("\t-> Data received: (hex) 0x%04X | (dec) %d\n", received_data[i], received_data[i]);
    }
}

void spi_init(bool non_blocking_flag)
{
    LOG("### SPI INITIALIZATION - NON-BLK: %d ###", (int) non_blocking_flag);

    LOG("Setting CS");
    gpio_set_direction(PIN_NUM_CS, GPIO_MODE_OUTPUT); // Setting the CS' pin to work in OUTPUT mode

    LOG("Setting BUS");
    spi_bus_config_t buscfg = { 
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 512 * 8  // 4095 bytes is the max size of data that can be sent because of hardware limitations
    };

    LOG("Setting DEVICE");
    spi_device_interface_config_t devcfg = {
        // configure device_structure
        .clock_speed_hz = 2 * 1000 * 1000, // Clock out at 2 MHz
        .mode = 3, // SPI mode 0: CPOL: 1 (INACTIVE WHEN CLK == 1) and CPHA: 1 (DATA CAPTURED ON FALLING EDGE)
        .spics_io_num = PIN_NUM_CS,                                     
        .queue_size = QUEUE_SIZE,    
        .post_cb = non_blocking_flag ? spi_transaction_print_callback : NULL, // Adding Callback Function in case of Non Blocking Comm.                                           
    };

    LOG("Init BUS and associating DEVICE");

    ret = spi_bus_initialize(ESP_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ERROR("spi_bus_initialize failed: %s", esp_err_to_name(ret));
        return;
    }
    ret = spi_bus_add_device(ESP_HOST, &devcfg, &spi);
    if (ret != ESP_OK) {
        ERROR("spi_bus_add_device failed: %s", esp_err_to_name(ret));
        return;
    }
}


uint16_t sync_spi_read_data(uint8_t addr)
{
    LOG("#### SYNC READ REQUEST - ADDR: 0x%02x ####", addr);

    uint8_t instruction_to_read_data[2] = {addr, 0xFF};             // 0xFF = dummy value
    spi_transaction_t trans_desc = {                                       // Configure the transaction_structure
                                    .flags = SPI_TRANS_USE_RXDATA,         // Set the Rx flag
                                    .tx_buffer = instruction_to_read_data, // Host need to first transmit the (command + address) to slave which the host wants to read
                                    .rxlength = 16,                        // 8*2 = 16 bit data transfer (MAX)
                                    .length = 16};
    gpio_set_level(PIN_NUM_CS, 0);

    ret = spi_device_transmit(spi, &trans_desc);

    if (ret != ESP_OK) { 
        ERROR("SPI read operation failed\n"); 
        return 0;
    }

    // Host can fetch the data that received from the slave from inbuild structure member- rx_data directly
    LOG("Data Read: (addr) 0x%02x =\n\t[0]:0x%02x\n\t[1]:0x%02x\n", addr, trans_desc.rx_data[0], trans_desc.rx_data[1]);

    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(PIN_NUM_CS, 1);
    LOG("Reading Request complete.");

    return (trans_desc.rx_data[0] << 8) | trans_desc.rx_data[1];
}

void spi_read_multiple_data_sync(uint8_t mult_read_addrs, uint8_t n_data, uint16_t * rx_buffer) {

    LOG("### SYNC BURST-READ REQUEST in ADDR: 0x%02x FOR %d BYTES ###", mult_read_addrs, 16 * n_data);

    // n_data += 5; // [DEBUG] Adding more reads to verify the buffer

    uint8_t *tx_buffer = (uint8_t *) malloc(n_data * 2 * sizeof(uint8_t)); // Buffer for Read Instruction sent to slave

    if (tx_buffer == NULL) {
        ERROR("Error allocating memory for buffers");
        return;
    }

    // Building a burst-read instruction buffer with the same length 
    // as the read buffer through byte stuffing (0xFF - dummy)
    // Rx buffer and Tx buffer have to be the same size

    tx_buffer[0] = mult_read_addrs;
    for(uint8_t i = 1; i < 2 * n_data; i++)  tx_buffer[i] = 0xFF; 

    spi_transaction_t full_duplex_read_cmd = {
        .length = 2 * sizeof(uint16_t) * n_data,
        .tx_buffer = tx_buffer,
        .rx_buffer = (uint8_t *) rx_buffer,
        .rxlength = sizeof(uint16_t) * n_data,
    };

    spi_device_acquire_bus(spi, portMAX_DELAY);

    esp_err_t ret = spi_device_transmit(spi, &full_duplex_read_cmd);
    if (ret != ESP_OK) {
        ERROR("SPI read operation failed: %s", esp_err_to_name(ret));
        return;
    }

    LOG("RECEIVED DATA IN SPI_READ_MULTIPLE_DATA_SYNC:\n");
    for (uint8_t i = 0; i < n_data; i++) {
        LOG("\t-> Data received in (%d): (hex) 0x%04X | (dec) %d\n", i, rx_buffer[i], rx_buffer[i]);
    }

    spi_device_release_bus(spi);
    free(tx_buffer);
    LOG("Reading request complete.");
}

void spi_read_multiple_data_assync(uint8_t mult_read_addrs, uint8_t n_data, uint16_t * rx_buffer) {

    LOG("### ASSYNC BURST-READ REQUEST in ADDR: %02x FOR %d BYTES ###", mult_read_addrs, 16 * n_data);

    uint8_t *tx_buffer = (uint8_t *) malloc(n_data * 2 * sizeof(uint8_t)); // Buffer for Read Instruction sent to slave

    if (tx_buffer == NULL) {
        ERROR("Error allocating memory for buffers");
        return;
    }

    // Building a burst-read instruction buffer with the same length 
    // as the read buffer through byte stuffing (0xFF - dummy)
    tx_buffer[0] = mult_read_addrs;
    for(uint8_t i = 1; i < 2 * n_data; i++)  tx_buffer[i] = 0xFF; 

    spi_transaction_t full_duplex_read_cmd = {
        .length = 2 * sizeof(uint16_t) * n_data,
        .tx_buffer = tx_buffer,
        .rx_buffer = (uint8_t *) rx_buffer,
        .rxlength = sizeof(uint16_t) * n_data,
    };

    spi_device_acquire_bus(spi, portMAX_DELAY);

    esp_err_t ret = spi_device_queue_trans(spi, &full_duplex_read_cmd, portMAX_DELAY);
    if (ret != ESP_OK) {
        ERROR("SPI read operation failed: %s", esp_err_to_name(ret));
        return;
    }

    spi_device_release_bus(spi);
    free(tx_buffer);
    LOG("Reading request complete. Processing the return in the Callback function associated with the interface");
}


void sync_spi_write_data(uint8_t addr, uint8_t data)  
{
    LOG("Writing Request in addr: %d ; data: %d", addr, data);

    spi_transaction_t trans_desc = {
        .flags = SPI_TRANS_USE_TXDATA,  // Set the Tx flag to use internal tx_data buffer
        .tx_data = {addr, data},       
        .length = 16,                   
    };

    gpio_set_level(PIN_NUM_CS, 0);                          // Lower the CS' line to select the slave
    printf("SYNC WRITE: (hex) '0x%x' DATA AT 0x%x\n", data, addr);
    ret = spi_device_polling_transmit(spi, &trans_desc);    // spi_device_polling_transmit starts to transmit entire 'trans_desc' structure.

    if (ret != ESP_OK){
        ERROR("SPI write operation failed\n"); 
        return;
    }

    vTaskDelay(1 / portTICK_PERIOD_MS);             // Once data is transferred, we provide the delay and then higher the CS'
    gpio_set_level(PIN_NUM_CS, 1);                  // After CS' is high, the slave sill get unselected
    LOG("Reading Request complete.");                                    
}
