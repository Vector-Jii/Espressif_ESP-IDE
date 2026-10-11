/*
 * rgb_led.c
 *
 *  Created on: 19-Aug-2026
 *      Author: Tejas Sawant
 */
#include <stdbool.h>
#include <sys/_intsup.h>

#include "driver/ledc.h"
#include "hal/ledc_types.h"
#include "rgb_led.h"

// Initialize the RGB LED settings like the gpio, channel, timer config
void rgb_led_pwn_init(){
	int rgb_ch;
	
	// red
	ledc_ch[0].channel = LEDC_CHANNEL_0;
	ledc_ch[0].gpio = RGB_LED_RED_GPIO;   //21
	ledc_ch[0].mode = LEDC_HIGH_SPEED_MODE;
	ledc_ch[0].timer_index = LEDC_TIMER_0;
	
	//Greem
	ledc_ch[1].channel = LEDC_CHANNEL_1;
	ledc_ch[1].gpio = RGB_LED_GREEN_GPIO;   //21
	ledc_ch[1].mode = LEDC_HIGH_SPEED_MODE;
	ledc_ch[1].timer_index = LEDC_TIMER_0;
	
	//Blue
	
	ledc_ch[2].channel = LEDC_CHANNEL_2;
	ledc_ch[2].gpio = RGB_LED_BLUE_GPIO;   //21
	ledc_ch[2].mode = LEDC_HIGH_SPEED_MODE;
	ledc_ch[2].timer_index = LEDC_TIMER_0;
} 