

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

	spiMap_t	spi;
	uint8_t		data;
	uint32_t	clk;
	gpioMap_t	miso;
	gpioMap_t	mosi;
	gpioMap_t	cs;
	bool_t		cpol;
	bool_t		cpha;

}tConfigSPI;


typedef struct{

	uint16_t			dataOut;
	uint16_t			dataIn;

}tConfigDataProcess;


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


typedef struct { //estructura de control de datos capturados por la interrupciÃ³n
	TickType_t Tiempo_inicial;
	fsmButtonState_t Flanco;

} fsmButtonISR_t;


typedef struct { //estructura de control de la mÃ¡quina de fsmStates de cada botÃ³n
	uint8_t Tecla;
	fsmButtonState_t fsmState;
	xQueueHandle Cola;
	TickType_t Tiempo_inicial;
}Buttons_SM_t;


typedef struct{
	uint8_t Tecla;
	TickType_t Tiempo_medido;
} Lectura_t;


