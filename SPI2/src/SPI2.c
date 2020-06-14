/*****************************************************************************
 ** SPI.c
 * Copyright (c) 2020, Carlos Germán Carreño Romano <charlieromano@gmail.com>
 * All rights reserved.
 * License: gpl-3.0 (see LICENSE.txt)
 * Date: 2020/06/07
 * Version: 1.0
 ****************************************************************************/

#include "SPI2.h"
#include "sapi.h"
#include "chip.h"
#include "MAX7219.h"
#define DEC 10

char* itoa(int value, char* result, int base);
void maxAll(uint16_t reg_addr, uint16_t data);
void putByte(uint8_t data) ;

int main( void )
{


   boardInit();
   uartConfig( UART_USB, 115200 ); /* UART */
   adcConfig( ADC_ENABLE ); /* ADC */
   gpioWrite(GPIO0, ON);
   gpioWrite(GPIO3, OFF);
   gpioWrite(GPIO2, ON);
  
  //initiation of the max 7219
  maxAll(max7219_reg_scanLimit, 0x07);      
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x01); // no display test
  for (uint16_t e=1; e<=8; e++) {    // empty registers, turn all LEDs off 
    maxAll(e,0);
  }
  maxAll(max7219_reg_intensity, 0x02 & 0x0f);    // the first 0x0f is the value you can set
                                                  // range: 0x00 to 0x0f

   /* Data */
   static char uartBuff_X[10];
   static char uartBuff_Y[10];
   uint16_t sample_X = 0;
   uint16_t sample_Y = 0;

   /* Time */
   delay_t delay1;
   delayConfig( &delay1, 100 );
   
   while(1) {

      if (delayRead( &delay1 )){

         sample_X = adcRead( CH1 );
         sample_Y = adcRead( CH2 );

         /* Conversión de sample_X entera a ascii con base decimal */
         itoa( sample_X, uartBuff_X, DEC ); 
         itoa( sample_Y, uartBuff_Y, DEC ); 

         /* Enviar sample_X y Enter */
         uartWriteString( UART_USB, "(x,y) = [ " );
         uartWriteString( UART_USB, uartBuff_X );
         uartWriteString( UART_USB, ", " );
         uartWriteString( UART_USB, uartBuff_Y );
         uartWriteString( UART_USB, "];\r\n" );     

         maxAll(2,1);   
     }

   }

   return 0 ;
}
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

void maxAll(uint16_t reg_addr, uint16_t data){
   
   gpioMap_t cs = GPIO0;
   gpioWrite(cs, OFF);
   putByte(reg_addr);
   putByte(data);
   gpioWrite(cs, OFF);
   gpioWrite(cs, ON);

}

void putByte(uint8_t data) {

  gpioMap_t clock = GPIO3;
  gpioMap_t dataIn= GPIO2;
  uint8_t i = 8;
  uint8_t mask;

  while(i > 0) {
    mask = 0x01 << (i - 1);      // get bitmask
    gpioWrite(clock, LOW);   // tick
    if (data & mask){            // choose bit
      gpioWrite(dataIn, ON);// send 1
    }else{
      gpioWrite(dataIn, OFF); // send 0
    }
    gpioWrite(clock, ON);   // tock
    --i;                         // move to lesser bit
  }
}
//Marilina's code
//lñop''''''''''''l,k.9¿8h