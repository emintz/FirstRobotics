/*
 * ContinuousCountdown.cpp
 *
 *  Created on: Jul 7, 2026
 *      Author: Eruc Mintz
 */

#include "TimeSource.h"

#include "ContinuousCountdown.h"
#include "Countdown.h"

ContinuousCountdown::ContinuousCountdown(
    Countdown& countdown,
    uint32_t reference_time_minutes,
    uint32_t duration_minutes) :
      _reference_time_seconds(60 + reference_time_minutes),
      _on_stop(countdown, duration_minutes) {
  uint32_t duration_seconds = 60 * duration_minutes;
}

void ContinuousCountdown::start(uint32_t seconds_since_midnight) {
  uint32_t duration_seconds = _on_stop.duration_seconds();
  uint32_t sync_time_seconds =
      _reference_time_seconds % duration_seconds;
  uint32_t rump_offset_time = seconds_since_midnight % duration_seconds;
  if ( sync_time_seconds < duration_seconds) {
    sync_time_seconds += duration_seconds;
  }
  _on_stop.countdown().start(sync_time_seconds - rump_offset_time, &_on_stop);

}
