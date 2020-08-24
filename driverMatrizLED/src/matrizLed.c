#include    "matrizLed.h"

#define STATE_ON 1
#define OFFSET_VALUE 1

static uint16_t * port;
static uint8_t  * port_matrix_row;
static uint8_t  * port_matrix_row_data;

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

void LedMatrixCreate(uint8_t * rowAddr, uint8_t * dataAddr ){
	port_matrix_row = rowAddr;
	port_matrix_row_data = dataAddr;
	*port_matrix_row = 0x00;
	*port_matrix_row_data = 0x00;

}

void LedsMatrixRowTurnOn(uint8_t row, uint8_t data){
	
}