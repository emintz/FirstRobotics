/*
 * CANDisplayCommandPublisher.h
 *
 *  Created on: Jun 9, 2025
 *      Author: Eric Mintz
 *
 * Sends the display time to followers. Only the
 * leading countdown timer invoke this class.
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

#ifndef CANDISPLAYCOMMANDPUBLISHER_H_
#define CANDISPLAYCOMMANDPUBLISHER_H_

#include "DisplayCommandPublisher.h"

class CanBus;
class DisplayCommand;

class CANDisplayCommandPublisher final :
    public DisplayCommandPublisher {
  CanBus& can_bus;

public:
  CANDisplayCommandPublisher(CanBus& can_bus);

  /*
   * Publishes a display command to the CAN bus and show
   * publication status on LEDs. When publication succeeds,
   * turn off the red LED and illuminate the green. When
   * publication fails, do the opposite.
   */
  virtual void operator()(const DisplayCommand& command);
};

#endif /* CANDISPLAYCOMMANDPUBLISHER_H_ */
