#ifndef MATRIZLED_H
#define MATRIZLED_H
#include <stdint.h>

void LedsCreate(uint16_t * direccion);
void LedsTurnOn(uint8_t led);
void LedsTurnOff(uint8_t led);
void LedsTurnOnAll(void);
void LedsTurnOffAll(void);
uint16_t LedGetState(uint8_t led);



/* Matriz led 8x8*/

#define    ROW_NUM 8
#define    COL_NUM 8
#define    MAX_ARRAY_SIZE   ROW_NUM*COL_NUM

typedef struct {

	uint8_t size;
	uint8_t data[ROW_NUM];

} tLedMatrix;


void LedMatrixCreate(tLedMatrix * matrix, uint8_t rows, uint8_t cols);
void ledMatrixRowOn(uint8_t row);
void ledMatrixColumnOn(uint8_t column);

#endif