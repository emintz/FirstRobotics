/*
 * BlinkErrorCodes.h
 *
 *  Created on: Apr 26, 2025
 *      Author: Eric Mintz
 *
 * Provides blink error codes. When a fatal error occurs, the timer
 * blinks the built in LED and enters an endless loop. The LED
 * indicates the error by blinking one of the codes defined
 * below. For example, if WiFi could not be configured, the
 * LED will blink once.
 *
 * Copyright(c) 2025, Eric Mintz
 * All Rights Reserved
 *
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

#ifndef BLINKERRORCODES_H_
#define BLINKERRORCODES_H_

#define WIFI_ACCESS_POINT_SETUP_FAILED 1
#define WIFI_IP_SETUP_FAILED 2
#define WIFI_SHUTDOWN_FAILED 3
#define SYSTEM_CONFIGURATION_FAILED 4
#define SYSTEM_NOT_CONFIGURED 5
#define NON_VOLATILE_OPEN_FAILED 6
#define CONFIG_NAMESPACE_OPEN_FAILED 7
#define SYSTEM_NOT_CONFIGURED 8
#define CORRUPT_CONFIGURATION 9
#define INCOMPLETE_CONFIGURATION 10
#define CONFIG_NAMESPACE_CLOSE_FAILED 11
#define NON_VOLATILE_CLOSE_FAILED 12
#define COMMAND_QUEUE_BEGIN_FAILED 13
#define UNKNOWN_PANEL_DRIVER 14
#define I2C_STARTUP_FAILED 15
#define DS3231_STARTUP_FAILED 16
#define GPIO_INTERRUPT_STARTUP_FAILED 17
#define GPIO_CHANGE_STARTUP_FAILED 18

#endif /* BLINKERRORCODES_H_ */
