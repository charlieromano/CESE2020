#ifndef TYPES_CONFIG_H_
#define TYPES_CONFIG_H_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "sapi.h"
#include "semphr.h"


typedef enum{

    STATE_BUTTON_UP,
    STATE_BUTTON_DOWN,
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


}tConfigUART;
	
#endif