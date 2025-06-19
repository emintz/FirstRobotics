/*
 * BaseTime.h
 *
 *  Created on: May 18, 2025
 *      Author: Eric Mintz
 *
 * Base class for time display. Provides utility functions.
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

  /*
   * Turn off every pixel in the panel
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * panel                The LED panel to clear
   */
  static void clear(LedPanel& panel);

  /*
   * Display a digit on the screen.
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * digit                Decimal digit to display, [0 .. 9]
   * start_column         Starting column in [0 .. 31]
   * foreground           RGB display color
   * panel                Display on this panel
   */
  static void display_digit(
      int digit,
      int start_column,
      const DisplayCommand::Color &foreground,
      LedPanel& panel);

  /*
   * Display a colon at the specified column
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * start_column         Starting column in [0 .. 31]
   * foreground           RGB display color
   * panel                Display on this panel
   */
  static void display_colon(
      int start_column,
      DisplayCommand::Color foreground,
      LedPanel& panel);

  /*
   * Display the current time
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * command              Contains the time to display and how it should
   *                      be displayed.
   * panel                Display on this panel
   */
  static void show_time(
      const DisplayCommand& command,
      LedPanel& panel);

  /*
   * Wait for the specified time if the the display is running. Do
   * nothing if the display is not running.
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * running              The current running status, true if the display
   *                      is running, false otherwise.
   * queue                Command queue used to wait and to sense the
   *                      running status.
   * milliseconds         The number of milliseconds to wait.
   *
   * Returns: the running status, as follows:
   *
   *          If running is false, the panel is not running. Return
   *          false.
   *
   *          If running is true, wait by peeking the queue. If a
   *          message arrives before the peek request times out,
   *          return false. Otherwise, return true.
   */
  static bool wait(
      bool running,
      PullQueueHT<DisplayCommand>& queue,
      int milliseconds);
};

#endif /* BASETIME_H_ */
