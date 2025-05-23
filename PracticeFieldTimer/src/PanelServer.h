/*
 * PanelServer.h
 *
 *  Created on: May 15, 2025
 *      Author: Eric Mintz
 *
 * Task action that receives display commands and forwards them to
 * a LED panel for display.
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

#ifndef PANELSERVER_H_
#define PANELSERVER_H_

#include "DisplayCommand.h"
#include "DisplayDriver.h"
#include "PullQueueHT.h"
#include "TaskAction.h"

class LedPanel;

class PanelServer : public TaskAction {
  PullQueueHT<DisplayCommand> &queue;
  LedPanel &panel;
public:
  PanelServer(
      PullQueueHT<DisplayCommand> &queue,
      LedPanel &panel);
  virtual ~PanelServer();

  virtual void run(void);
};

#endif /* PANELSERVER_H_ */
