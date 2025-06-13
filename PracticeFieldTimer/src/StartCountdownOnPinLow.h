/*
 * StartCountdownOnPinLow.h
 *
 *  Created on: Jun 13, 2025
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

#ifndef SRC_STARTCOUNTDOWNONPINLOW_H_
#define SRC_STARTCOUNTDOWNONPINLOW_H_

#include "Arduino.h"
#include "VoidFunction.h"

class BaseCountdown;

class StartCountdownOnPinLow : public VoidFunction {
  const uint8_t gpio;
  BaseCountdown& countdown;

public:
  /*
   * Constructor
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * gpio                 Pin to monitor. This must be the pin monitored
   *                      by the invoking debouncer.
   * countdown            Countdown to enable when gpio goes low.
   */
  StartCountdownOnPinLow(
      uint8_t gpio,
      BaseCountdown& countdown);
  virtual ~StartCountdownOnPinLow();

  /*
   * Starts the countdown if gpio is LOW.
   */
  virtual void apply(void) override;
};

#endif /* SRC_STARTCOUNTDOWNONPINLOW_H_ */
