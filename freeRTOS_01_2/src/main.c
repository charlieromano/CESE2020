#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"


DEBUG_PRINT_ENABLE;

void myTask( void* taskParmPtr );
void myTask_1_2( void* taskParmPtr );

int main(void)
{
   boardConfig();

   // UART for debug messages
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "Blinky con freeRTOS y sAPI." );

// Crear tarea en freeRTOS
   xTaskCreate(
      myTask,                     // Funcion de la tarea a ejecutar
      (const char *)"myTask",     // Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
      0,                          // Parametros de tarea
      tskIDLE_PRIORITY+1,         // Prioridad de la tarea
      0                           // Puntero a la tarea creada en el sistema
   );

   xTaskCreate(
      myTask_1_2,                     // Funcion de la tarea a ejecutar
      (const char *)"myTask_1_2",     // Nombre de la tarea como String amigable para el usuario
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

   gpioWrite( LEDG, ON );
   vTaskDelay( 500 / portTICK_RATE_MS );
   gpioWrite( LEDG, OFF ); 

   // Tarea periodica cada 500 ms
   portTickType xPeriodicity =  1000 / portTICK_RATE_MS;
   portTickType xLastWakeTime = xTaskGetTickCount();
   
   while(1) {
      gpioToggle( LEDR );
      printf( "Toggle!\r\n" );
      vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
   }
}

void myTask_1_2(void * taskParmPtr ){
   printf( "Ejercicio RTOS 1.2\r\n" );
   
   uint8_t i=0;
      
   portTickType xPeriodicity = 1000 / portTICK_RATE_MS;
   portTickType xLastWakeTime = xTaskGetTickCount();

   while(1){
      i++;
      printf("Ciclo: %d \r\n", i);
      gpioWrite(LED3, ON);
      vTaskDelay( i*100 / portTICK_RATE_MS );
      gpioWrite(LED3, OFF);
      vTaskDelayUntil( &xLastWakeTime,xPeriodicity);
      if(i>=10){i=0;}
   }

}