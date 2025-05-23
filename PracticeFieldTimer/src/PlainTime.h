/*
 * PlainTime.h
 *
 *  Created on: May 17, 2025
 *      Author: Eric Mintz
 *
 * Shows the time remaining without visual effects. No blinking, no
 * ripple, just static text and background.
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

#ifndef PLAINTIME_H_
#define PLAINTIME_H_

#include "BaseTime.h"

class PlainTime : public BaseTime {
public:
  PlainTime();
  virtual ~PlainTime();

  virtual void operator() (
      PullQueueHT<DisplayCommand> &command_queue,
      const DisplayCommand &command,
      LedPanel &panel) override;
};

#endif /* PLAINTIME_H_ */
