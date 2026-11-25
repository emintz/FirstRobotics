/**
* LcdAction.cpp
 *
 *  Created on: 11/24/26.
 *      Author: Eric Mintz
 *
 * Action that performs low-level LCD operations over I2C
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

#include "LcdAction.h"
#include <LiquidCrystal_I2C.h>

#define PRINT_DELAY_MS 2
#define CURSOR_MOVE_DELAY 5

void LcdAction::clear_display() const {
  lcd_.clear();
}

void LcdAction::clear_field(const LcdMessage& message) const {
  char blanks[22] = {};
  memset(blanks, ' ', message.size);
  set_cursor(message);
  print(blanks);
}

void LcdAction::print(const char* contents) const {
  lcd_.print(contents);
  vTaskDelay(pdMS_TO_TICKS(PRINT_DELAY_MS));
}

void LcdAction::set_cursor(const LcdMessage& message) const {
  lcd_.setCursor(message.column, message.row);
  vTaskDelay(pdMS_TO_TICKS(CURSOR_MOVE_DELAY));
}

void LcdAction::write_field(const LcdMessage& message) const {
  clear_field(message);
  set_cursor(message);
  print(message.contents);

}

LcdAction::LcdAction(
  PullQueueHT<LcdMessage>& queue,
  LiquidCrystal_I2C& lcd) :
  queue_(queue),
    lcd_(lcd) {
}

void LcdAction::run() {
  LcdMessage message;
  memset(&message, 0, sizeof(LcdMessage));
  for (;;) {
    if (queue_.pull_message(&message, 10000000)) {
      switch (message.command)
      {
      case LcdMessage::Command::CLEAR_SCREEN:
        clear_display();
        break;
      case LcdMessage::Command::CLEAR_FIELD:
        clear_field(message);
        break;
      case LcdMessage::Command::WRITE_FIELD:
        write_field(message);
        break;
      }
    }
  }
}
