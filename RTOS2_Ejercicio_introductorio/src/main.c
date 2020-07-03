#include  "FreeRTOS.h"
#include  "FreeRTOSConfig.h"
#include  "task.h"
#include  "semphr.h"
#include  "queue.h"
#include  "sapi.h"
#include  "board.h"
#include  "typesConfig.h"

DEBUG_PRINT_ENABLE;

void  vTaskA(void *xTaskParams);
void  vTaskB(void *xTaskParams);
void  vTaskC(void *xTaskParams);
char* itoa(int value, char* result, int base);

xQueueHandle    q1;
SemaphoreHandle_t mutexUART; 

tButtonFSM   xTaskParams[1];
tButtonFSM   xButton[2];

int main (void){
   
   boardConfig();
   debugPrintConfigUart( UART_USB, 115200 );

   uint8_t Error_state = 0;

   xButton[0].button      = TEC1;
   xButton[0].led         = LED1;
   xButton[0].index       = 1;
   xButton[0].semaphoreButton   = xSemaphoreCreateCounting( 3 , 0 );

   xButton[1].button      = TEC2;
   xButton[1].led         = LED2;
   xButton[1].index       = 2;
   xButton[1].semaphoreButton   = xSemaphoreCreateCounting( 3 , 0 );

   if (NULL == ( q1 = xQueueCreate(1,sizeof(char[20])))){ Error_state = 1; }
   if (NULL == (mutexUART = xSemaphoreCreateMutex())){ Error_state =1; }

   BaseType_t res;

   res=
   xTaskCreate( vTaskA, "LED", configMINIMAL_STACK_SIZE*2, &xButton[0], tskIDLE_PRIORITY+1, 0);
   if(res==pdFAIL) printf("ERROR_TASK_CREATE_BUTTON");

   res=
   xTaskCreate( vTaskB, "Button", configMINIMAL_STACK_SIZE*2, &xButton[0], tskIDLE_PRIORITY+1, 0);
   if(res==pdFAIL) printf("ERROR_TASK_CREATE_BUTTON");

   res=
   xTaskCreate( vTaskB, "Button2", configMINIMAL_STACK_SIZE*2, &xButton[1], tskIDLE_PRIORITY+1, 0);
   if(res==pdFAIL) printf("ERROR_TASK_CREATE_BUTTON");

    res=
   xTaskCreate( vTaskC, "Print msg", configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+1, 0);
   if(res==pdFAIL) printf("ERROR_TASK_CREATE_BUTTON");

  vTaskStartScheduler();

  while(1);

}

void vTaskA(void *xTaskParams){

    tButtonFSM* x = (tButtonFSM*)xTaskParams;
    //x-> data = (char*)"LED ON";

    portTickType timePeriod = 500 / portTICK_RATE_MS;
    portTickType timeDiff = xTaskGetTickCount();

    char dataOut[20]="LED ON";

  while(1){

    gpioWrite(LEDB,ON);
    vTaskDelay(250 / portTICK_RATE_MS );
    if( pdFALSE == (xQueueSend(q1, &dataOut, portMAX_DELAY )))  // QueueSend(q1)
    {
        printf("Unsent data\r\n");
    }
    gpioWrite(LEDB,OFF);
    vTaskDelayUntil( &timeDiff, timePeriod );
  }
}

void vTaskB(void *xTaskParams)
{

  tButtonFSM* x = (tButtonFSM*)xTaskParams;

  fsmButtonInit( x );

  while( 1 )
  {
    fsmButtonUpdate( x );
    vTaskDelay( 1 / portTICK_RATE_MS );
  }
}


void vTaskC(void * xTaskParams){

  tButtonFSM* x = (tButtonFSM*)xTaskParams;

  char str2[20];

  while(1){

    if ( pdTRUE == xQueueReceive(q1, &str2, portMAX_DELAY)) // QueueReceive(q2)
    {
      if (pdTRUE == xSemaphoreTake( mutexUART, portMAX_DELAY))
      {
        printf("QueueReceive: [ %s ];\r\n", str2);
        xSemaphoreGive( mutexUART );    
      }   
    }
  }
}

char* itoa(int value, char* result, int base) {

  if (base < 2 || base > 36) { *result = '\0'; return result; }

  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );

  // Apply negative sign
  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  
  return result;
}

