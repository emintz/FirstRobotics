/*
 * CANDisplayCommandPublisher.h
 *
 *  Created on: Jun 9, 2025
 *      Author: Eric Mintz
 *
 * Sends the display time to followers. Only the
 * leading countdown timer invoke this class.
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

#ifndef CANDISPLAYCOMMANDPUBLISHER_H_
#define CANDISPLAYCOMMANDPUBLISHER_H_

#include "DisplayCommandPublisher.h"
#include "OneShotBlink.h"
#include "StatusLcd.h"

#include <PullQueueHT.h>

class CanBus;
struct DisplayCommand;

class CANDisplayCommandPublisher final :
    public DisplayCommandPublisher {

  CanBus& can_bus_;
  PullQueueHT<OneShotBlinkCommand>& rj45_blink_queue_;
  StatusLcd& status_display_;
public:

  /*
   * Constructor
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * can_bus              Where to publish the command
   * rj45_blink_queue     Carries commands that blink the LEDs on the
   *                      RJ45 jack.
   * status_display       LCD status panel
   */
  CANDisplayCommandPublisher(
      CanBus& can_bus,
      PullQueueHT<OneShotBlinkCommand>& rj45_blink_queue,
      StatusLcd& status_display);

  virtual ~CANDisplayCommandPublisher() = default;

  /*
   * Publishes a display command to the CAN bus and show
   * publication status on LEDs. When publication succeeds,
   * turn off the red LED and illuminate the green. When
   * publication fails, do the opposite.
   */
  bool operator()(const DisplayCommand& command) override;
};

#endif /* CANDISPLAYCOMMANDPUBLISHER_H_ */
