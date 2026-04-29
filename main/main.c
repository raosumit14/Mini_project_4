#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sens.h"

void app_main(void)

{
    hx710b_init();
    gps_init();
    sdcard_init();

    while (1)
    {

        hx710b_user();
        gps_user();
        sdcard_write(pressure, gps_buffer);
        printf("------ cycle done ------\n");

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}