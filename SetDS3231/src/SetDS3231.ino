/*
 * SetDS3231.ino
 *
 * Calibrate a DS3231-based timer with the current time UTC from the
 * Internet.
 *
 *  Created on: June 11, 2025
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
 *
 * Note: users MUST provide WiFiCredentials.h containing their
 *       WiFi SSID and password. See WiFiConnection.h for details.
 */

#include "Arduino.h"

#include "NetworkTime.h"
#include "WiFiConnection.h"

#include <ctime>
#include <DS3231.h>
#include <Wire.h>

static DS3231 rtc;
static NetworkTime network_time;
static WiFiConnection wifi_connection;

/**
 * Copies the current network time to the DS3231.
 */
static void set_rtc(void) {
  auto raw_current_time = std::time(NULL);
  auto current_time = std::gmtime(&raw_current_time);

  int seconds = current_time->tm_sec;
  int minutes = current_time->tm_min;
  int hours = current_time->tm_hour;

  int day_of_month = current_time->tm_mday;
  int month = current_time->tm_mon;
  int year = current_time->tm_year;

  Serial.printf("Current time: %04d-%02d-%02d %02d:%02d:%02d\n",
      year + 1900, month + 1, day_of_month,
      hours, minutes, seconds);
  rtc.setClockMode(false);  // 24 hour clock
  rtc.setEpoch(raw_current_time);
  rtc.enableOscillator(true, false, 0);

  auto ds3231_time = RTClib::now();

  Serial.printf("DS3231 time set to: %04d-%02d-%02d %02d:%02d:%02d, %ld.\n",
      static_cast<int>(ds3231_time.year()),
      static_cast<int>(ds3231_time.month()),
      static_cast<int>(ds3231_time.day()),

      static_cast<int>(ds3231_time.hour()),
      static_cast<int>(ds3231_time.minute()),
      static_cast<int>(ds3231_time.second()),

      static_cast<long>(ds3231_time.unixtime()));
}


void setup() {
  Serial.begin(115200);
  Serial.println("Serial I/O initialized.");
  Serial.printf("SetDS3231 compiled on %s at %s.\n",
      __DATE__, __TIME__);
  Wire.begin();
  Serial.println("I2C bus initialized.");
  bool setup_status = wifi_connection.begin();
  if (setup_status) {
    Serial.println("WiFi connected \\o/");
  } else {
    Serial.println("WiFi connection failed :-(");
  }

  if (setup_status) {
    setup_status = network_time.begin();
    if (setup_status) {
      Serial.println("SNTP startup succeeded \\o/");
    } else {
      Serial.println("SNTP startup failed :-(");
    }
  }

  if (setup_status) {
    set_rtc();
  }
}

void loop() {
}
