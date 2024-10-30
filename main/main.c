#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "adis_lib/inc/adis_protocol.h"

/**
 * ################### COMENTÁRIOS ###################
 * (vicenzo) -> funções não bloqueantes não estão funcionando, usar somente assync_flag = FALSE
 * 
 * 
 * 
 */

#define MAIN_TAG "MAIN"
#define BURST_READ true
#define STALL_TIME_MS 5000

void app_main(void)
{
    ESP_LOGI(MAIN_TAG, "Iniciando Interface\n");
    init_adis_interface(false);
    output_data_id single_read = BARO_OUT;

    while(1){
        if(BURST_READ){
            ESP_LOGI(MAIN_TAG, "Burst Read Requisition\n");
            uint16_t * burst_data = malloc(OUTPUT_NUM_REGISTERS * sizeof(uint16_t));
            assert(burst_data);
            burst_read_register(burst_data, false);      
        }else{
            ESP_LOGI(MAIN_TAG, "Single Read Requisition: \n");
            single_read_register(single_read);
        }
        vTaskDelay(STALL_TIME_MS / portTICK_PERIOD_MS);      
    }
    
}
