/*
 * StatusDisplay.cpp
 *
 *  Created on: Jul 1, 2025
 *      Author: Eric Mintz
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

#include "StatusLcd.h"

#include <LiquidCrystal_I2C.h>

#define CAN_STATUS_COLUMN 7
#define CAN_STATUS_LENGTH 13
#define CAN_STATUS_ROW 1
#define LEAD_FOLLOW_COLUMN 0
#define LEAD_FOLLOW_ROW 1
#define OVERALL_HEALTH_COLUMN 0
#define OVERALL_HEALTH_LENGTH 20
#define OVERALL_HEALTH_ROW 3
#define TIME_COLUMN 7
#define TIME_ROW 0

void StatusLcd::clear_field(uint8_t column, uint8_t row, uint8_t size) {
  while (0 < size) {
    display(column + (--size), row, " ");
  }
}

size_t StatusLcd::display(uint8_t column, uint8_t row, const char *message) {
  lcd.setCursor(column, row);
  return lcd.print(message);
}

StatusLcd::StatusLcd(LiquidCrystal_I2C& lcd) :
    lcd(lcd) {
}

StatusLcd::~StatusLcd() {
}

void StatusLcd::can_bus_status(const char *message) {
  clear_field(CAN_STATUS_COLUMN, CAN_STATUS_ROW, CAN_STATUS_LENGTH);
  uint8_t message_column = CAN_STATUS_COLUMN
      + display(CAN_STATUS_COLUMN, CAN_STATUS_ROW, "CAN ");
  display(message_column, CAN_STATUS_ROW, message);
}

void StatusLcd::follower(void) {
  display(LEAD_FOLLOW_COLUMN, LEAD_FOLLOW_ROW, "Follow");
}

void StatusLcd::health(const char *message) {
  clear_field(OVERALL_HEALTH_COLUMN, OVERALL_HEALTH_ROW, OVERALL_HEALTH_LENGTH);
  display(OVERALL_HEALTH_COLUMN, OVERALL_HEALTH_ROW, message);
}

void StatusLcd::leader(void) {
  display(LEAD_FOLLOW_COLUMN, LEAD_FOLLOW_ROW, "Lead  ");
}

void StatusLcd::show_time(uint16_t time_remaining) {
  char formatted_time[6];
  uint16_t seconds = time_remaining % 60;
  uint16_t minutes = time_remaining / 60;
  memset(formatted_time, 0, sizeof(formatted_time));
  formatted_time[4] = seconds % 10 + '0';
  seconds /= 10;
  formatted_time[3] = seconds + '0';
  formatted_time[2] = ':';
  formatted_time[1] = minutes % 10 + '0';
  minutes /= 10;
  formatted_time[0] = minutes
      ? minutes + '0'
      : ' ';
  display(TIME_COLUMN, TIME_ROW, formatted_time);
}
