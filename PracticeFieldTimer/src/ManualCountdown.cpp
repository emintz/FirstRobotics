/*
 * ManualCountdown.cpp
 *
 *  Created on: Jul 6, 2026
 *      Author: Eric Mintz
 *
 * Copyright 2026 Metuchen Momentum LLC.
 * All Rights Reserved.
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

#include "ManualCountdown.h"

ManualCountdown::ManualCountdown(
    uint8_t start_pin,
    Countdown& countdown,
    PullQueueHT<DisplayCommand>& display_command_queue,
    uint16_t duration_in_seconds) :
      _start_countdown_function(
          countdown,
          display_command_queue,
          duration_in_seconds),
      _countdown_start_detector(
          start_pin,
          GpioChangeType::HIGH_TO_LOW,
          &_start_countdown_function) {

}

bool ManualCountdown::start(void) {
  return _countdown_start_detector.start();
}

