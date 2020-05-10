#include "sapi.h"
#include "StateMachines.h"


int main (void){

  boardInit();
  delay_t t1;
  delayConfig(&t1, 600);
  fsmSemaphoreState_t fsmSemaphoreState;
   
  struct semaphore s1 = {&t1,LED2, LED1, LED3, semaphoreGreen};
  fsmSemaphoreState = s1.semaphoreState(&s1);
  
  //fsmSemaphoreInit(&s1);
 
  while(1){
   fsmSemaphoreUpdate(&s1);
   delay(1);
   
   }
   return 0;
}


