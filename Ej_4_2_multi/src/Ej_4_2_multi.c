/*==================[inclusiones]============================================*/

// Includes de FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"

#include "FreeRTOSConfig.h"
#include "tipos.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/
tLedTecla tecla_led_config[CANT];


/*==================[definiciones de datos externos]=========================*/
DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/
TickType_t get_diff();
void clear_diff();

// Prototipo de funcion de la tarea
void tarea_led_a( void* taskParmPtr );
void tarea_led_b( void* taskParmPtr );
void tarea_tecla( void* taskParmPtr );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
    // ---------- CONFIGURACIONES ------------------------------
    // Inicializar y configurar la plataforma
    boardConfig();

    // UART for debug messages
    debugPrintConfigUart( UART_USB, 115200 );
    debugPrintlnString( "Ejercicio 4.2" );

    // Led para dar se�al de vida
    //gpioWrite( LED3 , ON );

    tecla_led_config[0].led_a 	= LEDB;
    tecla_led_config[0].led_b 	= LED1;
    tecla_led_config[0].tecla 	= TEC1;
    tecla_led_config[0].queue_tec_pulsada 	= xQueueCreate(1,sizeof(TickType_t));
    tecla_led_config[0].mutex 				=  xSemaphoreCreateMutex();

    tecla_led_config[1].led_a 	= LED2;
    tecla_led_config[1].led_b 	= LED3;
    tecla_led_config[1].tecla 	= TEC2;
    tecla_led_config[1].queue_tec_pulsada 	= xQueueCreate(1,sizeof(TickType_t));
    tecla_led_config[1].mutex 				=  xSemaphoreCreateMutex();

    // Crear tarea en freeRTOS

    xTaskCreate(
        tarea_led_a,                     // Funcion de la tarea a ejecutar
        ( const char * )"tarea_led1",   // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
        &tecla_led_config[0],                          // Parametros de tarea
        tskIDLE_PRIORITY+1,         // Prioridad de la tarea
        0                           // Puntero a la tarea creada en el sistema
    );

    xTaskCreate(
		tarea_led_a,                     	// Funcion de la tarea a ejecutar
		( const char * )"tarea_led2",   // Nombre de la tarea como String amigable para el usuario
		configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
		&tecla_led_config[1],        	// Parametros de tarea
		tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
		0                           	// Puntero a la tarea creada en el sistema
	);

    xTaskCreate(
		tarea_led_b,                     // Funcion de la tarea a ejecutar
		( const char * )"tarea_led1",   // Nombre de la tarea como String amigable para el usuario
		configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
		&tecla_led_config[0],                          // Parametros de tarea
		tskIDLE_PRIORITY+1,         // Prioridad de la tarea
		0                           // Puntero a la tarea creada en el sistema
	);

	xTaskCreate(
		tarea_led_b,                     	// Funcion de la tarea a ejecutar
		( const char * )"tarea_led2",   // Nombre de la tarea como String amigable para el usuario
		configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
		&tecla_led_config[1],        	// Parametros de tarea
		tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
		0                           	// Puntero a la tarea creada en el sistema
	);

    xTaskCreate(
    	tarea_tecla,                     // Funcion de la tarea a ejecutar
        ( const char * )"tarea_tecla1",  // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
		&tecla_led_config[0],           // Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
        0                           	// Puntero a la tarea creada en el sistema
    );

    xTaskCreate(
    	tarea_tecla,                     // Funcion de la tarea a ejecutar
        ( const char * )"tarea_tecla2",  // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, 	 // Cantidad de stack de la tarea
		&tecla_led_config[1],            // Parametros de tarea
        tskIDLE_PRIORITY+1,         	// Prioridad de la tarea
        0                           	// Puntero a la tarea creada en el sistema
    );

    	// Iniciar scheduler
    vTaskStartScheduler();

    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
        // Si cae en este while 1 significa que no pudo iniciar el scheduler
    }

    // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
    // directamenteno sobre un microcontroladore y no es llamado por ningun
    // Sistema Operativo, como en el caso de un programa para PC.
    return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

void tarea_tecla( void* taskParmPtr )
{
	tLedTecla* config = (tLedTecla*) taskParmPtr;

	fsmButtonInit( config );

	while( 1 )
	{
		fsmButtonUpdate( config );
	 	vTaskDelay( 1 / portTICK_RATE_MS );
	}
}

// Implementacion de funcion de la tarea
void tarea_led_a( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
	tLedTecla* config = (tLedTecla*) taskParmPtr;
	TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
	TickType_t xLastWakeTime = xTaskGetTickCount();
	TickType_t diferencia  = 0;
    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
    	xSemaphoreTake( config->mutex , portMAX_DELAY );			//abrir seccion critica
    	xLastWakeTime = xTaskGetTickCount();

    	gpioWrite( config->led_a , ON );
		vTaskDelay( xPeriodicity / 2 );
		gpioWrite( config->led_a , OFF );

		xSemaphoreGive( config->mutex );							//cerrar seccion critica

		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
    }
}

void tarea_led_b( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
	tLedTecla* config = (tLedTecla*) taskParmPtr;
	TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
	TickType_t xLastWakeTime = xTaskGetTickCount();
	TickType_t diferencia  = 0;
    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
    	xQueueReceive( config->queue_tec_pulsada , &diferencia,  portMAX_DELAY );			// Esperamos tecla

    	xSemaphoreTake( config->mutex , portMAX_DELAY );			//abrir seccion critica
    	gpioWrite( config->led_b , ON );
		vTaskDelay( diferencia );
		gpioWrite( config->led_b , OFF );
		xSemaphoreGive( config->mutex );							//cerrar seccion critica
    }
}

/*==================[fin del archivo]========================================*/
