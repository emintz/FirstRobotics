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
#include "PinAssignments.h"
#include "TaskPriorities.h"
#include "TaskWithActionH.h"

#define ON_TIME_MS 100
#define OFF_TIME_MS 100
#define INTER_GROUP_WAIT_MS 500

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
