/*
 * StatusDisplay.h
 *
 *  Created on: Jul 1, 2025
 *      Author: Eric Mintz
 *
 * Manages a 4 x 20 LCD status display.
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

#ifndef STATUSLCD_H_
#define STATUSLCD_H_

#include "Arduino.h"

class LiquidCrystal_I2C;

class StatusLcd {
  LiquidCrystal_I2C& lcd;

  void clear_field(uint8_t column, uint8_t row, uint8_t length);

  /*
   * Display a message starting at a specified column and row.
    *
   * Parameters:
   * ----------
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * column            Starting column, 0-based
   * row               Starting row, 0-based
   * message           Null-terminated message to display. Behavior is
   *                   unspecified if the message cannot fit in the
   *                   specified  row.
   *
   * Returns: the number of characters written
   */
  size_t display(uint8_t column, uint8_t row, const char *message);

public:
  /*
   * Constructs an instance that displays the countdown clock
   * status on the specified 20 column x 4 row I2C LCD.
   *
   * Parameters:
   * ----------
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * lcd               Display target, manages the physical liquid crystal
   *                   display.
   */
  StatusLcd(LiquidCrystal_I2C& lcd);
  virtual ~StatusLcd();

  /*
   * Displays the mode status as "Auto". Invoke this when the timer
   * is configured to run countdown cycles continuously.
   */
  void automatic_mode(void);

  /*
   * Display the CAN bus status.
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * message           NULL-terminated status message to display. Cannot
   *                   be NULL, and must fit in its allotted space.
   */
  void can_bus_status(const char *message);

  /**
   * Clear the display.
   */
  void clear(void);

  /*
   * Display follower status. Invoke this when the timer follows a
   * leader, i.e. a time provider.
   */
  void follow_mode(void);

  /*
   * Display the system's general health
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * message           NULL-terminated status message to display. Cannot
   *                   be NULL, and must fit in its allotted space.
   */
  void health(const char *message);

  /*
   * Displays the mode status as "manual". Invoke this when the
   * display is configured so that the user has to start each
   * countdown cycle.
   */
  void manual_mode(void);

  /*
   * Display the WiFi password during configuration
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * wifi_password     NULL-terminated WiFi password to display. Cannot
   *                   be NULL, and must fit in its allotted space.
   */
  void password(const char *wifi_password);

  /*
   * Display the WiFi access point SSID during configuration.
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * message           NULL-terminated SSID (network name) to display. Cannot
   *                   be NULL, and must fit in its allotted space.
   */
  void ssid(const char *ssid);

  /*
   * Display the number of seconds remaining in the current practice
   * session.
   *
   * Parameters:
   * ----------
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * time_remaining    The number of seconds remaining in the current
   *                   practice session.
   */
  void time(uint16_t time_remaining);

  /*
   * Display the configuration page URL during configuration. This is
   * the path of the URL that accesses the configuration page.
   *
   * Name              Contents
   * ----------------- ---------------------------------------------------
   * config_ip         NULL-terminated configuration page URL IP address
   *                   to display. Cannot be NULL, and must fit in its
   *                   allotted space.
   */
  void url_path(const char *config_ip);
};

#endif /* STATUSLCD_H_ */
