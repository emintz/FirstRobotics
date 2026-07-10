/*
 * ShowNoTimeRemaining.h
 *
 *  Created on: Jul 5, 2026
 *      Author: Eric Mintz
 *
 * A VoidFunction that displays 00:00 red on black. This
 * is used at the end of a manually started countdown.
 *
 * Copyright 2026 Metuchen Momentum, LLC.
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

#ifndef SHOWNOTIMEREMAINING_H_
#define SHOWNOTIMEREMAINING_H_

#include "DisplayCommand.h"

#include <PullQueueHT.h>
#include <VoidFunction.h>

class ShowNoTimeRemaining : public VoidFunction {
  const char * const _message;
  PullQueueHT<DisplayCommand>& _command_queue;
public:
  ShowNoTimeRemaining(
      const char * const message,
      PullQueueHT<DisplayCommand>& command_queue) :
      _message(message),
      _command_queue(command_queue) {
  }

  virtual ~ShowNoTimeRemaining() = default;

  virtual void apply(void) override;
};

#endif /* SHOWNOTIMEREMAINING_H_ */
