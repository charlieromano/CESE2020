#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "sapi.h"


DEBUG_PRINT_ENABLE;

TickType_t get_diff();
void clear_diff();

void taskButton( void* taskParmPtr );
void taskLED   ( void* taskParmPtr );

int main(void)
{
   boardConfig();

   xTaskCreate(taskLED, "task LED", configMINIMAL_STACK_SIZE*2, 0,tskIDLE_PRIORITY+1,0);
   xTaskCreate(taskButton, "task Button", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+1,0);

   vTaskStartScheduler();

   while(1);
   
   return 0;
}

// Implementacion de funcion de la tarea
void taskButton( void* taskParmPtr ){
   
   fsmButtonInit();

   while(1) {
      fsmButtonUpdate(TEC1);
      vTaskDelay( 1/ portTICK_RATE_MS);
   }
}

void taskLED(void * taskParmPtr ){

   portTickType   xPeriod = 2000/portTICK_RATE_MS;
   portTickType   xLastWakeTime = xTaskGetTickCount();

   while(1){

      TickType_t T0 = get_diff();

      if( T0 == 0  ){
         vTaskDelay( 40 / portTICK_RATE_MS); //40 ms es el debounce type.
      }
      else{

         gpioWrite(LEDB , ON);
         vTaskDelay(T0);
         gpioWrite(LEDB , OFF);
         vTaskDelayUntil(&xLastWakeTime, xPeriod);
         //clear_diff();
      }
    }
}
