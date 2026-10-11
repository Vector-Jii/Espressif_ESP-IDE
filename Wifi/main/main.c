#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "esp_err.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_event_loop.h"    // declare events and register handlers to those events
#include "esp_wifi.h"  // declare that a component depends on esp_wifi
#include "lwip/tcpip.h"
#include "nvs_flash.h" //store key-value pairs in flash
#include "portmacro.h"
#include "esp_netif.h"

void scan(){
	wifi_scan_config_t scan_config = {
		.ssid = 0,   //Name of network
		.bssid = 0,   // mac address 
	    .channel = 0,
	    .show_hidden = true,
	};
	
	printf("Start scanning \n");
	
	ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
	printf("Scan complete ");
	
	uint16_t ap_num;
	wifi_ap_record_t ap_records[20];    //struct to store scanned values
	
	ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_num, ap_records));
	
	//Print scanned vslue on console 
	printf("  SSID          | CHANNEL   |RSSI      |MAC   \n\n");
	
	for(int i=0;i<ap_num;i++){
		
		printf("%s   |%d   |%d   |%02x:%02x:%02x:%02x:%02x:%02x\n",
       ap_records[i].ssid,
       ap_records[i].primary,
       ap_records[i].rssi,
       ap_records[i].bssid[0],
       ap_records[i].bssid[1],
       ap_records[i].bssid[2],
       ap_records[i].bssid[3],
       ap_records[i].bssid[4],
       ap_records[i].bssid[5]);

		}
	
	
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    esp_netif_init();
    wifi_init_config_t wifi_config =WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    while(1){
		vTaskDelay(3000/portTICK_PERIOD_MS);
		
		scan();
		
	}
}
