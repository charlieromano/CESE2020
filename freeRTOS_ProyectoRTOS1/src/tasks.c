//tasks.c
#include "FreeRTOS.h"   //Motor del OS
#include "FreeRTOSConfig.h"
#include "task.h"		//Api de control de tareas y temporizaciÃ³n
#include "semphr.h"		//Api de sincronizaciÃ³n (sem y mutex)
#include "queue.h"      //Api de colas
#include "sapi.h"
#include "board.h"

#include "tasks.h"
#include "types_config.h"

#define ANTIREBOTE_MS 20
#define CANT_LEDS 4

extern SemaphoreHandle_t mutexUART;
extern SemaphoreHandle_t mutexSPI;
extern xQueueHandle Cola_Lecturas;
extern xQueueHandle q1;
extern xQueueHandle q2;
extern xQueueHandle q3;

void vTaskReadADC(void* xTaskParams){

    tConfigADC* x = (tConfigADC*)xTaskParams;

    portTickType timePeriod = REFRESH_TIME_ADC / portTICK_RATE_MS;
    portTickType timeDiff = xTaskGetTickCount();

    while(1){

        x-> value = adcRead( x-> adcChannel );
        if (pdTRUE == xSemaphoreTake( mutexUART, portMAX_DELAY)){
        	printf("ADC read: [ %d ];\r\n", x-> value);
        	xSemaphoreGive( mutexUART );   	
        }


        if( pdFALSE == (xQueueSend(q1, &(x->value), 2*REFRESH_TIME_ADC )))  // QueueSend(q1)
        {
        	printf("Unsent data.\r\n");
        }   
        vTaskDelayUntil( &timeDiff, timePeriod );
    }
}


void vTaskProcessing(void* xTaskParams){

	tConfigDataProcess* x = (tConfigDataProcess*)xTaskParams;

	uint16_t sample;
    uint8_t default_row = 0b11111111;
    x-> row = default_row;

	while(1){

		if( pdTRUE == (xQueueReceive(q1, &sample, portMAX_DELAY))) // QueueReceive(q1)
		{
			x-> dataIn = sample;
			if (pdTRUE == xSemaphoreTake( mutexUART, portMAX_DELAY)) // enter critical section UART
			{
				printf("Processing: [ %d ];\r\n", x-> dataIn);
				xSemaphoreGive( mutexUART );						  // exit critical section UART
			}

			if(x->counter==1){
				x-> row = 0b00011000;
			}else{x-> row = default_row;}

			x-> dataOut = (int) sample / 128 + 1;
			
			if (pdTRUE == xSemaphoreTake( mutexUART, portMAX_DELAY)) // enter critical section UART
			{
				printf("Processed: [ %d ];\r\n", x->dataOut);
				xSemaphoreGive( mutexUART );						  // exit critical section UART
			}
			
			if( pdFALSE == (xQueueSend(q2, &(x->dataOut), portMAX_DELAY ))) // QueueSend(q2)
        	{
        		printf("Unsent data.\r\n");
        	}
    	}
	}
}


void vTaskWriteSPI(void* xTaskParams){

    tConfigDataProcess* x = (tConfigDataProcess*)xTaskParams;

    uint8_t data;
    portTickType timePeriod = 100 / portTICK_RATE_MS;
    portTickType timeDiff = xTaskGetTickCount();


    while(1){

		if ( pdTRUE == xQueueReceive(q2, &data, portMAX_DELAY)) // QueueReceive(q2)
		{

			if (pdTRUE == xSemaphoreTake( mutexSPI, portMAX_DELAY)) // enter critical section SPI
			{
				spiWriteMAX7219(SPI0, data, x->row);
		  		xSemaphoreGive( mutexSPI );  // exit critical section SPI
		  	}
		}
    }
}


void vTaskButton( void* taskParmPtr )
{

	Buttons_SM_t* Config;
	
	Config = (Buttons_SM_t*) taskParmPtr;
	
	Config->fsmState = STATE_BUTTON_UP;
	
	Lectura_t Lectura;
	Lectura.Tecla = Config->Tecla;
   
	fsmButtonISR_t Control;
	TickType_t Last_Snapshot = 0;

	 if (pdTRUE == xSemaphoreTake( mutexUART, portMAX_DELAY))
	 {
	 	printf("Tarea TEC%d iniciada\r\n",Lectura.Tecla+1);
		xSemaphoreGive( mutexUART );
	  }

   while(1){

	   	if (xQueueReceive(Config->Cola, &Control, portMAX_DELAY)){
	   		switch (Config->fsmState)
	   		{
	   			case STATE_BUTTON_UP:
	   			if(Control.edge == STATE_BUTTON_FALLING)
	   			{
					if (pdFALSE == (xQueueReceive(Config->Cola, &Control, (ANTIREBOTE_MS / portTICK_RATE_MS))))   // QueueReceive()
					{
						Config->fsmState = STATE_BUTTON_DOWN;
					  	Config->initTime = Control.initTime;
					  	if (pdTRUE == xSemaphoreTake( mutexUART, portMAX_DELAY)) // enter critical section UART
					  	{
					  		printf("Se capturo una pulsacion\r\n");
					  		xSemaphoreGive( mutexUART );// exit critical section UART
					  	}
					}
				}
				break;

				case STATE_BUTTON_DOWN:
				if(Control.edge == STATE_BUTTON_RISING)
				{
					if (pdFALSE == (xQueueReceive(Config->Cola, &Control, (ANTIREBOTE_MS / portTICK_RATE_MS))))  // QueueReceive()
					{
						Config->fsmState = STATE_BUTTON_UP;

						Lectura.Tiempo_medido = xTaskGetTickCount() - Config->initTime;
						int t = Lectura.Tiempo_medido*portTICK_RATE_MS;

						if (pdTRUE == xSemaphoreTake( mutexUART, portMAX_DELAY)) // enter critical section UART
						{
							printf("Lectura completada en la tecla %d, presionada por %dms\r\n",Lectura.Tecla+1, t) ;
							xSemaphoreGive( mutexUART );// exit critical section UART
						}
					
						xQueueSend(q3, &t, portMAX_DELAY); // QueueSend()
					}
				}
				break;

				default:
				Config->fsmState = STATE_BUTTON_UP;
				break;
			}
		}
	}
}

void vTaskProcessFromISR(void* xTaskParams){

	tConfigDataProcess* x = (tConfigDataProcess*)xTaskParams;

	int value;
	int counter=0;

	while(1){

		if (pdTRUE == (xQueueReceive(q3, &value, portMAX_DELAY))) // QueueReceive(q3)
		{
			if (pdTRUE == xSemaphoreTake( mutexUART, portMAX_DELAY)) // enter critical section UART
			{
				printf("VALUE: [ %d ];\r\n", value);
				counter++;
				x-> counter = counter%2;
				xSemaphoreGive( mutexUART );						  // exit critical section UART
			}

		}
	}

}