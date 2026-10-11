/*
 * rgb_led.h
 *
 *  Created on: 19-Aug-2026
 *      Author: Tejas Sawant
 */

#ifndef MAIN_RGB_LED_H_
#define MAIN_RGB_LED_H_

//RGB LED GPIO
#define RGB_LED_RED_GPIO	21
#define RGB_LED_GREEN_GPIO	22
#define RGB_LED_BLUE_GPIO	23

//RGB led color mix channel
#define RGB_LED_CHANNEL_NUM 3

//RGB LED Configuration
typedef struct
{
	int channel;
	int gpio;
	int mode;
	int timer_index;
}ledc_info_t;

ledc_info_t ledc_ch[RGB_LED_CHANNEL_NUM];  // RGB_LED_CHANNEL_NUM 3

//Color to indicate wifi app is started
void rgb_led_wifi_app_started();

//Color to indicate HTTP server has started
void rgb_led_http_server_started(); 

//Color to indicate it is connected to access point
void rgb_led_wifi_connected();


#endif /* MAIN_RGB_LED_H_ */
