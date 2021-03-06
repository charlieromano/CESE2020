/*==================[inlcusiones]============================================*/

// Includes de FreeRTOS
#include "FreeRTOS.h"   //Motor del OS
#include "task.h"		//Api de control de tareas y temporizaciÃ³n
#include "semphr.h"		//Api de sincronizaciÃ³n (sem y mutex)
#include "queue.h"      //Api de colas

// sAPI header
#include "sapi.h"

#include "FreeRTOSConfig.h"
#include "board.h"

#define UP      1
#define FALLING 2
#define DOWN	3
#define RISING  4

#define CANT_TECLAS 1
#define CANT_LEDS 4
#define ANTIREBOTE_MS 20

DEBUG_PRINT_ENABLE;

void My_IRQ_Init (void);
void Tecla( void* taskParmPtr );
void Led_task( void* taskParmPtr );

enum Teclas_t {Tecla1}; //indices de teclas para el vector de estructuras

struct Button_Control { //estructura de control de datos capturados por la interrupciÃ³n
	TickType_t Tiempo_inicial;
	uint8_t Flanco;
};

struct Buttons_SM_t{ //estructura de control de la mÃ¡quina de estados de cada botÃ³n
	uint8_t Tecla;
	uint8_t Estado;
	xQueueHandle Cola;
	TickType_t Tiempo_inicial;
};

struct Lectura_t{
	uint8_t Tecla;
	TickType_t Tiempo_medido;
};



SemaphoreHandle_t Mutex_uart; //Mutex que protege la UART de concurrencia
xQueueHandle Cola_Lecturas;
struct Buttons_SM_t Buttons_SM[CANT_TECLAS];

int main(void)
{
   uint8_t Error_state = 0;

   boardConfig();
   My_IRQ_Init();

   // UART for debug messages
   debugPrintConfigUart( UART_USB, 115200 );
   printf( "Antirrebote con IRQ freeRTOS y sAPI\r\n" );

   /* Creamos colas de capturas de teclas */
	int8_t i;
   	for (i = CANT_TECLAS ; i-- ; i >= 0) {
   	  Buttons_SM[i].Tecla = i;
   	  if (NULL == (Buttons_SM[i].Cola = xQueueCreate(5,sizeof(struct Button_Control)))){
		   Error_state =1;
	   }
   	}

   	/* Creamos cola de lecturas completadas */
   	if (NULL == (Cola_Lecturas = xQueueCreate(10,sizeof(struct Lectura_t)))){
   	   	   Error_state =1;
   	   }

   if (NULL == (Mutex_uart = xSemaphoreCreateMutex())){
   	   Error_state =1;
   }

   // Crear tareas de Teclas en freeRTOS
   xTaskCreate(
	  Tecla,                     // Funcion de la tarea a ejecutar
      (const char *)"Tec1",     // Nombre de la tarea como String amigable para el usuario
      configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
	  &Buttons_SM[0],                 // Parametros de tarea
      tskIDLE_PRIORITY+1,         // Prioridad de la tarea
      0                           // Puntero a la tarea creada en el sistema
   );


   // Crear tarea LED en freeRTOS
  xTaskCreate(
   Led_task,                     // Funcion de la tarea a ejecutar
	 (const char *)"Led",     // Nombre de la tarea como String amigable para el usuario
	 configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
	 0,                          // Parametros de tarea
	 tskIDLE_PRIORITY+2,         // Prioridad de la tarea
	 0                           // Puntero a la tarea creada en el sistema
  );

   // Iniciar scheduler
   if (0 == Error_state){
  	  vTaskStartScheduler();
   } else{
	  printf("Error al iniciar el sistema !!!!!!!!!!!!!!");
   }

   while( TRUE );

   return 0;
}


/*==================[definiciones de funciones externas]=====================*/

// Implementacion de funcion de la tarea genÃ©rica Tecla
void Tecla( void* taskParmPtr )
{
   // ---------- DefiniciÃ³n de variables locales ------------------------------
	struct Buttons_SM_t* Config; //Me preparo para recibir la direcciÃ³n de la estructura y copiarla en una varibale local
	Config = (struct Buttons_SM_t*) taskParmPtr;
	Config->Estado = UP;
	struct Lectura_t Lectura;
	Lectura.Tecla = Config->Tecla;
   
	struct Button_Control Control;
	TickType_t Last_Snapshot = 0;

	 if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){

		 printf("Tarea TEC%d iniciada\r\n",Lectura.Tecla+1);

		  xSemaphoreGive( Mutex_uart );

	  }



   // ---------- REPETIR POR SIEMPRE --------------------------
   while(TRUE) {

	   if (xQueueReceive(Config->Cola, &Control, portMAX_DELAY)){

			   switch (Config->Estado){

					 case UP:
						  if(Control.Flanco == FALLING){ //AcÃ¡ adentro estÃ¡ el pseudo estado Falling

							  if (pdFALSE == (xQueueReceive(Config->Cola, &Control, (ANTIREBOTE_MS / portTICK_RATE_MS)))){
								  Config->Estado = DOWN;

								  //AcÃ¡ se mete cÃ³digo para ejecutar en flanco  de bajada
								  Config->Tiempo_inicial = Control.Tiempo_inicial;

								  if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){

									  printf("Se capturo una pulsacion\r\n");

									  xSemaphoreGive( Mutex_uart );

								  }

							  }
						  }
					 break;

					 case DOWN:
						 if(Control.Flanco == RISING){ //AcÃ¡ adentro estÃ¡ el pseudo estado Rising

							  if (pdFALSE == (xQueueReceive(Config->Cola, &Control, (ANTIREBOTE_MS / portTICK_RATE_MS)))){
								  Config->Estado = UP;

								  //AcÃ¡ se mete cÃ³digo para ejecutar en flanco  de subida
								  Lectura.Tiempo_medido = xTaskGetTickCount() - Config->Tiempo_inicial;

								  if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){

									  printf("Lectura completada en la tecla %d, presionada por %dms\r\n",Lectura.Tecla+1,Lectura.Tiempo_medido*portTICK_RATE_MS);

									  xSemaphoreGive( Mutex_uart );

								  }


								  xQueueSend(Cola_Lecturas, &Lectura, portMAX_DELAY);
							  }
						  }
					 break;

					 default:
						 Config->Estado = UP;
					 break;

			  }

	   	  }


	}

}

#define MY_ASSERT(CONDICION) my_assert_debug(CONDICION)
/*==================[definiciones de funciones internas]=====================*/
//FunciÃ³n de inicializaciÃ³n de IRQs
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


// Implementacion de funcion de la tarea Led
void Led_task( void* taskParmPtr ){

	struct Lectura_t Lectura;

	gpioMap_t Led_Map[CANT_LEDS] = {LEDR,LED1,LED2,LED3};

	while (TRUE){
		//Espero evento de Lectura completada
		if (xQueueReceive(Cola_Lecturas, &Lectura, portMAX_DELAY)){

			if (pdTRUE == xSemaphoreTake( Mutex_uart, portMAX_DELAY)){

				  printf("Se encendera el LED %d, por %dms\r\n",Lectura.Tecla+1,Lectura.Tiempo_medido*portTICK_RATE_MS);

				  xSemaphoreGive( Mutex_uart );

			}

			gpioWrite(Led_Map[Lectura.Tecla],ON);

			//Espero tiempo de encendido
			vTaskDelay( Lectura.Tiempo_medido );

			gpioWrite(Led_Map[Lectura.Tecla],OFF);
		}
	}
}


void GPIO0_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable


	if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH0){ //Verificamos que la interrupciÃ³n es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH0); //Borramos el flag de interrupciÃ³n
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n

		struct Button_Control Snapshot;
		Snapshot.Flanco = FALLING;
		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();

		xQueueSendFromISR( Buttons_SM[Tecla1].Cola, &Snapshot, &xHigherPriorityTaskWoken );


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
			struct Button_Control Snapshot;
			Snapshot.Flanco = RISING;
			Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
			xQueueSendFromISR( Buttons_SM[Tecla1].Cola, &Snapshot, &xHigherPriorityTaskWoken );
		}
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

