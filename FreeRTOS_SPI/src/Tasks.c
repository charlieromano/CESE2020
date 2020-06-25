#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "sapi.h"
#include "task.h"
#include "Tasks.h"
#include "types_config.h"
#include  "MAX7219.h"



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