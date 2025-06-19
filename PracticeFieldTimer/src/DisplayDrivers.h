/*
 * DisplayDrivers.h
 *
 *  Created on: May 15, 2025
 *      Author: Eric Mintz
 *
 * Maintains a map of display drivers (classes that drive the LED matrix)
 * and makes them available for use.
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

#ifndef DISPLAYDRIVERS_H_
#define DISPLAYDRIVERS_H_

#include "DisplayCommand.h"
#include "DisplayDriver.h"

#include <map>
#include <memory>

class DisplayDrivers {
  std::map<DisplayCommand::Pattern, std::shared_ptr<DisplayDriver>> driver_map;

  DisplayDrivers();
public:
  virtual ~DisplayDrivers();

  static DisplayDrivers drivers;

  /*
   * Returns a pointer to the display driver that displays the
   * specified pattern.
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * pattern              Display pattern (e.g. steady, unblinking remaining
   *                      time in the specified color)
   */
  std::shared_ptr<DisplayDriver> operator[](
      const DisplayCommand::Pattern pattern);
};

#endif /* DISPLAYDRIVERS_H_ */
