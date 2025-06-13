/*
 * StartCountdownOnPinLow.cpp
 *
 *  Created on: Jun 13, 2025
 *      Author: Eric Mintz
 *
 * A VoidFunction which starts a countdown when a specified pin
 * goes LOW. This is meant to be the target of a GPIO debouncer.
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

#include "StartCountdownOnPinLow.h"

#include "BaseCountdown.h"

StartCountdownOnPinLow::StartCountdownOnPinLow(
    uint8_t gpio,
    BaseCountdown& countdown) :
        gpio(gpio),
        countdown(countdown) {

}

StartCountdownOnPinLow::~StartCountdownOnPinLow() {
}

void StartCountdownOnPinLow::apply(void) {
  if (digitalRead(gpio) == LOW) {
    countdown.enable();
  }
}
