/*
 * LedPanel.h
 *
 *  Created on: May 14, 2025
 *      Author: Eric Mintz
 *
 * Implementation-independent LED panel management API. An LED panel
 * is an arbitrary array of individually addressable RGB LEDs.
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

#ifndef LEDPANEL_H_
#define LEDPANEL_H_

/*
 * Base LED panel API.
 *
 * Note: LED panels are an LED string bent into a zig-zag pattern.
 * Implementations must translate logical (row, column) coordinates
 * into their corresponding positions in the LED string.
 *
 * The logical layout is:
 *
 *   ^
 *   |
 *   |
 *
 *   Y+
 *   ^
 * 7 |
 * 6 |
 * 5 |
 * 4 |
 * 3 |
 * 2 |
 * 1 |
 * 0 |
 *   +--------------------------------> X + -->
 *              1111111111222222222233
 *    01234567890123456789012345678901
 *
 */
class LedPanel {
protected:
  LedPanel();
public:
  virtual ~LedPanel();

  /*
   * Returns: the number of rows in the panel
   */
  virtual int rows(void) const = 0;

  /*
   * Returns: the number of columns in the panel
   */
  virtual int columns(void) const = 0;

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
  virtual LedPanel& set(int row, int column, int red, int green, int blue) = 0;

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
  virtual LedPanel& flood(int red, int green, int blue) = 0;

  /*
   * Update the panel so that it displays all changes.
   */
  virtual void update(void) = 0;

  /*
   * Turn every LED in the array off. Equivalent to flood(0, 0, 0).
   */
  virtual LedPanel& clear(void) {
    return flood(0, 0, 0);
  }
};

#endif /* LEDPANEL_H_ */
