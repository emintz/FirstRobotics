/*
 * ErrorHalt.h
 *
 *  Created on: Apr 26, 2025
 *      Author: Eric Mintz
 *
 * Error halt, blinks an error code and runs an endless loop that
 * effectively halts the computer.
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

#ifndef ERRORHALT_H_
#define ERRORHALT_H_

class ErrorHalt {
  ErrorHalt() {}
public:

  /*
   * Displays a message no the console, starts an error blink, then
   * effectively halts the machine. This method never returns; reboot
   * is required.
   */
  static void halt_and_catch_fire(int error_code, const char *message);
};

#endif /* ERRORHALT_H_ */
