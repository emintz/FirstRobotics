/*
 * TestPattern.cpp
 *
 *  Created on: May 15, 2025
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

#include "TestPattern.h"

#include "Arduino.h"

#include "LedPanel.h"

static bool wait(
    int millis,
    PullQueueHT<DisplayCommand> &command_queue) {
  DisplayCommand dummy_buffer;
  return (!command_queue.peek_message(&dummy_buffer, millis));
}

static bool horizontal_crawl(
    bool running,
    PullQueueHT<DisplayCommand> &command_queue,
    LedPanel &panel,
    int16_t red,
    int16_t green,
    int16_t blue) {
  if (running) {
    for (int row = 0; running && row < 8; ++row) {
      for (int column = 0; running && column < 32; ++column) {
        panel.clear();
        panel.set(row, column, red, green, blue);
        panel.update();
        running = wait(25, command_queue);
      }
    }
  }
  return running;
}

static bool flood(
    bool running,
    PullQueueHT<DisplayCommand> &command_queue,
    LedPanel &panel,
    int16_t red,
    int16_t green,
    int16_t blue) {
  if (running) {
    panel.flood(red, green, blue);
    panel.update();
    running = wait(500, command_queue);
  }
  return running;
}

TestPattern::~TestPattern() {
}

void TestPattern::operator() (
    PullQueueHT<DisplayCommand> &command_queue,
    const DisplayCommand &command,
    LedPanel &panel) {
  bool running = true;
  while(running) {
    running = flood(
        running,
        command_queue,
        panel,
        63,
        0,
        0);
    running = flood(
        running,
        command_queue,
        panel,
        0,
        63,
        0);
    running = flood(
        running,
        command_queue,
        panel,
        0,
        0,
        63);
    running = horizontal_crawl(
        running,
        command_queue,
        panel,
        63,
        0,
        0);
    running = horizontal_crawl(
        running,
        command_queue,
        panel,
        0,
        63,
        0);
    running = horizontal_crawl(
        running,
        command_queue,
        panel,
        0,
        0,
        63);
  }
}
