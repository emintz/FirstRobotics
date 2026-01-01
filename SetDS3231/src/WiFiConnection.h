/*
 * WiFiConnection.h
 *
 *  Created on: Jun 11, 2025
 *      Author: Eric Mintz
 */

#ifndef WIFICONNECTION_H_
#define WIFICONNECTION_H_

class WiFiConnection final {
public:
  WiFiConnection();

  /*
   * Get the WiFi credentials from the user and connect to the
   * network.
   */
  bool begin(void);
};

#endif /* WIFICONNECTION_H_ */
