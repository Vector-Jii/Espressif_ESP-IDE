/*Esp logging library used for debugging the complex projects .It has coloring
   ESP Logging has 3 coloring and TAGs 
   
   This program focus on "ESP_LOGI , ESP_ERR  function" */
 
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "portmacro.h"
#include "esp_log.h"

#define TASK1_TAG "Task1"
#define TASK2_TAG "Task2"

TaskHandle_t myTask1Handler =NULL;
TaskHandle_t myTask2Handler =NULL;

void task1(void *args){
	while(1){
		ESP_LOGI(TASK1_TAG ,"hello  from task1 \n");
	vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void task2(void *args){
	while(1){
		ESP_LOGW(TASK2_TAG ,"hello from task2 \n");
	vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

 void app_main(){
	 
	 xTaskCreate(task1,"task1",4096,NULL,10,&myTask1Handler);
	 xTaskCreatePinnedToCore(task2,"task2",4096,NULL,10,&myTask2Handler,1);
 }  
   