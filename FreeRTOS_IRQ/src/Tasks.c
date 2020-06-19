#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "sapi.h"
#include "types_config.h"


DEBUG_PRINT_ENABLE;

void vTaskT0 (void* xTaskParams)
{
    tConfig* x = (tConfig*)xTaskParams;

    fsmButtonInit(x);

    while(1){

        fsmButtonUpdate(x);
        vTaskDelay(1 / portTICK_RATE_MS);
 
    }
}



void vTaskT1 (void* xTaskParams)
{
    tConfig* x = (tConfig*)xTaskParams;
    TickType_t  t;
    
    while(1){

        xQueueReceive(x->queue, &t, portMAX_DELAY);
        gpioWrite(x->pinOut, ON);
        vTaskDelay(t);
        gpioWrite(x->pinOut, OFF);
        //t = 0;

    }
}


void vTaskReadADC(void* xTaskParams){
    
    tConfigADC* x = (tConfigADC*)xTaskParams;

    while(1){
        x-> value = adcRead( x-> adcChannel );
       //if( 
        xQueueSend(x->queue, &(x->value), portMAX_DELAY );
        //    != pdPASS)
        vTaskDelay(10/ portTICK_RATE_MS);
        debugPrintlnString( "Error TaskADC\r\n" );
    }
}

void vTaskWriteSPI(void* xTaskParams);
void vTaskButtonRead(void* xTaskParams);

void vTaskWriteUART(void* xTaskParams){


}

void vTaskProcessing(void* xTaskParams){

    tConfigADC* x = (tConfigADC*)xTaskParams;

    uint16_t sample;
    portTickType timePeriod = 500 / portTICK_RATE_MS;
    portTickType timeDiff = xTaskGetTickCount();

    while(1){
        xQueueReceive(x->queue, &sample, portMAX_DELAY);
        debugPrintlnString( "TaskProcessing:\r\n");

        gpioWrite(LEDR, ON);
        vTaskDelay(sample);
        gpioWrite(LEDR, OFF);
        vTaskDelay(sample);
    }
    
}
