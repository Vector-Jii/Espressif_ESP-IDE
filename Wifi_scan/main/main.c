#include <stdio.h>
#include "esp_err.h"
#include "esp_interface.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "portmacro.h"


#define AP_SSID "Akshata"
#define AP_PASSWORD "123456789"
#define AP_CHANNEL 1
#define AP_MAX_CONNECTIONS 4

void scann(){
// configure and run the scan process in blocking mode
  wifi_scan_config_t scan_config = {
    .ssid = 0,
    .bssid = 0,
    .channel = 0,
        .show_hidden = true
    };
  printf("Start scanning...");
  ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
  printf(" completed!\n");
 
  // get the list of APs found in the last scan
  uint16_t ap_num;
  wifi_ap_record_t ap_records[20];
     ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_num));
  ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_num, ap_records));
  
  // print the list 
  printf("Found %d access points:\n", ap_num);

  printf("               SSID              | Channel | RSSI |   MAC \n\n");
  //printf("----------------------------------------------------------------\n");
  for(int i = 0; i < ap_num; i++)
    printf("%32s | %7d | %4d   %2x:%2x:%2x:%2x:%2x:%2x   \n", ap_records[i].ssid, ap_records[i].primary, ap_records[i].rssi , *ap_records[i].bssid, *(ap_records[i].bssid+1), *(ap_records[i].bssid+2), *(ap_records[i].bssid+3), *(ap_records[i].bssid+4), *(ap_records[i].bssid+5));
  printf("----------------------------------------------------------------\n");   
 
}


void app_main()
{
  // initialize NVS
  ESP_ERROR_CHECK(nvs_flash_init());
  
esp_netif_init();

  wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();  //set default configuration 
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_start());// starts wifi usage
  
  wifi_config_t ap_config ={
	  .ap ={
		  .ssid = AP_SSID,
		  .password = AP_PASSWORD,
		  .channel = AP_CHANNEL,
		  .max_connection = AP_MAX_CONNECTIONS,
		  .ssid_hidden = 0
		  
	  },
  };
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_config));
  
      while(1) {  vTaskDelay(3000 / portTICK_PERIOD_MS);
  scann();
      }
  }