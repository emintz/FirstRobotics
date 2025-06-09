/*
 * TimeChangeHandler.h
 *
 *  Created on: Jun 9, 2025
 *      Author: Eric Mintz
 *
 * Handle incoming time change messages. This handler should
 * be active ONLY on followers. However, since a sending node
 * does not receive its own messages (under normal conditions,
 * anyway), it should be safe to run this on the leader.
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

#ifndef TIMECHANGEHANDLER_H_
#define TIMECHANGEHANDLER_H_

#include "CanPayloadHandler.h"
#include "DisplayCommand.h"
#include "PullQueueHT.h"

class CanBus;
class CanPayload;

class TimeChangeHandler final : public CanPayloadHandler {
  PullQueueHT<DisplayCommand>& panel_command_queue;

public:
  TimeChangeHandler(
      PullQueueHT<DisplayCommand>& panel_command_queue);
  virtual ~TimeChangeHandler();

  virtual void operator() (CanBus& bus, CanPayload& payload);
};

#endif /* TIMECHANGEHANDLER_H_ */
