#include "fsmTrain.h"
#include "sapi.h"

fsmTrainState_t fsmTrainState;

void fsmTrainInit(void){
	//boardInit();
	fsmTrainState=STATE_DETENIDO;
}

void fsmTrainUpdate(void){
	static uint32_t contador=0;

	switch(fsmTrainState){
		case STATE_DETENIDO:
		   printf("Detenido \n");
		   contador++;
		   if(contador >=100){
            delay(1000);
		   	fsmTrainState=STATE_CIRCULANDO;
		   	contador=0;
		   }
		break;

		case STATE_CABECERA:
		    printf("Estacion cabecera \n");
		    contador++;
		    if(contador >=20){
             delay(8000);
		   		fsmTrainState=STATE_DETENIDO;
		   		contador=0;
		   }
		break;

		case STATE_CIRCULANDO:
			printf("Circulando\n");
         contador++;
		    if(contador >=30){
               delay(4000);
		   		fsmTrainState=STATE_ARRIBANDO;
		   		contador=0;
		   }
		break;

		case STATE_ARRIBANDO:
			printf("Proxima estacion...\n");
         contador++;
		    if(contador >=30){
               delay(4000);
		   		fsmTrainState=STATE_ESTACION;
		   		contador=0;
		   }
		break;

		case STATE_ESTACION:
         printf("Has llegado a la estacion...\n");
         contador++;
		    if(contador >=30){
             delay(2000);
		   		fsmTrainState=STATE_DETENIDO;
		   		contador=0;
		   }
      break;

		default:
         printf("Default\n");
			fsmTrainInit();
		break;
	}
}
