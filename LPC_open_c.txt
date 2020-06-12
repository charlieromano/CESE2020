/*=============================================================================
 * Copyright (c) 2020, Carlos Germán Carreño Romano <charlieromano@gmail.com>
 * All rights reserved.
 * License: gpl-3.0 (see LICENSE.txt)
 * Date: 2020/05/31
 *===========================================================================*/

#include "LPC_open.h"
#include "sapi.h"
#include "chip.h"

#define SPI_SCK 120
#define SPI_SCLK_LOW_TIME   200
#define SPI_SCLK_HIGH_TIME   150

int main(void)
{
   boardInit();

   delay_t t1, t2;
   delayConfig(&t1, SPI_SCLK_LOW_TIME);
   spiConfig(SPI0);

   //delayConfig(&t2, SPI_SCLK_HIGH_TIME);
   uint32_t    contador=0;

   while (1)
   { 
   
      if( delayRead( &t1 ) ){
        contador++;
      }
      if( contador >= 1000){
         gpioWrite(SPI_MOSI, ON);
      }
      if( contador >= 1500){
         gpioWrite(SPI_MOSI, OFF);
      }
      if( contador >= 3500){
         contador=0;
      }
   }
}

/*
bool_t spiInit( spiMap_t spi );

bool_t spiRead( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize );

bool_t spiWrite( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize);
*/

/*
delay_t t4;
   delayConfig( &t1 , T_RED);
      if(delayRead(&t2)){
*/

//bool_t spiWrite( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize)
/*
uint8_t SPI_transfer_byte( uint8_t byte_out){
   uint8_t byte_in = 0;
   uint8_t bit;

   for (bit = 0x80; bit; bit>>1){
      write_MOSI(( byte_out & bit) ? HIGH : LOW);

      delay(SPI_SCLK_LOW_TIME);
      write_SCLK(HIGH);

      if(read_MISO()==HIGH)
         byte_in |= bit;

      delay(SPI_SCLK_HIGH_TIME);

      write_SCLK(LOW);
   }
   return byte_in;
}

void read_MISO (){
   gpioToogle(SPI_MISO, ON)
}
void write_MOSI (){
   SPI_MOSI
}



void write_SCLK (){
   spiMap_t SPI0
}
*/