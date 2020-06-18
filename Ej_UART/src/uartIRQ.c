#include "uartIRQ.h"
#include "semphr.h"

void onTxCallback( void* );
void onRxCallback( void* );

extern char mensaje[];

//rxInterruptEnable(driverConfig_t* Uart_driver) Habilita la interrupcion de la UART para recepcion
//indicada por selectedUart
bool_t rxInterruptEnable( driver_t* Uart_driver )
{
	switch( Uart_driver-> uartValue)
	{
		case UART_GPIO:
		case UART_USB:
		case UART_232:
		{
			uartCallbackSet( Uart_driver->uartValue, UART_RECEIVE, onRxCallback, (void*)Uart_driver );
			break;
		}
		default:
		{
			return (FALSE); // No se eligio una UART correcta
		}
	}

	return TRUE;
}

//txInterruptEnable(driverConfig_t* Uart_driver) Habilita la interrupcion de la UART para transmision
//indicada por selectedUart
 bool_t txInterruptEnable( driver_t* Uart_driver )
 {
	 switch( Uart_driver-> uartValue)
	{
		case UART_GPIO:
		case UART_USB:
		case UART_232:
		{
			uartCallbackSet( Uart_driver->uartValue, UART_TRANSMITER_FREE, onTxCallback, (void*)Uart_driver );
			break;
		}
		default:
		{
			return (FALSE); // No se eligio una UART correcta
		}
	}

	return TRUE;

 }
/*=====[Implementations of interrupt functions]==============================*/

// Callback para la recepcion
 void onRxCallback( void *param )
 {
	 BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	 char c;
	 // Elegir los datos que necesita el cb

	 driver_t* Uart_driver = param;

	 //abrir seccion critica
	 xSemaphoreTakeFromISR( Uart_driver->mutex 	  , &xHigherPriorityTaskWoken );

	 // Si llegan mas bytes de los que puedo guardar, no los leo.
	 if ( Uart_driver -> rxLen < LARGO )
	 {
		 // Leemos el byte de la UART seleccionado
		 c = uartRxRead( Uart_driver->uartValue );
		 if(c == '>')		//"<523452345234723523-X>"
		 {
			 mensaje[Uart_driver -> rxLen] = '\0';
			 xSemaphoreGiveFromISR( Uart_driver->permiso , &xHigherPriorityTaskWoken);
		 }
		 else
			 mensaje[Uart_driver -> rxLen] = c;
	 }
	 else
	 {
		 Uart_driver -> rxLen = LARGO + 1; // revisar ese +1
	 }

	 // Incrementar el indice
	 Uart_driver->rxLen ++;

	 xSemaphoreGiveFromISR( Uart_driver->mutex , &xHigherPriorityTaskWoken);
	 //cerrar seccion critica

	 portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
 }
//	uartSendDataCRC transmite los bloques que llegan por una cola y los envia por la UART seleccionada
//	Todas las callback de interrupcion de UART llaman a esta función para hacer la transmisión.
 void onTxCallback( void*param )
 {
	 driver_t* Uart_driver;
	 UBaseType_t uxSavedInterrumptStatus;
	 BaseType_t xTaskWokenByReceive = pdFALSE;

	 Uart_driver = ( driver_t* ) param;

	 // Armo una seccion critica para proteger tx_counter
	 uxSavedInterrumptStatus = taskENTER_CRITICAL_FROM_ISR();

	 // Si el contador es 0 -> puedo tomar un paquete de la cola
	 if ( Uart_driver -> txCounter == 0)
	 {
		 Uart_driver -> txLen = Uart_driver -> rxLen-1; // "Hola>"

		 //Uart_driver -> txCounter++;
	 }

	 if ( Uart_driver-> txCounter < Uart_driver->txLen)
	 { 	// Transmision
		uartTxWrite(Uart_driver->uartValue, mensaje[Uart_driver->txCounter]);
		Uart_driver -> txCounter ++;
	 }

	 if ( Uart_driver-> txCounter == Uart_driver->txLen)
	 {
		 uartTxWrite(Uart_driver->uartValue, '\r');
		 uartTxWrite(Uart_driver->uartValue, '\n');
		 uartCallbackClr(Uart_driver->uartValue, UART_TRANSMITER_FREE); // Deshabilito la interrupcion de Tx
		 //xTimerStartFromISR(Uart_driver->onTxTimeOut, &xTaskWokenByReceive); // Inicio el timer de timeout Tx
		 Uart_driver -> rxLen = 0;
		 Uart_driver-> txCounter = 0;
	 }

	 taskEXIT_CRITICAL_FROM_ISR( uxSavedInterrumptStatus );

	 // Evaluamos el pedido de cambio de contexto necesario
	 if ( xTaskWokenByReceive != pdFALSE )
	 {
		 taskYIELD();
	 }

 }

// rx cb (dato) -> encolar(dato) -> procesar(dato)-> encolar(dato_modificado) -> tx cb (dato_modificado)


// Interrupcion	-- tx_task




// tarea de prioridad N
//	...
// Tarea de prioridad 1

