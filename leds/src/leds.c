#include    "leds.h"

#define    LEDS_NUM    16

static uint16_t * port;

void LedsCreate(uint16_t * direccion){
	port = direccion;
	*port=0x0000;
}

void LedsTurnOn(uint8_t led){
	*port |=(1<<(led-1));
}

void LedsTurnOff(uint8_t led){
	*port &=~(1<<(led-1));
}

void LedsTurnOnAll(void){
	*port=0xffff;
}

void LedsTurnOffAll(void){
	*port=0x0000;
}