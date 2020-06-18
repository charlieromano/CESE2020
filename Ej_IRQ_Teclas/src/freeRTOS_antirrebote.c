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


/*==================[definiciones y macros]==================================*/
#define UP      1
#define FALLING 2
#define DOWN	3
#define RISING  4

#define CANT_TECLAS 4
#define CANT_LEDS 4
#define ANTIREBOTE_MS 20

enum Teclas_t {Tecla1, Tecla2, Tecla3, Tecla4}; //indices de teclas para el vector de estructuras

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



/*==================[definiciones de datos internos]=========================*/
SemaphoreHandle_t Mutex_uart; //Mutex que protege la UART de concurrencia

xQueueHandle Cola_Lecturas;

//DefiniciÃ³n de vector de estructuras de control
struct Buttons_SM_t Buttons_SM[CANT_TECLAS];

/*==================[definiciones de datos externos]=========================*/

DEBUG_PRINT_ENABLE;

/*==================[declaraciones de funciones internas]====================*/
//FunciÃ³n de inicializaciÃ³n de interrupciones
void My_IRQ_Init (void);

/*==================[declaraciones de funciones externas]====================*/

// Prototipo de funcion de la tarea
void Tecla( void* taskParmPtr );

void Led_task( void* taskParmPtr );

/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void)
{
   uint8_t Error_state = 0;
	// ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();

   //Iniciamos las interrupciones
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

   xTaskCreate(
		 Tecla,                     // Funcion de la tarea a ejecutar
         (const char *)"Tec2",     // Nombre de la tarea como String amigable para el usuario
         configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
		 &Buttons_SM[1],                          // Parametros de tarea
         tskIDLE_PRIORITY+1,         // Prioridad de la tarea
         0                           // Puntero a la tarea creada en el sistema
      );

   xTaskCreate(
		 Tecla,                     // Funcion de la tarea a ejecutar
         (const char *)"Tec3",     // Nombre de la tarea como String amigable para el usuario
         configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
		 &Buttons_SM[2],                         // Parametros de tarea
         tskIDLE_PRIORITY+1,         // Prioridad de la tarea
         0                           // Puntero a la tarea creada en el sistema
      );

   xTaskCreate(
		 Tecla,                     // Funcion de la tarea a ejecutar
         (const char *)"Tec4",     // Nombre de la tarea como String amigable para el usuario
         configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
		 &Buttons_SM[3],                          // Parametros de tarea
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

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE ) {
      // Si cae en este while 1 significa que no pudo iniciar el scheduler
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
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

		// TEC2 FALL
		Chip_SCU_GPIOIntPinSel(2, 0, 8);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH2);
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH2);

		// TEC1 RISE
		Chip_SCU_GPIOIntPinSel(3, 0, 8);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH3);
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH3);

		// TEC3 FALL
		Chip_SCU_GPIOIntPinSel(4, 0, 9);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH4);
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH4);

		// TEC1 RISE
		Chip_SCU_GPIOIntPinSel(5, 0, 9);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH5);
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH5);

		// TEC4 FALL
		Chip_SCU_GPIOIntPinSel(6, 1, 9);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH6);
		Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH6);

		// TEC1 RISE
		Chip_SCU_GPIOIntPinSel(7, 1, 9);
		Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH7);
		Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH7);


		//Una vez que se han configurado los eventos para cada canal de interrupcion
		//Se activan las interrupciones para que comiencen a llamar al handler
		NVIC_SetPriority(PIN_INT0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
		NVIC_EnableIRQ(PIN_INT0_IRQn);
		NVIC_SetPriority(PIN_INT1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
		NVIC_EnableIRQ(PIN_INT1_IRQn);
		NVIC_SetPriority(PIN_INT2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
		NVIC_EnableIRQ(PIN_INT2_IRQn);
		NVIC_SetPriority(PIN_INT3_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
		NVIC_EnableIRQ(PIN_INT3_IRQn);
		NVIC_SetPriority(PIN_INT4_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
		NVIC_EnableIRQ(PIN_INT4_IRQn);
		NVIC_SetPriority(PIN_INT5_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
		NVIC_EnableIRQ(PIN_INT5_IRQn);
		NVIC_SetPriority(PIN_INT6_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
		NVIC_EnableIRQ(PIN_INT6_IRQn);
		NVIC_SetPriority(PIN_INT7_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
		NVIC_EnableIRQ(PIN_INT7_IRQn);

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

void GPIO2_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable


	if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH2){ //Verificamos que la interrupciÃ³n es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH2); //Borramos el flag de interrupciÃ³n
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
		struct Button_Control Snapshot;
		Snapshot.Flanco = FALLING;
		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
		xQueueSendFromISR( Buttons_SM[Tecla2].Cola, &Snapshot, &xHigherPriorityTaskWoken );
	}

	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO3_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH3){
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH3);
			//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
			struct Button_Control Snapshot;
			Snapshot.Flanco = RISING;
			Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
			xQueueSendFromISR( Buttons_SM[Tecla2].Cola, &Snapshot, &xHigherPriorityTaskWoken );
		}
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO4_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable


	if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH4){ //Verificamos que la interrupciÃ³n es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH4); //Borramos el flag de interrupciÃ³n
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
		struct Button_Control Snapshot;
		Snapshot.Flanco = FALLING;
		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
		xQueueSendFromISR( Buttons_SM[Tecla3].Cola, &Snapshot, &xHigherPriorityTaskWoken );
	}

	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO5_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH5){
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH5);
			//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
			struct Button_Control Snapshot;
			Snapshot.Flanco = RISING;
			Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
			xQueueSendFromISR( Buttons_SM[Tecla3].Cola, &Snapshot, &xHigherPriorityTaskWoken );
		}
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO6_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; //Comenzamos definiendo la variable


	if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH6){ //Verificamos que la interrupciÃ³n es la esperada
		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH6); //Borramos el flag de interrupciÃ³n
		//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
		struct Button_Control Snapshot;
		Snapshot.Flanco = FALLING;
		Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
		xQueueSendFromISR( Buttons_SM[Tecla4].Cola, &Snapshot, &xHigherPriorityTaskWoken );
	}

	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

void GPIO7_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH7){
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH7);
			//codigo a ejecutar si ocurriÃ³ la interrupciÃ³n
			struct Button_Control Snapshot;
			Snapshot.Flanco = RISING;
			Snapshot.Tiempo_inicial = xTaskGetTickCountFromISR();
			xQueueSendFromISR( Buttons_SM[Tecla4].Cola, &Snapshot, &xHigherPriorityTaskWoken );
		}
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
/*==================[fin del archivo]========================================*/
