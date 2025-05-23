/*
 * WS2812B8x32vertical.cpp
 *
 *  Created on: May 14, 2025
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

#include "WS2812B8x32vertical.h"

#include "PinAssignments.h"

WS2812B8x32vertical::LedIndexer::LedIndexer(void) {
  int16_t current_offset = 256;
  for (int i = 0; i < 32; ++i) {
    current_offset -= ((i & 1) ? 15 : 1);
    column_offset[i] = current_offset;
  }
}

int16_t WS2812B8x32vertical::LedIndexer::offset_of(int row, int column) const {
    if (!(column & 1)) {
      row = -row;
    }
    return column_offset[column] + row;
}

WS2812B8x32vertical::LedIndexer WS2812B8x32vertical::row_column_to_index;

WS2812B8x32vertical::WS2812B8x32vertical() {
  clear();
  FastLED.addLeds<WS2812, LED_ARRAY_PIN, GRB>(leds, 256);
}

WS2812B8x32vertical::~WS2812B8x32vertical() {
}

int WS2812B8x32vertical::rows(void) const {
  return 8;
}

int WS2812B8x32vertical::columns(void) const {
  return 32;
}

LedPanel& WS2812B8x32vertical::set(int row, int column, int red, int green, int blue) {
  if (0 <= row && row < 8 && 0 <= column && column < 32) {
  int16_t offset = row_column_to_index.offset_of(row, column);
    leds[offset] = CRGB(red, green, blue);
  } else {
    Serial.printf("Coordinates out of bound: (%d, %d)\n",
        row,
        column);
    Serial.flush();
  }
  return *this;
}

LedPanel& WS2812B8x32vertical::flood(int red, int green, int blue) {
  CRGB fill_color(red, green, blue);

  for (int offset = 0; offset < 256; ++ offset) {
    leds[offset] = fill_color;
  }
  return *this;
}

void WS2812B8x32vertical::update(void) {
  vTaskDelay(pdMS_TO_TICKS(1));
  FastLED.show();

}
