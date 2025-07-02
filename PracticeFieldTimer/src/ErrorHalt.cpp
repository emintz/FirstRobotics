/*
 * ErrorHalt.cpp
 *
 *  Created on: Apr 26, 2025
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

#include "ErrorHalt.h"

#include "Arduino.h"
#include "BlinkAction.h"
#include "BlinkErrorCodes.h"
#include "PinAssignments.h"
#include "StatusLcd.h"
#include "TaskPriorities.h"
#include "TaskWithActionH.h"

#define ON_TIME_MS 100
#define OFF_TIME_MS 100
#define INTER_GROUP_WAIT_MS 500

#include <map>

std::map<int, const char *> MESSAGES = {
    {WIFI_ACCESS_POINT_SETUP_FAILED, "ERR: WiFi Setup"},
    {WIFI_IP_SETUP_FAILED, "ERR: WiFi IP"},
    {WIFI_SHUTDOWN_FAILED, "ERR: WiFi Shutdown"},
    {SYSTEM_CONFIGURATION_FAILED, "ERR: Sys Config"},
    {SYSTEM_NOT_CONFIGURED, "ERR: No Sys Config"},
    {NON_VOLATILE_OPEN_FAILED, "ERR: NVS Open"},
    {CONFIG_NAMESPACE_OPEN_FAILED, "ERR: NVS Namespace"},
    {CORRUPT_CONFIGURATION, "ERR: Corrupt Config"},
    {INCOMPLETE_CONFIGURATION, "ERR: Cfg Incomplete"},
    {CONFIG_NAMESPACE_CLOSE_FAILED, "ERR: Cfg Nmspc Close"},
    {NON_VOLATILE_CLOSE_FAILED, "ERR: NVS Close"},
    {COMMAND_QUEUE_BEGIN_FAILED, "ERR: Queue Begin"},
    {UNKNOWN_PANEL_DRIVER, "ERR: Bad Pnl Driver"},
    {DS3231_STARTUP_FAILED, "ERR: DS3231 Start"},
    {GPIO_INTERRUPT_STARTUP_FAILED, "GPIO Interrupt"},
    {GPIO_CHANGE_STARTUP_FAILED, "ERR: GPIO Change"},
    {CAN_INIT_FAILED, "ERR: CAN Init"},
    {CAN_START_FAILED, "CAN Start"},
};

ErrorHalt::ErrorHalt() :
    status_display(NULL) {
}

const ErrorHalt ErrorHalt::INSTANCE;

void ErrorHalt::operator() (int error_code, const char *console_message) const {
  if (status_display) {
    status_display->health(MESSAGES.contains(error_code)
        ? MESSAGES[error_code]
        : "ERR: Unrecognized");
  }
  halt_and_catch_fire(error_code, console_message);
}

void ErrorHalt::halt_and_catch_fire(int error_code, const char *message) {
  BlinkAction action(
      BUILTIN_LED_PIN,
      error_code,
      ON_TIME_MS,
      OFF_TIME_MS,
      INTER_GROUP_WAIT_MS);
  TaskWithActionH blink(
      "ErrorBlink",
      HALT_AND_CATCH_FIRE_PRIORITY,
      &action,
      2048);
  Serial.println(message);
  Serial.flush();
  blink.start();
  while (1) {
    vTaskDelay(portMAX_DELAY);
  }
}
