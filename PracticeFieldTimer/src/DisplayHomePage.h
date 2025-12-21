/*
 * SetTimeHandler.h
 *
 *  Created on: Dec 9, 2025
 *      Author: Eric Mintz
 *
 * TimeRequestHandle that displays the home page. Note that the
 * page does NOT process the submitted timestamp.
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

#ifndef DISPLAYHOMEPAGE_H_
#define DISPLAYHOMEPAGE_H_

#include "TimeRequestHandler.h"

#include <HTTP_Method.h>
#include <WebServer.h>

#include <map>
#include <string>

class DS3231_TimeSource;


class DisplayHomePage : public TimeRequestHandler {
  DS3231_TimeSource& ds3231;

public:
  DisplayHomePage(DS3231_TimeSource& ds3231) :
      TimeRequestHandler("DisplayHomePage"),
          ds3231(ds3231) {
  }
  virtual ~DisplayHomePage() = default;

  virtual bool handle(
      WebServer& server, HTTPMethod method, const String& uri) override;

};

#endif /* DISPLAYHOMEPAGE_H_ */
