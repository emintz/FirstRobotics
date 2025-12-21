/*
 * DS3231CalibratedPage.cpp
 *
 *  Created on: Dec 10, 2025
 *      Author: Eric Mintz
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

#include "DS3231CalibratedPage.h"

#include <ctime>
#include <DS3231TimeSource.h>
#include <string>
#include <time.h>
#include <WebServer.h>

static constexpr const char *page_start =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "  <body>\n"
    "    <h1>Countdown timer calendar calibrated</h1>\n";

static constexpr const char *page_end =
    "      <form action='/configuration-unchanged' accept-charset='utf-8'"
    "            name='finish-form' id='finish-form'>\n"
    "      </form>\n"
    "      <form action='/configure-sessions' accept-charset='utf-8'\n"
    "            name='configure-sessions-form' id='configure-sessions-form'>\n"
    "      </form>\n"
    "    <button type='submit'\n"
    "            style='background-color:MediumSeaGreen'\n"
    "            form='configure-sessions-form'>Configure Sessions</button>\n"
    "     &nbsp;\n"
    "     <button type='submit'\n'"
    "            style='background-color:Red'\n"
    "            form='finish-form'>Finish</button>\n"
    "  </body>\n"
    "</html>\n"
;

static const String unix_time_millis("unix-time-millis");

static constexpr const char *paragraph_end = "</p>\n";
static constexpr const char *paragraph_start = "<p>\n";

bool DS3231CalibratedPage::handle(
    WebServer& server, HTTPMethod method, const String& uri) {
  Serial.printf("In DS3231CalibratedPage::handle, uri: %s\n", uri.c_str());
  auto time_interval = server.arg(unix_time_millis);
  const char *append_me = time_interval.c_str();
  uint64_t numeric_time_interval_millis = std::stoull(append_me);
  uint64_t numeric_time_interval_seconds = numeric_time_interval_millis / 1000;
  auto time_interval_seconds = std::to_string(numeric_time_interval_seconds);
  const time_t time_t_unix_time_seconds = numeric_time_interval_seconds;

  String delta_t_message;
  uint64_t ds3231_unix_time = ds3231.unix_time();
  time_t ds3231_time_t_unix_time = static_cast<time_t>(ds3231_unix_time);
  auto ds3231_unix_time_string = std::to_string(ds3231_unix_time);

  std::string difference;

  ds3231.set_date_and_time(numeric_time_interval_seconds);
  time_t time_t_new_time = static_cast<time_t>(ds3231.unix_time());

  if (numeric_time_interval_seconds < ds3231_unix_time) {
    difference
        .append(std::to_string(ds3231_unix_time - numeric_time_interval_seconds))
        .append(" seconds ahead of");
  } else if (ds3231_unix_time < numeric_time_interval_seconds) {
    difference
        .append(std::to_string(numeric_time_interval_seconds - ds3231_unix_time))
        .append(" seconds behind");
  } else {
    difference.append("equal to");
  }

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  server.sendContent(page_start);
  server.sendContent("<p>Countdown timer calendar set to ");  // TODO: add to page_start
  server.sendContent(asctime(std::gmtime(&time_t_new_time)));
  server.sendContent(".</p>\n");
  server.sendContent("<p><small>Submitted at ");
  server.sendContent(asctime(std::gmtime(&time_t_unix_time_seconds)));
  server.sendContent(" UTC.</small></p>\n");
  server.sendContent(page_end);
  server.sendContent("");
  return true;
}
