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

xQueueHandle    q1;
SemaphoreHandle_t mutexUART; 

tButtonFSM   xTaskParams[1];
tButtonFSM  xButton[2];

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

   if (NULL == (q1 = xQueueCreate(10,sizeof(tButtonFSM)))){ Error_state =1; }
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

  vTaskStartScheduler();

  while(1);

}

void vTaskA(void *xTaskParams){

    tButtonFSM* x = (tButtonFSM*)xTaskParams;
    //x-> data = (char*)"LED ON";

    portTickType timePeriod = 250 / portTICK_RATE_MS;
    portTickType timeDiff = xTaskGetTickCount();

    int counter=1;
    char str[20]="LED ON";

  while(1){

    counter++;
    gpioToggle(LEDB);
    if (counter%2 == 0)
    {
      printf("%sr\n",str);
      if( pdFALSE == (xQueueSend(q1, &str, portMAX_DELAY )))  // QueueSend(q1)
      {
        printf("Unsent data\r\n");
      }   
    }
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

  char str[20];

  while(1){

    if ( pdTRUE == xQueueReceive(q1, &str, portMAX_DELAY)) // QueueReceive(q2)
    {
      if (pdTRUE == xSemaphoreTake( mutexUART, portMAX_DELAY))
      {
        printf("QueueReceive: [ %s ];\r\n", str);
        xSemaphoreGive( mutexUART );    
      }   
    }
  }
}
