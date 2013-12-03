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

#include <InterfaceKit.h>
#include <string.h>
#include <stdio.h>
#include "TeamListView.h"
#include "CLVEasyItem.h"

class ThreadItem : public CLVEasyItem {
public:
	int32 refreshed;
	int32 changed;
	static const int32 name_chg = 1,
		priority_chg = 2,
		sem_id_chg = 4,
		state_chg = 0x20;
		
	char		name[B_OS_NAME_LENGTH];
	thread_id	thread;
	team_id		team;
	thread_state state;
	int32		priority;
	sem_id		sem;
	bigtime_t	user_time;
	bigtime_t	kernel_time;
	bigtime_t	CPU_diff;
	float		CPU;
	
	ThreadItem();
	
	ThreadItem(thread_info *info);
	void update(thread_info *info);
	virtual void DrawItemColumn(BView *owner, BRect itemColumnRect, int32
		columnIndex, bool complete = false);
};
/*
class ThreadItem : public BStringItem {
public:
	thread_id	thread;
	team_id		team;
	thread_state state;
	int32		priority;
	sem_id		sem;
	bigtime_t	user_time;
	bigtime_t	kernel_time;
	
	ThreadItem() : BStringItem("") {}
	
	ThreadItem(const char *text, uint32 level = 0, bool expanded = true)
		: BStringItem(text, level, expanded)
		{}
		
	ThreadItem(thread_info *info) : BStringItem("")
	{
		thread = info->thread;
		team = info->team;
		state = info->state;
		priority = info->priority;
		sem = info->sem;
		user_time = info->user_time;
		kernel_time = info->kernel_time;
	
		SetText(info->name);	
	}
		
};
*/
#endif
