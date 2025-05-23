/*
 * TestPattern.h
 *
 *  Created on: May 15, 2025
 *      Author: Eric Mintz
 *
 * DisplayDriver implementation that displays a test pattern.
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

#ifndef TESTPATTERN_H_
#define TESTPATTERN_H_

#include "DisplayDriver.h"

class TestPattern : public DisplayDriver {
public:
  virtual ~TestPattern();

  virtual void operator() (
      PullQueueHT<DisplayCommand> &command_queue,
      const DisplayCommand &command,
      LedPanel &panel) override;
};

#endif /* TESTPATTERN_H_ */
