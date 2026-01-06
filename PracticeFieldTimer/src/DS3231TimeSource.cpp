/*
 * DS3231TimeSource.cpp
 *
 *  Created on: May 19, 2025
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

#include "DS3231TimeSource.h"

#define DS3231_CONTROL_REGISTER 0xe
#define SQW_1_HZ_MASK 0xE1

DS3231_TimeSource::DS3231_TimeSource() {
}

DS3231_TimeSource::~DS3231_TimeSource() {
}

bool DS3231_TimeSource::begin(void) {
  ds3231.begin();
  uint8_t control_register = ds3231.read(DS3231_CONTROL_REGISTER);
  Serial.printf("DS3231 control register (0xE) on read: %x.\n",
      static_cast<int>(control_register));
  control_register &= ~0x04; // turn off INTCON
  control_register &= ~0x18; // set freq bits to 0
  Serial.printf("Setting DS3231 control register to %x.\n",
      static_cast<int>(control_register));
  ds3231.write(DS3231_CONTROL_REGISTER, control_register);
  return true;
}

uint64_t DS3231_TimeSource::seconds_since_midnight(void) {
  auto now = ds3231.now();
  return now.hour() * 3600 + now.minute() * 60 + now.second();
}

bool DS3231_TimeSource::set_date_and_time(uint64_t unix_timestamp) {
  DateTime time_to_set(unix_timestamp);
  ds3231.adjust(time_to_set);
  return true;
}

uint64_t DS3231_TimeSource::unix_time(void) {
  return static_cast<uint64_t>(ds3231.now().unixtime());
}
