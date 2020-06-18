#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"
#include "chip.h"
#include "types_config.h"

void tarea_led( void* taskParmPtr );
void tarea_tecla( void* taskParmPtr );
void vTaskT0 (void* xTaskParams);
void vTaskT1 (void* xTaskParams);
 
DEBUG_PRINT_ENABLE;

tConfig xTaskParams[2];

int main (void){

    boardConfig();
    debugPrintConfigUart( UART_USB, 115200 );
    debugPrintlnString( "Ejercicio 3.1" );
    gpioWrite( LED3 , ON );

    xTaskParams[0].pinIn = TEC1;
    xTaskParams[0].pinOut   = LEDB;
    xTaskParams[0].queue = xQueueCreate(1,sizeof(TickType_t));

    xTaskParams[1].pinIn = TEC2;
    xTaskParams[1].pinOut = LEDG;
    xTaskParams[1].queue = xQueueCreate(1,sizeof(TickType_t));
    //xTaskParams[1].semMutex = xSemaphoreCreate();
    
    xTaskCreate(vTaskT0, (const char *)"Task Button", configMINIMAL_STACK_SIZE*2, &xTaskParams[0], tskIDLE_PRIORITY+1,0  );
    xTaskCreate(vTaskT1, (const char *)"Task Led",    configMINIMAL_STACK_SIZE*2, &xTaskParams[0], tskIDLE_PRIORITY+1,0  );

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
