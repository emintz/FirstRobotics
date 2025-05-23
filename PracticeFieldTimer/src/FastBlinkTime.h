/*
 * FastBlinkTime.h
 *
 *  Created on: May 18, 2025
 *      Author: Eric Mintz
 *
 * Displays the remaining time as rapidly blinking digits.
 *
 * A count down that runs continuously. The countdown start is pinned
 * to a start time that is specified at construction. All times
 * *should* be in UTC.
 *
 * Main program for the FIRST Robotics competition practice field.
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

#ifndef FASTBLINKTIME_H_
#define FASTBLINKTIME_H_

#include "BaseTime.h"

#include "DisplayCommand.h"
#include "LedPanel.h"
#include "PullQueueHT.h"

class FastBlinkTime : public BaseTime {
public:
  FastBlinkTime();
  virtual ~FastBlinkTime();

  virtual void operator() (
      PullQueueHT<DisplayCommand> &command_queue,
      const DisplayCommand &command,
      LedPanel &panel);
};

#endif /* FASTBLINKTIME_H_ */
