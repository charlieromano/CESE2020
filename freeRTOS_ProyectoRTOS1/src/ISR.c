//Interrupt
#include "FreeRTOS.h"   //Motor del OS
#include "FreeRTOSConfig.h"
#include "task.h"		//Api de control de tareas y temporizaciÃ³n
#include "semphr.h"		//Api de sincronizaciÃ³n (sem y mutex)
#include "queue.h"      //Api de colas
#include "sapi.h"
#include "board.h"

#include "ISR.h"
#include "types_config.h"


#define MY_ASSERT(CONDICION) my_assert_debug(CONDICION)

extern Buttons_SM_t Buttons_SM[1];



void My_IRQ_Init (void){
		//Inicializamos las interrupciones (LPCopen)
		Chip_PININT_Init(LPC_GPIO_PIN_INT);

		//Inicializamos de cada evento de interrupcion (LPCopen)
		/*
		GLOBAL! extern pinInitGpioLpc4337_t gpioPinsInit[];
		Chip_SCU_GPIOIntPinSel( j,  gpioPinsInit[i].gpio.port, gpioPinsInit[i].gpio.pin );   // TECi
		Chip_PININT_ClearIntStatus( LPC_GPIO_PIN_INT, PININTCH( j ) );                      // INTj (canal j -> hanlder GPIOj)       //Borra el pending de la IRQ
		Chip_PININT_SetPinModeEdge( LPC_GPIO_PIN_INT, PININTCH( j ) );                      // INTj //Selecciona activo por flanco
		Chip_PININT_EnableIntLow( LPC_GPIO_PIN_INT, PININTCH( j ) );                        // INTj //Selecciona activo por flanco descendente
		Chip_PININT_EnableIntHigh( LPC_GPIO_PIN_INT, PININTCH( j ) );                       // INTj //Selecciona activo por flanco ascendente
		*/

		// TEC1 FALL
		Chip_SCU_GPIOIntPinSel(0, 0, 4); 	//(Canal 0 a 7, Puerto GPIO, Pin GPIO)
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH0);//Se configura el canal para que se active por flanco
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH0);//Se configura para que el flanco sea el de bajada

		// TEC1 RISE
		Chip_SCU_GPIOIntPinSel(1, 0, 4);	//(Canal 0 a 7, Puerto GPIO, Pin GPIO)
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH1);//Se configura el canal para que se active por flanco
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH1);//En este caso el flanco es de subida

		//Una vez que se han configurado los eventos para cada canal de interrupcion
		//Se activan las interrupciones para que comiencen a llamar al handler
		NVIC_SetPriority(PIN_INT0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
		NVIC_EnableIRQ(PIN_INT0_IRQn);
		NVIC_SetPriority(PIN_INT1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
		NVIC_EnableIRQ(PIN_INT1_IRQn);

}

void GPIO0_IRQHandler(void){

	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable


	if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH0){ //Verificamos que la interrupciÃ³n es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH0); //Borramos el flag de interrupciÃ³n
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n

		fsmButtonISR_t Snapshot;
		Snapshot.Flanco = STATE_BUTTON_FALLING;
		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();

		xQueueSendFromISR( Buttons_SM[0].Cola, &Snapshot, &xHigherPriorityTaskWoken );
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
//A 2 <
//B 3
void GPIO1_IRQHandler(void){

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH1){
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH1);
			//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
			fsmButtonISR_t Snapshot;
			Snapshot.Flanco = STATE_BUTTON_RISING;
			Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
			
			xQueueSendFromISR( Buttons_SM[0].Cola, &Snapshot, &xHigherPriorityTaskWoken );
		}
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

