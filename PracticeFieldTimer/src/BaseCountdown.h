/*
 * ContinuousCountdown.h
 *
 *  Created on: May 19, 2025
 *      Author: Eric Mintz
 *
 * Count down logic. Implements all logic except end of cycle handling,
 * which is set during construction.
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

#ifndef BASECOUNTDOWN_H_
#define BASECOUNTDOWN_H_

#include "Arduino.h"
#include "CountDownTimer.h"
#include "DisplayCommand.h"
#include "GpioChangeDetector.h"
#include "PullQueueHT.h"
#include "TaskAction.h"
#include "TaskWithActionH.h"
#include "VoidFunction.h"

#include <atomic>

class DisplayCommandPublisher;
class TimeSource;

#include <memory>

class BaseCountdown {
  enum class State {
    CREATED,
    RUNNING,
    PAUSED,
  };

  std::atomic<State> state;
  const int16_t duration_in_seconds;
  std::unique_ptr<VoidFunction> on_completion;
  std::unique_ptr<VoidFunction> notify_function;
  std::unique_ptr<CountDownTimer> timer;
  std::unique_ptr<GpioChangeDetector> sqw_detector;
  std::unique_ptr<TaskWithActionH> task;

  /*
   * Sets the duration of the next count down if
   * the provided duration is greater than 0. Does
   * nothing otherwise. This method SHOULD only be
   * invoked when the counter is disabled. This
   * is not enforced.
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        --------------------------------------------
   * initial_duration_seconds Count down length in seconds. Ignored
   *                          if <= 0.
   */
  void maybe_set_initial_duration(int16_t initial_duration_seconds);

protected:

  /*
   * Constructs a newly initialized instance.
   *
   * Note that all times are 24 hour UTC.
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * sqw_pin              GPIO pin that receives a precise 1Hz square wave,
   *                      the count down driver.
   * duration_in_seconds  Countdown duration (length) in seconds. Must be
   *                      > 0.
   * end_phase_seconds    Number of seconds to blink the time at session
   *                      expiry. Should be > 0.
   * reference_time       A time when a session must start. This is used
   *                      to synchronize session start with clock time.
   *                      This is useful for running, say, 12 minute
   *                      sessions starting at 0900.
   * command_queue        Carries display commands to the panel server.
   * command_publisher    Publishes the command to the CAN bus if the
   *                      latter is running.
   *
   */
  BaseCountdown(
      uint8_t sqw_pin,
      int16_t duration_in_seconds,
      int16_t end_phase_seconds,
      int16_t reference_time,
      PullQueueHT<DisplayCommand>& command_queue,
      DisplayCommandPublisher& command_pubisher);
public:
  virtual ~BaseCountdown();

  /*
   * Enable (i.e.) start the counter and optionally
   * synchronize it to the current time. The one-shot
   * count down ignores the parameter.
   *
   * Parameters:
   * ----------
   *
   * Name                     Contents
   * -----------------        ------------------------------------------------
   * initial_duration_seconds Number of seconds to count down in the first
   *                          iteration. Ignored if 0.
   */
  void enable(int initial_duration_seconds = 0);

  /*
   * Disable (i.e.) stop the counter
   */
  void disable(void);

  /*
   * Invoked when a countdown cycle completes.
   */
  virtual void on_countdown_complete(void) = 0;
};

/*
 * Task action that enables a countdown when it is
 * notified. The countdown is specified at construction.
 */
class CountdownEnableAction final : public TaskAction {

  BaseCountdown& to_enable;
public:

  /*
   * Constructor
   *
   * Name              Contents
   * ----------------- ------------------------------------------------
   * to_enable         Countdown to be enabled upon notification
   *                   receipt.
   */
  CountdownEnableAction(BaseCountdown& to_enable);

  virtual ~CountdownEnableAction();

  virtual void run(void) override;
};

#endif /* BASECOUNTDOWN_H_ */
