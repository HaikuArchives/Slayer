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
#ifndef _REFRESH_THREAD_H
#define _REFRESH_THREAD_H
#include "SlayerApp.h"
#include "OS.h"

class RefreshThread {
	static int32 thread_func(void *arg);
	int32 thread_id;
	int32 delay;
	class MainWindow *wnd;
	void DoWork();
public:
	RefreshThread();
	~RefreshThread();
	void Go();
	void Kill();
};

#endif
