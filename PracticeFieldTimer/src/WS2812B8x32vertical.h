/*
 * WS2812B8x32vertical.h
 *
 *  Created on: May 14, 2025
 *      Author: Eric Mintz
 *
 * An 8 row by 32 panel array of WS2812b LEDS arranged in a vertical
 * zig-zag.
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
 *
 * The WS2812B8x32vertical class is a LedPanel that controls an 8 row by 32
 * column array of WS2812b LEDs formed from a string arranged in a vertical
 * zig-zag arranged in the following geometry..
 *
 *   ^
 *   |
 *
 *   Y+
 *   ^
 * 7 | 248 247 232 231    ...    007
 * 6 | 249 246 233 230    ...    006
 * 5 | 250 245 234 229    ...    005
 * 4 | 251 244 235 228    ...    004
 * 3 | 252 243 236 227    ...    003
 * 2 | 253 242 237 226    ...    002
 * 1 | 254 241 238 225    ...    001
 * 0 | 255 240 239 224    ...    000
 *   +------------------------------> X+ -->
 *     0    0   0   0             3
 *     0    1   2   3             1
 *
 * TODO(emintz): refactor to pass the indeer to the constructor
 *               should we need to support horizontal zig-zag.
 */

#ifndef WS2812B8X32VERTICAL_H_
#define WS2812B8X32VERTICAL_H_

#include "LedPanel.h"

#include "Arduino.h"
#include "FastLED.h"

class WS2812B8x32vertical : public LedPanel {
  class LedIndexer {
    friend WS2812B8x32vertical;

    int16_t column_offset[32];

    LedIndexer(void);

    /*
     * Translates a (row, column) pair into an offset, as explained
     * above.
     *
     * Parameters:
     * ----------
     *
     * Name                 Contents
     * -----------------    ------------------------------------------------
     * row                  Desired row in [0 .. 7]
     * column               Desired column in [0 .. 31]
     *
     * Returns: the corresponding offset.
     */
    int16_t offset_of(int row, int column) const;
  };

  static LedIndexer row_column_to_index;

  CRGB leds[256];

public:
  WS2812B8x32vertical();
  virtual ~WS2812B8x32vertical();


  /*
   * Returns: the number of rows in the panel
   */
  virtual int rows(void) const;

  /*
   * Returns: the number of columns in the panel
   */
  virtual int columns(void) const;

  virtual LedPanel& flood(int red, int green, int blue);

  /*
   * Sets the specified panel to the specified RGB color
   *
   * Parameters:
   * ----------
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * row               Panel row, 0-based as indicated above
   * column            Panel column, 0-based as indicated above
   * red               Red brightness, typically in [0 .. 255]
   * green             Green brightness, typically in [0 .. 255]
   * blue              Blue brightness, typically in [0 .. 255]
   */
  LedPanel& set(int row, int column, int red, int green, int blue);

  /*
   * Set every LED in the panel to the specified LED color.
   *
   * Parameters:
   * ----------
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * red               Red brightness, typically in [0 .. 255]
   * green             Green brightness, typically in [0 .. 255]
   * blue              Blue brightness, typically in [0 .. 255]
   */

  /*
   * Update the panel so that it displays all changes.
   */
  virtual void update(void);
};

#endif /* WS2812B8X32VERTICAL_H_ */
