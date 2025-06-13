/*
 * ContinuousCountdown.cpp
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
 */

#include "BaseCountdown.h"
#include "DisplayCommandPublisher.h"
#include "NotifyFromISR.h"
#include "TaskPriorities.h"

class OnCompletion final : public VoidFunction {
  BaseCountdown& countdown;

public:
  inline OnCompletion(BaseCountdown *countdown) :
  countdown(*countdown) {
  }

  virtual void apply(void) override {
    countdown.on_countdown_complete();
  }
};

void BaseCountdown::maybe_set_initial_duration(int16_t initial_duration_seconds)  {
  if (0 < initial_duration_seconds) {
    timer->set_remaining(initial_duration_seconds);
  }

  Serial.printf("Initial duration: %d.\n", (int) initial_duration_seconds);
}

BaseCountdown::BaseCountdown(
    uint8_t sqw_pin,
    int16_t duration_in_seconds,
    int16_t end_phase_seconds,
    int16_t reference_time,
    PullQueueHT<DisplayCommand>& command_queue,
    DisplayCommandPublisher& command_publisher) :
        duration_in_seconds(duration_in_seconds),
        state(State::CREATED) {
  on_completion = std::make_unique<OnCompletion>(this);
  timer = std::make_unique<CountDownTimer>(
      duration_in_seconds,
      end_phase_seconds,
      *(on_completion.get()),
      command_queue,
      command_publisher);
  task = std::make_unique<TaskWithActionH>(
      "continuous",
      COUNTDOWN_TASK_PRIORITY,
      timer.get(),
      4096);
  notify_function = std::make_unique<NotifyFromISR>(task.get());
  sqw_detector = std::make_unique<GpioChangeDetector>(
      sqw_pin,
      GpioChangeType::HIGH_TO_LOW,
      notify_function.get());
}

BaseCountdown::~BaseCountdown() {
}

void BaseCountdown::enable(int initial_duration_seconds) {
  switch (state) {
  case State::CREATED:
    maybe_set_initial_duration(
        static_cast<int16_t>(initial_duration_seconds));
    task->start();
    sqw_detector->start();
    state = State::RUNNING;
    break;
  case State::PAUSED:
    maybe_set_initial_duration(
        static_cast<int16_t>(initial_duration_seconds));
    task->start();
    sqw_detector->start();
    state = State::RUNNING;
    break;
  case State::RUNNING:
    break;
  }
}

void BaseCountdown::disable(void) {
  switch (state) {
  case State::CREATED:
    // Not running, nothing to do.
    break;
  case State::PAUSED:
    // Not running here, either. Nothing to do.
    break;
  case State::RUNNING:
    sqw_detector->stop();
    state = State::PAUSED;
    break;
  }
}
