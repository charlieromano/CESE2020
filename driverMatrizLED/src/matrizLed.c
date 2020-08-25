#include    "matrizLed.h"

#define STATE_ON         1
#define OFFSET_VALUE     1

static uint16_t * port;
static uint8_t  * port_matrix[MAX_ARRAY_SIZE];

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

void LedMatrixCreate(uint8_t rows, uint8_t columns, uint8_t * direccion[MAX_ARRAY_SIZE]){

	uint16_t size = rows * columns;
	*port_matrix = *direccion;
	for(uint8_t i=0; i<size; i++){
		*port_matrix[i] = i;
	}
	
}

void LedsMatrixRowTurnOn(uint8_t row, uint8_t data){
	
}