/*
 * NotifyTaskOnPinChange.h
 *
 *  Created on: Jun 17, 2025
 *      Author: Eric Mintz
 *
 * Convenience class that creates a task that runs a specified
 * action and notifies that class when a pin level changes.
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

#ifndef NOTIFYTASKONPINCHANGE_H_
#define NOTIFYTASKONPINCHANGE_H_

#include "Arduino.h"
#include "GpioChangeDetector.h"
#include "TaskWithActionH.h"
#include "TaskNotifierVoidFunction.h"

class TaskAction;

class NotifyTaskOnPinChange {

  TaskWithActionH task_to_notify;
  TaskNotifierVoidFunction notifier_function;
  GpioChangeDetector change_detector;
public:
  /*
   * Constructor
   *
   * Parameters:
   * ----------
   *
   * Name                 Contents
   * -----------------    ------------------------------------------------
   * pin                  GPIO pin to monitor
   * change_type          Signal change to detect: HIGH to LOW, LOW to HIGH,
   *                      or both.
   * name                 Name of the task to notify. Note that the class
   *                      creates the task.
   * task_priority        Run priority of the created task
   * on_change            The action that the created task runs. Note
   *                      that the run loop should start by waiting
   *                      for a notification.
   */
  NotifyTaskOnPinChange(
      uint8_t pin,
      GpioChangeType change_type,
      const char *task_name,
      UBaseType_t task_priority,
      TaskAction& on_change);
  virtual ~NotifyTaskOnPinChange();

  /*
   * Start the created task.
   *
   * Returns: true if and only if the task starts successfully.
   */
  bool start(void);
};

#endif /* NOTIFYTASKONPINCHANGE_H_ */
