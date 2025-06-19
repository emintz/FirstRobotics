/*
 * CANDisplayCommandPublisher.cpp
 *
 *  Created on: Jun 9, 2025
 *      Author: Eric Mintz
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

#include "CANDisplayCommandPublisher.h"

#include "Arduino.h"
#include "CanBus.h"
#include "CanBusMaps.h"
#include "CanPayload.h"
#include "CanEnumerations.h"
#include "DisplayCommand.h"
#include "PinAssignments.h"
#include "TimeChangePayload.h"

CANDisplayCommandPublisher::CANDisplayCommandPublisher(
    CanBus& can_bus) :
    can_bus(can_bus) {
}

void CANDisplayCommandPublisher::operator()(const DisplayCommand& command) {
  CanPayload time_display_message;
  TimeChangePayload time_change;
  time_change.seconds_since_midnight = command.time_in_seconds;
  time_change.red = command.foreground.red;
  time_change.green = command.foreground.green;
  time_change.blue = command.foreground.blue;

  time_display_message.set_id(static_cast<int>(command.command))
      .set_data(time_change);
  CanBusOpStatus send_status = can_bus.transmit(time_display_message, 2);
  if (CanBusOpStatus::SUCCEEDED == send_status) {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
  } else {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
  }
}
