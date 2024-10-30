#include "../inc/adis_protocol.h"

#define ERROR(fmt, ...) printf("[ERROR] : (ADIS PROTOCOL) - " fmt "\n", __VA_ARGS__)
#define LOG(fmt, ...) printf("[LOG] : (ADIS PROTOCOL) - " fmt "\n", __VA_ARGS__)

const Register output_registers[] = {
    {"FLASH_CNT",  R,      0x00, 0, "Flash memory write count"},
    {"SUPPLY_OUT", R,      0x02, 0, "Power supply measurement"},
    {"XGYRO_OUT",  R,      0x04, 0, "X-axis gyroscope output"},
    {"YGYRO_OUT",  R,      0x06, 0, "Y-axis gyroscope output"},
    {"ZGYRO_OUT",  R,      0x08, 0, "Z-axis gyroscope output"},
    {"XACCL_OUT",  R,      0x0A, 0, "X-axis accelerometer output"},
    {"YACCL_OUT",  R,      0x0C, 0, "Y-axis accelerometer output"},
    {"ZACCL_OUT",  R,      0x0E, 0, "Z-axis accelerometer output"},
    {"XMAGN_OUT",  R,      0x10, 0, "X-axis magnetometer measurement"},
    {"YMAGN_OUT",  R,      0x12, 0, "Y-axis magnetometer measurement"},
    {"ZMAGN_OUT",  R,      0x14, 0, "Z-axis magnetometer measurement"},
    {"BARO_OUT",   R,      0x16, 0, "Barometer pressure measurement, high word"},
    {"BARO_OUTL",  R,      0x18, 0, "Barometer pressure measurement, low word"},
    {"TEMP_OUT",   R,      0x1A, 0, "Temperature output"},
    {"AUX_ADC",    R,      0x1C, 0, "Auxiliary ADC measurement"}
};

const Register cntrl_registers[] = {
    {"XGYRO_OFF",  RW,     0x1E, 0x0000, "X-axis gyroscope bias offset factor"},
    {"YGYRO_OFF",  RW,     0x20, 0x0000, "Y-axis gyroscope bias offset factor"},
    {"ZGYRO_OFF",  RW,     0x22, 0x0000, "Z-axis gyroscope bias offset factor"},
    {"XACCL_OFF",  RW,     0x24, 0x0000, "X-axis acceleration bias offset factor"},
    {"YACCL_OFF",  RW,     0x26, 0x0000, "Y-axis acceleration bias offset factor"},
    {"ZACCL_OFF",  RW,     0x28, 0x0000, "Z-axis acceleration bias offset factor"},
    {"XMAGN_HIC",  RW,     0x2A, 0x0000, "X-axis magnetometer, hard iron factor"},
    {"YMAGN_HIC",  RW,     0x2C, 0x0000, "Y-axis magnetometer, hard iron factor"},
    {"ZMAGN_HIC",  RW,     0x2E, 0x0000, "Z-axis magnetometer, hard iron factor"},
    {"XMAGN_SIC",  RW,     0x30, 0x0800, "X-axis magnetometer, soft iron factor"},
    {"YMAGN_SIC",  RW,     0x32, 0x0800, "Y-axis magnetometer, soft iron factor"},
    {"ZMAGN_SIC",  RW,     0x34, 0x0800, "Z-axis magnetometer, soft iron factor"},
    {"GPIO_CTRL",  RW,     0x36, 0x0000, "Auxiliary digital input/output control"},
    {"MSC_CTRL",   RW,     0x38, 0x0006, "Miscellaneous control"},
    {"SMPL_PRD",   RW,     0x3A, 0x0001, "Internal sample period rate control"},
    {"SENS_AVG",   RW,     0x3C, 0x0402, "Dynamic range and digital filter control"},
    {"SLP_CTRL",   W,      0x3E, 0, "Sleep mode control"},
    {"DIAG_STAT",  R,      0x40, 0x0000, "System status"},
    {"GLOB_CMD",   W,      0x42, 0x0000, "System command"},
    {"ALM_MAG1",   RW,     0x44, 0x0000, "Alarm 1 amplitude threshold"},
    {"ALM_MAG2",   RW,     0x46, 0x0000, "Alarm 2 amplitude threshold"},
    {"ALM_SMPL1",  RW,     0x48, 0x0000, "Alarm 1 sample size"},
    {"ALM_SMPL2",  RW,     0x4A, 0x0000, "Alarm 2 sample size"},
    {"ALM_CTRL",   RW,     0x4C, 0x0000, "Alarm control"},
    {"AUX_DAC",    RW,     0x4E, 0x0000, "Auxiliary DAC data"},
    {"Reserved",   RW,     0x50, 0, "Reserved"},
    {"LOT_ID1",    R,      0x52, 0, "Lot identification number"},
    {"LOT_ID2",    R,      0x54, 0, "Lot identification number"},
    {"PROD_ID",    R,      0x56, 0x4107, "Product identifier"},
    {"SERIAL_NUM", R,      0x58, 0, "Serial number"},
};

const size_t OUTPUT_NUM_REGISTERS = sizeof(output_registers) / sizeof(output_registers[0]);
const size_t CNTRL_NUM_REGISTERS =  sizeof(cntrl_registers) / sizeof(cntrl_registers[0]);

void init_adis_interface(bool assync_flag){
    LOG("Initializing ADIS Interface - Assync Flag: %d", (int) assync_flag);
    spi_init(assync_flag);
}

uint16_t single_read_register(output_data_id id){

    if(id > OUTPUT_NUM_REGISTERS) ERROR("Invalid Register ID : %d", id);
    uint8_t addr = output_registers[id].address;
    uint16_t data = 0;
    data = sync_spi_read_data(addr);
    LOG("Reading Register %s : %d", output_registers[id].name, data);
    return data;
}

void burst_read_register(uint16_t * data, bool assync_flag){
    if(assync_flag) {spi_read_multiple_data_assync(cntrl_registers[GLOB_CMD].address, OUTPUT_NUM_REGISTERS, data);}
    else {spi_read_multiple_data_sync(cntrl_registers[GLOB_CMD].address, OUTPUT_NUM_REGISTERS, data);}
    return;
}