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
#ifndef _TEAM_ITEM_H
#define _TEAM_ITEM_H

#include "Hashtable.h"
#include "TeamListView.h"
#include "ThreadItem.h"
#include "miscSlayer.h"
#include <stdio.h>

class TeamItem : public BRow
{
  public:
	int32 refreshed;
	int32 changed;
	static const int32 name_chg = 1, areas_chg = 2;

	team_id team;
	char args[64];
	char* name;
	char* fullName;
	int32 area_count;
	size_t memory_usage;
	bigtime_t CPU_diff;
	float CPU;

	BBitmap* team_icon;

	//	ThreadItemList	*thread_items_list;
	Hashtable* thread_items_list;

	TeamItem();

	TeamItem(team_info* info);
	int32 update(team_info* info);
	size_t CountMemory(); // counts memory usage
	virtual void Update(BView* owner, const BFont* font);
	virtual void
	DrawItemColumn(BView* owner, BRect itemColumnRect, int32 columnIndex, bool complete = false);
	virtual ~TeamItem();
};

/*
class TeamItem : public BStringItem {
public:
	team_id		team;
	char args[64];
	char *name;

	ThreadItemList	*thread_items_list;

	TeamItem() : BStringItem("") { thread_items_list = 0; }

	TeamItem(const char *text, uint32 level = 0, bool expanded = true)
		: BStringItem(text, level, expanded)
	{
		thread_items_list = 0;
	}

	TeamItem(team_info *info) : BStringItem("")
	{
		thread_items_list = 0;

		team = info->team;
		strcpy(args, info->args);

		// remove path from team name
		char *tmp;
		for (name = tmp = args; *tmp != 0; tmp++)
			if (*tmp == '/') name = tmp + 1;

		SetText(name);
	}

	virtual ~TeamItem() {
		if (thread_items_list) delete thread_items_list;
	}
};
*/
#endif
