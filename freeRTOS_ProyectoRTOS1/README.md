# freeRTOS_ProyectoRTOS1

Este proyecto implementa tareas que permiten recibir una señal analógica y visualizar un mensaje en una matriz led.
Este proyecto utiliza freeRTOS y consiste de las siguientes tareas:

* vTaskReadADC(): lee una señal analógica como entrada del conversor AD, entrega el valor usando una cola q1
* vTaskProcessData(): recibe los datos de entrada usando la cola q1, los procesa y los envía como datos de salida usando una cola q2
* vTaskWriteSPI(): recibe los datos procesados para escribir en el puerto SPI
* max7219(): estas funciones se implementaron para usar el driver MAX7219 para visualizar datos sobre una matriz led 8x8
* ISR: las interrupciones reciben una señal de una tecla y asignan un valor a la estructura de datos de procesamiento, para indicar si se debe cambiar el mensaje de visualizacion.


