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

SemaphoreHandle_t    Mutex_uart; //Mutex que protege la UART de concurrencia
SemaphoreHandle_t    mutexSPI;

xQueueHandle         Cola_Lecturas;
xQueueHandle         q1;
xQueueHandle         q2;

Buttons_SM_t         Buttons_SM[1];
tConfig              xTaskParams[1];
tConfigADC           xTaskParamsADC[2];
tConfigSPI           xTaskParamsSPI[1];
tConfigDataProcess   xTaskParamsDataProc[1];

int main(void)
{

   uint8_t Error_state = 0;

   boardConfig();
   My_IRQ_Init();
   spiInit(SPI0); 
   max7219Init();


   // UART for debug messages
   debugPrintConfigUart( UART_USB, 115200 );
   printf( "Antirrebote con IRQ freeRTOS y sAPI\r\n" );

   xTaskParamsADC[0].adcChannel = CH1;
   xTaskParamsADC[1].adcChannel = CH2;
   xTaskParamsSPI[0].spi = SPI0;

   if(NULL == ( q1 = xQueueCreate(1,sizeof(uint16_t)))){ Error_state = 1; }
   if(NULL == ( q2 = xQueueCreate(1,sizeof(uint16_t)))){ Error_state = 1; }
   /* Creamos colas de capturas de teclas */
	int8_t i;
   	for (i = 1 ; i-- ; i >= 0) {
   	  Buttons_SM[i].Tecla = i;
   	  if (NULL == (Buttons_SM[i].Cola = xQueueCreate(5,sizeof(fsmButtonISR_t))))
        {
		   Error_state =1;
         }
   	}

   if (NULL == (Cola_Lecturas = xQueueCreate(10,sizeof(Lectura_t)))){ Error_state =1; }
   if (NULL == (Mutex_uart = xSemaphoreCreateMutex())){ Error_state =1; }
   if (NULL == (mutexSPI = xSemaphoreCreateMutex())){ Error_state =1; }

   BaseType_t res;

   res=
   xTaskCreate( vTaskButton, (const char *)"Tec1", configMINIMAL_STACK_SIZE*2, &Buttons_SM[0], tskIDLE_PRIORITY+1, 0);
   if(res==pdFAIL) printf("ERROR_TASK_CREATE_BUTTON");

   res=
   xTaskCreate(  Led_task,    (const char *)"Led",  configMINIMAL_STACK_SIZE*2, 0, tskIDLE_PRIORITY+2, 0  );
   if(res==pdFAIL) printf("ERROR_TASK_CREATE_LED");

   res=
   xTaskCreate( vTaskReadADC, (const char *)"TaskReadADC()", configMINIMAL_STACK_SIZE*4, &xTaskParamsADC[0], tskIDLE_PRIORITY+1,0  );
   if(res==pdFAIL) printf("ERROR_TASK_CREATE_ADC");

   res=
   xTaskCreate( vTaskProcessing, (const char *)"TaskProcessing()", configMINIMAL_STACK_SIZE*4, &xTaskParamsDataProc[0], tskIDLE_PRIORITY+1,0  );
   if(res==pdFAIL) printf("ERROR_TASK_CREATE_DATAPROC");

   res=
   xTaskCreate( vTaskWriteSPI, (const char *)"TaskWriteSPI()", configMINIMAL_STACK_SIZE*4, &xTaskParamsDataProc[0], tskIDLE_PRIORITY+1,0  );
   if(res==pdFAIL) printf("ERROR_TASK_CREATE_DATAPROC");

   // Iniciar scheduler
   if (0 == Error_state){
  	  vTaskStartScheduler();
   } else{
	  printf("Error al iniciar el sistema.");
   }

   while(1);

   return 0;
}
