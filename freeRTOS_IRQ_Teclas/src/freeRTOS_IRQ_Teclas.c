/*==================[inlcusiones]============================================*/

// Includes de FreeRTOS
#include "FreeRTOS.h"   //Motor del OS
#include "FreeRTOSConfig.h"
#include "task.h"		//Api de control de tareas y temporizaciÃ³n
#include "semphr.h"		//Api de sincronizaciÃ³n (sem y mutex)
#include "queue.h"      //Api de colas
#include "sapi.h"
#include "board.h"

#include "ISR.h"
#include "tasks.h"
#include "types_config.h"

DEBUG_PRINT_ENABLE;

SemaphoreHandle_t Mutex_uart; //Mutex que protege la UART de concurrencia
xQueueHandle Cola_Lecturas;
Buttons_SM_t Buttons_SM[1];

int main(void)
{
   uint8_t Error_state = 0;

   boardConfig();
   My_IRQ_Init();

   // UART for debug messages
   debugPrintConfigUart( UART_USB, 115200 );
   printf( "Antirrebote con IRQ freeRTOS y sAPI\r\n" );

   /* Creamos colas de capturas de teclas */
	int8_t i;
   	for (i = 1 ; i-- ; i >= 0) {
   	  Buttons_SM[i].Tecla = i;
   	  if (NULL == (Buttons_SM[i].Cola = xQueueCreate(5,sizeof(Button_Control)))){
		   Error_state =1;
	   }
   	}

   	/* Creamos cola de lecturas completadas */
   	if (NULL == (Cola_Lecturas = xQueueCreate(10,sizeof(Lectura_t)))){
   	   	   Error_state =1;
   	   }

   if (NULL == (Mutex_uart = xSemaphoreCreateMutex())){
   	   Error_state =1;
   }

   // Crear tareas de Teclas en freeRTOS
   xTaskCreate(
	  Tecla,                     // Funcion de la tarea a ejecutar
      (const char *)"Tec1",     // Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
	  &Buttons_SM[0],                 // Parametros de tarea
      tskIDLE_PRIORITY+1,         // Prioridad de la tarea
      0                           // Puntero a la tarea creada en el sistema
   );


   // Crear tarea LED en freeRTOS
  xTaskCreate(
   Led_task,                     // Funcion de la tarea a ejecutar
	 (const char *)"Led",     // Nombre de la tarea como String amigable para el usuario
	 configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
	 0,                          // Parametros de tarea
	 tskIDLE_PRIORITY+2,         // Prioridad de la tarea
	 0                           // Puntero a la tarea creada en el sistema
  );

   // Iniciar scheduler
   if (0 == Error_state){
  	  vTaskStartScheduler();
   } else{
	  printf("Error al iniciar el sistema !!!!!!!!!!!!!!");
   }

   while( TRUE );

   return 0;
}
