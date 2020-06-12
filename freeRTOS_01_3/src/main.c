#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"


DEBUG_PRINT_ENABLE;

TickType_t get_diff();
void clear_diff();

void taskButton( void* taskParmPtr );
void taskLED( void* taskParmPtr );

int main(void)
{
   boardConfig();

   // UART for debug messages
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "Blinky con freeRTOS y sAPI." );

// Crear tarea en freeRTOS
   xTaskCreate(
      taskLED,                     // Funcion de la tarea a ejecutar
      (const char *)"taskLED",     // Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
      0,                          // Parametros de tarea
      tskIDLE_PRIORITY+1,         // Prioridad de la tarea
      0                           // Puntero a la tarea creada en el sistema
   );

   xTaskCreate(
      taskButton,                     // Funcion de la tarea a ejecutar
      (const char *)"taskButton",     // Nombre de la tarea como String amigable para el usuario
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
void taskButton( void* taskParmPtr ){
   
   fsmButtonInit();

   printf( "Task Button\r\n" );

   while(1) {
      fsmButtonUpdate(TEC1);
      vTaskDelay( 1/ portTICK_RATE_MS);
   }
}

void taskLED(void * taskParmPtr ){
   printf( "Task LED\r\n" );
   while(1){
      TickType_t dif = get_diff();

      if( dif == 0  )
      {
         vTaskDelay( 40 / portTICK_RATE_MS); //40 ms es el debounce type.
      }
      else
      {
         gpioWrite( LEDB , 1 );
         vTaskDelay( dif );
         gpioWrite( LEDB , 0 );

         clear_diff();
      }
    }
}
