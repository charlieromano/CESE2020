#include <stdint.h>

void LedsCreate(uint16_t * direccion);
void LedsTurnOn(uint8_t led);
void LedsTurnOff(uint8_t led);
void LedsTurnOnAll(void);
void LedsTurnOffAll(void);
uint16_t LedGetState(uint8_t led);

/* Matriz led 8x8*/

#define    ROW_INDEX   0
#define    DATA_INDEX  1

void LedMatrixCreate(uint8_t * rowAddr, uint8_t * dataAddr );
void LedsMatrixRowTurnOn(uint8_t row, uint8_t data);
