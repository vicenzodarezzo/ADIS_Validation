#ifndef ADIS_PROTOCOL_H
#define ADIS_PROTOCOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "spi_esp_interface.h"

// ------------------------------------------------------------------
// --------------------------- DEFINES ------------------------------
// ------------------------------------------------------------------

typedef enum {
    R,       // Read
    W,       // Write
    RW       // Read/Write
} AccessType; // For Register


typedef struct {
    const char *name;        
    AccessType access;       
    uint8_t address;         
    uint16_t defaultValue; 
    const char *description;  
} Register;

typedef enum{
    SUPPLY_OUT,
    XGYRO_OUT,
    YGYRO_OUT,
    ZGYRO_OUT,
    XACCL_OUT,
    YACCL_OUT,
    ZACCL_OUT,
    XMAGN_OUT,
    YMAGN_OUT,
    ZMAGN_OUT,
    BARO_OUT,
    BARO_OUTL,
    TEMP_OUT,
    AUX_ADC,
}output_data_id;

extern const Register output_registers[];

typedef enum{
    XGYRO_OFF,
    YGYRO_OFF,
    ZGYRO_OFF,
    XACCL_OFF,
    YACCL_OFF,
    ZACCL_OFF,
    XMAGN_HIC,
    YMAGN_HIC,
    ZMAGN_HIC,
    XMAGN_SIC,
    YMAGN_SIC,
    ZMAGN_SIC,
    GPIO_CTRL,
    MSC_CTRL,
    SMPL_PRD,
    SENS_AVG,
    SLP_CTRL,
    DIAG_STAT,
    GLOB_CMD,
    ALM_MAG1,
    ALM_MAG2,
    ALM_SMPL1,
    ALM_SMPL2,
    ALM_CTRL,
    AUX_DAC,
    Reserved,
    LOT_ID1,
    LOT_ID2,
    PROD_ID,
    SERIAL_NUM,
} cntrl_data_id;

extern const Register cntrl_registers[];

extern const size_t OUTPUT_NUM_REGISTERS;
extern const size_t CNTRL_NUM_REGISTERS;

// ------------------------------------------------------------------
// ------------------------- SETUP FEATURES -------------------------
// ------------------------------------------------------------------

void init_adis_interface(bool assync_flag);

// ------------------------------------------------------------------
// ----------------------- READ DATA FEATURES -----------------------
// ------------------------------------------------------------------

uint16_t single_read_register(output_data_id id);
void burst_read_register(uint16_t * data, bool assync_flag);

// ------------------------------------------------------------------
// ----------------------- WRITE DATA FEATURES ----------------------
// ------------------------------------------------------------------

// ------------------------------------------------------------------
// ------------------------ CONTROL FEATURES ------------------------
// ------------------------------------------------------------------

#endif // ADIS_PROTOCOL_H