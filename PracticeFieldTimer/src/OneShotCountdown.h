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
  OneShotCountdown(
      uint8_t sqw_pin,
      int16_t duration_in_seconds,
      int16_t end_phase_seconds,
      PullQueueHT<DisplayCommand>& command_queue,
      DisplayCommandPublisher& command_publisher);
  virtual ~OneShotCountdown();

  virtual void on_countdown_complete(void) override;
};

#endif /* SRC_ONESHOTCOUNTDOWN_H_ */
