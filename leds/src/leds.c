#include "leds.h"

static uint16_t * puerto;

void LedsCreate(uint16_t * direccion){
	puerto = direccion;
	*puerto=0x0000;
}

void LedsTurnOn(uint8_t led){
	*puerto |=(1<<(led-1));
}

void LedsTurnOff(uint8_t led){
	*puerto &=~(1<<(led-1));
}


