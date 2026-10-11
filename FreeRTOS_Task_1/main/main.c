#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/task.h"
#include "portmacro.h"

TaskHandle_t TaskHandle = NULL;
TaskHandle_t TaskHandle2 = NULL;

void task1(void *args){
	printf("Task2 has name : %s \n",pcTaskGetName(TaskHandle2));
	int c = 0;
	while(true){   //Infinite loop
	    c=xTaskGetTickCount();
	    printf("[%d]Hell0 from task1 \n",c);
	    
		printf(" Hello from task1 \n");
		vTaskDelay(1000/portTICK_PERIOD_MS );
		
		if (c ==301){
			vTaskSuspend(TaskHandle2);
			printf("Task2 suspended %d \n",eTaskGetState(TaskHandle2));
		}
		if (c ==501){
			vTaskResume(TaskHandle2);
			printf("Task2 resumed %d \n",eTaskGetState(TaskHandle2));
		}
		if (c ==701){
			vTaskDelete(TaskHandle2);
			printf("Task2 deleted %d \n",eTaskGetState(TaskHandle2));
		}
	}
}

void task2(void *args){
	for(int i=0 ;i<15;i++){ //Finite loop This will cause the esp32 to reset 
		printf(" Hello from task2 \n");
		vTaskDelay(1000/portTICK_PERIOD_MS );
	}
}

void app_main(void)
{
   xTaskCreate(task1,"task1",4096,NULL ,10,&TaskHandle);
   xTaskCreatePinnedToCore(task2, "task2",4096,NULL, 10,&TaskHandle2,1);
}
/*NOTE : The tasks in xTaskCreate must be a infinite loop otherwise it will reset the esp32 */
