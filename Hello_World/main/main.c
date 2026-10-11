#include <stdio.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "hal/ledc_types.h"

void app_main(void)
{
    // Log tag for identification
    const char *TAG = "HELLO_WORLD";

    // Print "Hello, World!" continuously every second
    while (true)
    {
        ESP_LOGI(TAG, "Hello, World!");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1-second delay
    }
    
   
}
