#include "sapi.h"
#include "antibouncing.h"



int main(void){
   boardInit();
   gpioMap_t b;
   b = TEC1;
   fsmButtonInit();
   
   while(1){
      fsmButtonUpdate(b);
      delay(1);
    };
    return 0;
}
