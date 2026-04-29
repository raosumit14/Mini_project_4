#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"  
#include <string.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdspi_host.h"

#define TAG "GPS"
#define SD_CS 13
#define UART_NUM UART_NUM_2
#define TXD_PIN GPIO_NUM_17
#define RXD_PIN GPIO_NUM_16
#define BUF_SIZE 1024

#define HX710B_DT GPIO_NUM_4
#define HX710B_SCK GPIO_NUM_5
#define OFFSET 567000
#define SCALE 200.0 // for calibration : when we know pressure applied and change in raw value, we can calculate scale factor
float pressure = 0.0;
char gps_buffer[64] = "No Fix";

void hx710b_init()
{
    gpio_set_direction(HX710B_DT, GPIO_MODE_INPUT);
    gpio_set_direction(HX710B_SCK, GPIO_MODE_OUTPUT);
    gpio_set_level(HX710B_SCK, 0);
}

// Wait until data is ready 
void hx710b_wait_ready()
{
    while (gpio_get_level(HX710B_DT) == 1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Read 24-bit raw data  : 24 bit becuse there is very small 
// change in voltage when press change so it is better to use 24 bit to get more precise value

int32_t hx710b_read()
{
    int32_t data = 0;

    hx710b_wait_ready();

    for (int i = 0; i < 24; i++)
    {
        gpio_set_level(HX710B_SCK, 1);
        ets_delay_us(1);

        data = data << 1;

        gpio_set_level(HX710B_SCK, 0);
        ets_delay_us(1);

        if (gpio_get_level(HX710B_DT))
        {
            data++;
        }
    }

    // 25th pulse (sets gain/channel)
    gpio_set_level(HX710B_SCK, 1);
    ets_delay_us(1);
    gpio_set_level(HX710B_SCK, 0);
    ets_delay_us(1);

    // Convert to signed 24-bit
    if (data & 0x800000)
    {
        data |= ~0xFFFFFF;
    }

    return data;
}

void hx710b_user(void)
{
    //  hx710b_init();

    {
        int32_t raw = hx710b_read();

        pressure = (raw - OFFSET) / SCALE;

        printf("Raw: %d | Pressure: %.2f kPa\n", raw, pressure);
    }
}
//  //// // for gps
void gps_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}
void gps_user(void)
{
    uint8_t data[BUF_SIZE];

    int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, pdMS_TO_TICKS(2000));

    if (len <= 0)
    {
        strcpy(gps_buffer, "No Fix");
        printf("GPS: Not connected\n");
        return;
    }

    data[len] = '\0';

    char *gprmc = strstr((char *)data, "$GPRMC");

    if (gprmc == NULL)
    {
        strcpy(gps_buffer, "No Fix");
        printf("GPS: No valid data\n");
        return;
    }

    char status;
    char lat[15], lon[15];

    sscanf(gprmc, "$GPRMC,%*[^,],%c,%[^,],%*c,%[^,]", &status, lat, lon);

    if (status == 'A') // a means satellite is lock with gps and we have valid data
    {
        sprintf(gps_buffer, "Lat:%s Lon:%s", lat, lon);
        printf("GPS FIXED → %s\n", gps_buffer);
    }
    else
    {
        strcpy(gps_buffer, "No Fix");
        printf("GPS: Not connected\n");
    }
}
///////////////   SD card

void sdcard_init(void)
{
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.max_freq_khz = 200; // VERY LOW speed

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = 23,
        .miso_io_num = 19,
        .sclk_io_num = 18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1};

    spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_CS;
    slot_config.host_id = host.slot;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5};

    sdmmc_card_t *card;
    esp_err_t ret = esp_vfs_fat_sdspi_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK)
    {
        printf(" SD card mount failed\n");
        return;
    }

    printf(" SD card mounted\n");
}
void sdcard_write(float pressure, char *gps)
{
    FILE *f = fopen("/sdcard/data.txt", "a");

    if (f == NULL)
    {
        printf(" File open failed\n");
        return;
    }
    if (f > 0)
    {
        printf(" Data stored successfully\n");
    }
    else
    {
        printf("Write failed\n");
    }
    fprintf(f, "Pressure: %.2f kPa | GPS: %s\n", pressure, gps);

    fclose(f);
}