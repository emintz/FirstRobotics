/*
 * CountDownTimer.h
 *
 *  Created on: May 19, 2025
 *      Author: Eric Mintz
 *
 * Manages a fixed duration count down life cycle. The cycle length is
 * set during creation, and can be overridden for the first cycle that
 * runs. This allows the start of free-running cycles to be synchronized
 * with a specified start time.
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

#ifndef COUNTDOWNTIMER_H_
#define COUNTDOWNTIMER_H_

#include "Arduino.h"

#include "DisplayCommand.h"
#include "PullQueueHT.h"
#include "TaskAction.h"

class VoidFunction;

class CountDownTimer : public TaskAction {
  const int16_t duration_in_seconds;
  VoidFunction& completed;
  PullQueueHT<DisplayCommand>& command_queue;

  int16_t seconds_remaining;
  const int16_t color_change_halfway_mark;
  const int16_t fast_blink_start;
  const int16_t slow_blink_start;

  void show_blink_time(void);

  void show_plain_time(void);

public:
  CountDownTimer(
      int16_t duration_in_seconds,
      int16_t end_phase_seconds,
      VoidFunction &completed,
      PullQueueHT<DisplayCommand>& command_queue);
  virtual ~CountDownTimer();

  virtual void run(void) override;

  void set_remaining(uint16_t seconds_remaining) {
    this->seconds_remaining = seconds_remaining;
  }
};

#endif /* COUNTDOWNTIMER_H_ */
