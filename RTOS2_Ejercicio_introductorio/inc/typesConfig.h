// typesConfig.h

#include 	"FreeRTOS.h"   //Motor del OS
#include 	"FreeRTOSConfig.h"
#include 	"task.h"		//Api de control de tareas y temporizaciÃ³n
#include 	"semphr.h"		//Api de sincronizaciÃ³n (sem y mutex)
#include 	"queue.h"      //Api de colas
#include	"sapi.h"
#include 	"board.h"

#define 	MSG_LEN	20
#define 	MSG1	"LED ON"
#define 	MSG2	"TECx Tyyyy"
#define 	ANTIREBOTE_MS 20
#define 	REFRESH_TIME_ADC 50


typedef enum{

    STATE_BUTTON_UP,
    STATE_BUTTON_DOWN,
    STATE_BUTTON_FALLING,
    STATE_BUTTON_RISING

} fsmButtonState_t;


typedef struct 
{
	uint8_t				index;
	gpioMap_t			button;
	gpioMap_t			led;
	TickType_t 			tiempo_medido;
	fsmButtonState_t	fsmButtonState;
	TickType_t 			tiempo_down;
	TickType_t 			tiempo_up;

	uint8_t 			contFalling;
	uint8_t 			contRising;

	char *				data;

	SemaphoreHandle_t 	semaphoreButton;


} tButtonFSM;


typedef struct 
{
	char data[MSG_LEN];

} tConfig;



