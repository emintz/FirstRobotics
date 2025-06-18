/*
 * ManualCountdown.h
 *
 *  Created on: Jun 15, 2025
 *      Author: Eric Mintz
 *
 * A manual countdown that runs once when a specified GPIO goes
 * low. It's meant to be started manually via a mechanical switch,
 * so the start signal is debounced.
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

#ifndef SRC_MANUALCOUNTDOWN_H_
#define SRC_MANUALCOUNTDOWN_H_

#include "Arduino.h"

#include "NotifyTaskOnPinChange.h"
#include "OneShotCountdown.h"
#include "TaskAction.h"
#include "TaskWithActionH.h"
#include "TaskNotifierVoidFunction.h"

class ManualCountdown final {

  PullQueueHT<DisplayCommand>& command_queue;
  DisplayCommandPublisher& command_publisher;
  OneShotCountdown actual_countdown;
  CountdownEnableAction countdown_enable;
  NotifyTaskOnPinChange on_pin_change;

public:
  ManualCountdown(
      uint8_t start_pin,
      uint8_t sqw_pin,
      int16_t duration_in_seconds,
      int16_t end_phase_seconds,
      PullQueueHT<DisplayCommand>& command_queue,
      DisplayCommandPublisher& command_publisher);
  virtual ~ManualCountdown();

  inline void run_one_time(void) {
    actual_countdown.enable();
  }

  void send(const DisplayCommand& command);

  inline bool start(void) {
    return on_pin_change.start();
  }
};

#endif /* SRC_MANUALCOUNTDOWN_H_ */
