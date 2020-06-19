#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"
#include "chip.h"
#include "types_config.h"
#include "Tasks.h"


DEBUG_PRINT_ENABLE;

tConfig xTaskParams[1];
tConfigADC xTaskParamsADC[2];
tConfigSPI xTaskParamsSPI[1];

int main (void){

    boardConfig();
    adcConfig( ADC_ENABLE );
    spiInit(SPI0); 

    debugPrintConfigUart( UART_USB, 115200 );
    debugPrintlnString( "FreeRTOS project" );
    gpioWrite( LED3 , ON );

    max7219Init();



    xTaskParams[0].pinIn = TEC1;
    xTaskParams[0].pinOut   = LEDB;
    xTaskParams[0].queue = xQueueCreate(1,sizeof(TickType_t));

    xTaskParamsADC[0].adcChannel = CH1;
    xTaskParamsADC[0].queue = xQueueCreate(1,sizeof(TickType_t));
    xTaskParamsADC[1].adcChannel = CH2;
    xTaskParamsADC[1].queue = xQueueCreate(1,sizeof(TickType_t));

    //xTaskParamsSPI[0].queue = xQueueCreate(1,sizeof(TickType_t));

    BaseType_t res;

/*
    res=
    xTaskCreate(vTaskT0, (const char *)"Task Button", configMINIMAL_STACK_SIZE*2, &xTaskParams[0], tskIDLE_PRIORITY+1,0  );
    if(res==pdFAIL) ERROR_TASK_CREATE;
    
    res=
    xTaskCreate(vTaskT1, (const char *)"Task Led",    configMINIMAL_STACK_SIZE*2, &xTaskParams[0], tskIDLE_PRIORITY+1,0  );
    if(res==pdFAIL) ERROR_TASK_CREATE;
    
    res=
    xTaskCreate(vTaskReadADC, (const char *)"Task readADC()",    configMINIMAL_STACK_SIZE*4, &xTaskParamsADC[0], tskIDLE_PRIORITY+1,0  );
    if(res==pdFAIL) ERROR_TASK_CREATE;

    res=
    xTaskCreate(vTaskProcessing, (const char *)"Task readADC()", configMINIMAL_STACK_SIZE*4, &xTaskParamsADC[0], tskIDLE_PRIORITY+1,0  );
    if(res==pdFAIL) ERROR_TASK_CREATE;
  */
    res=
    xTaskCreate(vTaskReadADC, (const char *)"Task readADC()",    configMINIMAL_STACK_SIZE*4, &xTaskParamsADC[1], tskIDLE_PRIORITY+1,0  );
    if(res==pdFAIL) ERROR_TASK_CREATE;
    
    res=
    xTaskCreate(vTaskWriteSPI, (const char *)"Task readADC()", configMINIMAL_STACK_SIZE*4, &xTaskParamsADC[1], tskIDLE_PRIORITY+1,0  );
    if(res==pdFAIL) ERROR_TASK_CREATE;
    

    vTaskStartScheduler();

    while(1);

}

