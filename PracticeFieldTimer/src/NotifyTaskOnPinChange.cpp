/*
 * NotifyTaskOnPinChange.cpp
 *
 *  Created on: Jun 17, 2025
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

#include "NotifyTaskOnPinChange.h"

NotifyTaskOnPinChange::NotifyTaskOnPinChange(
    uint8_t pin,
    GpioChangeType change_type,
    const char *task_name,
    UBaseType_t task_priority,
    TaskAction& on_change) :
        task_to_notify(
            task_name,
            task_priority,
            &on_change,
            4096),
        notifier_function(
            task_to_notify),
        change_detector(
            pin,
            change_type,
            &notifier_function) {
}

NotifyTaskOnPinChange::~NotifyTaskOnPinChange() {
}

bool NotifyTaskOnPinChange::start(void) {
  return task_to_notify.start()
      && change_detector.start();
}
