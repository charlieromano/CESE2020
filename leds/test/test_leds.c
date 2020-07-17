#include "/var/lib/gems/2.5.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"
#include "leds.h"

//! @test Todos los leds empiezan apagados
void test_todos_los_leds_inician_apagaods(void){

	uint16_t virtuales = 0xFFFF;
	LedsCreate(&virtuales);
	TEST_ASSERT_EQUAL(0x0000, virtuales);

}

//! @test Encender un led
void test_encender_un_led(void){
	uint16_t virtuales;
	LedsCreate(&virtuales);
	LedsTurnOn(1);
	TEST_ASSERT_EQUAL_UINT16(0x0001, virtuales);

}

void test_encender_y_apagar_un_led(void){
	uint16_t virtuales;
	LedsCreate(&virtuales);
	LedsTurnOn(1);
	LedsTurnOff(1);
	TEST_ASSERT_EQUAL_UINT16(0x0000,virtuales);
}

void test_encender_y_apagar_los_leds(void){
	uint16_t virtuales;
	LedsCreate(&virtuales);
	LedsTurnOn(1);
	LedsTurnOff(1);
	TEST_ASSERT_EQUAL_UINT16(0x0000,virtuales);
}
