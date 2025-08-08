/*
 * PanelServer.cpp
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

#include "PanelServer.h"

#include "Arduino.h"
#include "DisplayDriver.h"
#include "DisplayDrivers.h"
#include "LedPanel.h"
#include "StatusLcd.h"

#include <memory>

PanelServer::PanelServer(
    PullQueueHT<DisplayCommand> &queue,
    LedPanel &panel,
    StatusLcd& lcd) :
        queue(queue),
        panel(panel),
        lcd(lcd) {
}

PanelServer::~PanelServer() {
}

void PanelServer::run(void) {
  DisplayCommand display_command;
  bool got_one = true;
  Serial.println("Panel server running.");
  while (true) {
    do {
       got_one = queue.pull_message(&display_command);
    } while (!got_one);
    DisplayCommand::Pattern pattern =
        static_cast<DisplayCommand::Pattern>(display_command.command);
    std::shared_ptr<DisplayDriver> driver_ptr =
        DisplayDrivers::drivers[pattern];
    if (driver_ptr) {
      lcd.time(display_command.time_in_seconds);
      (*driver_ptr)(queue, display_command, panel);
    } else {
      Serial.println("Error: NULL display driver.");
      lcd.health("NULL display driver");
      Serial.flush();
    }
  }
  stop();
}
