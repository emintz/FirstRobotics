/*
 * NotifyTaskOnPinChange.h
 *
 *  Created on: Jun 17, 2025
 *      Author: Eric Mintz
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
  NotifyTaskOnPinChange(
      uint8_t pin,
      GpioChangeType change_type,
      const char *task_name,
      UBaseType_t task_priority,
      TaskAction& on_change);
  virtual ~NotifyTaskOnPinChange();

  bool start(void);
};

#endif /* NOTIFYTASKONPINCHANGE_H_ */
