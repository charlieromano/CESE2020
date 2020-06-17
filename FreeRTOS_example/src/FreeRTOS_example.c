#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"
#include "chip.h"
#include "types_config.h"

tConfig xTaskParams[2];

void vTaskT0 (void* xTaskParams);
void vTaskT1 (void* xTaskParams);
 

int main (void){

    boardInit();

    xTaskParams[0].queue = xQueueCreate(1,sizeof(TickType_t));
    //xTaskParams[1].semMutex = xSemaphoreCreate();
    
    vTaskCreate(vTaskT0, "Task Button", configMINIMAL_STACK_SIZE*2, &xTaskParams[0], taskiDLE_PRIORITY+1,0  );
    vTaskCreate(vTaskT1, "Task Led",    configMINIMAL_STACK_SIZE*2, &xTaskParams[1], taskiDLE_PRIORITY+1,0  );

    vTaskStartScheduler();

    while(1);

}


void vTaskT0 (void* xTaskParams)
{
    tConfig* x = (tConfig*)xTaskParams;

    fsmButtonInit(x);

    while(1){

        fsmButtonUpdate(x);
        vTaskDelay(1 / portTICK_RATE_MS);
        if (x->tiempo_medido > 0)
            xQueueSend( x->tiempo_medido );      
    }
}

void vTaskT1 (void* xTaskParams)
{
    tConfig* x = (tConfig*)xTaskParams;
    TickType_t  t;
    
    while(1){

        xQueueReceive(x->queue,&t, portMAX_DELAY);
        gpioWrite(LEDB, ON);
        vTaskDelay(t);
        gpioWrite(LEDB, OFF);
        //t = 0;

    }
}
