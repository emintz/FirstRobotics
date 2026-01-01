/*
 * WiFiConnection.cpp
 *
 *  Created on: Jun 11, 2025
 *      Author: Eric Mintz
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
  Serial.printf("WiFi credentials, ssid: %s, password: %s\n",
      WIFI_SSID, WIFI_PASSWORD);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (!WiFi.status() != WL_CONNECTED && attempts < 10) {
    vTaskDelay(pdMS_TO_TICKS(500));
    ++attempts;
  }
  Serial.printf("WiFi connection status: %d\n",
      static_cast<int>(WiFi.status()));
  return WiFi.status() == WL_CONNECTED;
}
