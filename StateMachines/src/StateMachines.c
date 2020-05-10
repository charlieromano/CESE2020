#include "sapi.h"
#include "StateMachines.h"


fsmSemaphoreState_t semaphoreRed(const struct semaphore* s){
   gpioWrite( s->redLamp, ON);
   gpioWrite( s->yellowLamp, OFF);
   gpioWrite( s->greenLamp, OFF);
   return SEMAPHORE_STATE_RED;
}

fsmSemaphoreState_t semaphoreYellow(const struct semaphore* s){
   gpioWrite( s->redLamp, OFF);
   gpioWrite( s->yellowLamp, ON);
   gpioWrite( s->greenLamp, OFF);
   return SEMAPHORE_STATE_YELLOW;
}

fsmSemaphoreState_t semaphoreGreen(const struct semaphore* s){
   gpioWrite( s->redLamp, OFF);
   gpioWrite( s->yellowLamp, OFF);
   gpioWrite( s->greenLamp, ON);
   return SEMAPHORE_STATE_GREEN;
}

fsmSemaphoreState_t semaphoreYellowRed(const struct semaphore* s){
   gpioWrite( s->redLamp, ON);
   gpioWrite( s->yellowLamp, ON);
   gpioWrite( s->greenLamp, OFF);
   return SEMPAHORE_STATE_YELLOW_RED;
}

fsmSemaphoreState_t semaphoreOff(const struct semaphore* s){
   gpioWrite( s->redLamp, ON);
   gpioWrite( s->yellowLamp, ON);
   gpioWrite( s->greenLamp, OFF);
   return SEMAPHORE_STATE_OFF;
}

void fsmSemaphoreInit( struct semaphore* s ){
    delayConfig(s->cycle, T_INIT);
    s->semaphoreState = semaphoreOff;
    ;
}

static fsmSemaphoreState_t fsmSemaphoreState;


void fsmSemaphoreUpdate( struct semaphore* s ){
   
   static uint32_t contador = 0;

   fsmSemaphoreState_t fsmSemaphoreState;
   fsmSemaphoreState = s->semaphoreState(s);
   
   switch(fsmSemaphoreState){
      
      case SEMAPHORE_STATE_RED:
         s->semaphoreState = semaphoreRed;
         s->semaphoreState(s);
         contador++;
         if( contador >= T_RED ){
            s->semaphoreState = semaphoreYellowRed;
            s->semaphoreState(s);
            contador = 0;
         };
      break;

      case SEMPAHORE_STATE_YELLOW_RED:
         s->semaphoreState = semaphoreYellowRed;
         s->semaphoreState(s);
         contador++;
         if( contador >= T_RED ){
            s->semaphoreState = semaphoreGreen;
            s->semaphoreState(s);
            contador = 0;
         }
      break;

      case SEMAPHORE_STATE_GREEN:
         s->semaphoreState = semaphoreGreen;
         s->semaphoreState(s);
         contador++;
         if(contador >= T_GREEN ){
            s->semaphoreState = semaphoreYellow;
            s->semaphoreState(s);
            contador = 0;
         }
      break;

      case SEMAPHORE_STATE_YELLOW:
         s->semaphoreState = semaphoreYellow;
         s->semaphoreState(s);
         contador++;
         if(contador >= T_YELLOW ){
            s->semaphoreState = semaphoreRed;
            s->semaphoreState(s);
            contador = 0;
         }
      break;

      default:
         s->semaphoreState = semaphoreOff;
         s->semaphoreState(s);
      break;

  }
}