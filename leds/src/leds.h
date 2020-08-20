#include <stdint.h>

void LedsCreate(uint16_t * direccion);
void LedsTurnOn(uint8_t led);
void LedsTurnOff(uint8_t led);
void LedsTurnOnAll(void);
void LedsTurnOffAll(void);
uint16_t LedGetState(uint8_t led);
