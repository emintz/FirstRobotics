/*
 * NotifyTaskOnPinChange.cpp
 *
 *  Created on: Jun 17, 2025
 *      Author: Eric Mintz
 */

#include "NotifyTaskOnPinChange.h"

NotifyTaskOnPinChange::NotifyTaskOnPinChange(
    uint8_t pin,
    GpioChangeType change_type,
    const char *task_name,
    UBaseType_t task_priority,
    TaskAction& on_change) :
        task_to_notify(
            task_name,
            task_priority,
            &on_change,
            4096),
        notifier_function(
            task_to_notify),
        change_detector(
            pin,
            change_type,
            &notifier_function) {
}

NotifyTaskOnPinChange::~NotifyTaskOnPinChange() {
}

bool NotifyTaskOnPinChange::start(void) {
  return task_to_notify.start()
      && change_detector.start();
}
