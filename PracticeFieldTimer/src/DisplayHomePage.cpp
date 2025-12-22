/*
 * SetTimeHandler.cpp
 *
 *  Created on: Dec 9, 2025
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

#include "DisplayHomePage.h"

#include <DS3231TimeSource.h>
#include <string>

static constexpr const char *set_time_html_start =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "  <body>\n"
    "    <h1>Set the Countdown Timer Clock to the Current and Time</h1>\n"
    "    <p>The current time is <span name='local-time' id='local-time'>\n"
    "        local time here</span> (<span name='utc-time' id='utc-time'> UTC here</span>)."
    "    </p>\n"
    "    <p>The countdown date/time is <span name='delta_t' id='delta_t'></span></p>\n"
    "    <form name='set-time' id='set-time' action='/ds3231-calibrated'>\n"
    "      <input type='number' id = 'unix-time-millis' readonly hidden\n"
    "             name = 'unix-time-millis' /><br/>\n"
    "    </form>\n"
    "\n"
    "    <form id='bypass-time-set' action='/bypass-time-calibration'>\n"
    "    </form>\n"
    "    <button type='button'\n"
    "            style='background-color:MediumSeaGreen'\n"
    "            onclick='set_unix_time_millis_and_submit()'\n"
    "       >Calibrate Time</button>&nbsp;\n"
    "    <button\n"
    "        type='submit'\n"
    "        form='bypass-time-set'\n"
    "        style='background-color:red'\n"
    "        >Skip Calibration</button>\n"
    "\n"
    "    <p name='message' id='message'></p>\n"
    "    <script>\n"
    "      ds3231_time = ";
static constexpr const char *set_time_html_end =
    "      ;\ndisplay_current_time();\n"
    "      setInterval('display_current_time()', 1000);\n"
    "      display_delta_t(ds3231_time);\n"
    "\n"
    "      function display_delta_t(ds3231_time) {\n"
    "        delta_t_span = document.getElementById('delta_t');\n"
    "        current_time = new Date().getTime() / 1000;\n"
    "        if (ds3231_time < current_time) {\n"
    "          delta_t_span.innerHTML = '' + Math.round(current_time - ds3231_time) + ' seconds slow.'\n"
    "        } else if (current_time < ds3231_time) {\n"
    "          delta_t_span.innerHTML = '' + Math.round(ds3231_time - current_time) + ' seconds fast.'\n"
    "        } else {\n"
    "           delta_t_span.innerHTML = 'on time.'\n"
    "        }\n"
    "      }\n"
    "\n"
    "      function display_current_time() {\n"
    "        d = new Date();\n"
    "        document.getElementById('local-time').innerHTML = d;\n"
    "        document.getElementById('utc-time').innerHTML = "
    "            d.toUTCString();\n"
    "      }\n"
    "\n"
    "      function set_unix_time_millis_and_submit() {\n"
    "        document.getElementById('unix-time-millis').value = "
    "            new Date().getTime();\n"
    "        document.getElementById('set-time').submit();\n"
    "      }\n"
    "    </script>\n"
    "  </body>\n"
    "</html>\n"
;

bool DisplayHomePage::handle(
    WebServer& server, HTTPMethod method, const String& uri) {
  uint64_t ds3231_unix_time = ds3231.unix_time();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  server.sendContent(set_time_html_start);
  server.sendContent(std::to_string(ds3231_unix_time).c_str());
  server.sendContent(set_time_html_end);
  server.sendContent("");
  return true;
}
