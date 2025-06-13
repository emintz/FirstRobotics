/*
 * PinAssignments.h
 *
 *  Created on: Apr 26, 2025
 *      Author: Eric Mintz
 *
 * GPIO pin assignments. Every used pin is defined here.
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

#ifndef PINASSIGNMENTS_H_
#define PINASSIGNMENTS_H_

#define BUILTIN_LED_PIN 2

#define SET_CONFIGURATION_PIN 4
#define DISPLAY_FOLLOWER_RED_PIN 5

#define RED_LED_PIN 12
#define GREEN_LED_PIN 13
#define YELLOW_LED_PIN 14

#define LED_ARRAY_PIN 15
#define DISPLAY_FOLLOWER_GREEN_PIN 16
#define DISPLAY_FOLLOWER_BLUE_PIN 17

#define I2C_SCL_PIN 22
#define I2C_SDA_PIN 21

#define FOLLOWER_NOT_PIN 25
#define CAN_ENABLE_NOT_PIN 26

#define CAN_RECEIVE_PIN 32
#define CAN_TRANSMIT_PIN 33
#define SQUARE_WAVE_PIN 34

#endif /* PINASSIGNMENTS_H_ */
