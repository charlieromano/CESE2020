#include    "leds.h"

#define STATE_ON 1
#define OFFSET_VALUE 1

static uint16_t * port;

void LedsCreate(uint16_t * direccion){
	port = direccion;
	*port=0x0000;
}

void LedsTurnOn(uint8_t led){
	*port |=(STATE_ON<<(led-OFFSET_VALUE));
}

void LedsTurnOff(uint8_t led){
	*port &=~(STATE_ON<<(led-OFFSET_VALUE));
}

void LedsTurnOnAll(void){
	*port=0xffff;
}

void LedsTurnOffAll(void){
	*port=0x0000;
}

uint16_t LedGetState(uint8_t led){
	return *port;
}