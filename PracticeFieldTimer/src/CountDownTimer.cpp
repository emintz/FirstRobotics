/*
 * CountDownTimer.cpp
 *
 *  Created on: May 19, 2025
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
 *
 */

#include "CountDownTimer.h"

#include "VoidFunction.h"

void CountDownTimer::publish_command(DisplayCommand& display_command) {
  command_queue.send_message(&display_command);

}

void CountDownTimer::show_blink_time(void) {
  DisplayCommand display_command;
  memset(&display_command, 0, sizeof(display_command));
  display_command.command =
      fast_blink_start <= seconds_remaining
      ? DisplayCommand::Pattern::SLOW_BLINK_TIME
      : DisplayCommand::Pattern::FAST_BLINK_TIME;
  display_command.time_in_seconds = seconds_remaining;
  display_command.foreground.red = 63;
  publish_command(display_command);
 }

void CountDownTimer::show_plain_time(void) {
  DisplayCommand display_command;
  memset(&display_command, 0, sizeof(display_command));
  display_command.command = DisplayCommand::Pattern::PLAIN_TIME;
  display_command.time_in_seconds = seconds_remaining;

  int16_t color_change_time = seconds_remaining - slow_blink_start;
  if (color_change_time <= color_change_halfway_mark) {
      int green_intensity =
          (63 * color_change_time) / color_change_halfway_mark;
      display_command.foreground.green = green_intensity;
      display_command.foreground.red = 63 - display_command.foreground.green;
    } else {
      color_change_time -= color_change_halfway_mark;
      int blue_intensity =
          (63 * color_change_time) / color_change_halfway_mark;
      display_command.foreground.blue = blue_intensity;
      display_command.foreground.green = 63 - display_command.foreground.blue;
    }
  publish_command(display_command);
}

CountDownTimer::CountDownTimer(
    int16_t duration_in_seconds,
    int16_t end_phase_seconds,
    VoidFunction &completed,
    PullQueueHT<DisplayCommand>& command_queue) :
        duration_in_seconds(duration_in_seconds),
        completed(completed),
        command_queue(command_queue),
        seconds_remaining(duration_in_seconds),
        color_change_halfway_mark(
            (duration_in_seconds - end_phase_seconds) / 2),
        fast_blink_start(end_phase_seconds / 2),
        slow_blink_start(end_phase_seconds) {
}

CountDownTimer::~CountDownTimer() {
}

void CountDownTimer::run(void) {
  while (true) {
    wait_for_notification();
    if (seconds_remaining < slow_blink_start) {
      show_blink_time();
    } else {
      show_plain_time();
    }
    if (0 < seconds_remaining) {
      --seconds_remaining;
    } else {
      completed.apply();
      seconds_remaining = duration_in_seconds;
    }
  }
}

