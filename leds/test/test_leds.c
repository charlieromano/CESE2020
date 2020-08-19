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

void test_encender_y_apagar_los_leds(void){
	uint16_t virtualLed;


	TEST_ASSERT_EQUAL_UINT16(0x0000,virtualLed);	
}

void test_encender_todos_los_leds(void){

	uint16_t virtualLed;
	LedsCreate(&virtualLed);
	LedsTurnOnAll();
	TEST_ASSERT_EQUAL_UINT16(0x8000,virtualLed);
}

void test_encender_y_apagar_todos_los_leds_de_una_vez(void){
	
}

void test_consultar_estado_de_led(void){

}


/*
void test_promedio_normal(void) {
   int datos[] = {1,2,3,4,5};
   int resultado;

   TEST_ASSERT_EQUAL(0, promediar(datos, 5, &resultado));
   TEST_ASSERT_EQUAL(3, resultado);
}

int promediar(const int valores[], int cantidad, int * promedio) {
   int resultado;
   int indice;
   int acumulado = 0;

   for (indice = 0; indice < cantidad; indice++) {
      resultado = acumular(&acumulado, valores[indice]);
      if (resultado != 0) break;
   }
   if (resultado == 0) {
      *promedio = (acumulado / cantidad);
   }
   return resultado;
}
*/