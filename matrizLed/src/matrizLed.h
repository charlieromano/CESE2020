/**
 * Matriz led 8x8
 * @file matrizLed.h
 * @author Carlos G. C. Romano <charlieromano@gmail.com>
 * @brief driver para matrices led 8x8. Se accede como 
 *        registro (fila) + data. Ejemplo: fila 3, led 5 ON es 
 *        registro=0x03; data=0x10
 * @version 0.1
 * @date 2020-08-24
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef MATRIZLED_H
#define MATRIZLED_H
#include <stdint.h>

#define STATE_ON         1
#define OFFSET_VALUE     1

void LedsCreate(uint16_t * direccion);
void LedsTurnOn(uint8_t led);
void LedsTurnOff(uint8_t led);
void LedsTurnOnAll(void);
void LedsTurnOffAll(void);
uint16_t LedGetState(uint8_t led);




#define    ROW_NUM 8
#define    COL_NUM 8
#define    MAX_ARRAY_SIZE   ROW_NUM*COL_NUM

typedef struct {

	uint8_t size;
	uint8_t data[ROW_NUM];

} tLedMatrix;


void LedMatrixCreate(tLedMatrix * matrix, uint8_t rows, uint8_t cols);
void ledMatrixRowOn(uint8_t row);
void ledMatrixRowOff(uint8_t row);
void ledMatrixColumnOn(uint8_t column);
void ledMatrixColumnOff(uint8_t column);
void ledMatrixOn(void);
void ledMatrixOff(void);

#endif