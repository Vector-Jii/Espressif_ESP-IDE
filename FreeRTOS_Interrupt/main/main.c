//Interrupt example to blink led with a button

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "driver/gpio.h"
#include "esp_attr.h"
#include "esp_rom_gpio.h"
#include "hal/gpio_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ESP_INTR_FLAG_DEF 0
#define BUTTON 1
#define LED 2 

TaskHandle_t ISR = NULL;

void IRAM_ATTR button_isr_handler(){
	xTaskResumeFromISR(ISR);
} 
void button_task(){
	bool led_status = false;
	
	while(1){
		vTaskSuspend(NULL);
		led_status = !led_status;
		gpio_set_level(LED, led_status);;
		printf("Button pressed");
	}
}

void app_main(void)
{
	//set as gpio pins
   esp_rom_gpio_pad_select_gpio(BUTTON);
   esp_rom_gpio_pad_select_gpio(LED);
   
   //set weater input or output
   gpio_set_direction(BUTTON,GPIO_MODE_INPUT);
   gpio_set_direction(LED, GPIO_MODE_OUTPUT);
   
   //Enable interrupt on falling edge of the button pin
   gpio_set_intr_type(BUTTON, GPIO_INTR_NEGEDGE);
   
   //Install driver's gpio ISR handler service 
   gpio_install_isr_service(ESP_INTR_FLAG_DEF);
   
   //attach interrupt service routine 
   gpio_isr_handler_add(BUTTON, button_isr_handler , NULL);
   
   //Create Task 
   xTaskCreate(button_task, "button_task", 4096, NULL, 10, &ISR);
   
   }
