/*
 * CANDisplayCommandPublisher.cpp
 *
 *  Created on: Jun 9, 2025
 *      Author: Eric Mintz
 *
 * Copyright (C) 2025 Eric Mintz
 * All Rights Reserved
 *
 * Time display manager that posts time changes to the local LCD and
 * publishes them over the CAN bus. Use this class when CAN is
 * enabled.
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
#include "CanPayload.h"
#include "CanEnumerations.h"
#include "DisplayCommand.h"
#include "PinAssignments.h"
#include "TimeChangePayload.h"

static const OneShotBlinkCommand blink_rj45_green = {
    ._on_time_ms = 50,
    ._pin_count = 2,
    ._blink_pin = {RJ45_A_GREEN, RJ45_B_GREEN},
};

CANDisplayCommandPublisher::CANDisplayCommandPublisher(
    CanBus& can_bus,
    PullQueueHT<OneShotBlinkCommand>& rj45_blink_queue,
    StatusLcd& status_display) :
        can_bus_(can_bus),
        rj45_blink_queue_(rj45_blink_queue),
        status_display_(status_display) {
}

bool CANDisplayCommandPublisher::operator()(const DisplayCommand& command) {
  bool status = true;
  CanPayload time_display_message;
  TimeChangePayload time_change;
  time_change.seconds_since_midnight = command.time_in_seconds;
  time_change.red = command.foreground.red;
  time_change.green = command.foreground.green;
  time_change.blue = command.foreground.blue;

  time_display_message.set_id(static_cast<int>(command.command))
      .set_data(time_change);
  CanBusOpStatus send_status = can_bus_.transmit(time_display_message, 2);
  const char *can_status_message = "";
  if (CanBusOpStatus::SUCCEEDED == send_status) {
    rj45_blink_queue_.send_message(&blink_rj45_green, 1);
    can_status_message = "OK";
  } else {
    digitalWrite(RED_LED_PIN, HIGH);
    can_status_message = "FAILED";
    status = false;
  }
  status_display_.can_bus_status(can_status_message)
;  return status;
}
