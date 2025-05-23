/*
 * PlainTime.cpp
 *
 *  Created on: May 17, 2025
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

#include "PlainTime.h"

#include "DisplayCommand.h"
#include "FastLED.h"
#include "Font.h"
#include "LedPanel.h"
#include "PinAssignments.h"
#include "PullQueueHT.h"

PlainTime::PlainTime() {
}

PlainTime::~PlainTime() {
}

void PlainTime::operator ()(
    PullQueueHT<DisplayCommand> &command_queue,
    const DisplayCommand &command,
    LedPanel &panel) {
  show_time(command, panel);

  DisplayCommand dummy_buffer;
  while (!command_queue.peek_message(&dummy_buffer)) {}
}
