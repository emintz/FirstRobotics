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
    _can_bus(can_bus),
    _rj45_blink_queue(rj45_blink_queue),
    _status_display(status_display) {
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
  CanBusOpStatus send_status = _can_bus.transmit(time_display_message, 2);
  // This is a hack. For some reason, we need to let the CAN bus settle
  // before we can display its status on the LCD. Danged if I know why,
  // but there it is.
  const char *can_status_message = "";
  vTaskDelay(pdMS_TO_TICKS(15));
  if (CanBusOpStatus::SUCCEEDED == send_status) {
    _rj45_blink_queue.send_message(&blink_rj45_green, 1);
    can_status_message = "OK";
  } else {
    digitalWrite(RED_LED_PIN, HIGH);
    can_status_message = "FAILED";
    status = false;
  }
  _status_display.can_bus_status(can_status_message)
;  return status;
}
