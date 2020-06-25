#ifndef TYPES_CONFIG_H_
#define TYPES_CONFIG_H_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "sapi.h"
#include "semphr.h"


typedef enum{

    STATE_BUTTON_DOWN,
    STATE_BUTTON_UP,
    STATE_BUTTON_FALLING,
    STATE_BUTTON_RISING

} fsmButtonState_t;

typedef struct{

	gpioMap_t			pinIn;
	gpioMap_t			pinOut;
	
	TickType_t 			tiempo_medido;	
	fsmButtonState_t 	fsmButtonState;
	
	TickType_t 			tiempo_down;		
	TickType_t 			tiempo_up;		

	uint8_t 			contFalling;
	uint8_t 			contRising;		

	SemaphoreHandle_t	semaphore;
	QueueHandle_t		queue;

} tConfig;

typedef struct{

	adcMap_t		adcChannel;
	uint16_t		value;
	QueueHandle_t	queue;

}tConfigADC;

typedef struct{

	uint8_t			dataOut;
	uint8_t			dataIn;
	QueueHandle_t	queue;

}tConfigSPI;

typedef struct{

	SemaphoreHandle_t semaphoreUART;

}tConfigUART;

typedef struct{
	gpioMap_t		button;
	uint8_t 		fsmState;
	TickType_t		timeDiff;
	xQueueHandle	queue;

}tConfigIRQ;

typedef struct {
	uint8_t Tecla;
	fsmButtonState_t Estado;
	xQueueHandle Cola;
	xQueueHandle queue2;
	TickType_t Tiempo_inicial;
	TickType_t Tiempo_medido;
	SemaphoreHandle_t semaphoreUART;
}tConfigButton;

typedef struct {
	uint8_t Tecla;
	TickType_t Tiempo_medido;
}Lectura_t;

typedef struct { //estructura de control de datos capturados por la interrupciÃ³n
	TickType_t Tiempo_inicial;
	uint8_t Flanco;
} Button_Control;

	
#endif