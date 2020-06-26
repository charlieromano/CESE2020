

// types_config.h
#include "FreeRTOS.h"   //Motor del OS
#include "FreeRTOSConfig.h"
#include "task.h"		//Api de control de tareas y temporizaciÃ³n
#include "semphr.h"		//Api de sincronizaciÃ³n (sem y mutex)
#include "queue.h"      //Api de colas
#include "sapi.h"
#include "board.h"

#define ANTIREBOTE_MS 20
#define REFRESH_TIME_ADC 50


typedef enum{

    STATE_BUTTON_UP,
    STATE_BUTTON_DOWN,
    STATE_BUTTON_FALLING,
    STATE_BUTTON_RISING

} fsmButtonState_t;


typedef struct{

	adcMap_t		adcChannel;
	uint16_t		value;

}tConfigADC;


typedef struct{

	spiMap_t		spi;
	uint8_t			data;
	uint32_t		clk;
	gpioMap_t		miso;
	gpioMap_t		mosi;
	gpioMap_t		cs;
	bool_t			cpol;
	bool_t			cpha;

}tConfigSPI;


typedef struct{

	uint16_t		dataOut;
	uint16_t		dataIn;
	uint8_t			counter;
	uint8_t			row;
	uint8_t			col;

}tConfigDataProcess;


typedef struct { 

	TickType_t 			initTime;
	fsmButtonState_t 	edge;

} fsmButtonISR_t;


typedef struct { 

	uint8_t 			Tecla;
	TickType_t 			initTime;
	fsmButtonState_t 	fsmState;
	xQueueHandle 		Cola;

}Buttons_SM_t;


typedef struct{

	uint8_t 		Tecla;
	TickType_t 		Tiempo_medido;

} Lectura_t;


