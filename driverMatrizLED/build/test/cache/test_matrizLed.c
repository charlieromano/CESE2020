#include "build/temp/_test_matrizLed.c"
#include "src/matrizLed.h"
#include "/var/lib/gems/2.5.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"








static uint16_t virtualLed;





void setUp(void){

 LedsCreate(&virtualLed);

}







void test_todos_los_leds_inician_apagados(void){



 uint16_t virtualLed = 0xFFFF;

 LedsCreate(&virtualLed);

 UnityAssertEqualNumber((UNITY_INT)((0x0000)), (UNITY_INT)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(20), UNITY_DISPLAY_STYLE_INT);



}





void test_encender_un_led(void){

 setUp();

 LedsTurnOn(1);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0001)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(28), UNITY_DISPLAY_STYLE_UINT16);



}





void test_encender_y_apagar_un_led(void){

 setUp();

 LedsTurnOn(1);

 LedsTurnOff(1);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(37), UNITY_DISPLAY_STYLE_UINT16);

}





void test_encender_todos_los_leds(void){

 setUp();

 LedsTurnOnAll();

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0xFFFF)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(44), UNITY_DISPLAY_STYLE_UINT16);

}





void test_encender_y_apagar_los_leds(void){



 setUp();

 LedsTurnOn(2);

 LedsTurnOn(4);

 LedsTurnOn(6);

 LedsTurnOn(8);

 LedsTurnOff(2);

 LedsTurnOff(4);

 LedsTurnOff(6);

 LedsTurnOff(8);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(59), UNITY_DISPLAY_STYLE_UINT16);

}







void test_encender_y_apagar_todos_los_leds(void){

 setUp();

 LedsTurnOnAll();

 LedsTurnOffAll();

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(68), UNITY_DISPLAY_STYLE_UINT16);

}





void test_consultar_estado_de_led(void){



 setUp();

 LedsTurnOff(3);

 LedGetState(3);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(77), UNITY_DISPLAY_STYLE_UINT16);

}

static uint8_t * virtualMatrix[64];





void encender_fila(void){



 LedMatrixCreate(8,8, virtualMatrix);

 UnityAssertEqualIntArray(( const void*)((0x01)), ( const void*)((virtualMatrix)), (UNITY_UINT32)((64)), (

((void *)0)

), (UNITY_UINT)(97), UNITY_DISPLAY_STYLE_UINT8, UNITY_ARRAY_TO_ARRAY);





}
