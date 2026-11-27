/*
 * VacuousDisplayCommandPublisher.h
 *
 *  Created on: Jun 9, 2025
 *      Author: Eric Mintz
 *
 * A time change publisher that posts time changes to the local
 * LCD only. Use this class when  CAN is disabled.
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

#ifndef VACUOUSDISPLAYCOMMANDPUBLISHER_H_
#define VACUOUSDISPLAYCOMMANDPUBLISHER_H_

#include "DisplayCommandPublisher.h"

struct DisplayCommand;

class VacuousDisplayCommandPublisher final :
    public DisplayCommandPublisher {

public:
  VacuousDisplayCommandPublisher() = default;

  ~VacuousDisplayCommandPublisher() override = default;

  bool operator()(const DisplayCommand& command_to_publish) override;
};

#endif /* VACUOUSDISPLAYCOMMANDPUBLISHER_H_ */
