/*=============================================================================
 * Copyright (c) 2020, Carlos Germán Carreño Romano <charlieromano@gmail.com>
 * All rights reserved.
 * License: gpl-3.0 (see LICENSE.txt)
 * Date: 2020/06/07
 * Version: 1.0
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __MAX7219_H__
#define __MAX7219_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include <stdint.h>
#include <stddef.h>

/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

const uint8_t max7219_reg_noop        = 0x00;
const uint8_t max7219_reg_digit0      = 0x01;
const uint8_t max7219_reg_digit1      = 0x02;
const uint8_t max7219_reg_digit2      = 0x03;
const uint8_t max7219_reg_digit3      = 0x04;
const uint8_t max7219_reg_digit4      = 0x05;
const uint8_t max7219_reg_digit5      = 0x06;
const uint8_t max7219_reg_digit6      = 0x07;
const uint8_t max7219_reg_digit7      = 0x08;
const uint8_t max7219_reg_decodeMode  = 0x09;
const uint8_t max7219_reg_intensity   = 0x0a;
const uint8_t max7219_reg_scanLimit   = 0x0b;
const uint8_t max7219_reg_shutdown    = 0x0c;
const uint8_t max7219_reg_displayTest = 0x0f;

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __MAX7219_H__ */
