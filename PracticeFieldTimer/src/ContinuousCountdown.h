/*
 * ContinuousCountdown.h
 *
 *  Created on: May 23, 2025
 *      Author: Eric Mintz
 *
 * A count down that runs continuously. The countdown start is pinned
 * to a start time that is specified at construction. All times
 * *should* be in UTC.
 *
 * Main program for the FIRST Robotics competition practice field.
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

#ifndef CONTINUOUSCOUNTDOWN_H_
#define CONTINUOUSCOUNTDOWN_H_

#include "BaseCountdown.h"

class ContinuousCountdown : public BaseCountdown {
public:
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
   *
   */
  ContinuousCountdown(
      uint8_t sqw_pin,
      int16_t duration_in_seconds,
      int16_t end_phase_seconds,
      int16_t reference_time,
      PullQueueHT<DisplayCommand>& command_queue);
  virtual ~ContinuousCountdown();
};

#endif /* CONTINUOUSCOUNTDOWN_H_ */
