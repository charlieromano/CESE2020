#include "antibouncing.h"
#include "sapi.h"




fsmButtonState2_t fsmButtonState;

void buttonPressed(void){
	static uint8_t cont = 0;
	gpioWrite(LEDR, OFF);
	gpioWrite(LEDB, ON);
	gpioWrite(LEDG, OFF);

}

void buttonReleased(void){
	static uint8_t cont = 0;
	gpioWrite(LEDR, OFF);
	gpioWrite(LEDG, ON);
	gpioWrite(LEDB, OFF);
}

void fsmButtonInit(void){
	fsmButtonState = BUTTON_DOWN;
}

void fsmButtonUpdate(gpioMap_t b){
	
	switch(fsmButtonState){
	   case STATE_BUTTON_UP:
	      if(gpioRead(b)){
	         fsmButtonState = STATE_BUTTON_FALLING;
	      }
	    break;

	    case STATE_BUTTON_FALLING:
	      delay(40);
	      if(gpioRead(b)){
	       fsmButtonState = STATE_BUTTON_DOWN;
	       buttonPressed();
	      } else {
	        fsmButtonState = STATE_BUTTON_UP;
	      }
	    break;

	    case STATE_BUTTON_DOWN:
	      if(!gpioRead(b)){
	         fsmButtonState = STATE_BUTTON_RISING;
	      }
	    break;

	    case STATE_BUTTON_RISING:
	       delay(40);
          if(!gpioRead(b)){
	         fsmButtonState = STATE_BUTTON_UP;
	         buttonReleased();
	      } else {
	       fsmButtonState = STATE_BUTTON_DOWN;
	      }
	    break;


	    default:
	       fsmButtonInit();
	    break;

	}
}