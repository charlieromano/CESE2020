//#include "SPI.h"
#include "sapi.h"
//#include "sapi_spi.h"
#include "stdint.h"

void productoEscalar32 (volatile uint32_t * vectorIn, volatile uint32_t * vectorOut, volatile uint32_t longitud, volatile uint32_t escalar);


int main( void )
{
   boardInit();

   volatile uint32_t vector_in[2]={1,2};//[1]	= {10};
   volatile uint32_t vector_out[2]={1,1};// = {0};
   volatile uint32_t longitud = 17;
   volatile uint32_t escalar =  30;
   
   for (uint32_t i=0; i<longitud; i++){
	   vector_out[i] = escalar*vector_in[i];
   }

//   productoEscalar32(vector_in,vector_out,longitud,escalar);//comment

   while( 1 ) {
	   gpioWrite(LED1, ON);
   }

   return 0;
}


void productoEscalar32 (volatile uint32_t * vectorIn, volatile uint32_t * vectorOut, volatile uint32_t longitud, volatile uint32_t escalar)
{
  for (uint8_t i=0; i<longitud; i++)
	  vectorOut[i] = escalar*vectorIn[i];

}
