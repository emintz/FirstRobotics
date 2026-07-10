/*
 * ShowNoTimeRemaining.cpp
 *
 *  Created on: Jul 5, 2026
 *      Author: Eric Mintz
 *
 * Copyright 2026 Metuchen Momentum, LLC.
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

#include "ShowNoTimeRemaining.h"

#include <Arduino.h>


void ShowNoTimeRemaining::apply(void) {
  DisplayCommand command;
  memset(&command, 0, sizeof(DisplayCommand));
  command.command = DisplayCommand::Pattern::PLAIN_TIME;
  command.foreground.red = 63;
  _command_queue.send_message(&command);
  // TODO: deprecated -- remove.
  if (_message) {
    Serial.println(_message);
  }
}
