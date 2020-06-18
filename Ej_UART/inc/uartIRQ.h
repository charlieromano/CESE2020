#ifndef UARTIRQ_H_
#define UARTIRQ_H_

#include "../../Ej_UART/inc/driver.h"

bool_t txInterruptEnable( driver_t* );
bool_t rxInterruptEnable( driver_t* );
void onTxTimeOutCallback( TimerHandle_t );
void onRxTimeOutCallback( TimerHandle_t );

driver_t *uartData[5];  //Puntero a las variables con los datos de la UART instanciada
//se usa para que cada callback sepa sobre que UART esta trabajando.

#endif
