/*
 * TimeSource.h
 *
 *  Created on: May 19, 2025
 *      Author: Eric Mintz
 *
 * An API for reading the current time from an unspecified
 * source.
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

#ifndef TIMESOURCE_H_
#define TIMESOURCE_H_

#include "Arduino.h"

class TimeSource {

protected:
  TimeSource(void) {}
public:
  virtual ~TimeSource() = default;

  /*
   * Connects to the time keeping peripheral. Incoming
   * square waves will be enabled if available.
   *
   * Returns: true if connect succeeded, false otherwise.
   */
  virtual bool begin(void) = 0;

  /*
   * Returns the number of seconds since the most recent midnight.
   * The results will be in [0 .. 86400).
   */
  virtual int seconds_since_midnight(void) = 0;
};

#endif /* TIMESOURCE_H_ */
