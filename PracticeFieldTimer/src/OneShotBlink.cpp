/*
 * OneShotBlink.cpp
 *
 *  Created on: Jun 27, 2026
 *      Author: OneShotBlink
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

#include "OneShotBlink.h"

void OneShotBlink::run(void) {
  OneShotBlinkCommand command;
  for (;;) {
    memset(&command, '\0', sizeof(command));
    if (_command_queue.pull_message(&command)
        && 0 < command._on_time_ms
        && 0 < command._pin_count) {
      for (uint16_t i = 0; i < command._pin_count; ++i) {
        digitalWrite(command._blink_pin[i], HIGH);
      }
      uint32_t max_wait_ms = command._on_time_ms;
      _command_queue.peek_message(&command, max_wait_ms);
      for (uint16_t i = 0; i < command._pin_count; ++i) {
        digitalWrite(command._blink_pin[i], LOW);
      }
    }
  }
}

