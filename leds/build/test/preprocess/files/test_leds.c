#include "build/temp/_test_leds.c"
#include "src/leds.h"
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

), (UNITY_UINT)(17), UNITY_DISPLAY_STYLE_INT);



}





void test_encender_un_led(void){

 setUp();

 LedsTurnOn(1);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0001)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(25), UNITY_DISPLAY_STYLE_UINT16);



}





void test_encender_y_apagar_un_led(void){

 setUp();

 LedsTurnOn(1);

 LedsTurnOff(1);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(34), UNITY_DISPLAY_STYLE_UINT16);

}





void test_encender_todos_los_leds(void){

 setUp();

 LedsTurnOnAll();

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0xFFFF)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(41), UNITY_DISPLAY_STYLE_UINT16);

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

), (UNITY_UINT)(56), UNITY_DISPLAY_STYLE_UINT16);

}







void test_encender_y_apagar_todos_los_leds(void){

 setUp();

 LedsTurnOnAll();

 LedsTurnOffAll();

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(65), UNITY_DISPLAY_STYLE_UINT16);

}





void test_consultar_estado_de_led(void){



 setUp();

 LedsTurnOff(3);

 LedGetState(3);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(74), UNITY_DISPLAY_STYLE_UINT16);

}
