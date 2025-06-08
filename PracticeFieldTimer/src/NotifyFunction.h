/*
 * NotifyFunction.h
 *
 *  Created on: May 20, 2025
 *      Author: Eric Mintz
 *
 * A void function that notifies a task that is set during
 * construction. This function MUST be used only in application
 * code. DO NOT use this in an interrupt service routine.
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

#ifndef NOTIFYFUNCTION_H_
#define NOTIFYFUNCTION_H_

#include "VoidFunction.h"

class BaseTaskWithAction;

class NotifyFunction : public VoidFunction {
  BaseTaskWithAction *task;
public:
  NotifyFunction(
      BaseTaskWithAction *task);
  virtual ~NotifyFunction();

  virtual void apply(void) override;
};

#endif /* NOTIFYFUNCTION_H_ */
