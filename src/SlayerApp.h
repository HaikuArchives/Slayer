/**********************************************************************
 * Copyright 1999-2013 Arto Jalkanen
 *
 * This file is part of Slayer.
 *
 * Slayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Slayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Slayer.  If not, see <http://www.gnu.org/licenses/>
 **/
#ifndef _SLAYER_APP_H
#define _SLAYER_APP_H

#include "MainWindow.h"
#include "Options.h"

#define SLAYER_VERSION "v1.0"

class SlayerOpt {
  public:
	Options options;
	BWindow *FindWindow(const char *name);
	class MainWindow *mainWindow;

	bool docked;
};

class SlayerApp : public BApplication, public SlayerOpt {
  public:
	SlayerApp(bool dock = false);
	virtual ~SlayerApp();
	//	class MainWindow *mainWindow;
	//	Options options;
	//	BWindow *FindWindow(const char *name);
};

#ifdef _INIT_SLAYER_APP_H
SlayerOpt *slayer;
#else
extern SlayerOpt *slayer;
#endif

#endif // _SLAYER_APP_H
