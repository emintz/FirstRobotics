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
    _timer->set_remaining(initial_duration_seconds);
  }
}

BaseCountdown::BaseCountdown(
    uint8_t sqw_pin,
    int16_t duration_in_seconds,
    int16_t end_phase_seconds,
    int16_t reference_time,
    PullQueueHT<DisplayCommand>& command_queue,
    DisplayCommandPublisher& command_publisher,
    PullQueueHT<OneShotBlinkCommand>& rj45_led_blink) :
        _duration_in_seconds(duration_in_seconds),
        _rj45_led_blink(rj45_led_blink),
        _state(State::CREATED) {
  _on_completion = std::make_unique<OnCompletion>(this);
  _timer = std::make_unique<CountDownTimer>(
      duration_in_seconds,
      end_phase_seconds,
      *(_on_completion.get()),
      command_queue,
      command_publisher,
      rj45_led_blink);
  _task = std::make_unique<TaskWithActionH>(
      "continuous",
      COUNTDOWN_TASK_PRIORITY,
      _timer.get(),
      8192);
  _notify_function = std::make_unique<NotifyFromISR>(_task.get());
  _sqw_detector = std::make_unique<GpioChangeDetector>(
      sqw_pin,
      GpioChangeType::HIGH_TO_LOW,
      _notify_function.get());
  Serial.println("Countdown created.");
}

BaseCountdown::~BaseCountdown() {
}

void BaseCountdown::enable(int initial_duration_seconds) {
  Serial.printf("Countdown starting with initial duration: %d.\n", initial_duration_seconds);
  switch (_state) {
  case State::CREATED:
    maybe_set_initial_duration(
        static_cast<int16_t>(initial_duration_seconds));
    _task->start();
    _sqw_detector->start();
    _state = State::RUNNING;
    break;
  case State::PAUSED:
    maybe_set_initial_duration(
        static_cast<int16_t>(initial_duration_seconds));
    _task->start();
    _sqw_detector->start();
    _state = State::RUNNING;
    break;
  case State::RUNNING:
    break;
  }
}

void BaseCountdown::disable(void) {
  switch (_state) {
  case State::CREATED:
    // Not running, nothing to do.
    break;
  case State::PAUSED:
    // Not running here, either. Nothing to do.
    break;
  case State::RUNNING:
    _sqw_detector->stop();
    _state = State::PAUSED;
    break;
  }
}

CountdownEnableAction::CountdownEnableAction(BaseCountdown& to_enable) :
    to_enable(to_enable) {
}

CountdownEnableAction::~CountdownEnableAction() {
}

void CountdownEnableAction::run(void) {
  while (true) {
    wait_for_notification();
    to_enable.enable();
  }
}
