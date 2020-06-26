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

extern SemaphoreHandle_t Mutex_uart;
extern SemaphoreHandle_t mutexSPI;
extern xQueueHandle Cola_Lecturas;
extern xQueueHandle q1;
extern xQueueHandle q2;

void vTaskReadADC(void* xTaskParams){

    tConfigADC* x = (tConfigADC*)xTaskParams;

    portTickType timePeriod = REFRESH_TIME_ADC / portTICK_RATE_MS;
    portTickType timeDiff = xTaskGetTickCount();

    while(1){

        x-> value = adcRead( x-> adcChannel );
        if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){
        	printf("ADC read: [ %d ];\r\n", x-> value);
        	xSemaphoreGive( Mutex_uart );   	
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

	while(1){

		if( pdTRUE == (xQueueReceive(q1, &sample, portMAX_DELAY))) // QueueReceive(q1)
		{
			x-> dataIn = sample;
			if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){
				printf("Processing: [ %d ];\r\n", x-> dataIn);
				xSemaphoreGive( Mutex_uart );
			}

			x-> dataOut = (int) sample / 128 + 1;
			
			if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){
				printf("Processed: [ %d ];\r\n", x->dataOut);
				xSemaphoreGive( Mutex_uart );
			}
			
			if( pdFALSE == (xQueueSend(q2, &(x->dataOut), portMAX_DELAY ))) // QueueSend(q2)
        	{
        		printf("Unsent data.\r\n");
        	}
    	}
	}
}


void vTaskWriteSPI(void* xTaskParams){

    tConfigSPI* x = (tConfigSPI*)xTaskParams;

    int data;
    uint8_t j;
    portTickType timePeriod = 100 / portTICK_RATE_MS;
    portTickType timeDiff = xTaskGetTickCount();


    while(1){

		if ( pdTRUE == xQueueReceive(q2, &data, portMAX_DELAY)) // QueueReceive(q2)
		{
			if (pdTRUE == xSemaphoreTake( mutexSPI, portMAX_DELAY)) // enter critical section
			{
				spiWriteMAX7219(x-> spi, data);
		  		xSemaphoreGive( mutexSPI );  // exit critical section
		  	}
		}
    }
}


void vTaskButton( void* taskParmPtr )
{

	Buttons_SM_t* Config; //Me preparo para recibir la direcciÃ³n de la estructura y copiarla en una varibale local
	Config = (Buttons_SM_t*) taskParmPtr;
	Config->fsmState = STATE_BUTTON_UP;
	Lectura_t Lectura;
	Lectura.Tecla = Config->Tecla;
   
	fsmButtonISR_t Control;
	TickType_t Last_Snapshot = 0;

	 if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY))
	 {
	 	printf("Tarea TEC%d iniciada\r\n",Lectura.Tecla+1);
		xSemaphoreGive( Mutex_uart );
	  }

   while(1){

	   	if (xQueueReceive(Config->Cola, &Control, portMAX_DELAY)){
	   		switch (Config->fsmState)
	   		{
	   			case STATE_BUTTON_UP:
	   			if(Control.Flanco == STATE_BUTTON_FALLING)
	   			{
					if (pdFALSE == (xQueueReceive(Config->Cola, &Control, (ANTIREBOTE_MS / portTICK_RATE_MS))))
					{
						Config->fsmState = STATE_BUTTON_DOWN;
					  	Config->Tiempo_inicial = Control.Tiempo_inicial;
					  	if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY))
					  	{
					  		printf("Se capturo una pulsacion\r\n");
					  		xSemaphoreGive( Mutex_uart );
					  	}
					}
				}
				break;

				case STATE_BUTTON_DOWN:
				if(Control.Flanco == STATE_BUTTON_RISING)
				{
					if (pdFALSE == (xQueueReceive(Config->Cola, &Control, (ANTIREBOTE_MS / portTICK_RATE_MS))))
					{
						Config->fsmState = STATE_BUTTON_UP;
						Lectura.Tiempo_medido = xTaskGetTickCount() - Config->Tiempo_inicial;
						if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY))
						{
							printf("Lectura completada en la tecla %d, presionada por %dms\r\n",Lectura.Tecla+1,Lectura.Tiempo_medido*portTICK_RATE_MS);
							xSemaphoreGive( Mutex_uart );
						}
					
						xQueueSend(Cola_Lecturas, &Lectura, portMAX_DELAY);
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



// Implementacion de funcion de la tarea Led
void Led_task( void* taskParmPtr ){

	Lectura_t Lectura;

	gpioMap_t Led_Map[CANT_LEDS] = {LEDR,LED1,LED2,LED3};

	while (TRUE){
		//Espero evento de Lectura completada
		if (xQueueReceive(Cola_Lecturas, &Lectura, portMAX_DELAY)){

			if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){

				  printf("Se encendera el LED %d, por %dms\r\n",Lectura.Tecla+1,Lectura.Tiempo_medido*portTICK_RATE_MS);

				  xSemaphoreGive( Mutex_uart );

			}

			gpioWrite(Led_Map[Lectura.Tecla],ON);

			//Espero tiempo de encendido
			vTaskDelay( Lectura.Tiempo_medido );

			gpioWrite(Led_Map[Lectura.Tecla],OFF);
		}
	}
}
