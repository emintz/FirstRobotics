/*
 * DisplayDriver.h
 *
 *  Created on: May 15, 2025
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

#ifndef DISPLAYDRIVER_H_
#define DISPLAYDRIVER_H_

#include "DisplayCommand.h"
#include "PullQueueHT.h"
class LedPanel;

class DisplayDriver {
public:
  virtual ~DisplayDriver();

  /*
   * Display control API.
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * command_queue        The pull queue that provides the commands. This
   *                      is used to generate time delays that expire when
   *                      a message arrives (via PullQueueHT::peek()).
   * command              What to display
   * panel                The display panel, the display command's target.
   */
  virtual void operator() (
      PullQueueHT<DisplayCommand> &command_queue,
      const DisplayCommand &command,
      LedPanel &panel) = 0;
};

#endif /* DISPLAYDRIVER_H_ */
