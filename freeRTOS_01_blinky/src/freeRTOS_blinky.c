#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"


DEBUG_PRINT_ENABLE;

void myTask( void* taskParmPtr );

int main(void)
{
   boardConfig();

   // UART for debug messages
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "Blinky con freeRTOS y sAPI." );

   // Led para dar se�al de vida
   //gpioWrite( LED3, ON );

// Crear tarea en freeRTOS
   xTaskCreate(
      myTask,                     // Funcion de la tarea a ejecutar
      (const char *)"myTask",     // Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
      0,                          // Parametros de tarea
      tskIDLE_PRIORITY+1,         // Prioridad de la tarea
      0                           // Puntero a la tarea creada en el sistema
   );

   // Iniciar scheduler
   vTaskStartScheduler();

   // ---------- REPETIR POR SIEMPRE --------------------------
   while(1) {
      // Si cae en este while 1 significa que no pudo iniciar el scheduler
   }

   return 0;
}

// Implementacion de funcion de la tarea
void myTask( void* taskParmPtr ){
   
   printf( "Ejercicio RTOS 1.1\r\n" );

    gpioWrite( LED1, ON );
   // Envia la tarea al estado bloqueado durante 1 s (delay)
   vTaskDelay( 1000 / portTICK_RATE_MS );
   gpioWrite( LED1, OFF ); 

   // Tarea periodica cada 500 ms
   portTickType xPeriodicity =  500 / portTICK_RATE_MS;
   portTickType xLastWakeTime = xTaskGetTickCount();
   
   // ---------- REPETIR POR SIEMPRE --------------------------
   while(TRUE) {
      gpioToggle( LEDB );
      printf( "Blink!\r\n" );
      // Envia la tarea al estado bloqueado durante xPeriodicity (delay periodico)
      vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
   }
}
