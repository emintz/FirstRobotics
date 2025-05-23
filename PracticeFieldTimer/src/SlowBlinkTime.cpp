/*
 * ShowBlinkTime.cpp
 *
 *  Created on: May 18, 2025
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

#include "SlowBlinkTime.h"

#include "LedPanel.h"

SlowBlinkTime::SlowBlinkTime() {
}

SlowBlinkTime::~SlowBlinkTime() {
}

void SlowBlinkTime::operator() (
      PullQueueHT<DisplayCommand> &command_queue,
      const DisplayCommand &command,
      LedPanel &panel) {
  bool running = true;
  show_time(command, panel);
  if (running = wait(running, command_queue, 450)) {
    clear(panel);
  }
  if (running = wait(running, command_queue, 100)) {
    show_time(command, panel);
  }
}
