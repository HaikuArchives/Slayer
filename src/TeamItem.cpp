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
#include "TeamItem.h"
#include "MainWindow.h"
#include "RgbColor.h"
#include <stdlib.h>
#include <private/shared/StringForSize.h>
#include <ColumnTypes.h>

TeamItem::TeamItem() : BRow() { thread_items_list = 0; }
	
TeamItem::TeamItem(team_info *info) : BRow()
{
	team_icon = NULL;
//	thread_items_list = new ThreadItemList;
	thread_items_list = new Hashtable;
	
	team = info->team;
	area_count = info->area_count;
	// memory_usage = 0;
	if (slayer->options.shown_columns & Options::memory_col)
		memory_usage = CountMemory();
	else
		memory_usage = 0;
	
	char str[21];
	get_app_info(team, &team_icon, &name);
	if (name == NULL) {
		// if couldn't get app name from BRoster, use args
		strcpy(args, info->args);
	// remove path from team name
		char *tmp;
		for (name = tmp = args; *tmp != 0; tmp++)
			if (*tmp == '/') name = tmp + 1;
	}
	else args[0] = 0;
	/*
	SetColumnContent(TeamListView::icon_ndx, team_icon, 2.0, false);
	SetColumnContent(TeamListView::name_ndx, name, false);
	sprintf(str, "%ld", team);
	SetColumnContent(TeamListView::id_ndx, str, false);
	SetColumnContent(TeamListView::priority_ndx, "-", false);
	SetColumnContent(TeamListView::state_ndx, "-", false);

	if (memory_usage <= 0) strcpy(str,"-");
	else string_for_size(memory_usage, str, sizeof(str));

	SetColumnContent(TeamListView::areas_ndx, str, false);

	SetColumnContent(TeamListView::CPU_ndx, "-", false); */

	int32 i = 0;
	SetField(new BBitmapField(team_icon), i++);
	SetField(new BStringField(name), i++);
	sprintf(str, "%ld", team);
	SetField(new BStringField(str), i++);
	SetField(new BStringField("-"), i++);
	SetField(new BStringField("-"), i++);
	SetField(new BSizeField(memory_usage < 0 ? 0 : memory_usage), i++);
	SetField(new BIntegerField(0), i++);
	
	changed = 0;
}

int32 TeamItem::update(team_info *info)
{
	// team id is always the same, no checking done for that
/*	if (strcmp(args, info->args)) {
		strcpy(args, info->args);
		// remove path from team name
		char *tmp;
		for (name = tmp = args; *tmp != 0; tmp++)
			if (*tmp == '/') name = tmp + 1;
	
		SetColumnContent(TeamListView::name_ndx, name, false);
		
		changed |= name_chg;
	} */
	
	// no updating unless necessary
	if (slayer->options.shown_columns & Options::memory_col) {
		size_t mem = CountMemory();
		if (mem != memory_usage) {
	
			area_count = info->area_count;
			memory_usage = mem;
/*
			char str[21];
			string_for_size(memory_usage, str, sizeof(str));
	
			SetColumnContent(TeamListView::areas_ndx, str, false);
*/
			//SetField(new BSizeField(memory_usage < 0 ? 0 : memory_usage), 5);
			BSizeField* memoryField = (BSizeField*)GetField(5);
			if (memoryField->Size() != memory_usage) {
				memoryField->SetSize(memory_usage < 0 ? 0 : memory_usage);
				changed |= areas_chg;
			}
		}
	}

	//SetField(new BIntegerField(CPU * 100), 6);
	//((BIntegerField*)GetField(6))->SetValue(CPU*100);
	return changed;
}

size_t TeamItem::CountMemory() {
	area_info info;
	ssize_t cookie = 0;
	size_t mem = 0;

	while (get_next_area_info(team, &cookie, &info) == B_OK)
		mem += info.ram_size;
		
	return mem;
}

void TeamItem::Update(BView *owner, const BFont *font)
{

}

void TeamItem::DrawItemColumn(BView *owner, BRect itemColumnRect, int32
		columnIndex, bool complete = false)
{ /*
	if (columnIndex != TeamListView::CPU_ndx)
		return CLVEasyItem::DrawItemColumn(owner, itemColumnRect, columnIndex,
			complete);
			
	BRect colRect = ItemColumnFrame(columnIndex, (ColumnListView *)owner);
	float sright = colRect.right;
	colRect.bottom -= 1.0;
	colRect.right = colRect.left + (colRect.right - colRect.left) * CPU;
	owner->SetHighColor(200 * CPU, 200 * (1.0 - CPU), 0);
	owner->FillRect(colRect & itemColumnRect);
	if (complete) {
		owner->SetHighColor(255, 255, 255);
		owner->FillRect(BRect(colRect.right + 1.0, colRect.top, sright, colRect.bottom) &
			itemColumnRect);
	} */
}

TeamItem::~TeamItem() {
	if (thread_items_list) delete thread_items_list;
	if (!args[0]) free(name);
	if (team_icon) delete team_icon;
}
