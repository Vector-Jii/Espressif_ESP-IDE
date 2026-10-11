/*
 * rgb_led.h
 *
 *  Created on: 02-Jan-2025
 *      Author: Tejas Sawant
 */

#ifndef MAIN_RGB_LED_H_
#define MAIN_RGB_LED_H_

//Declare RGB led GPIO pins
#define RED_COLOR  21
#define GREEN_COLOR 22
#define BLUE_COLOR 23

// No of channel
#define  RGB_LED_CHANNEL_NUM 3

// Declaring structure for LED Configuration
typedef struct{
	int channel;
	int gpio;
	int mode;
	int timer_index;
} ledc_info_t;
 


/*****************************************    FUNCTION PROTOTYPES     *******************************************************/

//Color to indicate WiFi application is started
void rgb_led_wifi_app_started(void);

//Color to indicate HTTP server started 
void rgb_led_http_server_started(void);

//Color to indicate ESP32 has connected to access point
void rgb_led_wifi_connected(void);


#endif /* MAIN_RGB_LED_H_ */
