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
#define MAX_ARRAY_SIZE   64

void LedMatrixCreate(uint8_t rows, uint8_t columns, uint8_t * direccion[MAX_ARRAY_SIZE]);
void LedsMatrixRowTurnOn(uint8_t row, uint8_t data);
