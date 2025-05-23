/*
 * Font.h
 *
 *  Created on: May 2, 2025
 *      Author: Eric Mintz
 *
 * A limited font for the clock. Each character represents an
 * 8 row, 6 column cell. Rows are numbered 0 - 7, with 0
 * representing the top row; columns are numbered 0 - 5, with
 * 5 representing the rightmost column. Column 5's pixels
 * are always off to separate the characters.
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

#ifndef FONT_H_
#define FONT_H_

class Font {
  Font() {}
public:

  /*
   * Returns the bit for the row and column of the specified column.
   * If the bit is false, the character's pixel at (row, column)
   * should be set to the background color. If, on the other hand,
   * the bit is true, the corresponding pixel should be set to
   * the foreground color.
   *
   * Parameters:
   * ----------
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * digit             The digit to format, 0 .. 9
   * row               The pixel row relative to the character's upper
   *                   left corner. Must be between 0 and 7 inclusive,
   *                   with 0 representing the top row.
   * column            The pixel column, relative to the character's
   *                   upper left corner. Must be between 0 and 4
   *                   inclusive, with 0 representing the left hand
   *                   column. Column 5 nominally belongs to the display
   *                   cell, but since its pixels are always set to
   *                   the background, there is no need to represent
   *                   them in the table
   */
  static bool digit_bit_at(uint8_t digit, uint8_t row, uint8_t column);

  /*
   * Returns the bit for the specified row and column of the
   * colon (':') character.
   *
   * Parameters:
   * ----------
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * row               The pixel row relative to the character's upper
   *                   left corner. Must be between 0 and 7 inclusive,
   *                   with 0 representing the top row.
   * column            The pixel column, relative to the character's
   *                   upper left corner. Must be between 0 and 4
   *                   inclusive, with 0 representing the left hand
   *                   column. Column 5 nominally belongs to the display
   *                   cell, but since its pixels are always set to
   *                   the background, there is no need to represent
   *                   them in the table
   */
  static bool colon_bit_at(uint8_t row, uint8_t column);
};

#endif /* FONT_H_ */
