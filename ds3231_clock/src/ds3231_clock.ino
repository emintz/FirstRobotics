/*
 *  Created on Nov 26, 2025
 *      Author: Eric Mintz
 *
 * Tests the DS3231 and LCD by using them as a date/time clock.
 * When the DS3231's 1 Hz signal transitions from low to high,
 * the sketch reads its time, formats it, and displays the
 * result on the LCD.
 *
 * Copyright (C) 2026 Eric Mintz
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
 * Note: Requires that the DS3231 time has been set properly.
 *       See PinAssignments.h for wiring instructions.
 */
#include <Arduino.h>
#include <LCDi2c.h>
#include <memory.h>
#include "PinAssignments.h"
#include <RTClib.h>
#include <stdio.h>
#include <time.h>
#include <WiFi.h>
#include <Wire.h>

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 20
#define LCD_ROWS 4

static LCDi2c lcd;
static RTC_DS3231 ds3231;

static int yellow_level = LOW;
static int prior_sqw_level = LOW;

/**
 * Retrieves the current date/time from the DS3231 and displays
 * it on the LCD.
 */
static void display_time() {
  char display_buffer[22];
  memset(display_buffer, 0, sizeof(display_buffer));
  auto now = ds3231.now();
  snprintf(display_buffer, 20, "%d-%02d-%02d %02d:%02d:%02d",
      now.year(), now.month(), now.day(),
      now.hour(), now.minute(), now.second());
  lcd.home();
  lcd.print(display_buffer);
}

/**
 * Blink an error code in an infinite loop.
 */
static void halt_and_catch_fire(const char *message) {
  Serial.println(message);
  Serial.println("Halting ...");
  for (;;) {
    digitalWrite(RED_LED_PIN, HIGH);
    delay(250);
    digitalWrite(RED_LED_PIN, LOW);
    delay(250);
  }
}

/**
 * Extinguish all LEDs.
 */
static void all_off(void) {
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
}

static void blinkem(void) {
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, yellow_level);
  digitalWrite(RED_LED_PIN, HIGH);
  vTaskDelay(pdMS_TO_TICKS(250));
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  yellow_level = yellow_level == HIGH ? LOW : HIGH;
  vTaskDelay(pdMS_TO_TICKS(250));
}

void setup() {
  Serial.begin(115200);
  Serial.println("Serial I/O initialized.");
  Serial.printf("DS3231-powered digital clock compiled on %s at %s.\n",
      __DATE__, __TIME__);

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  pinMode(SET_CONFIGURATION_PIN, INPUT_PULLUP);
  pinMode(SQUARE_WAVE_PIN, INPUT);

  for (int i = 0; i < 4; ++i) {
    blinkem();
  }
  all_off();

  Serial.println("Initializing I2C bus.");
  if (!Wire.begin()) {
    halt_and_catch_fire("I2C bus initialization failed.");
  }

  Serial.println("Starting LCD.");
  lcd.begin(4, 20);
  lcd.home();

  lcd.cls();
  lcd.print("Starting...");

  Serial.println("Starting real time clock.");
  if (!ds3231.begin()) {
    halt_and_catch_fire("DS3231 initialization failed.");
  }
  ds3231.writeSqwPinMode(DS3231_SquareWave1Hz);

  auto now = ds3231.now();
  Serial.printf("Starting time: %d-%02d-%02d %02d:%02d:%02d\n",
      now.year(), now.month(), now.day(),
      now.hour(), now.minute(), now.second());
  delay(1000);
  lcd.cls();
}

void loop() {
  digitalWrite(BUILTIN_LED, digitalRead(SET_CONFIGURATION_PIN));
  auto current_sqw = digitalRead(SQUARE_WAVE_PIN);
  digitalWrite(GREEN_LED_PIN, current_sqw);
  if (LOW == prior_sqw_level && HIGH == current_sqw) {
    display_time();
  }
  prior_sqw_level = current_sqw;
}
