/*
 * BypassCalibration.h
 *
 *  Created on: Dec 10, 2025
 *      Author: Eric Mintz
 *
 * Displays the "Date/Time Unchanged" page and lets the user
 * change the practice field session field settings or cancel
 * the update entirely.
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

#ifndef BYPASSCALIBRATION_H_
#define BYPASSCALIBRATION_H_

#include "TimeRequestHandler.h"

class BypassCalibration: public TimeRequestHandler {
public:
  BypassCalibration() :
      TimeRequestHandler("BypassCalibration") {

  }
  virtual ~BypassCalibration() = default;

  virtual bool handle(
        WebServer& server, HTTPMethod method, const String& uri) override;
};

#endif /* BYPASSCALIBRATION_H_ */
