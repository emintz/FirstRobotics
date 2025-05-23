/*
 * NotifyFromISR.cpp
 *
 *  Created on: May 20, 2025
 *      Author: Eric Mintz
 *
 * A count down that runs continuously. The countdown start is pinned
 * to a start time that is specified at construction. All times
 * *should* be in UTC.
 *
 * Main program for the FIRST Robotics competition practice field.
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

#include "NotifyFromISR.h"

#include "BaseTaskWithAction.h"

NotifyFromISR::NotifyFromISR(BaseTaskWithAction *task) :
    task(task) {
}

NotifyFromISR::~NotifyFromISR() {
}

void NotifyFromISR::apply(void) {
  task->notify_from_isr();
}
