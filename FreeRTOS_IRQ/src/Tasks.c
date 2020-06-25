#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "sapi.h"
#include "task.h"
#include "Tasks.h"
#include "types_config.h"
#include  "MAX7219.h"



DEBUG_PRINT_ENABLE;

void vTaskButton(void* xTaskParams){

    tConfigButton* x = (tConfigButton*)xTaskParams;
    x = (tConfigButton*) xTaskParams;

    x->Estado = STATE_BUTTON_UP;
    Lectura_t Lectura;
    Lectura.Tecla = x->Tecla;
    Button_Control Control;
    TickType_t Last_Snapshot = 0;

    if (pdTRUE == xSemaphoreTake( x->semaphoreUART, portMAX_DELAY)){
         printf("Tarea TEC%d iniciada\r\n",Lectura.Tecla+1);
         xSemaphoreGive( x->semaphoreUART );
    }

   while(1) {

       if (xQueueReceive(x->Cola, &Control, portMAX_DELAY)){
           switch (x->Estado){

                case STATE_BUTTON_UP:
                    if(Control.Flanco == STATE_BUTTON_FALLING){ //AcÃ¡ adentro estÃ¡ el pseudo estado Falling
                        if (pdFALSE == (xQueueReceive(x->Cola, &Control, (40 / portTICK_RATE_MS)))){
                            x->Estado = STATE_BUTTON_DOWN;
                            //AcÃ¡ se mete cÃ³digo para ejecutar en flanco  de bajada
                            x->Tiempo_inicial = Control.Tiempo_inicial;
                            if (pdTRUE == xSemaphoreTake( x->semaphoreUART, portMAX_DELAY)){
                                printf("Se capturo una pulsacion\r\n");
                                xSemaphoreGive( x->semaphoreUART );
                                }
                        }
                    }
                 break;
                 
                 case STATE_BUTTON_DOWN:
                     if(Control.Flanco == STATE_BUTTON_RISING){ //AcÃ¡ adentro estÃ¡ el pseudo estado Rising
                        if (pdFALSE == (xQueueReceive(x->Cola, &Control, (40 / portTICK_RATE_MS)))){
                            x->Estado = STATE_BUTTON_UP;
                            //AcÃ¡ se mete cÃ³digo para ejecutar en flanco  de subida
                            Lectura.Tiempo_medido = xTaskGetTickCount() - x->Tiempo_inicial;
                            if (pdTRUE == xSemaphoreTake( x->semaphoreUART, portMAX_DELAY)){
                                printf("Lectura completada en la tecla %d, presionada por %dms\r\n",Lectura.Tecla+1,Lectura.Tiempo_medido*portTICK_RATE_MS);
                                xSemaphoreGive( x->semaphoreUART );
                            }
                            xQueueSend(x->queue2, &Lectura, portMAX_DELAY);
                        }
                    }
                 break;
                 default: x->Estado = STATE_BUTTON_UP;
                 break;
            }
        }
    }
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


void vTaskReadADC(void* xTaskParams){
    
    tConfigADC* x = (tConfigADC*)xTaskParams;

    while(1){
        x-> value = adcRead( x-> adcChannel );
       //if( 
        xQueueSend(x->queue, &(x->value), portMAX_DELAY );
        //    != pdPASS)
        vTaskDelay(10/ portTICK_RATE_MS);
    }
}

void vTaskWriteSPI(void* xTaskParams){

    tConfigADC* x = (tConfigADC*)xTaskParams;

    uint16_t sample;
    uint8_t j;
    portTickType timePeriod = 10 / portTICK_RATE_MS;
    portTickType timeDiff = xTaskGetTickCount();


    while(1){
        xQueueReceive(x->queue, &sample, portMAX_DELAY);
        maxAll(0x00,0x00);
        j = (int)sample/128 + 1; 
        maxAll(j,0b00011000);
        vTaskDelay(100 / portTICK_RATE_MS);
        maxAll(0x00,0x00);
        maxClear();
    }
}


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


void maxAll(uint8_t reg_addr, uint8_t data){
   
   gpioMap_t cs = GPIO0;

   gpioWrite(cs, OFF);
   spiWrite(SPI0, &reg_addr, 1);
   gpioWrite(cs, OFF);
   gpioWrite(cs, ON);

   gpioWrite(cs, OFF);
   spiWrite(SPI0, &data, 1);
   gpioWrite(cs, OFF);
   gpioWrite(cs, ON);
   
}

void max7219Init( void ){

    maxAll(max7219_reg_scanLimit, 0x07);
    maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    maxAll(max7219_reg_displayTest, 0x01); // no display test
    maxClear();
    maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set

}

void maxClear( void ){

    for (uint16_t e=1; e<=8; e++) {    // empty registers, turn all LEDs off 
        maxAll(e,0);
    }
}