#include "build/temp/_test_matrizLed.c"
#include "src/matrizLed.h"
#include "/var/lib/gems/2.5.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"








static uint16_t virtualLed;

static uint8_t virtualMatrix[2];





void setUp(void){

 LedsCreate(&virtualLed);

}







void test_todos_los_leds_inician_apagados(void){



 uint16_t virtualLed = 0xFFFF;

 LedsCreate(&virtualLed);

 UnityAssertEqualNumber((UNITY_INT)((0x0000)), (UNITY_INT)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(21), UNITY_DISPLAY_STYLE_INT);



}





void test_encender_un_led(void){

 setUp();

 LedsTurnOn(1);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0001)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(29), UNITY_DISPLAY_STYLE_UINT16);



}





void test_encender_y_apagar_un_led(void){

 setUp();

 LedsTurnOn(1);

 LedsTurnOff(1);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(38), UNITY_DISPLAY_STYLE_UINT16);

}





void test_encender_todos_los_leds(void){

 setUp();

 LedsTurnOnAll();

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0xFFFF)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(45), UNITY_DISPLAY_STYLE_UINT16);

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

), (UNITY_UINT)(60), UNITY_DISPLAY_STYLE_UINT16);

}







void test_encender_y_apagar_todos_los_leds(void){

 setUp();

 LedsTurnOnAll();

 LedsTurnOffAll();

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(69), UNITY_DISPLAY_STYLE_UINT16);

}





void test_consultar_estado_de_led(void){



 setUp();

 LedsTurnOff(3);

 LedGetState(3);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(78), UNITY_DISPLAY_STYLE_UINT16);

}

void encender_fila(void){



 LedMatrixCreate(&virtualMatrix[0], &virtualMatrix[1]);





}
