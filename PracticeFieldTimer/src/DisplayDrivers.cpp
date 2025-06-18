/*
 * DisplayDrivers.cpp
 *
 *  Created on: May 15, 2025
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

#include "DisplayDrivers.h"

#include "BlinkErrorCodes.h"
#include "ErrorHalt.h"
#include "FloodPanel.h"
#include "FastBlinkTime.h"
#include "PlainTime.h"
#include "SlowBlinkTime.h"
#include "TestPattern.h"

#include <memory>

DisplayDrivers::DisplayDrivers() :
    driver_map() {
  driver_map[DisplayCommand::Pattern::TEST_PATTERN] = std::make_shared<TestPattern>();
  driver_map[DisplayCommand::Pattern::PLAIN_TIME] = std::make_shared<PlainTime>();
  driver_map[DisplayCommand::Pattern::SLOW_BLINK_TIME] = std::make_shared<SlowBlinkTime>();
  driver_map[DisplayCommand::Pattern::FAST_BLINK_TIME] = std::make_shared<FastBlinkTime>();
  driver_map[DisplayCommand::Pattern::FLOOD] = std::make_shared<FloodPanel>();
}

DisplayDrivers::~DisplayDrivers() {
}

std::shared_ptr<DisplayDriver> DisplayDrivers::operator[](const DisplayCommand::Pattern pattern) {
  if (driver_map.end() == driver_map.find(pattern)) {
    ErrorHalt::halt_and_catch_fire(
        UNKNOWN_PANEL_DRIVER,
        "Driver not found.");
  }
  return driver_map[pattern];
}

DisplayDrivers DisplayDrivers::drivers;
