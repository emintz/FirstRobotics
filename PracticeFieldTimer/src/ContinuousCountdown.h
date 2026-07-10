/*
 * ContinuousCountdown.h
 *
 *  Created on: Jul 7, 2026
 *      Author: Eric Mintz
 *
 * A countdown that runs continuously, automatically restarting
 * after it reaches 00:00. Countdown starts relative to a reference
 * time expressed in seconds since midnight, UTC
 */

#ifndef CONTINUOUSCOUNTDOWN_H_
#define CONTINUOUSCOUNTDOWN_H_

#include <Arduino.h>

#include "Countdown.h"

#include <VoidFunction.h>

class Countdown;

class ContinuousCountdown {

  /**
   * VoidFunction that starts the count. The countdown invokes this method
   * when the count ends, leading to an endless loop of countdowns, as
   * desired. Lather, rinse, repeat!
   *
   * RestartCount instances are used ONLY within a ContinuousCountdown
   * instance and hold data used by the containing instance.
   */
  class RestartCount : public VoidFunction {
    Countdown& _countdown;
    const uint32_t _duration_in_seconds;
  public:

    /**
     * Construct new instance from the specified values.
     *
     * Parameters:
     *
     * Name                Contents
     * ------------------- -------------------------------------------------
     * countdown           Runs a single countdown and stops
     * duration_in_minutes Session (i.e. countdown) duration in minutes
     *                     MUST be strictly positive (i.e. greater than 0)
     */
    RestartCount(
        Countdown& countdown,
        uint32_t duration_in_minutes) :
          _countdown(countdown),
          _duration_in_seconds(duration_in_minutes * 60) {
    }

    virtual ~RestartCount() = default;

    virtual void apply(void) override {
      _countdown.start_from_ISR(_duration_in_seconds, this);
    }

    Countdown& countdown(void) {
      return _countdown;
    }

    uint32_t duration_seconds(void) {
      return _duration_in_seconds;
    }
  };

  /**
   * Start times MUST be set so that a countdown starts
   * at the reference time, which is seconds since
   * midnight UTC.
   */
  const uint32_t _reference_time_seconds;

  /**
   * Invoked when the currently running countdown ends.
   */
  RestartCount _on_stop;

public:
  /**
   * Constructs an instance from the specified values.
   *
   * Parameters:
   *
   * Name                   Contents
   * ---------------------- --------------------------------------------------
   * countdown              Low level class that performs a single countdown
   *                        then stops.
   * reference_time_minutes Start time anchor in minutes since midnight
   *                        UTC
   * duration_minutes       Session (i.e. countdown) duration in minutes
   */
  ContinuousCountdown(
      Countdown& countdown,
      uint32_t reference_time_minutes,
      uint32_t duration_minutes);

  virtual ~ContinuousCountdown() = default;

  /**
   * Starts a countdown.
   *
   * Parameters:
   *
   * Name                   Contents
   * ---------------------- -------------------------------------------
   * seconds_since_midnight The current time of day in seconds since
   *                        midnight UTC.
   */
  void start(uint32_t seconds_since_midnight);
};

#endif /* CONTINUOUSCOUNTDOWN_H_ */
