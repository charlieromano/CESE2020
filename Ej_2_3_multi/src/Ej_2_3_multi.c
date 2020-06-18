/* Copyright 2017-2018, Eric Pernia
 * All rights reserved.
 *
 * This file is part of sAPI Library.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*==================[inlcusiones]============================================*/

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
void fsmButtonInit( tLedTecla* config );
void fsmButtonUpdate( tLedTecla* config );

// Prototipo de funcion de la tarea
void tarea_led( void* taskParmPtr );
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
    debugPrintlnString( "Ejercicio 2_3_multi" );

    // Led para dar se�al de vida
    //gpioWrite( LED3 , ON );

    tecla_led_config[0].led 	= LED1;
    tecla_led_config[0].tecla 	= TEC1;
    tecla_led_config[0].tec_pulsada = xSemaphoreCreateBinary();

    tecla_led_config[1].led 	= LED2;
    tecla_led_config[1].tecla 	= TEC2;
    tecla_led_config[1].tec_pulsada = xSemaphoreCreateBinary();

    // Crear tarea en freeRTOS

    xTaskCreate(
        tarea_led,                     // Funcion de la tarea a ejecutar
        ( const char * )"tarea_led1",   // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
        &tecla_led_config[0],                          // Parametros de tarea
        tskIDLE_PRIORITY+1,         // Prioridad de la tarea
        0                           // Puntero a la tarea creada en el sistema
    );

    xTaskCreate(
		tarea_led,                     	// Funcion de la tarea a ejecutar
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
void tarea_led( void* taskParmPtr )
{
    // ---------- CONFIGURACIONES ------------------------------
	tLedTecla* config = (tLedTecla*) taskParmPtr;

	TickType_t xPeriodicity =  1000 / portTICK_RATE_MS;		// Tarea periodica cada 1000 ms
	TickType_t xLastWakeTime = xTaskGetTickCount();
    // ---------- REPETIR POR SIEMPRE --------------------------
    while( TRUE )
    {
    	TickType_t dif = 500 / portTICK_RATE_MS;

    	if ( xSemaphoreTake(  config->tec_pulsada  ,  xPeriodicity ) == pdTRUE)	//esperar tecla
    		config->led = (config->tecla == TEC1)?(LEDR):(LED2);				// llego semaforo
    	else
    		config->led = (config->tecla == TEC1)?(LED1):(LED3);				// no llego semaforo

		gpioWrite( config->led , ON );
		vTaskDelay( dif );
		gpioWrite( config->led , OFF );

		vTaskDelayUntil( &xLastWakeTime , xPeriodicity );
    }
}

/*==================[fin del archivo]========================================*/
