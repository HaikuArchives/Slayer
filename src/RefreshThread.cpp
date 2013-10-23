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
#include "RefreshThread.h"
#include <time.h>

RefreshThread::RefreshThread()
{
	thread_id = -1;
}

RefreshThread::~RefreshThread()
{
	if (thread_id == -1) return;
	Kill();
	status_t exit_value;
	wait_for_thread(thread_id, &exit_value);
}

void RefreshThread::Kill()
{
	kill_thread(thread_id);
	thread_id = -1; // no thread anymore
}

void RefreshThread::Go()
{
	if (thread_id != -1) return;
	wnd = slayer->mainWindow;
	delay = slayer->options.refresh;
		
	thread_id = spawn_thread(thread_func, "RefreshTeams",
	           B_NORMAL_PRIORITY, (void *)this);
	resume_thread(thread_id);
}

int32 RefreshThread::thread_func(void *arg)
{
	// snoozing does good for you
	snooze(10);
	((RefreshThread *)arg)->DoWork();
	return 0;
}
void RefreshThread::DoWork()
{
	clock_t last = clock();
	
	while (true) {
		if (delay) 
			snooze(1000*(delay - (clock() - last)*1000/CLOCKS_PER_SEC));
		last = clock();
		
		if (wnd->Lock()) {
			wnd->UpdateTeams();
			wnd->Unlock();
		}
	}
}
