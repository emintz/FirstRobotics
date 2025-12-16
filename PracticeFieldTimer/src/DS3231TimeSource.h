/*
 * DS3231TimeSource.h
 *
 *  Created on: May 19, 2025
 *      Author: Eric Mintz
 *
 * A TimeSource that uses a DS3231 IC as its source of truth.
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

#ifndef DS3231TIMESOURCE_H_
#define DS3231TIMESOURCE_H_

#include "TimeSource.h"

#include <Arduino.h>
#include <RTClib.h>

class DS3231_TimeSource : public TimeSource {

  RTC_DS3231 ds3231;

public:
  /*
   * Constructor: creates an instance that reads the current date
   * and time from a DS3231. Time will be unavailable until the
   * user invokes begin();
   */
  DS3231_TimeSource();

  /*
   * Place holder destructor.
   */
  virtual ~DS3231_TimeSource();

  /*
   * Connects to the DS3231. Connects to the default I2C bus (the
   * one used by the Wire class), which must be up and running.
   * Be sure to invoke Wire.begin() before calling this.
   *
   * Returns: true if and only if the DS3231 is connected.
   */
  virtual bool begin(void) override;

  /*
   * Provides the current time as seconds since midnight UTC.
   */
  virtual uint64_t seconds_since_midnight(void) override;

  virtual bool set_date_and_time(uint64_t unix_timestamp);

  virtual uint64_t unix_time(void) override;
};

#endif /* DS3231TIMESOURCE_H_ */
