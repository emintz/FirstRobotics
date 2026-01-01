/*
 * NetworkTime.h
 *
 *  Created on: Dec 31, 2025
 *      Author: Eric Mintz
 *
 * Synchronizes the system time with network time and sets
 * the time zone to UTC.
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

#ifndef NETWORKTIME_H_
#define NETWORKTIME_H_

/**
 * Network Time Protocol management class
 */
class NetworkTime {
  bool running;
public:
  NetworkTime(void);
  virtual ~NetworkTime() = default;

  /**
   * Sets time zone to UTC and connects to the NTP server.
   *
   * Preconditions: WiFi must be connected to the Internet.
   *
   * Returns: status, true if successful, false otherwise.
   */
  bool begin(void);

  bool system_running(void) {
    return running;
  }
};

#endif /* NETWORKTIME_H_ */
