/*
 * rgb_led.c
 *
 *  Created on: 02-Jan-2025
 *      Author: Tejas Sawant
 */

#include "driver/ledc.h"
#include "hal/ledc_types.h"
#include "stdbool.h"
#include "driver/gpio.h"
#include "rgb_led.h"
#include <stdint.h>

//Handle for pwm init 
bool g_pwm_init_handle = false;

ledc_info_t ledc_ch[RGB_LED_CHANNEL_NUM];  //Cleaner declaration of structure variable

// Initialize RGB LED settings [channel,color,timer,mode]
static void rgb_led_pwn_init(void){
	int rgb_ch;
	
	//red 
	ledc_ch[0].channel = LEDC_CHANNEL_0;
	ledc_ch[0].gpio = RED_COLOR;
	ledc_ch[0].mode = LEDC_HIGH_SPEED_MODE;
	ledc_ch[0].timer_index = LEDC_TIMER_0;
	
	//green
	ledc_ch[1].channel = LEDC_CHANNEL_0;
	ledc_ch[1].gpio = RED_COLOR;
	ledc_ch[1].mode = LEDC_HIGH_SPEED_MODE;
	ledc_ch[1].timer_index = LEDC_TIMER_0;
	
	//blue
	ledc_ch[2].channel = LEDC_CHANNEL_0;
	ledc_ch[2].gpio = RED_COLOR;
	ledc_ch[2].mode = LEDC_HIGH_SPEED_MODE;
	ledc_ch[2].timer_index = LEDC_TIMER_0;
	
	//configure TIMER 0 
	ledc_timer_config_t ledc_timer = {
		.duty_resolution = LEDC_TIMER_8_BIT,
		.freq_hz = 100,
		.speed_mode = LEDC_HIGH_SPEED_MODE,
		.timer_num = LEDC_TIMER_0
	};
	ledc_timer_config(&ledc_timer);
	
	//Configure channels 
	for(rgb_ch = 0;rgb_ch < RGB_LED_CHANNEL_NUM;rgb_ch++)
	{
		ledc_channel_config_t ledc_channel = {
			.channel = ledc_ch[rgb_ch].channel,
			.duty = 0,
			.hpoint = 0,
			.gpio_num = ledc_ch[rgb_ch].gpio,
			.intr_type = LEDC_INTR_DISABLE,
			.speed_mode = ledc_ch[rgb_ch].mode,
			.timer_sel = ledc_ch[rgb_ch].timer_index,				
		};
		ledc_channel_config(&ledc_channel);
	}
	g_pwm_init_handle = true;
}

// Set rgb color
static void rgb_led_set_color(uint8_t red ,uint8_t green ,uint8_t blue){
	
	ledc_set_duty(ledc_ch[0].mode, ledc_ch[0].channel,red);
	ledc_update_duty(ledc_ch[0].mode, ledc_ch[0].channel);
	
	ledc_set_duty(ledc_ch[1].mode, ledc_ch[1].channel,green);
	ledc_update_duty(ledc_ch[1].mode, ledc_ch[1].channel);
	
	ledc_set_duty(ledc_ch[2].mode, ledc_ch[2].channel,blue);
	ledc_update_duty(ledc_ch[2].mode, ledc_ch[2].channel);
}


//Color to indicate WiFi application is started
void rgb_led_wifi_app_started(void){
	if (g_pwm_init_handle == false){
		rgb_led_pwn_init();
	}
	rgb_led_set_color(255,  0, 0);
}

//Color to indicate HTTP server started 
void rgb_led_http_server_started(void){
	if (g_pwm_init_handle == false){
		rgb_led_pwn_init();
	}
	rgb_led_set_color(0,  255, 0);
}

//Color to indicate ESP32 has connected to access point
void rgb_led_wifi_connected(void){
	if (g_pwm_init_handle == false){
		rgb_led_pwn_init();
	}
	rgb_led_set_color(0,  0, 255);
}

