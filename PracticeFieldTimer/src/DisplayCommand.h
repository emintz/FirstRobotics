/*
 * Command.h
 *
 *  Created on: May 15, 2025
 *      Author: Eric Mintz
 *
 * Holds a command that directs the display manager to paint
 * a specified set of information on the display panel. Note
 * that this only loosely related to CAN messages.
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

#ifndef DISPLAY_COMMAND_H_
#define DISPLAY_COMMAND_H_

#include "Arduino.h"

struct DisplayCommand {
  enum class Pattern {
    TEST_PATTERN = 1,
    PLAIN_TIME = 2,
    SLOW_BLINK_TIME = 3,
    FAST_BLINK_TIME = 4,
  };

  struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
  };

  Pattern command;
  Color foreground;
  Color background;
  int16_t time_in_seconds;
};

#endif /* DISPLAY_COMMAND_H_ */
