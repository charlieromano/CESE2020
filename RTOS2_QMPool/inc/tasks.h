//tasks.h
#ifndef _TASKS_H_
#define _TASKS_H_

#include "task.h"
#include "FreeRTOS.h"

void _TaskCreate_ ( TaskFunction_t pvTaskName, 
					const char * const taskName, 
					unsigned short usStackDepth, 
					void *pvParams, 
					BaseType_t uxPriority, 
					TaskHandle_t *pxCreatedTask);

void _QueueCreate_ (UBaseType_t uxQueueLength, UBaseType_t uxItemSize);

void  vTaskA(void *xTaskParams);
void  vTaskB(void *xTaskParams);
void  vTaskC(void *xTaskParams);
void vTaskD(void * xTaskParams);


char* itoa(int value, char* result, int base);

#endif
