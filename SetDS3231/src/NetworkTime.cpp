/*
 * NetworkTime.cpp
 *
 *  Created on: Dec 31, 2025
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
#include <Arduino.h>

#include "NetworkTime.h"

#include <esp_netif_sntp.h>
#include <esp_sntp.h>
#include <stdlib.h>


#define UNIVERSAL_UTC_TIMEZONE"Etc/UTC"
#define NETWORK_TIME_SERVERS "pool.ntp.org"
#define NTP_START_TIMEOUT_MS 2000

NetworkTime::NetworkTime(void) :
    running(false) {
}

bool NetworkTime::begin(void) {
  bool status = true;
  setenv("TZ", UNIVERSAL_UTC_TIMEZONE, 1);
  esp_sntp_config_t ntp_config =
      ESP_NETIF_SNTP_DEFAULT_CONFIG(NETWORK_TIME_SERVERS);
  ntp_config.smooth_sync = false;
  esp_err_t ntp_status = esp_netif_sntp_init(&ntp_config);
  if (ESP_OK != ntp_status) {
    Serial.printf("SNTP initialization failed with status: %d.\n",
        static_cast<int>(ntp_status));
    status = false;
  }

  if (status) {
    ntp_status = esp_netif_sntp_start();
    if (ESP_OK != ntp_status) {
      Serial.printf("NTP service start failed with status: %d.\n",
          static_cast<int>(ntp_status));
      status = false;
    }
  }

  if (status) {
    ntp_status = esp_netif_sntp_sync_wait(pdMS_TO_TICKS(NTP_START_TIMEOUT_MS));
    if (ESP_OK != ntp_status) {
      Serial.printf(
          "System synchronization failed with status: %d.\n",
          static_cast<int>(ntp_status));
      status = false;
    }
  }
  return running = status;
}
