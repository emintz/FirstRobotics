/*
 * FloodPanel.h
 *
 *  Created on: Jun 18, 2025
 *      Author: Eric Mintz
 *
 * Display driver that floods the panel with a specified color.
 * The color is specified in the incoming command.
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

#ifndef SRC_FLOODPANEL_H_
#define SRC_FLOODPANEL_H_

#include "DisplayDriver.h"

#include "CanBus.h"
#include "CanPayload.h"

class LedPanel;

class FloodPanel final : public DisplayDriver {

public:
  FloodPanel();

  virtual ~FloodPanel();
  virtual void operator() (
      PullQueueHT<DisplayCommand> &command_queue,
      const DisplayCommand &command,
      LedPanel &panel) override;
};

#endif /* SRC_FLOODPANEL_H_ */
