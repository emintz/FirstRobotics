/*
 * CanBusEventHandlers.cpp
 *
 *  Created on: Jul 2, 2025
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

#include "CanBusEventHandlers.h"

#include "CanAlertHandler.h"
#include "CanBus.h"
#include "PracticeFieldTimer.h"
#include "StatusLcd.h"

#include "driver/twai.h"

class OnBusRecovered : public CanAlertHandler {

  StatusLcd& status_display;
public:
  inline OnBusRecovered(StatusLcd& status_display) :
      status_display(status_display) {
  }

  virtual void operator() (CanBus& can_bus, uint32_t alerts) {
    status_display.can_bus_status("Starting");
    if (ESP_OK == can_bus.api().start()) {
      status_display.can_bus_status("Running");
    } else {
      status_display.can_bus_status("Failed Start");
    }
  }
};

CanBusEventHandlers::CanBusEventHandlers() {

}

CanBusEventHandlers::~CanBusEventHandlers() {
}

