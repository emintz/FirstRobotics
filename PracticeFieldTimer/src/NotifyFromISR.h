/*
 * NotifyFromISR.h
 *
 *  Created on: May 20, 2025
 *      Author: Eric Mintz
 *
 * A void function that notifies a task that is determined
 * at construction. This function MUST be used from an
 * interrupt service routine. DO NOT use this in
 * application code.
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

#ifndef NOTIFYFROMISR_H_
#define NOTIFYFROMISR_H_

#include "VoidFunction.h"

class BaseTaskWithAction;

class NotifyFromISR : public VoidFunction {
  BaseTaskWithAction *task;
public:
  NotifyFromISR(BaseTaskWithAction *task);
  virtual ~NotifyFromISR();

  virtual void apply(void) override;
};

#endif /* NOTIFYFROMISR_H_ */
