/*
 * OneShotBlink.h
 *
 *  Created on: Jun 27, 2026
 *      Author: Eric Mintz
 *
 * Task logic that blinks multiple LEDs once as directed
 * by incoming messages. The logic is WAIT-ON-OFF.
 *
 *
 * Copyright (C) 2026 Eric Mintz
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

#ifndef ONESHOTBLINK_H_
#define ONESHOTBLINK_H_

#include <Arduino.h>
#include <PullQueueHT.h>
#include <TaskAction.h>

/**
 * The maximum number of pins that can blink at
 * one time.
 */
#define MAX_BLINK_PINS 4

struct OneShotBlinkCommand {
  /**
   * Time to illuminate the pins in milliseconds. Nothing
   * happens if 0.
   */
  uint16_t _on_time_ms;
  /**
   * Number of pins to blink. Nothing happens if 0.
   */
  uint8_t _pin_count;
  /**
   * The pins to blink. Be sure to set all values.
   */
  uint8_t _blink_pin[MAX_BLINK_PINS];
};

class OneShotBlink : public TaskAction {
  PullQueueHT<OneShotBlinkCommand>& _command_queue;
public:
  OneShotBlink(PullQueueHT<OneShotBlinkCommand>& command_queue) :
    _command_queue(command_queue) {
  }
  virtual ~OneShotBlink() = default;

protected:
  virtual void run(void) override;
};

#endif /* ONESHOTBLINK_H_ */
