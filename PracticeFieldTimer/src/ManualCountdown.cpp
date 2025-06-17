/*
 * ManualCountdown.cpp
 *
 *  Created on: Jun 15, 2025
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

#include "ManualCountdown.h"

#include "TaskAction.h"


ManualCountdown::ManualCountdown(
    uint8_t start_pin,
    uint8_t sqw_pin,
    int16_t duration_in_seconds,
    int16_t end_phase_seconds,
    PullQueueHT<DisplayCommand>& command_queue,
    DisplayCommandPublisher& command_publisher) :
        actual_countdown(
            sqw_pin,
            duration_in_seconds,
            end_phase_seconds,
            command_queue,
            command_publisher),
       countdown_enable(actual_countdown),
       on_pin_change(
           start_pin,
           GpioChangeType::HIGH_TO_LOW,
           "ManualCount",
           19,
           countdown_enable) {
}

ManualCountdown::~ManualCountdown() {
}

