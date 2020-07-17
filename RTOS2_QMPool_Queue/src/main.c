#include  "FreeRTOS.h"
#include  "FreeRTOSConfig.h"
#include  "task.h"
#include  "semphr.h"
#include  "queue.h"
#include  "sapi.h"
#include  "board.h"
#include  "tasks.h"
#include  "types.h"
#include  "qmpool.h"

#define PACKET_SIZE  50
#define POOL_TOTAL_BLOCKS 5
#define POOL_SIZE POOL_TOTAL_BLOCKS*PACKET_SIZE //Tamanio total del pool

DEBUG_PRINT_ENABLE;

xQueueHandle    q1;
xQueueHandle    q2;
SemaphoreHandle_t mutexUART; 
tButtonFSM   xTaskParams[1];
tButtonFSM   xButton[2];
Error_t      errorStatus;
QMPool memoryPool;

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

   if (NULL == ( q1 = xQueueCreate(1,sizeof(char[MSG_LEN])))){ errorStatus = ERROR_QUEUE_CREATE; }
   if (NULL == ( q2 = xQueueCreate(1,sizeof(char[MSG_LEN])))){ errorStatus = ERROR_QUEUE_CREATE; } 
   if (NULL == (mutexUART = xSemaphoreCreateMutex())){ printf("ERROR_MUTEX_CREATE");}

   BaseType_t res;
   tString*   p;

   p = ( tString* ) pvPortMalloc( POOL_SIZE * sizeof( tString ) );
   //  Creo el pool de memoria que va a usarse para la transmision
   QMPool_init( &memoryPool, ( tString* )p,POOL_SIZE*sizeof( tString ),PACKET_SIZE ); //Tamanio del segmento de memoria reservado
   if(p==NULL){
      printf("Error MemPool\n");
      errorStatus = ERROR_MEMORY_POOL_INIT;
      while(1);
   }

   _TaskCreate_( vTaskA, "LED", 2, &xButton[0], 1, 0);
   _TaskCreate_( vTaskB, "Button1", 2, &xButton[0], 1, 0);
   //_TaskCreate_( vTaskB, "Button2", 2, &xButton[1],1, 0);
   _TaskCreate_( vTaskC, "Print msg", 2, 0, 1, 0);
   _TaskCreate_( vTaskD, "Memory allocation", 2, 0, 1, 0);
   _TaskCreate_( vTaskE, "Post", 2, 0, 1, 0);
   _TaskCreate_( vTaskF, "Get", 2, 0, 1, 0);



  vTaskStartScheduler();

  while(1);

}
