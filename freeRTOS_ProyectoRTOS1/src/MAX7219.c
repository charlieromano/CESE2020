//max7219

#include "MAX7219.h"
#include "sapi.h"


void maxAll(uint8_t reg_addr, uint8_t data){
   
   gpioMap_t cs = GPIO0;

   gpioWrite(cs, OFF);
   spiWrite(SPI0, &reg_addr, 1);
   gpioWrite(cs, OFF);
   gpioWrite(cs, ON);

   gpioWrite(cs, OFF);
   spiWrite(SPI0, &data, 1);
   gpioWrite(cs, OFF);
   gpioWrite(cs, ON);
   
}

void max7219Init( void ){

    maxAll(max7219_reg_scanLimit, 0x07);
    maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    maxAll(max7219_reg_displayTest, 0x01); // no display test
    maxClear();
    maxAll(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set

}

void maxClear( void ){

    for (uint16_t e=1; e<=8; e++) {    // empty registers, turn all LEDs off 
        maxAll(e,0);
    }
}