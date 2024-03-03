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
#ifndef _THREAD_ITEM_H
#define _THREAD_ITEM_H

#include "TeamListView.h"
#include <InterfaceKit.h>
#include <stdio.h>
#include <string.h>

class ThreadItem : public BRow {
  public:
	int32 refreshed;
	int32 changed;
	static const int32 name_chg = 1, priority_chg = 2, sem_id_chg = 4, state_chg = 0x20;

	char name[B_OS_NAME_LENGTH];
	thread_id thread;
	team_id team;
	thread_state state;
	int32 priority;
	sem_id sem;
	bigtime_t user_time;
	bigtime_t kernel_time;
	bigtime_t CPU_diff;
	float CPU;

	ThreadItem();

	ThreadItem(thread_info *info);
	void update(thread_info *info);
	virtual void
	DrawItemColumn(BView *owner, BRect itemColumnRect, int32 columnIndex, bool complete = false);
	const char *RetrieveStateString(thread_state state);
};

#endif
