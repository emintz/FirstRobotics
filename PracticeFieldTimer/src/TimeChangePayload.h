/*
 * TimeChangePayload.h
 *
 *  Created on: Jun 9, 2025
 *      Author: Eric Mintz
 *
 * Payload sent to followers over the CAN bus. Note that
 * the message ID determines the display patterns.
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
