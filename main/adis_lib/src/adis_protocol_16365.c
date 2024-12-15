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
    {"XTEMP_OUT",  R,      0x10, 0, "X-axis gyroscope temperature output "},
    {"YTEMP_OUT",  R,      0x12, 0, "Y-axis gyroscope temperature output"},
    {"ZTEMP_OUT",  R,      0x14, 0, "Z-axis gyroscope temperature output "},
    {"AUX_ADC",    R,      0x1C, 0, "Auxiliary ADC measurement"}
};

const Register cntrl_registers[] = {
    {"XGYRO_OFF",  RW,     0x1A, 0x0000, "X-axis gyroscope bias offset factor"},
    {"YGYRO_OFF",  RW,     0x1C, 0x0000, "Y-axis gyroscope bias offset factor"},
    {"ZGYRO_OFF",  RW,     0x1E, 0x0000, "Z-axis gyroscope bias offset factor"},
    {"XACCL_OFF",  RW,     0x20, 0x0000, "X-axis acceleration bias offset factor"},
    {"YACCL_OFF",  RW,     0x22, 0x0000, "Y-axis acceleration bias offset factor"},
    {"ZACCL_OFF",  RW,     0x24, 0x0000, "Z-axis acceleration bias offset factor"},
    {"ALM_MAG1",   RW,     0x26, 0x0000, "Alarm 1 amplitude threshold"},
    {"ALM_MAG2",   RW,     0x28, 0x0000, "Alarm 2 amplitude threshold"},
    {"ALM_SMPL1",  RW,     0x2A, 0x0000, "Alarm 1 sample size"},
    {"ALM_SMPL2",  RW,     0x2C, 0x0000, "Alarm 2 sample size"},
    {"ALM_CTRL",   RW,     0x2E, 0x0000, "Alarm control"},
    {"AUX_DAC",    RW,     0x30, 0x0000, "Auxiliary DAC data"},
    {"GPIO_CTRL",  RW,     0x32, 0x0000, "Auxiliary digital input/output control"},
    {"MSC_CTRL",   RW,     0x34, 0x0006, "Miscellaneous control"},
    {"SMPL_PRD",   RW,     0x36, 0x0001, "Internal sample period rate control"},
    {"SENS_AVG",   RW,     0x38, 0x0402, "Dynamic range and digital filter control"},
    {"SLP_CTRL",   W,      0x3A, 0x0000, "Sleep mode control"},
    {"DIAG_STAT",  R,      0x3C, 0x0000, "System status"},
    {"GLOB_CMD",   W,      0x3E, 0x0000, "System command"},
    {"Reserved",   RW,     0x40, 0,      "Reserved"},                                   //Adress -> 0x40 to 0x51
    {"LOT_ID1",    R,      0x52, 0,      "Lot identification number"},
    {"LOT_ID2",    R,      0x54, 0,      "Lot identification number"},
    {"PROD_ID",    R,      0x56, 0x3FED, "Product identifier"},
    {"SERIAL_NUM", R,      0x58, 0,      "Serial number"},
};

const size_t OUTPUT_NUM_REGISTERS = sizeof(output_registers) / sizeof(output_registers[0]);
const size_t CNTRL_NUM_REGISTERS =  sizeof(cntrl_registers) / sizeof(cntrl_registers[0]);