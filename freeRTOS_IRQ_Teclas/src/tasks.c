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
extern xQueueHandle Cola_Lecturas;


// Implementacion de funcion de la tarea genÃ©rica Tecla
void Tecla( void* taskParmPtr )
{
   // ---------- DefiniciÃ³n de variables locales ------------------------------
	Buttons_SM_t* Config; //Me preparo para recibir la direcciÃ³n de la estructura y copiarla en una varibale local
	Config = (Buttons_SM_t*) taskParmPtr;
	Config->Estado = UP;
	Lectura_t Lectura;
	Lectura.Tecla = Config->Tecla;
   
	Button_Control Control;
	TickType_t Last_Snapshot = 0;

	 if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){

		 printf("Tarea TEC%d iniciada\r\n",Lectura.Tecla+1);

		  xSemaphoreGive( Mutex_uart );

	  }



   // ---------- REPETIR POR SIEMPRE --------------------------
   while(TRUE) {

	   if (xQueueReceive(Config->Cola, &Control, portMAX_DELAY)){

			   switch (Config->Estado){

					 case UP:
						  if(Control.Flanco == FALLING){ //AcÃ¡ adentro estÃ¡ el pseudo estado Falling

							  if (pdFALSE == (xQueueReceive(Config->Cola, &Control, (ANTIREBOTE_MS / portTICK_RATE_MS)))){
								  Config->Estado = DOWN;

								  //AcÃ¡ se mete cÃ³digo para ejecutar en flanco  de bajada
								  Config->Tiempo_inicial = Control.Tiempo_inicial;

								  if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){

									  printf("Se capturo una pulsacion\r\n");

									  xSemaphoreGive( Mutex_uart );

								  }

							  }
						  }
					 break;

					 case DOWN:
						 if(Control.Flanco == RISING){ //AcÃ¡ adentro estÃ¡ el pseudo estado Rising

							  if (pdFALSE == (xQueueReceive(Config->Cola, &Control, (ANTIREBOTE_MS / portTICK_RATE_MS)))){
								  Config->Estado = UP;

								  //AcÃ¡ se mete cÃ³digo para ejecutar en flanco  de subida
								  Lectura.Tiempo_medido = xTaskGetTickCount() - Config->Tiempo_inicial;

								  if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){

									  printf("Lectura completada en la tecla %d, presionada por %dms\r\n",Lectura.Tecla+1,Lectura.Tiempo_medido*portTICK_RATE_MS);

									  xSemaphoreGive( Mutex_uart );

								  }


								  xQueueSend(Cola_Lecturas, &Lectura, portMAX_DELAY);
							  }
						  }
					 break;

					 default:
						 Config->Estado = UP;
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
