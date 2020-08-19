#include "build/temp/_test_leds.c"
#include "src/leds.h"
#include "/var/lib/gems/2.5.0/gems/ceedling-0.30.0/vendor/unity/src/unity.h"




void test_todos_los_leds_inician_apagados(void){



 uint16_t virtualLed = 0xFFFF;

 LedsCreate(&virtualLed);

 UnityAssertEqualNumber((UNITY_INT)((0x0000)), (UNITY_INT)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(9), UNITY_DISPLAY_STYLE_INT);



}





void test_encender_un_led(void){

 uint16_t virtualLed;

 LedsCreate(&virtualLed);

 LedsTurnOn(1);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0001)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(18), UNITY_DISPLAY_STYLE_UINT16);



}



void test_encender_y_apagar_un_led(void){

 uint16_t virtualLed;

 LedsCreate(&virtualLed);

 LedsTurnOn(1);

 LedsTurnOff(1);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(27), UNITY_DISPLAY_STYLE_UINT16);

}



void test_encender_todos_los_leds(void){



 uint16_t virtualLed;

 LedsCreate(&virtualLed);

 LedsTurnOnAll();

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0xFFFF)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(35), UNITY_DISPLAY_STYLE_UINT16);

}



void test_encender_y_apagar_los_leds(void){



 uint16_t virtualLed;

 LedsCreate(&virtualLed);

 LedsTurnOn(2);

 LedsTurnOn(4);

 LedsTurnOn(6);

 LedsTurnOn(8);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x00AA)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(46), UNITY_DISPLAY_STYLE_UINT16);

}





void test_encender_y_apagar_todos_los_leds(void){

 uint16_t virtualLed;

 LedsCreate(&virtualLed);

 LedsTurnOnAll();

 LedsTurnOffAll();

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0000)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(55), UNITY_DISPLAY_STYLE_UINT16);

}



void test_consultar_estado_de_led(void){



 uint16_t virtualLed;

 LedsCreate(&virtualLed);

 LedsTurnOn(3);

 UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0x0004)), (UNITY_INT)(UNITY_UINT16)((virtualLed)), (

((void *)0)

), (UNITY_UINT)(63), UNITY_DISPLAY_STYLE_UINT16);

}
