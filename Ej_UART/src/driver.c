/*=============================================================================
 * Copyright (c) 2019, Gonzalo Gontad <gonzalogontad@gmail.com>
 * All rights reserved.
 * License: MIT (see LICENSE.txt)
 * Date: 2019/09/29
 * Version: v1.0
 *===========================================================================*/

/*=====[Inclusion of own header]=============================================*/

#include "driver.h"

#include "uartIRQ.h"

/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

// Inicializacion de parte de transmision del driver
// Requiere que se le pase una variable del tipo driverConfig_t
// con los siguientes datos:
// 	selectedUart.uartValue = (por ej:UART_USB);
//	selectedUart.baudRate = (por ej: 115200);
bool_t driverInit( driver_t* Uart_driver )
{
	uartConfig(Uart_driver->uartValue,Uart_driver->baudRate);	// Inicializamos la UART

	Uart_driver->txCounter  = 0;	// ya transmiti
	Uart_driver->rxLen      = 0;
	Uart_driver->txLen      = 0;	// tengo que tx

	// Configuramos el cb de rx
	if (( rxInterruptEnable( Uart_driver )))
	{}
	else
	{
		return FALSE;
	}

	// NO OLVIDAR: Habilitar interrupciones
	uartInterrupt(Uart_driver -> uartValue, TRUE);

	return TRUE;

}




