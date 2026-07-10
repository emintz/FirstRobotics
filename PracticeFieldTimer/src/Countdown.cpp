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

Countdown::CommandMessage Countdown::_on_tick_command = {
  ._command = Command::TICK,
  ._duration = 0,
  ._on_stop = NULL,
};

Countdown::Countdown(
    uint8_t sqw_pin,
    uint16_t slow_blink_seconds,
    uint16_t fast_blink_seconds,
    PullQueueHT<DisplayCommand>& display_command_queue,
    DisplayCommandPublisher& command_to_can_bus) :
      _on_stop(NULL),
      _slow_blink_seconds(slow_blink_seconds),
      _fast_blink_seconds(fast_blink_seconds),
      _slow_blink_start_time(_slow_blink_seconds + _fast_blink_seconds),
      _display_command_queue(display_command_queue),
      _command_to_can_bus(command_to_can_bus),
      _internal_event_queue(3),
      _on_tick(_internal_event_queue),
      _sqw_detector(
          sqw_pin,
          GpioChangeType::HIGH_TO_LOW,
          &_on_tick),
      _duration(0),
      _color_change_halfway_mark(0),
      _time_remaining(0) {
}

StateTransitionTable<Countdown::Command, Countdown::State>
    Countdown::_transition_table(State::IGNORING_EVENT);

void Countdown::begin(void) {
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

void Countdown::cancel_count(void) {
  _sqw_detector.stop();
  _internal_event_queue.reset();
  DisplayCommand display_command;
  memset(&display_command, '\0', sizeof(display_command));
  display_command.command = DisplayCommand::Pattern::FLOOD;
  _display_command_queue.send_message(&display_command, 1);
  _duration = 0;
  _color_change_halfway_mark = 0;
  _time_remaining = 0;
  _on_stop = NULL;
  vTaskDelay(1);  // So lower priority tasks can respond.
  digitalWrite(YELLOW_LED_PIN, LOW);
}

DisplayCommand::Pattern Countdown::display_pattern(void) {
  if (_slow_blink_start_time < _time_remaining) {
    return DisplayCommand::Pattern::PLAIN_TIME;
  } else if (_fast_blink_seconds < _time_remaining) {
    return DisplayCommand::Pattern::SLOW_BLINK_TIME;
  }
  return DisplayCommand::Pattern::FAST_BLINK_TIME;
}

void Countdown::display_remaining_time(void) {
  DisplayCommand command;
  memset(&command, '\0', sizeof(DisplayCommand));
  command.time_in_seconds = _time_remaining;
  foreground(command);
  command.command = display_pattern();
  _display_command_queue.send_message(&command, 1);
  _command_to_can_bus(command);
}

void Countdown::foreground(DisplayCommand& display_command) {
  if (_time_remaining <= _slow_blink_start_time) {
    display_command.foreground.red = 63;
  } else {
    uint16_t color_change_time = _time_remaining - _slow_blink_start_time;
    if (color_change_time <= _color_change_halfway_mark) {
      int green_intensity =
          (63 * color_change_time) / _color_change_halfway_mark;
      display_command.foreground.green = green_intensity;
      display_command.foreground.red = 63 - display_command.foreground.green;
    } else {
      color_change_time -= _color_change_halfway_mark;
      int blue_intensity =
          (63 * color_change_time) / _color_change_halfway_mark;
      display_command.foreground.blue = blue_intensity;
      display_command.foreground.green = 63 - display_command.foreground.blue;
    }
  }
}

void Countdown::handle_delayed_start(const CommandMessage& command) {
  vTaskDelay(pdMS_TO_TICKS(20));
  _internal_event_queue.reset();
  handle_start(command);
}

void Countdown::handle_start(const CommandMessage& command) {
  digitalWrite(YELLOW_LED_PIN, HIGH);
  _duration = command._duration;
  _time_remaining = command._duration;
  _on_stop = command._on_stop;
  _color_change_halfway_mark = (_duration - _slow_blink_start_time) / 2;
  display_remaining_time();
  _sqw_detector.start();
}

void Countdown::handle_stop(void) {
  _internal_event_queue.reset();
  _sqw_detector.stop();
  if (_on_stop) {
    _on_stop->apply();
  }
  _duration = 0;
  _color_change_halfway_mark = 0;
  _time_remaining = 0;
  _on_stop = NULL;
  vTaskDelay(1);  // So lower priority tasks can respond.
  digitalWrite(YELLOW_LED_PIN, LOW);
}

void Countdown::handle_tick(void) {
  if (0 == _time_remaining) {
    CommandMessage stop_counting;
    memset(&stop_counting, '\0', sizeof(stop_counting));
    stop_counting._command = Countdown::Command::STOP;
    _internal_event_queue.send_message_to_front(&stop_counting, 1);
  } else {
    --_time_remaining;
    display_remaining_time();
  }
}

void Countdown::run(void) {
  start_internal_queue();
  State state = State::CREATED;
  CommandMessage display_command;
  for (;;) {
    if (_internal_event_queue.pull_message(&display_command)) {
      auto maybe_new_state = _transition_table.to(
          state, display_command._command);
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

void Countdown::start_internal_queue(void) {
  if (!_internal_event_queue.begin()) {
    ErrorHalt::halt_and_catch_fire(
        COUNTDOWN_INTERNAL_QUEUE_FAILURE,
        "Countdown event queue startup failed.");
  }
}

bool Countdown::start(uint16_t duration_in_seconds, VoidFunction *on_stop) {
  CommandMessage start_countdown;
  memset(&start_countdown, '\0', sizeof(start_countdown));
  start_countdown._command = Command::START;
  start_countdown._duration = duration_in_seconds;
  start_countdown._on_stop = on_stop;
  return _internal_event_queue.send_message(&start_countdown, 1);
}

bool Countdown::start_after_delay(uint16_t duration_in_seconds, VoidFunction *on_stop) {
  CommandMessage start_countdown;
  memset(&start_countdown, '\0', sizeof(start_countdown));
  start_countdown._command = Command::START_AFTER_DELAY;
  start_countdown._duration = duration_in_seconds;
  start_countdown._on_stop = on_stop;
  return _internal_event_queue.send_message(&start_countdown, 1);
}

bool Countdown::start_from_ISR(
    uint16_t duration_in_seconds, VoidFunction *on_stop) {
  CommandMessage start_countdown;
  memset(&start_countdown, '\0', sizeof(start_countdown));
  start_countdown._command = Command::START;
  start_countdown._duration = duration_in_seconds;
  start_countdown._on_stop = on_stop;
  return _internal_event_queue.send_message_from_ISR(&start_countdown);
}

bool Countdown::start_after_delay_from_ISR(
    uint16_t duration_in_seconds, VoidFunction *on_stop) {
  CommandMessage start_countdown;
  memset(&start_countdown, '\0', sizeof(start_countdown));
  start_countdown._command = Command::START_AFTER_DELAY;
  start_countdown._duration = duration_in_seconds;
  start_countdown._on_stop = on_stop;
  return _internal_event_queue.send_message_from_ISR(&start_countdown);
}

