#include <stdio.h>
#include <sys/_intsup.h>
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "portmacro.h"
#include "freertos/queue.h"

TaskHandle_t myTask1Handler =NULL;
TaskHandle_t myTask2Handler =NULL;

QueueHandle_t queue1;

void task1(void *args)
{
	char txbuff[50];
	
	queue1 = xQueueCreate(5,sizeof(txbuff)); // @param  (no.of elements in queue , size of each element ) 
	
	if(queue1 == 0)
	{
		printf("failed to create queue1= %p \n",queue1)	;  
			
	}
	
	sprintf(txbuff,"hello world 1 \n");
	xQueueSend(queue1,(void*)txbuff,(TickType_t)0);
	
	sprintf(txbuff,"hello world 2 \n");
	xQueueSend(queue1,(void*)txbuff,(TickType_t)0);
	
	sprintf(txbuff,"hello world 3 \n");
	xQueueSend(queue1,(void*)txbuff,(TickType_t)0);
	
	while(1)
	{
		printf("data waiting to be read %d available spaces : %d \n",uxQueueMessagesWaiting(queue1),uxQueueSpacesAvailable(queue1));
		vTaskDelay(pdMS_TO_TICKS(1000));
	} 
}

void task2(void *args)
{
	char rxbuff[40];
	
	while(1){
		if(xQueueReceive(queue1, &rxbuff, (TickType_t)5)){
			
		printf("got the data from the queue ! === %s",rxbuff);
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
	}
}

void app_main()
{
	xTaskCreate(task1,"task2",4096,NULL,10,&myTask1Handler);
	xTaskCreatePinnedToCore(task2,"task2",4096,NULL,11,&myTask2Handler,0);
	
}