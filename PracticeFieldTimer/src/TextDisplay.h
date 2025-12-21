/*
 * TextDisplay.h
 *
 *  Created on: Dec 21, 2025
 *      Author: Eric Mintz
 *
 * Displays fixed text on a panel. Text is supplied in a 32 byte
 * bitmap, and the foreground color is provided as rgb with
 * each intensity in an unsigned byte.
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

#ifndef SRC_TEXTDISPLAY_H_
#define SRC_TEXTDISPLAY_H_

#include "DisplayCommand.h"

#include <Arduino.h>

class LedPanel;

class TextDisplay {
public:
  TextDisplay() = default;
  virtual ~TextDisplay() = default;

  /**
   * Display the bitmap in the specified color. The background will
   * be black (i.e. all LEDs off).
    *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * bitmap               A 32 byte bitmap. Each byte represents a panel
   *                      column. Cells having 1 bits are illuminated in
   *                      the specified color. The other cells are turned
   *                      completely off.
   * foreground_color     RGB representation of the text color.
   * panel                The LED panel display
   */
  void operator() (
      const uint8_t bitmap[32],
      const DisplayCommand::Color& foreground_color,
      LedPanel& panel);
};

#endif /* SRC_TEXTDISPLAY_H_ */
