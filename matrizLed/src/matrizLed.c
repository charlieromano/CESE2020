#include    "matrizLed.h"
#include <stdio.h>


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


static tLedMatrix * port_matrix;

void LedMatrixCreate(tLedMatrix * matrix, uint8_t rows, uint8_t cols){

	matrix->size  = rows*cols;
	for(int i=0; i<ROW_NUM; i++)
		matrix->data[i]  = 0x00;
	
	port_matrix   = matrix;

}

void ledMatrixRowOn(uint8_t row){

	port_matrix->data[row] = 0xFF;

}

void ledMatrixRowOff(uint8_t row){

	port_matrix->data[row] = 0x00;

}


void ledMatrixColumnOn(uint8_t column){
	for(int i=0; i<ROW_NUM; i++)
		port_matrix->data[i]  |= (STATE_ON<<(column-OFFSET_VALUE));
}

void ledMatrixColumnOff(uint8_t column){
	for(int i=0; i<ROW_NUM; i++)
		port_matrix->data[i]  &=~(STATE_ON<<(column-OFFSET_VALUE));
}

void ledMatrixOn(void){
	for(int i=0; i<ROW_NUM; i++)
		port_matrix->data[i] = 0xFF;
}

void ledMatrixOff(void){
	for(int i=0; i<ROW_NUM; i++)
		port_matrix->data[i] = 0x00;
}