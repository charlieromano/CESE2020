#include "fsmTrain.h"
#include "sapi.h"


int main(void){

	boardInit();
	fsmTrainInit();
	while(1){
		fsmTrainUpdate();
		delay(1);
	}
	return 0;
}
