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
#include "semphr.h"

// sAPI header
#include "sapi.h"
#include "FreeRTOSConfig.h"

/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/
SemaphoreHandle_t mutex;
/*==================[declaraciones de funciones externas]====================*/

// Prototipo de funcion de la tarea
void sacerdote_a( void* taskParmPtr );
void sacerdote_b( void* taskParmPtr );
void sacerdote_c( void* taskParmPtr );
void sacerdote_d( void* taskParmPtr );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
	// ---------- CONFIGURACIONES ------------------------------
	// Inicializar y configurar la plataforma
	boardConfig();

	// UART for debug messages
	debugPrintConfigUart( UART_USB, 9600 );
	debugPrintlnString( "Ejercicio printf.\n" );

	// Led para dar se�al de vida
	//gpioWrite( LED3, ON );

	// Crear tarea en freeRTOS

	xTaskCreate(
			sacerdote_a,                     // Funcion de la tarea a ejecutar
			( const char * )"myTask",   	// Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
			0,                          	// Parametros de tarea
			tskIDLE_PRIORITY+2,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
			0                          		// Puntero a la tarea creada en el sistema
	);

	xTaskCreate(
			sacerdote_b,                     // Funcion de la tarea a ejecutar
			( const char * )"myTask",   	// Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
			0,                          	// Parametros de tarea
			tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
			0                          		// Puntero a la tarea creada en el sistema
	);

	xTaskCreate(
			sacerdote_c,                     // Funcion de la tarea a ejecutar
			( const char * )"myTask",   	// Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
			0,                          	// Parametros de tarea
			tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
			0                          		// Puntero a la tarea creada en el sistema
	);

	xTaskCreate(
			sacerdote_d,                     // Funcion de la tarea a ejecutar
			( const char * )"myTask",   	// Nombre de la tarea como String amigable para el usuario
			configMINIMAL_STACK_SIZE*2, 	// Cantidad de stack de la tarea
			0,                          	// Parametros de tarea
			tskIDLE_PRIORITY+1,         	// Prioridad de la tarea -> Queremos que este un nivel encima de IDLE
			0                          		// Puntero a la tarea creada en el sistema
	);


	mutex 	=  xSemaphoreCreateMutex();

	// Iniciar scheduler
	vTaskStartScheduler(); // Enciende tick | Crea idle y pone en ready | Evalua las tareas creadas | Prioridad mas alta pasa a running

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

// Implementacion de funcion de la tarea
void sacerdote_a( void* taskParmPtr )
{
	// ---------- CONFIGURACIONES ------------------------------

	// ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE )
	{
		xSemaphoreTake( mutex , portMAX_DELAY );			//abrir seccion critica
		debugPrintlnString("El aldeano es ROJO \n");
		xSemaphoreGive( mutex );							//cerrar seccion critica

		vTaskDelay( 10 / portTICK_RATE_MS );
	}
}

void sacerdote_b( void* taskParmPtr )
{
	// ---------- CONFIGURACIONES -----------------------------

	// ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE )
	{
		xSemaphoreTake( mutex , portMAX_DELAY );			//abrir seccion critica
		debugPrintlnString("El aldeano es AZUL \n");
		xSemaphoreGive( mutex );							//cerrar seccion critica

		vTaskDelay( 10 / portTICK_RATE_MS );
	}
}

void sacerdote_c( void* taskParmPtr )
{
	// ---------- CONFIGURACIONES -----------------------------

	// ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE )
	{
		xSemaphoreTake( mutex , portMAX_DELAY );			//abrir seccion critica
		debugPrintlnString("El aldeano es GRIS \n");
		xSemaphoreGive( mutex );							//cerrar seccion critica

		vTaskDelay( 10 / portTICK_RATE_MS );
	}
}

void sacerdote_d( void* taskParmPtr )
{
	// ---------- CONFIGURACIONES -----------------------------

	// ---------- REPETIR POR SIEMPRE --------------------------
	while( TRUE )
	{
		xSemaphoreTake( mutex , portMAX_DELAY );			//abrir seccion critica
		debugPrintlnString("El aldeano es CIAN \n");
		xSemaphoreGive( mutex );							//cerrar seccion critica

		vTaskDelay( 10 / portTICK_RATE_MS );
	}
}
/*==================[fin del archivo]========================================*/
