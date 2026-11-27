/*
 * Countdown.h
 *
 *  Created on: Jul 4, 2026
 *      Author: Eric Mintz
 *
 * Session time countdown. Runs one countdown cycle, stops, and
 * invokes an optional user-provided notification function. Users
 * MAY restart a running countdown. Note that time is measured
 * in seconds. The maximum supported duration is 99:59, one hours
 * and forty minutes. Since the longest session encountered
 * so far is fifteen minutes, that should do.
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

#ifndef COUNTDOWN_H_
#define COUNTDOWN_H_

#include "DisplayCommand.h"
#include "DisplayCommandPublisher.h"
#include "StateTransitionTable.h"

#include <GpioChangeDetector.h>
#include <PullQueueHT.h>
#include <TaskAction.h>
#include <VoidFunction.h>

#include <memory>

#include "fl/stl/unique_ptr.h"

class Countdown : public TaskAction {
  /**
   * Commands, a.k.a. event types. Directs countdown
   * behavior.
   */
  typedef enum class Command_ {
    /**
     * @brief Start a countdown immediately
     *
     * If a countdown is in progress, stop it and reset. Start
     * a new countdown as directed.
     */
    START,
    /**
     * @brief Start a countdown after a delay.
     */
    START_AFTER_DELAY,
    /**
     * @brief Process a clock tick
     *
     * Ticks arrive every second. Decrement and display the remaining
     * time, and stop if the time has expired (i.e., reached 0 seconds).
     */
    TICK,
    /**
     * Stop counting down. This is a self-directed event only.
     */
    STOP,
  } Command;

  struct CommandMessage {
    /**
     * What to do.
     */
    Command command_;
    /**
     * How long to do it -- only relevant to START
     */
    uint16_t duration_;
    /**
     * Whom to notify when done -- only relevant to START.
     */
    VoidFunction *on_stop_;
  };

  /**
   * States. State entry is bound to an action.
   */
  typedef enum class State_ {
    /**
     * Initial state. Does nothing on entry.
     */
    CREATED,
    /**
     * Starting a new countdown. Binds the end of count notification
     * function (which MAY be user-provided), starts pulse per second
     * detection, and displays the session duration.
     */
    STARTING,
    /**
     * Starting a new countdown with a delay. Waits for hardware
     * (read: start button) to settle, clear spurious pending
     * messages, waits for a tick, then starts counting down.
     */
    STARTING_WITH_DELAY,
    /**
     * Restarting a running countdown. Stops (see STOPPING) then
     * starts (see STARTING).
     */
    RESTARTING,
    /**
     * Restarting a running countdown with delay. Stops (see STOPPING) then
     * starts with delay (see STARTING_WITH_DELAY).
     */
    RESTARTING_WITH_DELAY,
    /**
     * State that ignores a tick. Ignores START_AFTER_DELAY to
     * suppress button bounce.
     */
    IGNORING_TICK,
    /**
     * Actively counting down. Decrements the count and displays
     * the new value. Posts a STOP event when the available time
     * reaches 0. Ignores START_AFTER_DELAY to suppress button
     * bounce.
     */
    COUNTING,
    /**
     * Count ended. Stops pulse per second detection, invokes and
     * unbinds the end of count notification function, and waits
     * for some action.
     */
    STOPPING,
    /**
     * Sentinel that indicates the incoming event should be
     * ignored. The state machine never enters this state.
     */
    IGNORING_EVENT,
  } State;

  static CommandMessage on_tick_command_;

  class OnTick : public VoidFunction {
    PullQueueHT<CommandMessage>& countdown_command_queue_;
  public:
    OnTick(PullQueueHT<CommandMessage>& countdown_command_queue) :
        countdown_command_queue_(countdown_command_queue) {
    }

    void apply() override {
      countdown_command_queue_.send_message(&on_tick_command_, 1);
    }
  };

  static StateTransitionTable<Command, State> _transition_table;

  VoidFunction *on_stop_;
  const uint16_t slow_blink_seconds_;
  const uint16_t fast_blink_seconds_;
  const uint16_t slow_blink_start_time_;
  PullQueueHT<DisplayCommand>& display_command_queue_;
  PullQueueHT<CommandMessage> internal_event_queue_;
  OnTick on_tick_;
  GpioChangeDetector sqw_detector_;
  uint16_t duration_;
  uint16_t color_change_halfway_mark_;
  uint16_t time_remaining_;
  std::unique_ptr<DisplayCommandPublisher> display_command_publisher_;

  /**
   * Ignominiously cancels a countdown and clears the display.
   * Cancellation is immediate and does not invoke _on_stop.
   * Used to stop a count during a restart.
   */
  void cancel_count();

  /**
   * Sets the display pattern -- solid, slow blink, or fast blink --
   * based on the time remaining.
   */
  [[nodiscard]] DisplayCommand::Pattern display_pattern() const;

  /**
   * Display the remaining time.
   */
  void display_remaining_time() const;

  /**
   * Sets the foreground color based on time remaining. The
   * caller must initialize all intensities to 0.
   */
  void foreground(DisplayCommand& display_command) const;

  /**
   * Waits for the system to settle (read: start switch to
   * stabilize) then starts a count.
   */
  void handle_delayed_start(const CommandMessage& command);

  /**
   * Starts a count.
   */
  void handle_start(const CommandMessage& command);

  /**
   * Handle a stop event. Stop the GPIO change detector, invoke the
   * notification function, and clear the duration.
   */
  void handle_stop();

  /**
   * Handle a tick. Decrement and display the time remaining. Stop
   * when expires.
   */
  void handle_tick();

  /**
   * Starts the internal queue. Prints an error message and hangs
   * if startup fails. TODO: better behavior!
   */
  void start_internal_queue();

protected:

  /**
   * Task logic.
   */
  void run() override;

public:
  /**
   * Constructs a newly initialized instance.
   *
   * Parameters:
   * ----------
   *
   * Name                  Contents
   * -----------------     ------------------------------------------------
   * sqw_pin               GPIO pin that receives a precise 1Hz square wave,
   *                       the count down driver.
   * slow_blink_seconds    Number of seconds to slow blink the time before
   *                       starting the fast blink. This is ONLY the slow
   *                       blink duration that DOES NOT include the fast blink.
   * fast_blink_seconds    Number of seconds to fast blink the time before
   *                       the session expires. Must be > 0.
   * display_command_queue Carries display commands to the panel server.
   * command_publisher     Publishes the command to the CAN bus if the
   *                       latter is running.
   */
  Countdown(
      uint8_t sqw_pin,
      uint16_t slow_blink_seconds,
      uint16_t fast_blink_seconds,
      PullQueueHT<DisplayCommand>& display_command_queue
      );
  /**
   * Forbid copying
   */
  Countdown(Countdown&) = delete;

  ~Countdown() override = default;

  /**
   * Forbid assignment.
   */
  Countdown& operator=(const Countdown&) = delete;

  /**
   * Global initializer. Be sure to invoke this before creating
   * an instance.
   */
  static void begin();

  void set_command_publisher(
    std::unique_ptr<DisplayCommandPublisher> publisher) {
    display_command_publisher_ = std::move(publisher);
  }

  /**
   * Starts a countdown immediately. Invoke ONLY from application
   * code.
   *
   * @param duration_in_seconds duration of the countdown in
   *        seconds, typically 720 (12 minutes) in automatic
   *        mode and 90 (i5 minutes) in manual mode.
   * @param on_stop invoked when the countdown stops. Provide
   *        a vacuous implementation for manual mode, and an
   *        implementation that restarts the countdown in
   *        auto mode.
   * @return true if the countdown started successfully, false otherwise
   */
  bool start(uint16_t duration_in_seconds, VoidFunction *on_stop);

  /**
   * Starts a countdown after waiting a tick. Used to start a countdown
   * in response to a start/cfg button press. The delay allows for
   * button bounce. Invoke ONLY from application code.
   *
  * @param duration_in_seconds duration of the countdown in
   *        seconds, typically 720 (12 minutes) in automatic
   *        mode and 90 (i5 minutes) in manual mode.
   * @param on_stop invoked when the countdown stops. Provide
   *        a vacuous implementation for manual mode, and an
   *        implementation that restarts the countdown in
   *        auto mode.
   * @return true if the countdown started successfully, false otherwise
   */
  bool start_after_delay(uint16_t duration_in_seconds, VoidFunction *on_stop);

  /**
   * Starts a countdown immediately. Invoke ONLY from an interrupt
   * service routing.
   *
   * @param duration_in_seconds duration of the countdown in
   *        seconds, typically 720 (12 minutes) in automatic
   *        mode and 90 (i5 minutes) in manual mode.
   * @param on_stop invoked when the countdown stops. Provide
   *        a vacuous implementation for manual mode, and an
   *        implementation that restarts the countdown in
   *        auto mode.
   * @return true if the countdown started successfully, false otherwise
   */
  bool start_from_ISR(uint16_t duration_in_seconds, VoidFunction *on_stop);

  /**
   * Starts a countdown after waiting a tick. Used to start a countdown
   * in response to a start/cfg button press. The delay allows for
   * button bounce. Invoke ONLY from an interrupt service
   * routine..
   *
  * @param duration_in_seconds duration of the countdown in
   *        seconds, typically 720 (12 minutes) in automatic
   *        mode and 90 (i5 minutes) in manual mode.
   * @param on_stop invoked when the countdown stops. Provide
   *        a vacuous implementation for manual mode, and an
   *        implementation that restarts the countdown in
   *        auto mode.
   * @return true if the countdown started successfully, false otherwise
   */
  bool start_after_delay_from_ISR(uint16_t duration_in_seconds, VoidFunction *on_stop);
};

#endif /* COUNTDOWN_H_ */
