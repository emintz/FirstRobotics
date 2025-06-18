/*
 * TimeChangeHandler.cpp
 *
 *  Created on: Jun 9, 2025
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

#include "TimeChangeHandler.h"

#include "CanBus.h"
#include "CanPayload.h"
#include "PinAssignments.h"
#include "TimeChangePayload.h"

TimeChangeHandler::TimeChangeHandler(
    PullQueueHT<DisplayCommand>& panel_command_queue) :
    panel_command_queue(panel_command_queue) {
}

TimeChangeHandler::~TimeChangeHandler() {
}

void TimeChangeHandler::operator()(
    CanBus& bus, CanPayload& payload) {
  TimeChangePayload time_change_payload;
  memset(&time_change_payload, 0, sizeof(time_change_payload));
  payload.copy_payload_to(
      &time_change_payload);
  DisplayCommand send_to_panel;
  memset(&send_to_panel, 0, sizeof(send_to_panel));
  send_to_panel.command = static_cast<DisplayCommand::Pattern>(
      payload.message_id());
  send_to_panel.time_in_seconds = time_change_payload.seconds_since_midnight;
  send_to_panel.foreground.red = time_change_payload.red;
  send_to_panel.foreground.green = time_change_payload.green;
  send_to_panel.foreground.blue = time_change_payload.blue;
  if (!panel_command_queue.send_message(&send_to_panel)) {
    digitalWrite(RED_LED_PIN, HIGH);
  }
}
