/*
 * OneShotCountdown.h
 *
 *  Created on: Jun 13, 2025
 *      Author: Eric Mintz
 *
 * A countdown that, when started, runs one cycle and stops.
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

#ifndef SRC_ONESHOTCOUNTDOWN_H_
#define SRC_ONESHOTCOUNTDOWN_H_

#include "Arduino.h"
#include "BaseCountdown.h"

#include "PullQueueHT.h"

class DisplayCommandPublisher;

class OneShotCountdown final : public BaseCountdown {

  PullQueueHT<DisplayCommand>& command_queue;
  DisplayCommandPublisher& command_publisher;
public:
  /*
   * Constructor
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * sqw_pin              Time source, provides a 1 Hz square wave
   * duration_in_seconds  Length of the countdown in seconds, e.g.
   *                      900 for 15 minutes
   * end_phase_seconds    The length of the end phase, the last bit
   *                      of the countdown where the time display
   *                      blinks annoyingly.
   * command_queue        Carries display commands to the panel server.
   * command_publisher    Publishes the command to the CAN bus if the
   */
  OneShotCountdown(
      uint8_t sqw_pin,
      int16_t duration_in_seconds,
      int16_t end_phase_seconds,
      PullQueueHT<DisplayCommand>& command_queue,
      DisplayCommandPublisher& command_publisher);
  virtual ~OneShotCountdown();

  virtual void on_countdown_complete(void) override;

  void publish(const DisplayCommand& command);
};

#endif /* SRC_ONESHOTCOUNTDOWN_H_ */
