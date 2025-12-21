/*
 * TimeRequestHandlers.h
 *
 *  Created on: Dec 9, 2025
 *      Author: Eric Mintz
 *
 * Copyright (C) 2025 Eric Mintz
 * All Rights Reserved
 *
 * Web server request handler for DS3231 calibration-related uris.
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

#ifndef TIMEREQUESTHANDLERS_H_
#define TIMEREQUESTHANDLERS_H_

#include "TimeRequestHandlers.h"
#include "TimeRequestHandler.h"

#include <detail/RequestHandler.h>
#include <map>
#include <memory>
#include <string>

class DS3231_TimeSource;
class WebServer;

class TimeRequestHandlers : public RequestHandler {
  std::map<std::string, std::unique_ptr<TimeRequestHandler>> handlers;

public:
  TimeRequestHandlers(
      DS3231_TimeSource &ds3231);
  virtual ~TimeRequestHandlers() = default;

  virtual bool canHandle(
      WebServer &server, HTTPMethod method, const String &uri) override;

  virtual bool handle(
      WebServer &server, HTTPMethod requestMethod, const String &requestUri)
      override;
};

#endif /* TIMEREQUESTHANDLERS_H_ */
