#include "/var/lib/gems/2.5.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"
#include "leds.h"

//! @test Todos los leds empiezan apagados
void test_todos_los_leds_inician_apagados(void){

	uint16_t virtualLed = 0xFFFF;
	LedsCreate(&virtualLed);
	TEST_ASSERT_EQUAL(0x0000, virtualLed);

}

//! @test Encender un led
void test_encender_un_led(void){
	uint16_t virtualLed;
	LedsCreate(&virtualLed);
	LedsTurnOn(1);
	TEST_ASSERT_EQUAL_UINT16(0x0001, virtualLed);

}

void test_encender_y_apagar_un_led(void){
	uint16_t virtualLed;
	LedsCreate(&virtualLed);
	LedsTurnOn(1);
	LedsTurnOff(1);
	TEST_ASSERT_EQUAL_UINT16(0x0000,virtualLed);
}

void test_encender_todos_los_leds(void){

	uint16_t virtualLed;
	LedsCreate(&virtualLed);
	LedsTurnOnAll();
	TEST_ASSERT_EQUAL_UINT16(0xFFFF,virtualLed);
}

void test_encender_y_apagar_los_leds(void){
	//leds 2,4,6 y 8
	uint16_t virtualLed;
	LedsCreate(&virtualLed);
	LedsTurnOn(2);
	LedsTurnOn(4);
	LedsTurnOn(6);
	LedsTurnOn(8);
	TEST_ASSERT_EQUAL_UINT16(0x00AA,virtualLed); 
}


void test_encender_y_apagar_todos_los_leds(void){
	uint16_t virtualLed;
	LedsCreate(&virtualLed);
	LedsTurnOnAll();
	LedsTurnOffAll();
	TEST_ASSERT_EQUAL_UINT16(0x0000,virtualLed);		
}

void test_consultar_estado_de_led(void){
	//led 3
	uint16_t virtualLed;
	LedsCreate(&virtualLed);
	LedsTurnOn(3);
	TEST_ASSERT_EQUAL_UINT16(0x0004,virtualLed);
}
