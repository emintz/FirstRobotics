/*
 * BypassCalibration.cpp
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

#include "BypassCalibration.h"

static constexpr const char *page_html =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "  <body>\n"
    "    <h1>Time Has Not Been Changed</h1>\n"
    "      <form action='/configuration-unchanged' accept-charset='utf-8'"
    "            name='finish-form' id='finish-form'>\n"
    "      </form>\n"
    "      <form action='/configure-sessions' accept-charset='utf-8'\n"
    "            name='configure-sessions-form' id='configure-sessions-form'>\n"
    "      </form>\n"
    "    <p>The current date and time have not been changed.</p>\n"
    "    <br/>\n"
    "    <button type='submit'\n"
    "            style='background-color:MediumSeaGreen'\n"
    "            form='configure-sessions-form'>Configure Sessions</button>&nbsp;\n"
    "     <button type='submit'\n'"
    "            style='background-color:Red'\n"
    "            form='finish-form'>Finish</button>\n"
    "  </body>\n"
    "</html>\n"
    ;

bool BypassCalibration::handle(
        WebServer& server, HTTPMethod method, const String& uri) {
  server.send(200, "text/html", page_html);
  return true;
}
