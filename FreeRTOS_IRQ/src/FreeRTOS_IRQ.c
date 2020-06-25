#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"
#include "chip.h"
#include "types_config.h"
#include "Tasks.h"
#define MY_ASSERT(CONDICION) my_assert_debug(CONDICION)

void initIRQ(void);

DEBUG_PRINT_ENABLE;

tConfig xTaskParams[1];
tConfigADC xTaskParamsADC[2];
tConfigSPI xTaskParamsSPI[1];
tConfigButton xTaskParamsButton[1];
tConfigUART xTaskParamsUART[1];
xQueueHandle queueIRQ;

int main (void){

    boardConfig();
    adcConfig( ADC_ENABLE );
    initIRQ();
    spiInit(SPI0); 
    max7219Init();

    debugPrintConfigUart( UART_USB, 115200 );
    debugPrintlnString( "FreeRTOS project with IRQ" );
    gpioWrite( LED3 , ON );

    xTaskParams[0].pinIn  = TEC1;
    xTaskParams[0].pinOut = LEDB;
    xTaskParams[0].queue  = xQueueCreate(1,sizeof(TickType_t));

    xTaskParamsADC[0].adcChannel = CH2;
    xTaskParamsADC[0].queue = xQueueCreate(1,sizeof(TickType_t));
    xTaskParamsADC[1].adcChannel = CH1;
    xTaskParamsADC[1].queue = xQueueCreate(1,sizeof(TickType_t));

    xTaskParamsButton[0].Cola = xQueueCreate(1,sizeof(TickType_t));
    xTaskParamsButton[0].queue2 = xQueueCreate(2,sizeof(TickType_t));
    xTaskParamsButton[0].semaphoreUART = xSemaphoreCreateMutex();
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
  
    res=
    xTaskCreate(vTaskReadADC, (const char *)"TaskReadADC()",    configMINIMAL_STACK_SIZE*4, &xTaskParamsADC[1], tskIDLE_PRIORITY+1,0  );
    if(res==pdFAIL) ERROR_TASK_CREATE;
    
    res=
    xTaskCreate(vTaskWriteSPI, (const char *)"TaskWriteSPI()", configMINIMAL_STACK_SIZE*4, &xTaskParamsADC[1], tskIDLE_PRIORITY+1,0  );
    if(res==pdFAIL) ERROR_TASK_CREATE;
*/    
    res=
    xTaskCreate(vTaskButton, (const char *)"TaskButtonIRQ()", configMINIMAL_STACK_SIZE*4, &xTaskParamsADC[1], tskIDLE_PRIORITY+1,0  );
    if(res==pdFAIL) ERROR_TASK_CREATE;

    vTaskStartScheduler();

    while(1);

}

void initIRQ(void){

    Chip_PININT_Init(LPC_GPIO_PIN_INT);

    // TEC1 FALL
    Chip_SCU_GPIOIntPinSel(0, 0, 4);    //(Canal 0 a 7, Puerto GPIO, Pin GPIO)
    Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH0);//Se configura el canal para que se active por flanco
    Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH0);//Se configura para que el flanco sea el de bajada

    // TEC1 RISE
    Chip_SCU_GPIOIntPinSel(1, 0, 4);    //(Canal 0 a 7, Puerto GPIO, Pin GPIO)
    Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH1);//Se configura el canal para que se active por flanco
    Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH1);//En este caso el flanco es de subida

    //Una vez que se han configurado los eventos para cada canal de interrupcion
    //Se activan las interrupciones para que comiencen a llamar al handler
    NVIC_SetPriority(PIN_INT0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(PIN_INT0_IRQn);
    NVIC_SetPriority(PIN_INT1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(PIN_INT1_IRQn);

}

void GPIO0_IRQHandler(void){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable


    if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH0){ //Verificamos que la interrupciÃ³n es la esperada
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH0); //Borramos el flag de interrupciÃ³n
        //codigo a ejecutar si ocurriÃ³ la interrupciÃ³n

        struct Button_Control Snapshot;
        Snapshot.Flanco = FALLING;
        Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();

        xQueueSendFromISR( xTaskParamsButton[0].Cola , &Snapshot, &xHigherPriorityTaskWoken );


    }

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
//A 2 <
//B 3
void GPIO1_IRQHandler(void){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH1){
            Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH1);
            //codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
            struct Button_Control Snapshot;
            Snapshot.Flanco = RISING;
            Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
            xQueueSendFromISR( xTaskParamsButton[0].Cola , &Snapshot, &xHigherPriorityTaskWoken );
        }
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
