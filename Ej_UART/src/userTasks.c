/*=============================================================================
 * Copyright (c) 2019, Agustin Curcio Berardi <acurber91@gmail.com>
 * All rights reserved.
 * License: mit (see LICENSE.txt)
 * Date: 2019/09/22
 * Version: v1.0
 *===========================================================================*/

/*=====[Inclusion of own header]=============================================*/

#include "userTasks.h"
#include "semphr.h"

extern char mensaje[];

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

void led_task( void* pvParameters )
{
	while( TRUE )
	{
		gpioToggle( LEDG );
		vTaskDelay(100/portTICK_RATE_MS);
	}
}
//checkTask recibe como parametro los datos de una UART con el driver inicializado
//se queda esperando a que llegue un paquete, lo procesa y lo envia por la UART

void uart_task( void* pvParameters )
{

	driver_t *Uart_driver;
	Uart_driver = ( driver_t * )pvParameters;

	while( TRUE )
	{
		gpioToggle( LED3 );

		xSemaphoreTake( Uart_driver->permiso  , portMAX_DELAY );

		packetTX( Uart_driver , mensaje );

		vTaskDelay(100/portTICK_RATE_MS);
	}

// habilito cb rx
// recibo algo
// libero semaforo en cb rx
// uart_task toma ese semaforo
// llama a packetTX -> habilita cb tx
// cb tx -> imprime


}

void packetTX( driver_t* Uart_driver, char *mensaje )
{

    //xQueueSend( Uart_driver->onTxQueue, &mensaje, portMAX_DELAY ); //Envio a la cola de transmision el blocke a transmitir
    taskENTER_CRITICAL();  //no permito que se modifique txcounter
    if ( Uart_driver->txCounter == 0 ) //si se esta enviando algo no llamo a la interrupcion para no interrumpir el delay
    {
        txInterruptEnable( Uart_driver );
    }
    taskEXIT_CRITICAL();
    uartSetPendingInterrupt( Uart_driver->uartValue );
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

