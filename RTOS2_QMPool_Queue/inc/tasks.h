//tasks.h
#ifndef _TASKS_H_
#define _TASKS_H_

#include "task.h"
#include  "queue.h"

void _TaskCreate_ ( TaskFunction_t pvTaskName, 
					const char * const taskName, 
					unsigned short usStackDepth, 
					void *pvParams, 
					BaseType_t uxPriority, 
					TaskHandle_t *pxCreatedTask);

void  vTaskA(void *xTaskParams);
void  vTaskB(void *xTaskParams);
void  vTaskC(void *xTaskParams);
void  vTaskD(void * xTaskParams);
void  vTaskE(void * xTaskParams);
void  vTaskF(void * xTaskParams);




char* itoa(int value, char* result, int base);

#endif
