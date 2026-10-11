/*
 * wifi_app.h
 *
 *  Created on: 06-Jan-2025
 *      Author: Tejas Sawant
 */

#ifndef MAIN_WIFI_APP_H_
#define MAIN_WIFI_APP_H_

#include "esp_netif.h"
#include "esp_netif_types.h"
#include "portmacro.h"

//wifi application settings
#define WIFI_AP_SSID              "ESP32_AP"
#define WIFI_AP_PASSWORD          "password"
#define WIFI_AP_CHANNEL            1
#define WIFI_AP_SSID_HIDDEN        0
#define WIFI_AP_MAX_CONNECTION     2
#define WIFI_AP_BEACON_INTERVAL    100
#define WIFI_AP_IP                 "192.168.0.1"
#define WIFI_AP_GATEWAY            "192.168.0.1"
#define WIFI_AP_NETMASK            "255.255.255.0"
#define WIFI_AP_BANDWIDTH          WIFI_BW_HT20
#define WIFI_STA_POWER_SAVE        WIFI_PS_NONE
#define MAX_SSID_LENGTH            32
#define MAX_PASSWORD_LENGTH        64
#define MAX_CONNECTION_RETRIES     4

//Creating object for access and station point each
extern esp_netif_t* esp_netif_ap;
extern esp_netif_t* esp_netif_sta;

//Message ID for wifi application 
typedef enum wifi_app_message
{
	WIFI_APP_MSG_START_HTTP_SERVER=0,
	WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER,
	WIFI_APP_MSG_STA_CONNECTED_GOT_IP,
}wifi_app_message_e;

//Structure for message queue
typedef struct wifi_app_queue_message
{
	wifi_app_message_e msgID;
}wifi_app_queue_message_t ;


//Function prototypes

// 1)To send message to the queue {FreeRTOS task}
BaseType_t wifi_app_send_message(wifi_app_message_e msgID);

// 2) To start wifi RTOS task
void wifi_app_start(void);















#endif /* MAIN_WIFI_APP_H_ */
