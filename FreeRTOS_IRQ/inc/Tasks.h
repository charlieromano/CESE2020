#ifndef TASKS_H_
#define TASKS_H_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "sapi.h"
#include "semphr.h"

#define  ERROR_TASK_CREATE    printf("Error xTaskCreate()")

void vTaskT0 (void* xTaskParams);
void vTaskT1 (void* xTaskParams);

void vTaskReadADC(void* xTaskParams);
void vTaskWriteSPI(void* xTaskParams);
void vTaskButtonRead(void* xTaskParams);
void vTaskWriteUART(void* xTaskParams);

void vTaskProcessing(void* xTaskParams);

#endif