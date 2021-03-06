//tasks.h

// Includes de FreeRTOS
#include "FreeRTOS.h"   //Motor del OS
#include "FreeRTOSConfig.h"
#include "task.h"		//Api de control de tareas y temporizaciÃ³n
#include "semphr.h"		//Api de sincronizaciÃ³n (sem y mutex)
#include "queue.h"      //Api de colas
#include "sapi.h"
#include "board.h"


#define ANTIREBOTE_MS 20

void vTaskReadADC(void* xTaskParams);
void vTaskWriteSPI(void* xTaskParams);
void vTaskButtonRead(void* xTaskParams);
void vTaskWriteUART(void* xTaskParams);
void vTaskProcessing(void* xTaskParams);
void vTaskProcessFromISR(void* xTaskParams);

void vTaskButton( void* taskParmPtr );
void Led_task( void* taskParmPtr );


