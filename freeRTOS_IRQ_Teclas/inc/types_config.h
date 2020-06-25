

// types_config.h
#include "FreeRTOS.h"   //Motor del OS
#include "FreeRTOSConfig.h"
#include "task.h"		//Api de control de tareas y temporizaciÃ³n
#include "semphr.h"		//Api de sincronizaciÃ³n (sem y mutex)
#include "queue.h"      //Api de colas
#include "sapi.h"
#include "board.h"

#define ANTIREBOTE_MS 20
#define UP      1
#define FALLING 2
#define DOWN	3
#define RISING  4

typedef struct { //estructura de control de datos capturados por la interrupciÃ³n
	TickType_t Tiempo_inicial;
	uint8_t Flanco;
}Button_Control;

typedef struct { //estructura de control de la mÃ¡quina de estados de cada botÃ³n
	uint8_t Tecla;
	uint8_t Estado;
	xQueueHandle Cola;
	TickType_t Tiempo_inicial;
}Buttons_SM_t;

typedef struct{
	uint8_t Tecla;
	TickType_t Tiempo_medido;
} Lectura_t;


