/*
 * TimeRequestHandlers.cpp
 *
 *  Created on: Dec 9, 2025
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

#include "TimeRequestHandlers.h"
#include "BypassCalibration.h"

#include "DisplayHomePage.h"
#include "DS3231CalibratedPage.h"
#include "DS3231TimeSource.h"

#include <memory>

TimeRequestHandlers::TimeRequestHandlers(
    DS3231_TimeSource &ds3231) {
  handlers["/bypass-time-calibration"] =
      std::make_unique<BypassCalibration>();
  handlers["/"] =
      std::make_unique<DisplayHomePage>(ds3231);
  handlers["/ds3231-calibrated"] =
      std::make_unique<DS3231CalibratedPage>(ds3231);
}

bool TimeRequestHandlers::canHandle(
    WebServer &server, HTTPMethod method, const String &uri) {
  Serial.printf("TimeRequestHandlers::canHandle line %d: uri: %s\n",
      __LINE__, uri.c_str());
  return
      HTTPMethod::HTTP_GET == method
      && handlers.contains(std::string(uri.c_str()));
}

bool TimeRequestHandlers::handle(
    WebServer &server, HTTPMethod requestMethod, const String &requestUri) {
  const char *uri = requestUri.c_str();
  bool status = canHandle(server, requestMethod, requestUri);
  if (status) {
    status = handlers[uri]->handle(server, requestMethod, requestUri);
  } else {
    Serial.printf("No handler registered for: '%s'\n", uri);
  }

  return status;
}
