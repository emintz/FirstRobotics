/*
 * ManualCountdown.h
 *
 *  Created on: Jul 6, 2026
 *      Author: Eric Mintz
 *
 * An aggregate of everything required to implement
 * a manual countdown, including the Countdown to
 * time the session, a GpioChangeDetector that
 * that watches the start button and a VoidFunction
 * that starts the Coundown (nb: from an ISR)
 *
 * Copyright 2026 Metuchen Momentum LLC.
 * All Rights Reserved.
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

#ifndef MANUALCOUNTDOWN_H_
#define MANUALCOUNTDOWN_H_

#include <Arduino.h>

#include "Countdown.h"
#include "DisplayCommand.h"
#include "ShowNoTimeRemaining.h"

#include <GpioChangeDetector.h>
#include <PullQueueHT.h>
#include <VoidFunction.h>

class ManualCountdown {

  class StartCountdown : public VoidFunction {
    friend class ManualCountdown;

    Countdown& _countdown;
    const uint16_t _duration_in_seconds;
    ShowNoTimeRemaining _on_stop;

    StartCountdown(
        Countdown& countdown,
        PullQueueHT<DisplayCommand>& display_command_queue,
        uint16_t duration_in_seconds) :
      _countdown(countdown),
      _duration_in_seconds(duration_in_seconds),
      _on_stop(nullptr, display_command_queue) {
    }
  public:

    /**
     * Starts the count.
     */
    void apply() override {
      _countdown.start_after_delay_from_ISR(_duration_in_seconds, &_on_stop);
    }
  };

  StartCountdown _start_countdown_function;
  GpioChangeDetector _countdown_start_detector;

public:
  /**
   * Constructs a newly initialized instance.
   *
   * Parameters:
   * ----------
   *
   * Name                  Contents
   * -----------------     ------------------------------------------------
   * start_pin             GPIO pin connected to the start button. The count
   *                       starts when the pin goes LOW.
   *                       latter is running.
   * countdown             The countdown that actually runs the display
   * display_command_queue Carries commands to the panel (i.e display) task.
   *                       Used to display 00:00 in steady red when the
   *                       countdown ends.
   */
  ManualCountdown(
      uint8_t start_pin,
      Countdown& countdown,
      PullQueueHT<DisplayCommand>& display_command_queue,
      uint16_t duration_in_seconds);

  virtual ~ManualCountdown() = default;

  /**
   * Starts display processing, which starts the GPIO pin change detector
   * that fires when the user presses the start button, thereby starting
   * an event sequence that eventually starts the countdown that is
   * bound to this instance.
   */
  boolean start();
};

#endif /* MANUALCOUNTDOWN_H_ */
