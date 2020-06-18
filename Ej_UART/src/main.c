#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"

#include "driver.h"
#include "FreeRTOSConfig.h"
#include "userTasks.h"

char mensaje[100];

int main( void )
{
    static driver_t Uart_driver; //Variable de la instanciacion del driver que va a usar la tarea principal

    boardConfig();	 // Inicializar la placa

    //Inicializacion del driver de UART

    Uart_driver.uartValue = UART_USB;
    Uart_driver.baudRate = 115200;

    if ( driverInit( &Uart_driver ) == FALSE )
    {
        gpioWrite( LED3, ON ); //Error al inicializar el driver
        printf( "Error al inicializar el driver" );
        while( 1 );
    }

    xTaskCreate(
        led_task,                     // Funcion de la tarea a ejecutar
        ( const char * )"led_task",   // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
        0,                          // Parametros de tarea
        tskIDLE_PRIORITY+1,         // Prioridad de la tarea
        0                           // Puntero a la tarea creada en el sistema
    );

    xTaskCreate(
    	uart_task,                     // Funcion de la tarea a ejecutar
        ( const char * )"uart_task",   // Nombre de la tarea como String amigable para el usuario
        configMINIMAL_STACK_SIZE*2, // Cantidad de stack de la tarea
        ( void* )&Uart_driver,                        // Parametros de tarea
        tskIDLE_PRIORITY+2,         // Prioridad de la tarea
        0                           // Puntero a la tarea creada en el sistema
    );

    Uart_driver.mutex = xSemaphoreCreateMutex();
    Uart_driver.permiso = xSemaphoreCreateBinary();

    // gestion de errores de los semaforos y mutex


    vTaskStartScheduler();


    return 0;
}
