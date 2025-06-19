/*
 * TimeChangePayload.h
 *
 *  Created on: Jun 9, 2025
 *      Author: Eric Mintz
 *
 * Payload sent to followers over the CAN bus. Note that
 * the message ID determines the display patterns.
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

#ifndef TIMECHANGEPAYLOAD_H_
#define TIMECHANGEPAYLOAD_H_

#include "Arduino.h"

typedef struct {
  uint16_t seconds_since_midnight;
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} TimeChangePayload;

#endif /* TIMECHANGEPAYLOAD_H_ */
