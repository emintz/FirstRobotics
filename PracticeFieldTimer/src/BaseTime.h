/*
 * BaseTime.h
 *
 *  Created on: May 18, 2025
 *      Author: Eric Mintz
 *
 * Base class for time display. Provides utility functions.
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

#ifndef BASETIME_H_
#define BASETIME_H_

#include "DisplayCommand.h"  // No forward declaration of member classes.
#include "DisplayDriver.h"
#include "PullQueueHT.h"


class LedPanel;

class BaseTime : public DisplayDriver {
protected:
  BaseTime() {};

  static void clear(LedPanel& panel);

  static void display_digit(
      int digit,
      int start_column,
      const DisplayCommand::Color &foreground,
      LedPanel& panel);

  static void display_colon(
      int start_column,
      DisplayCommand::Color foreground,
      LedPanel& panel);

  static void show_time(
      const DisplayCommand& command,
      LedPanel& panel);

  static bool wait(
      bool running,
      PullQueueHT<DisplayCommand>& queue,
      int milliseconds);
};

#endif /* BASETIME_H_ */
