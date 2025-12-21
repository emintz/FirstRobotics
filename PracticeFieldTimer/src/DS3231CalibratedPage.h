/*
 * DS3231CalibratedPage.h
 *
 *  Created on: Dec 10, 2025
 *      Author: Eric Mintz
 *
 * Displays the "Clock Calibrated" web page.
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

#ifndef DS3231CALIBRATEDPAGE_H_
#define DS3231CALIBRATEDPAGE_H_

#include "TimeRequestHandler.h"

class DS3231_TimeSource;

class WebServer;

class DS3231CalibratedPage: public TimeRequestHandler {
  DS3231_TimeSource& ds3231;
public:
  DS3231CalibratedPage(DS3231_TimeSource& ds3231) :
      ds3231(ds3231),
      TimeRequestHandler("DS3231CCalibratedPage") {

  }
  virtual ~DS3231CalibratedPage() = default;

  virtual bool handle(
        WebServer& server, HTTPMethod method, const String& uri) override;
};

#endif /* DS3231CALIBRATEDPAGE_H_ */
