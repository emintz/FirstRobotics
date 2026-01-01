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
 *
 * Connect the DS3231 as follows:
 *
 * DS3231 Signal     GPIO Pin
 * ----------------- -------------------------------------
 * SCL (I2C clock)   22 (I2C SCL)
 * SDA (I2C data)    21 (I2C SDA)
 * SQW (1 Hz output) 34 (input-only pin)
 *
 * Provide +3.3 Volts to Vcc and connect GND to ground.
 *
 * See https://www.analog.com/media/en/technical-documentation/data-sheets/DS3231.pdf
 * and your breakout board documentation for DS3231 signals.
 */

#include "Arduino.h"

#include "NetworkTime.h"
#include "WiFiConnection.h"

#include <ctime>
#include <DS3231.h>
#include <Wire.h>

#define BUILTIN_LED_PIN 2
#define SQW_IN_PIN 34

static DS3231 rtc;
static NetworkTime network_time;
static WiFiConnection wifi_connection;

/**
 * Hangs the system and blinks an error signal.
 */
static void fast_blink(void) {
  for (;;) {
    digitalWrite(BUILTIN_LED_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(50));
    digitalWrite(BUILTIN_LED_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

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

  pinMode(BUILTIN_LED_PIN, OUTPUT);
  digitalWrite(BUILTIN_LED_PIN, LOW);
  pinMode(SQW_IN_PIN, INPUT);

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
  } else {
    fast_blink();
  }
}

void loop() {
  digitalWrite(BUILTIN_LED_PIN, digitalRead(SQW_IN_PIN));
  vTaskDelay(1);
}
