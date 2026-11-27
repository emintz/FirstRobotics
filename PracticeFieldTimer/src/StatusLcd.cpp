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
#include "BlinkErrorCodes.h"
#include "ErrorHalt.h"

#include <LiquidCrystal_I2C.h>

#define CAN_STATUS_COLUMN 7
#define CAN_STATUS_LENGTH 13
#define CAN_STATUS_ROW 1
#define MODE_COLUMN 0
#define MODE_ROW 1
#define MODE_LENGTH 6
#define OVERALL_HEALTH_COLUMN 0
#define OVERALL_HEALTH_LENGTH 20
#define OVERALL_HEALTH_ROW 3
#define PRINT_DELAY_MS 2
#define CURSOR_MOVE_DELAY 5
#define PRINT_CLOCK_SPEED 500000
#define TIME_COLUMN 7
#define TIME_ROW 0

void StatusLcd::clear_field(uint8_t column, uint8_t row, uint8_t size) const {
  LcdMessage clear_field_command;
  clear_field_command.command = LcdMessage::Command::CLEAR_FIELD;
  clear_field_command.column = column;
  clear_field_command.row = row;
  clear_field_command.size = size;
  send(clear_field_command);
}

size_t StatusLcd::display(uint8_t column, uint8_t row, const char *message) const {
  size_t len = strlen(message);
  if (20 < len) {
    len = 20;
  }
  LcdMessage display_message;
  display_message.command = LcdMessage::Command::WRITE_FIELD;
  display_message.column = column;
  display_message.row = row;
  display_message.size = len;
  strncpy(display_message.contents, message, len);
  send(display_message);
  return len;
}

void StatusLcd::display_mode(const char *mode_name) const {
  Serial.printf("Mode: %s.\n", mode_name);
  (void) display(MODE_COLUMN, MODE_ROW, mode_name);
}

void StatusLcd::send(const LcdMessage& command) const{
  if (!status_message_queue_.send_message(&command, 1)) {
    ErrorHalt::halt_and_catch_fire(
      LCD_QUEUE_SEND_FAILED,
      "LCD command send failed.");
  }

}

StatusLcd::StatusLcd(
  PullQueueHT<LcdMessage>& status_message_queue) :
    status_message_queue_(status_message_queue) {
}

void StatusLcd::automatic_mode() const {
  display_mode("Auto");
}

void StatusLcd::can_bus_status(const char *message) const {
  char buffer[22] = {};
  strcpy(buffer, "CAN ");
  strncat(buffer, message, CAN_STATUS_LENGTH - 4);
  display(CAN_STATUS_COLUMN, CAN_STATUS_ROW, buffer);
}

void StatusLcd::clear() const {
  LcdMessage clear_command;
  clear_command.command = LcdMessage::Command::CLEAR_SCREEN;
  send(clear_command);
}

void StatusLcd::follow_mode(void) const {
  display_mode("Follow");
}

void StatusLcd::health(const char *message) const{
  display(OVERALL_HEALTH_COLUMN, OVERALL_HEALTH_ROW, message);
}

void StatusLcd::manual_mode() const {
  display_mode("Manual");
}

void StatusLcd::password(const char *wifi_password) const {
  size_t password_column = display(0, 1, "PW: ");
  display(password_column, 1, wifi_password);
}

void StatusLcd::ssid(const char *ssid) const {
  size_t ssid_column = display(0, 0, "SSID: ");
  display(ssid_column, 0, ssid);
}

void StatusLcd::time(uint16_t time_remaining) const {
  char formatted_time[10] = {};
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

void StatusLcd::url_path(const char *config_ip) const{
  char ip_message[22] = {0};
  strcpy(ip_message, "IP: ");
  strncat(ip_message, config_ip, 16);
  Serial.printf("Config IP message: %s.\n", ip_message);
  display(0, 2, ip_message);
}
