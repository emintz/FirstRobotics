/*
 * Font.cpp
 *
 *  Created on: May 2, 2025
 *      Author: Eric Mintz
 *
 * Character bitmaps
 *
 * Copyright (C) 2025 Eric Mintz
 * All Rights Reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Arduino.h"

#include "Font.h"

// The font width. The pixels in column 5 are always
// off, so there's no need to represent them in the
// font.
#define FONT_WIDTH 5

static const uint8_t digits[][FONT_WIDTH] = {
    {  // 0
        0b01111110,
        0b10000001,
        0b10010001,
        0b10000001,
        0b01111110,
    },
    {  // 1
        0b00000000,
        0b01000001,
        0b11111111,
        0b00000001,
        0b00000000,
    },
    {  // 2
        0b01000011,
        0b10000101,
        0b10001001,
        0b10010001,
        0b01100001,
    },
    {  // 3
        0b01000010,
        0b10000001,
        0b10010001,
        0b10101001,
        0b01000110,
    },
    {  // 4
        0b00111000,
        0b01001000,
        0b10001000,
        0b11111111,
        0b00001000,
    },
    {  // 5
        0b11100010,
        0b10010001,
        0b10010001,
        0b10010001,
        0b10001110,
    },
    {  // 6
        0b01111110,
        0b10010001,
        0b10010001,
        0b10010001,
        0b01001110,
    },
    {  // 7
        0b10000000,
        0b10000111,
        0b10001000,
        0b10010000,
        0b11100000,
    },
    {  // 8
        0b01101110,
        0b10010001,
        0b10010001,
        0b10010001,
        0b01101110,
    },
    {  // 9
        0b01100010,
        0b10010001,
        0b10010001,
        0b10010001,
        0b01111110,
    },
};

static const uint8_t colon[FONT_WIDTH] = {
    0b00000000,
    0b00000000,
    0b00101000,
    0b00000000,
    0b00000000,
};

static uint8_t row_mask(uint8_t row_no) {
  return 0b00000001 << row_no;
}

bool Font::digit_bit_at(uint8_t digit, uint8_t row, uint8_t column) {
  return digits[digit][column] & row_mask(row);
}

bool Font::colon_bit_at(uint8_t row, uint8_t column) {
  return colon[column] & row_mask(row);
}
