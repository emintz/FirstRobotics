/*
 * WiFiConnection.cpp
 *
 *  Created on: Jun 11, 2025
 *      Author: Eric Mintz
 *
 * Copyright (C) 2026 Eric Mintz
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

#include "WiFiConnection.h"

#include "WiFi.h"

/*
 * Installation-specific, user-provided header containing WiFi
 * credentials. The required definitions are:
 *
 *     WIFI_SSID     Local WIFI network identifier
 *     WIFI_PASSWORD Local WIFI password.
 *
 * The file MUST contain:
 *
 *   #define WIFI_SSID "<Your Network Name>"
 *   #define WIFI_PASSWORD "<Your Network Password>"
 */

#include "WiFiCredentials.h"

WiFiConnection::WiFiConnection() {

}

bool WiFiConnection::begin(void) {
  int attempts = 0;
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (!WiFi.status() != WL_CONNECTED && attempts < 10) {
    vTaskDelay(pdMS_TO_TICKS(500));
    ++attempts;
  }
  Serial.printf("WiFi connection status: %d\n",
      static_cast<int>(WiFi.status()));
  return WiFi.status() == WL_CONNECTED;
}
