/**
 * LcdMessage.h
 *
 *   Created on:  Nov 24, 2026
 *      Author: Eric Mintz
 *
 * Queue that carries LCD-related commands
*
 * Copyright (C) 2026 Eric Mintz
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

#ifndef PRACTICEFIELDTIMER_LCDMESSAGE_H
#define PRACTICEFIELDTIMER_LCDMESSAGE_H

#include <Arduino.h>

struct LcdMessage {
  enum class Command {
    CLEAR_SCREEN = 0,  // Clear the LCE
    CLEAR_FIELD = 1,   // Clear a field
    WRITE_FIELD = 2,   // Clear a field and write to it.
  };

  Command command;
  uint8_t row;
  uint8_t column;
  uint8_t size;
  char contents[22]{};

  LcdMessage() {
    command = Command::CLEAR_SCREEN;
    row = 0;
    column = 0;
    size = 0;
    memset(contents, 0, sizeof(contents));
  }
};

#endif //PRACTICEFIELDTIMER_LCDMESSAGE_H
