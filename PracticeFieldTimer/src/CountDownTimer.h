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
 * The timer counts down to zero and then signals completion by invoking
 * a VoidFunction that is set during construction.
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

class DisplayCommandPublisher;
class VoidFunction;

class CountDownTimer : public TaskAction {
  const int16_t duration_in_seconds;
  VoidFunction& completed;
  PullQueueHT<DisplayCommand>& command_queue;

  int16_t seconds_remaining;
  const int16_t color_change_halfway_mark;
  const int16_t fast_blink_start;
  const int16_t slow_blink_start;
  DisplayCommandPublisher& command_publisher;

  /*
   * Publish the command to the panel service and, if the CAN bus is
   * running, to the CAN bus as well.
   */
  void publish_command(DisplayCommand& display_command);

  /*
   * Create and publish a command to display the time, blinking
   * the display to attract attention. Time is displayed in red.
   * This is only done during the countdown's end stage, which
   * is set at construction.
   */
  void show_blink_time(void);

  /*
   * Create and publish a command to display the time steadily, i.e.
   * without blinking it. Vary the display color from blue to
   * red depending on the remaining time. This is done during all
   * but the end phase of a session, which is set at construction.
   */
  void show_plain_time(void);

public:
  /*
   * Constructor
   *
   * Parameters:
   *
   * Name                Contents
   * ------------------- ----------------------------------------------------
   * duration_in_seconds Countdown length in seconds
   * end_phase_seconds   End phase (blink display) time in seconds
   * completed           A VoidFunction to be invoked when the countdown
   *                     reaches 0
   * command_queue       Queue for publishing messages to the panel service
   * command_publisher   Publishes the command to the CAN bus if the latter
   *                     is running.
   */
  CountDownTimer(
      int16_t duration_in_seconds,
      int16_t end_phase_seconds,
      VoidFunction &completed,
      PullQueueHT<DisplayCommand>& command_queue,
      DisplayCommandPublisher& command_publisher);
  virtual ~CountDownTimer();

  /*
   * Runs the command loop
   */
  virtual void run(void) override;

  /*
   * Sets the length of the current or, if the action is not
   * yet running, the next countdown. This is used to synchronize
   * a continuously running countdown with the schedule.
   */
  void set_remaining(uint16_t seconds_remaining) {
    this->seconds_remaining = seconds_remaining;
  }
};

#endif /* COUNTDOWNTIMER_H_ */
