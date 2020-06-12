/*****************************************************************************
 ** SPI.c
 * Copyright (c) 2020, Carlos Germán Carreño Romano <charlieromano@gmail.com>
 * All rights reserved.
 * License: gpl-3.0 (see LICENSE.txt)
 * Date: 2020/06/07
 * Version: 1.0
 ****************************************************************************/

#include "SPI.h"
#include "sapi.h"
#include "chip.h"

#define SPI_SCK 120
#define SPI_SCLK_LOW_TIME   200
#define SPI_SCLK_HIGH_TIME   150
#define DEC    10

char* itoa(int value, char* result, int base);

LPC_SPI_T * tSPI;
SPI_CONFIG_FORMAT_T * tConfig;

uint8_t max7219_reg_noop        = 0x00;
uint8_t max7219_reg_digit0      = 0x01;
uint8_t max7219_reg_digit1      = 0x02;
uint8_t max7219_reg_digit2      = 0x03;
uint8_t max7219_reg_digit3      = 0x04;
uint8_t max7219_reg_digit4      = 0x05;
uint8_t max7219_reg_digit5      = 0x06;
uint8_t max7219_reg_digit6      = 0x07;
uint8_t max7219_reg_digit7      = 0x08;
uint8_t max7219_reg_decodeMode  = 0x09;
uint8_t max7219_reg_intensity   = 0x0a;
uint8_t max7219_reg_scanLimit   = 0x0b;
uint8_t max7219_reg_shutdown    = 0x0c;
uint8_t max7219_reg_displayTest = 0x0f;

int maxInUse = 1; //cantidad de drivers MAX7219
/*
void maxAll (uint8_t reg, uint8_t col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  gpioWrite(GPI0, OFF);  // begin     
  for ( c =1; c<= maxInUse; c++) {
  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
    }
  gpioWrite(GPI0, OFF);
  gpioWrite(GPI0,ON);
}

void putByte(uint8_t data) {
  uint8_t i = 8;
  uint8_t mask;
  while(i > 0) {
    mask = 0x01 << (i - 1);      // get bitmask
    digitalWrite( clock, LOW);   // tick
    if (data & mask){            // choose bit
      digitalWrite(dataIn, HIGH);// send 1
    }else{
      digitalWrite(dataIn, LOW); // send 0
    }
    digitalWrite(clock, HIGH);   // tock
    --i;                         // move to lesser bit
  }
}

*/
int main( void )
{
   
   spiInit(SPI0);

   uint8_t* buffer=NULL;
   uint32_t bufferSize = 10;
/*
//initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);      
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
   for (e=1; e<=8; e++) {    // empty registers, turn all LEDs off 
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set
                                                  // range: 0x00 to 0x0f
*/
   /*while(1) {
      spiRead(SPI0, buffer, bufferSize);
   }*/

   /* Init */
   boardConfig();
   //uartConfig( UART_USB, 115200 );
   adcConfig( ADC_ENABLE ); /* ADC */


   /* Data */
   static char uartBuff_X[10];
   static char uartBuff_Y[10];
   uint16_t sample_X = 0;
   uint16_t sample_Y = 0;
   uint32_t i = 0;

   /* Time */
   delay_t delay1;
   delay_t delay2;
   delayConfig( &delay1, 1 );

   while(1) {

      if (delayRead( &delay1 )){

         sample_X = adcRead( CH1 );
         //sample_Y = adcRead( CH2 );

         /* Conversión de sample_X entera a ascii con base decimal */
/*         itoa( sample_X, uartBuff_X, DEC ); 
         itoa( sample_Y, uartBuff_Y, DEC ); 
*/
         /* Enviar sample_X y Enter */
/*         uartWriteString( UART_USB, "(x,y) = [ " );
         uartWriteString( UART_USB, uartBuff_X );
         uartWriteString( UART_USB, ", " );
         uartWriteString( UART_USB, uartBuff_Y );
         uartWriteString( UART_USB, "];\r\n" );     
*/
         //gpioWrite(GPIO0, ON);
         //uint16_t spiBuff[1] = {sample_X};//, sample_Y};
/*
         
         spiBuff[1] = 0b1111110000000000;

         uint16_t spiBuff[1];
         spiBuff[0] = 0b000011110000001;
*/
//         for(int i=0; i<4; i++){
            //spiBuff[0] = spiBuff[0]<<i;
         uint16_t spiBuff[1];
          spiBuff[0]= 0x3D;

            spiWrite(SPI0, spiBuff, 2);
/*          spiWrite(SPI0, spiBuff, 2);
            spiWrite(SPI0, spiBuff, 2);
            spiWrite(SPI0, spiBuff, 2);
*/         
      }

   }

   return 0 ;
}
/* SAPI 

bool_t spiInit( spiMap_t spi );

bool_t spiRead( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize );

bool_t spiWrite( spiMap_t spi, uint8_t* buffer, uint32_t bufferSize);
*/


/* Wiki
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
/**
 * @brief   Send SPI 16-bit data
 * @param   pSPI  : The base of SPI peripheral on the chip
 * @param   data  : Transmit Data
 * @return  Nothing
 */
/*STATIC INLINE void Chip_SPI_SendFrame(LPC_SPI_T *pSPI, uint16_t data)
{
   pSPI->DR = SPI_DR_DATA(data);
}
*/



/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.

 */

char* itoa(int value, char* result, int base) {
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}


