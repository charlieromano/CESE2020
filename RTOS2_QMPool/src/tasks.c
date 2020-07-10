//tasks.c
#include  "FreeRTOS.h"
#include  "FreeRTOSConfig.h"
#include  "tasks.h"
#include  "sapi.h"
#include  "semphr.h"
#include  "queue.h"
#include  "types.h"
#include  "qmpool.h"



extern xQueueHandle    q1;
extern SemaphoreHandle_t mutexUART; 
extern Error_t      errorStatus;
extern QMPool memoryPool;

void _TaskCreate_ ( TaskFunction_t pvTaskName, 
					const char * const taskName, 
					unsigned short usStackDepth, 
					void *pvParams, 
					BaseType_t uxPriority, 
					TaskHandle_t *pxCreatedTask)
{
   BaseType_t res;
   res=
   xTaskCreate( pvTaskName, taskName, usStackDepth*configMINIMAL_STACK_SIZE, pvParams, tskIDLE_PRIORITY+uxPriority,0);
   if(res==pdFAIL){
   	errorStatus = ERROR_TASK_CREATE;
   	while(1);
   }

}

void _QueueCreate_(UBaseType_t uxQueueLength, UBaseType_t uxItemSize){

	q1 = xQueueCreate(1,sizeof(char[MSG_LEN]));

	if (NULL == q1)
	{ 
		errorStatus = ERROR_QUEUE_CREATE;
	}
}

void vTaskA(void *xTaskParams){

    tButtonFSM* x = (tButtonFSM*)xTaskParams;
    //x-> data = (char*)"LED ON";

    portTickType timePeriod = 500 / portTICK_RATE_MS;
    portTickType timeDiff = xTaskGetTickCount();

    char dataOut[MSG_LEN]="LED ON";

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

  char str2[MSG_LEN];

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


void vTaskD(void * xTaskParams){

  tString* pMsg;
  tString  msg="hello world!";

  portTickType timePeriod = 5000 / portTICK_RATE_MS;
  portTickType timeDiff = xTaskGetTickCount();

  while(1){

    pMsg = ( tString* ) QMPool_get( &memoryPool, 0 ); //pido un bloque del pool
    if(pMsg==NULL){
      printf("Error memory get\n");
      errorStatus = ERROR_MEMORY_GET;
    }

    *pMsg = msg;

    printf("%s\n", *pMsg);

    QMPool_put( &memoryPool, pMsg );

    vTaskDelayUntil( &timeDiff, timePeriod );
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

