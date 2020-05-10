#ifndef __STATEMACHINES_H__
#define __STATEMACHINES_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include <stdint.h>
#include <stddef.h>

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif


#include "sapi.h"

#define T_OFFLINE 800 
#define T_ALARM   800 
#define T_RED     800 
#define T_YELLOW  800
#define T_YELLOW_RED  800
#define T_GREEN   1000
#define T_INIT 1500

// FSM state names
typedef enum {
   SEMAPHORE_STATE_OFF,
   SEMAPHORE_STATE_RED, 
   SEMAPHORE_STATE_YELLOW,
   SEMPAHORE_STATE_YELLOW_RED,
   SEMAPHORE_STATE_GREEN
} fsmSemaphoreState_t;

struct semaphore{
   //delay_t    T; //delayConfig( &t1 , T_RED);
   //struct light lights[3]; //struct light l_red = {LED2, lightOff};
   //struct light * lamps[3];
   delay_t* cycle;
   gpioMap_t redLamp;
   gpioMap_t yellowLamp;
   gpioMap_t greenLamp;
   fsmSemaphoreState_t   (*semaphoreState)(const struct semaphore*);
};


fsmSemaphoreState_t semaphoreRed(const struct semaphore* );
fsmSemaphoreState_t semaphoreYellow(const struct semaphore* );
fsmSemaphoreState_t semaphoreGreen(const struct semaphore* );      
fsmSemaphoreState_t semaphoreYellowRed(const struct semaphore* );
fsmSemaphoreState_t semaphoreOff(const struct semaphore* );

void fsmSemaphoreInit( struct semaphore* );
void fsmSemaphoreUpdate( struct semaphore* );




#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __STATEMACHINES_H__ */
