/*
 * Countdown.cpp
 *
 *  Created on: Jul 4, 2026
 *      Author: Eric Mintz
 *
 * Copyright 2026 Metuchen Momentum, LLC.
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

#include <Arduino.h>

#include "BlinkErrorCodes.h"
#include "Countdown.h"
#include "ErrorHalt.h"
#include "PinAssignments.h"

#include "StateTransitionTable.h"

Countdown::CommandMessage Countdown::on_tick_command_ = {
  .command_ = Command::TICK,
  .duration_ = 0,
  .on_stop_ = nullptr,
};

Countdown::Countdown(
    uint8_t sqw_pin,
    uint16_t slow_blink_seconds,
    uint16_t fast_blink_seconds,
    PullQueueHT<DisplayCommand>& display_command_queue) :
      on_stop_(nullptr),
      slow_blink_seconds_(slow_blink_seconds),
      fast_blink_seconds_(fast_blink_seconds),
      slow_blink_start_time_(slow_blink_seconds_ + fast_blink_seconds_),
      display_command_queue_(display_command_queue),
      internal_event_queue_(3),
      on_tick_(internal_event_queue_),
      sqw_detector_(
          sqw_pin,
          GpioChangeType::HIGH_TO_LOW,
          &on_tick_),
      duration_(0),
      color_change_halfway_mark_(0),
      time_remaining_(0),
      display_command_publisher_(nullptr) {
}

StateTransitionTable<Countdown::Command, Countdown::State>
    Countdown::_transition_table(State::IGNORING_EVENT);

void Countdown::begin() {
  _transition_table
      .add(State::CREATED, Command::START, State::STARTING)
      .add(State::CREATED, Command::START_AFTER_DELAY, State::STARTING_WITH_DELAY)

      .add(State::STARTING, Command::START, State::RESTARTING)
      .add(State::STARTING, Command::START_AFTER_DELAY, State::RESTARTING_WITH_DELAY)
      .add(State::STARTING, Command::STOP, State::STOPPING)
      .add(State::STARTING, Command::TICK, State::COUNTING)

      .add(State::STARTING_WITH_DELAY, Command::START, State::RESTARTING)
      .add(State::STARTING_WITH_DELAY, Command::START_AFTER_DELAY, State::RESTARTING_WITH_DELAY)
      .add(State::STARTING_WITH_DELAY, Command::STOP, State::STOPPING)
      .add(State::STARTING_WITH_DELAY, Command::TICK, State::COUNTING) // TODO: Skipping tick.

      .add(State::RESTARTING, Command::START, State::RESTARTING)
      .add(State::RESTARTING, Command::START_AFTER_DELAY, State::RESTARTING_WITH_DELAY)
      .add(State::RESTARTING, Command::STOP, State::STOPPING)
      .add(State::RESTARTING, Command::TICK, State::COUNTING)

      .add(State::RESTARTING_WITH_DELAY, Command::START, State::STARTING)
      .add(State::RESTARTING_WITH_DELAY, Command::STOP, State::STOPPING)
      .add(State::RESTARTING_WITH_DELAY, Command::TICK, State::IGNORING_TICK)

      .add(State::IGNORING_TICK, Command::START, State::STARTING)
      .add(State::IGNORING_TICK, Command::STOP, State::STOPPING)
      .add(State::IGNORING_TICK, Command::TICK, State::COUNTING)

      .add(State::COUNTING, Command::START, State::RESTARTING)
      .add(State::COUNTING, Command::START_AFTER_DELAY, State::RESTARTING_WITH_DELAY)
      .add(State::COUNTING, Command::STOP, State::STOPPING)
      .add(State::COUNTING, Command::TICK, State::COUNTING)

      .add(State::STOPPING, Command::START, State::STARTING)
      .add(State::STOPPING, Command::START_AFTER_DELAY, State::STARTING_WITH_DELAY)
      ;
}

void Countdown::cancel_count() {
  sqw_detector_.stop();
  internal_event_queue_.reset();
  DisplayCommand display_command = {};
  display_command.command = DisplayCommand::Pattern::FLOOD;
  display_command_queue_.send_message(&display_command, 1);
  duration_ = 0;
  color_change_halfway_mark_ = 0;
  time_remaining_ = 0;
  on_stop_ = nullptr;
  vTaskDelay(1);  // So lower priority tasks can respond.
  digitalWrite(YELLOW_LED_PIN, LOW);
}

DisplayCommand::Pattern Countdown::display_pattern() const {
  if (slow_blink_start_time_ < time_remaining_) {
    return DisplayCommand::Pattern::PLAIN_TIME;
  } else if (fast_blink_seconds_ < time_remaining_) {
    return DisplayCommand::Pattern::SLOW_BLINK_TIME;
  }
  return DisplayCommand::Pattern::FAST_BLINK_TIME;
}

void Countdown::display_remaining_time() const
{
  DisplayCommand command = {};
  command.time_in_seconds = time_remaining_;
  foreground(command);
  command.command = display_pattern();
  display_command_queue_.send_message(&command, 1);
  display_command_publisher_->operator()(command);
}

void Countdown::foreground(DisplayCommand& display_command) const {
  if (time_remaining_ <= slow_blink_start_time_) {
    display_command.foreground.red = 63;
  } else {
    uint16_t color_change_time = time_remaining_ - slow_blink_start_time_;
    if (color_change_time <= color_change_halfway_mark_) {
      int green_intensity =
          (63 * color_change_time) / color_change_halfway_mark_;
      display_command.foreground.green = green_intensity;
      display_command.foreground.red = 63 - display_command.foreground.green;
    } else {
      color_change_time -= color_change_halfway_mark_;
      int blue_intensity =
          (63 * color_change_time) / color_change_halfway_mark_;
      display_command.foreground.blue = blue_intensity;
      display_command.foreground.green = 63 - display_command.foreground.blue;
    }
  }
}

void Countdown::handle_delayed_start(const CommandMessage& command) {
  vTaskDelay(pdMS_TO_TICKS(20));
  internal_event_queue_.reset();
  handle_start(command);
}

void Countdown::handle_start(const CommandMessage& command) {
  digitalWrite(YELLOW_LED_PIN, HIGH);
  duration_ = command.duration_;
  time_remaining_ = command.duration_;
  on_stop_ = command.on_stop_;
  color_change_halfway_mark_ = (duration_ - slow_blink_start_time_) / 2;
  display_remaining_time();
  sqw_detector_.start();
}

void Countdown::handle_stop() {
  internal_event_queue_.reset();
  sqw_detector_.stop();
  if (on_stop_) {
    on_stop_->apply();
  }
  duration_ = 0;
  color_change_halfway_mark_ = 0;
  time_remaining_ = 0;
  on_stop_ = nullptr;
  vTaskDelay(1);  // So lower priority tasks can respond.
  digitalWrite(YELLOW_LED_PIN, LOW);
}

void Countdown::handle_tick() {
  if (0 == time_remaining_) {
    CommandMessage stop_counting{};
    memset(&stop_counting, '\0', sizeof(stop_counting));
    stop_counting.command_ = Countdown::Command::STOP;
    internal_event_queue_.send_message_to_front(&stop_counting, 1);
  } else {
    --time_remaining_;
    display_remaining_time();
  }
}

void Countdown::run() {
  start_internal_queue();
  State state = State::CREATED;
  CommandMessage display_command{};
  for (;;) {
    if (internal_event_queue_.pull_message(&display_command)) {
      auto maybe_new_state = _transition_table.to(
          state, display_command.command_);
      if (State::IGNORING_EVENT != maybe_new_state) {
        switch (state = maybe_new_state) {
        case State::CREATED:
          // Shouldn't happen, but just in case...
          break;
        case State::STARTING:
          handle_start(display_command);
          break;
        case State::STARTING_WITH_DELAY:
          handle_delayed_start(display_command);
          break;
        case State::RESTARTING:
          cancel_count();
          handle_start(display_command);
          break;
        case State::RESTARTING_WITH_DELAY:
          cancel_count();
          handle_delayed_start(display_command);
          break;
        case State::IGNORING_TICK:
          // Nothing to do.
          break;
        case State::COUNTING:
          handle_tick();
          break;
        case State::STOPPING:
          handle_stop();
          break;
        case State::IGNORING_EVENT:
          // Can't happen, but for the sake of completeness.
          break;
        }
      }
    }
  }
}

void Countdown::start_internal_queue() {
  if (!internal_event_queue_.begin()) {
    ErrorHalt::halt_and_catch_fire(
        COUNTDOWN_INTERNAL_QUEUE_FAILURE,
        "Countdown event queue startup failed.");
  }
}

bool Countdown::start(uint16_t duration_in_seconds, VoidFunction *on_stop) {
  CommandMessage start_countdown{};
  memset(&start_countdown, '\0', sizeof(start_countdown));
  start_countdown.command_ = Command::START;
  start_countdown.duration_ = duration_in_seconds;
  start_countdown.on_stop_ = on_stop;
  return internal_event_queue_.send_message(&start_countdown, 1);
}

bool Countdown::start_after_delay(uint16_t duration_in_seconds, VoidFunction *on_stop) {
  CommandMessage start_countdown{};
  memset(&start_countdown, '\0', sizeof(start_countdown));
  start_countdown.command_ = Command::START_AFTER_DELAY;
  start_countdown.duration_ = duration_in_seconds;
  start_countdown.on_stop_ = on_stop;
  return internal_event_queue_.send_message(&start_countdown, 1);
}

bool Countdown::start_from_ISR(
    uint16_t duration_in_seconds, VoidFunction *on_stop) {
  CommandMessage start_countdown{};
  memset(&start_countdown, '\0', sizeof(start_countdown));
  start_countdown.command_ = Command::START;
  start_countdown.duration_ = duration_in_seconds;
  start_countdown.on_stop_ = on_stop;
  return internal_event_queue_.send_message_from_ISR(&start_countdown);
}

bool Countdown::start_after_delay_from_ISR(
    uint16_t duration_in_seconds, VoidFunction *on_stop) {
  CommandMessage start_countdown{};
  memset(&start_countdown, '\0', sizeof(start_countdown));
  start_countdown.command_ = Command::START_AFTER_DELAY;
  start_countdown.duration_ = duration_in_seconds;
  start_countdown.on_stop_ = on_stop;
  return internal_event_queue_.send_message_from_ISR(&start_countdown);
}

