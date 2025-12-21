/*
 * TextDisplay.cpp
 *
 *  Created on: Dec 21, 2025
 *      Author: Eric Mintz
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

#include "TextDisplay.h"
#include "LedPanel.h"

void TextDisplay::operator () (
    const uint8_t *bitmap,
    const DisplayCommand::Color& foreground_color,
    LedPanel& panel) {
  panel.flood(0, 0, 0);
  for (int column = 0; column < 32; ++column) {
    uint8_t current_char = *bitmap++;
    uint8_t mask = 1;
    for (int row = 0; row < 8; ++row) {
      if (current_char & mask) {
        panel.set(
            row,
            column,
            foreground_color.red,
            foreground_color.green,
            foreground_color.blue);
      }
      mask <<= 1;
    }
    panel.update();
  }
}
