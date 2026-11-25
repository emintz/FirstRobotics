/**
 * LcdAction.h
 *
 *  Created on: 11/24/26.
 *      Author: Eric Mintz
 *
 * Action that performs low-level LCD operations over I2C
*
 * Copyright (C) 2026 Eric Mintz
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

#ifndef PRACTICEFIELDTIMER_LCDACTION_H
#define PRACTICEFIELDTIMER_LCDACTION_H

#include <Arduino.h>
#include "LcdMessage.h"
#include <TaskAction.h>
#include <PullQueueHT.h>

class LiquidCrystal_I2C;

class LcdAction : public TaskAction {
  PullQueueHT<LcdMessage>& queue_;
  LiquidCrystal_I2C& lcd_;

  void clear_display() const;

  void clear_field(const LcdMessage& message) const;

  void print(const char *message) const;

  void set_cursor(const LcdMessage& message) const;


  void write_field(const LcdMessage& message) const;

public:
 LcdAction(
  PullQueueHT<LcdMessage>& queue,
  LiquidCrystal_I2C& lcd);

protected:
  virtual void run() override;
};


#endif //PRACTICEFIELDTIMER_LCDACTION_H
