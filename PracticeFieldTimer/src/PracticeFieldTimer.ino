/*
 * PracticeFieldTimer.ino
 *
 *  Created on: Apr 26, 2025
 *      Author: Eric Mintz
 *
 * Main program for the FIRST Robotics competition practice field.
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
#include "Arduino.h"

#include "BlinkErrorCodes.h"
#include "Configurator.h"
#include "ContinuousCountdown.h"
#include "DataTypes.h"
#include "DisplayCommand.h"
#include "DS3231TimeSource.h"
#include "ErrorHalt.h"
#include "Flash32.h"
#include "PanelServer.h"
#include "PinAssignments.h"
#include "PressAndHold.h"
#include "PullQueueHT.h"
#include "TaskPriorities.h"
#include "TaskWithActionH.h"
#include "WS2812B8x32vertical.h"

#include <WiFi.h>
#include <Wire.h>

#include <memory>

// User must press and hold the configuration reset button
// for three seconds within 5 seconds after power on.
#define PRESS_TIMEOUT_MS 3000
#define PRESS_WINDOW_MS 5000

// WiFi access point configuration
static constexpr const char *ssid = "FieldTimer";
static constexpr const char *password = "FirstRobotics";
static const IPAddress local_ip(192, 168, 251, 4);
static const IPAddress gateway(192, 168, 251, 1);
static const IPAddress subnet(255, 255, 255, 0);

static int32_t reference_time_utc = 0;
static int32_t auto_field_time_minutes = 0;
static int32_t manual_field_time_minutes = 0;

static int16_t time_remaining = 0;

// We want the time display to turn gradually from blue to
// red as time elapses. We want the display to be completely
// red for the last minute. setup() will set these constants
// to enable this.
static int16_t all_red_period = 60;

static constexpr const char *nvs_namespace = "field-timer";

static DS3231_TimeSource ds3231;
static PullQueueHT<DisplayCommand> command_queue(4);
static WS2812B8x32vertical panel;
static PanelServer server(command_queue, panel);
static TaskWithActionH server_task(
    "Panel",
    PANEL_PRIORITY,
    &server,
    4096);

//static VacuousVoidFunction do_nothing;

static std::unique_ptr<ContinuousCountdown> continuous_count_down;

static void blink_it(int pin_no) {
  for (int i = 0; i < 4; ++i) {
    digitalWrite(pin_no, HIGH);
    vTaskDelay(pdMS_TO_TICKS(125));
    digitalWrite(pin_no, LOW);
    vTaskDelay(pdMS_TO_TICKS(125));
  }
}

static void wifi_on(void) {
  if (!WiFi.mode(WIFI_STA) || !WiFi.begin(ssid, password)) {
    ErrorHalt::halt_and_catch_fire(
        WIFI_ACCESS_POINT_SETUP_FAILED,
        "Could not configure WIFI as an access_point");
  }
  if (!WiFi.softAPConfig(local_ip, gateway, subnet)) {
    ErrorHalt::halt_and_catch_fire(
        WIFI_IP_SETUP_FAILED,
        "Could not configure WIFI IP address.");
  }
}

static void set_configuration_parameters(void) {
  Configurator configurator(nvs_namespace);
  DataTypes data_types;
  configurator
      .add_field(
          "ref-time",
          "Auto Mode Reference Time (24 Hour UTC)",
          data_types.int32)
      .add_field(
          "auto-time",
          "Auto Mode Cycle Time (Minutes)",
          data_types.int32)
      .add_field(
          "manual-time",
          "Manual Mode Cycle Time (Minutes)",
          data_types.int32);
  if (!configurator.run()) {
    ErrorHalt::halt_and_catch_fire(
        SYSTEM_NOT_CONFIGURED,
        "Timer configuration failed.");
  }
  vTaskDelay(pdMS_TO_TICKS(2000));
}

static void wifi_off(void) {
  if (!WiFi.mode(WIFI_OFF)) {
    ErrorHalt::halt_and_catch_fire(
        WIFI_SHUTDOWN_FAILED, "Could not turn WIFI off.");
  }
}

static void configure_if_requested(void) {
  pinMode(SET_CONFIGURATION_PIN, INPUT_PULLUP);
  PressAndHold configuration_signal(
      SET_CONFIGURATION_PIN,
      PRESS_TIMEOUT_MS,
      PRESS_WINDOW_MS);
  if (configuration_signal.wait_for_press_and_hold()) {
      wifi_on();
      digitalWrite(BUILTIN_LED_PIN, HIGH);
      set_configuration_parameters();
      digitalWrite(BUILTIN_LED_PIN, LOW);
      wifi_off();
  }
}

static void read_eeprom(
    Flash32ReadOnlyNamespace &eeprom,
    const char *key,
    int32_t *value) {
  switch (eeprom.get_int32(key, value)) {
  case Flash32Status::OK:
    break;
  case Flash32Status::NOT_FOUND:
    ErrorHalt::halt_and_catch_fire(
        INCOMPLETE_CONFIGURATION,
        "Missing configuration parameter.");
  default:
    ErrorHalt::halt_and_catch_fire(
        CORRUPT_CONFIGURATION,
        "EEPROM corrupt.");
  }
}

static void load_configuration(void) {
  Serial.println("Loading configuration.");
  if (!Flash32.begin()) {
    ErrorHalt::halt_and_catch_fire(
        NON_VOLATILE_OPEN_FAILED,
        "EEPROM open failed.");
  }
  Flash32ReadOnlyNamespace eeprom(nvs_namespace);
  if (eeprom.open() != Flash32Status::OK) {
    ErrorHalt::halt_and_catch_fire(
        CONFIG_NAMESPACE_OPEN_FAILED,
        "EEPROM namespace failed to open.");
  }
  Serial.printf("Opened \"%s\" namespace.\n", eeprom.get_name());
  read_eeprom(eeprom, "ref-time", &reference_time_utc);
  read_eeprom(eeprom, "auto-time", &auto_field_time_minutes);
  read_eeprom(eeprom, "manual-time", &manual_field_time_minutes);
  if (eeprom.close() != Flash32Status::OK) {
    ErrorHalt::halt_and_catch_fire(
        CONFIG_NAMESPACE_CLOSE_FAILED,
        "EEPROM namespace failed to close");
  }

  if (!Flash32.end()) {
    ErrorHalt::halt_and_catch_fire(
        NON_VOLATILE_CLOSE_FAILED,
        "EEPROM close failed.");
  }

  Serial.printf(
      "Reference time: %d UTC\n",
      reference_time_utc);
  Serial.printf(
      "Field time in automatic mode: %d minutes\n",
      auto_field_time_minutes);
  Serial.printf(
      "Field time in manual mode: %d minutes\n",
      manual_field_time_minutes);
}

static void start_automatic_countdown(void) {
  int16_t auto_field_time_seconds = 60 * auto_field_time_minutes;
  int16_t reference_time_utc_seconds = 60 * reference_time_utc;
  continuous_count_down = std::make_unique<ContinuousCountdown>(
      SQUARE_WAVE_PIN,
      auto_field_time_seconds,
      all_red_period,
      reference_time_utc_seconds,
      command_queue);

  int16_t sync_time_seconds =
      reference_time_utc_seconds % auto_field_time_seconds;
  int16_t rump_offset_time =
      ds3231.seconds_since_midnight() % auto_field_time_seconds;
  if (sync_time_seconds < rump_offset_time) {
    sync_time_seconds += auto_field_time_seconds;
  }
  continuous_count_down->enable(
      sync_time_seconds - rump_offset_time);
}

void setup() {
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  digitalWrite(BUILTIN_LED_PIN, LOW);
  pinMode(RED_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(GREEN_LED_PIN, LOW);
  pinMode(SQUARE_WAVE_PIN, INPUT);

  Serial.begin(115200);
  Serial.println("Serial I/O initialized.");

  blink_it(BUILTIN_LED_PIN);
  blink_it(RED_LED_PIN);
  blink_it(GREEN_LED_PIN);

  if (!Wire.begin()) {
    ErrorHalt::halt_and_catch_fire(
        I2C_STARTUP_FAILED,
        "Two-Wire (I2C) bus initialization failed.");
  }

  if (!ds3231.begin()) {
    ErrorHalt::halt_and_catch_fire(
        DS3231_STARTUP_FAILED,
        "DS3231 real-time clock initialization failed.");
  }

  if (!GpioChangeService.begin()) {
    ErrorHalt::halt_and_catch_fire(
        GPIO_INTERRUPT_STARTUP_FAILED,
        "GPIO change interrupt service startup failed.");
  }

  Serial.println("Services started.");

  int seconds_since_midnight = ds3231.seconds_since_midnight();
  int hours = seconds_since_midnight / 3600;
  int minutes = (seconds_since_midnight - hours * 3600) / 60;
  int seconds = seconds_since_midnight % 60;

  Serial.printf("Seconds since midnight: %d (%d:%d:%d UTC).\n",
      seconds_since_midnight,
      hours,
      minutes,
      seconds);

  configure_if_requested();

  load_configuration();

  if (!command_queue.begin()) {
    ErrorHalt::halt_and_catch_fire(
        COMMAND_QUEUE_BEGIN_FAILED,
        "Display command queue startup failed.");
  }
  Serial.println("Display command queue started.");

  server_task.start();
  Serial.println("Panel server task started.");

  DisplayCommand display_test_pattern;
  memset(&display_test_pattern, 0, sizeof(display_test_pattern));
  display_test_pattern.command = DisplayCommand::Pattern::TEST_PATTERN;
  command_queue.send_message(&display_test_pattern);

  vTaskDelay(pdMS_TO_TICKS(10000));

  start_automatic_countdown();

  Serial.println("Setup complete.");
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}
