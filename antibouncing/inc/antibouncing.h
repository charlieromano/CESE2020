#ifndef __ANTIBOUNCING_H__
#define __ANTIBOUNCING_H__

#include <stdint.h>
#include <stddef.h>
#include "sapi.h"

#ifdef __cplusplus
extern "C" {
#endif
   
typedef enum{
	STATE_BUTTON_UP,
	STATE_BUTTON_DOWN,
	STATE_BUTTON_RISING,
	STATE_BUTTON_FALLING
} fsmButtonState2_t;

void fsmButtonInit(void);
void fsmButtonUpdate( gpioMap_t );

void buttonPressed(void);
void buttonReleased(void);


#ifdef __cplusplus
}
#endif


#endif /* __ANTIBOUNCING_H__ */
