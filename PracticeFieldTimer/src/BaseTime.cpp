/*
 * BaseTime.cpp
 *
 *  Created on: May 18, 2025
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

#include "BaseTime.h"

#include "Font.h"
#include "LedPanel.h"
#include "PinAssignments.h"

void BaseTime::clear(LedPanel& panel) {
  digitalWrite(RED_LED_PIN, LOW);
  panel.clear().update();
}

void BaseTime::display_digit(
    int digit,
    int start_column,
    const DisplayCommand::Color &foreground,
    LedPanel& panel) {
  for (int column = 0; column < 5; ++column) {
    int display_column = start_column + column;
    for (int display_row = 0; display_row < 8; ++display_row) {
      if (Font::digit_bit_at(digit, display_row, column)) {
        panel.set(
            display_row,
            display_column,
            foreground.red,
            foreground.green,
            foreground.blue);
      }
    }
  }
}

void BaseTime::display_colon(
    int start_column,
    DisplayCommand::Color foreground,
    LedPanel& panel) {
  for (int column = 0; column < 5; ++column) {
    int display_column = start_column + column;
    for (int display_row = 0; display_row < 8; ++display_row) {
      if (Font::colon_bit_at(display_row, column)) {
        panel.set(
            display_row,
            display_column,
            foreground.red,
            foreground.green,
            foreground.blue);
      }
    }
  }
}

void BaseTime::show_time(
      const DisplayCommand& command,
      LedPanel& panel) {
  digitalWrite(RED_LED_PIN, HIGH);
  panel.clear();

  int minutes = command.time_in_seconds / 60;
  int minutes_tens = minutes / 10;
  int minutes_units = minutes % 10;
  int seconds = command.time_in_seconds % 60;
  int seconds_tens = seconds / 10;
  int seconds_units = seconds % 10;

  display_digit(
      minutes_tens,
      1,
      command.foreground,
      panel);
  display_digit(
      minutes_units,
      7,
      command.foreground,
      panel);
  display_colon(
      13,
      command.foreground,
      panel);
  display_digit(
      seconds_tens,
      19,
      command.foreground,
      panel);
  display_digit(
      seconds_units,
      25,
      command.foreground,
      panel);
  panel.update();
}

bool BaseTime::wait(
    bool running,
    PullQueueHT<DisplayCommand>& queue,
    int milliseconds) {
  if (running) {
    DisplayCommand dummy_command;
    running = !queue.peek_message(&dummy_command, milliseconds);
  }
  return running;
}
