#include "/var/lib/gems/2.5.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"
#include "matrizLed.h"



//! @test common init
static uint16_t virtualLed;


void setUp(void){
	LedsCreate(&virtualLed);	
}


//! @test Todos los leds empiezan apagados
void test_todos_los_leds_inician_apagados(void){

	uint16_t virtualLed = 0xFFFF;
	LedsCreate(&virtualLed);
	TEST_ASSERT_EQUAL(0x0000, virtualLed);

}

//! @test Encender un led
void test_encender_un_led(void){
	setUp();
	LedsTurnOn(1);
	TEST_ASSERT_EQUAL_UINT16(0x0001, virtualLed);

}

//! @test Encender y apagar un led
void test_encender_y_apagar_un_led(void){
	setUp();
	LedsTurnOn(1);
	LedsTurnOff(1);
	TEST_ASSERT_EQUAL_UINT16(0x0000,virtualLed);
}

//! @test Encender todos los leds de una vez
void test_encender_todos_los_leds(void){
	setUp();
	LedsTurnOnAll();
	TEST_ASSERT_EQUAL_UINT16(0xFFFF,virtualLed);
}

//! @test Encender y apagar algunos los leds
void test_encender_y_apagar_los_leds(void){
	//leds 2,4,6 y 8
	setUp();
	LedsTurnOn(2);
	LedsTurnOn(4);
	LedsTurnOn(6);
	LedsTurnOn(8);
	LedsTurnOff(2);
	LedsTurnOff(4);
	LedsTurnOff(6);
	LedsTurnOff(8);
	TEST_ASSERT_EQUAL_UINT16(0x0000,virtualLed); 
}


//! @test Encender y apagar todos los leds
void test_encender_y_apagar_todos_los_leds(void){
	setUp();
	LedsTurnOnAll();
	LedsTurnOffAll();
	TEST_ASSERT_EQUAL_UINT16(0x0000,virtualLed);		
}

//! @test consultar el estado de un led
void test_consultar_estado_de_led(void){
	//led 3
	setUp();
	LedsTurnOff(3);
	LedGetState(3);
	TEST_ASSERT_EQUAL_UINT16(0x0000,virtualLed);
}

/* Grupo: matriz Led 8x8 */
/*
 * encender una fila de leds
 * encender una coumna de leds
 * encender un caracter
 * encender un numero
 * desplazar una fila
 * desplazar una columna
 * 
 */

static uint8_t * virtualMatrix[MAX_ARRAY_SIZE];


void encender_fila(void){

	LedMatrixCreate(8,8, virtualMatrix);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(0x01,virtualMatrix, 64); 
	//TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, actual, elements)

}
