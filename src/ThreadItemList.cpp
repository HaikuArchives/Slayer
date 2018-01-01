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
#include "ThreadItem.h"
#include <Catalog.h>
#include "SlayerApp.h"
#include "Options.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ThreadItem"

ThreadItem::ThreadItem() : CLVEasyItem(1) {}

ThreadItem::ThreadItem(thread_info *info) : CLVEasyItem(1)
{
	thread = info->thread;
	team = info->team;
	state = info->state;
	priority = info->priority;
	sem = info->sem;
	user_time = info->user_time;
	kernel_time = info->kernel_time;
	CPU_diff = user_time + kernel_time;
	CPU = 0.0;
	strcpy(name, info->name);

	char str[21], *strp;
	SetColumnContent(TeamListView::name_ndx, name,false);
	sprintf(str, "%ld", thread);
	SetColumnContent(TeamListView::id_ndx, str,false);
	sprintf(str, "%ld", priority);
	SetColumnContent(TeamListView::priority_ndx, str,false);
	switch (state) {
	case B_THREAD_RUNNING:
		strp = strdup(B_TRANSLATE("Running")); break;
	case B_THREAD_READY:
		strp = strdup(B_TRANSLATE("Ready")); break;
	case B_THREAD_SUSPENDED:
		strp = strdup(B_TRANSLATE("Suspended")); break;
	case B_THREAD_WAITING:
		strp = strdup(B_TRANSLATE("Waiting")); break;
	case B_THREAD_RECEIVING:
		strp = strdup(B_TRANSLATE("Receiving")); break;
	case B_THREAD_ASLEEP:
		strp = strdup(B_TRANSLATE("Sleeping")); break;
	default:
		strp = strdup(B_TRANSLATE("Undefined")); break;
	}
	SetColumnContent(TeamListView::state_ndx, strp, false);
	SetColumnContent(TeamListView::areas_ndx, "-", false);

	SetColumnContent(TeamListView::CPU_ndx, "-", false);

	changed = 0;
}

void ThreadItem::update(thread_info *info)
{
	char str[21], *strp;
	CPU_diff = (info->user_time-user_time)+(info->kernel_time-kernel_time);
	user_time = info->user_time;
	kernel_time = info->kernel_time;

	if (strcmp(info->name, name) &&
	    (slayer->options.shown_columns & Options::name_col)) {

		strcpy(name, info->name);
		SetColumnContent(TeamListView::name_ndx, name, false);
		changed |= name_chg;
	}
	if ((priority != info->priority) &&
		(slayer->options.shown_columns & Options::priority_col)) {

		priority = info->priority;
		sprintf(str, "%ld", priority);
		SetColumnContent(TeamListView::priority_ndx, str, false);
		changed |= priority_chg;
	}
	if ((state != info->state) &&
	    (slayer->options.shown_columns & Options::state_col)) {

		state = info->state;

		switch (state) {
		case B_THREAD_RUNNING:
			strp = strdup(B_TRANSLATE("Running")); break;
		case B_THREAD_READY:
			strp = strdup(B_TRANSLATE("Ready")); break;
		case B_THREAD_SUSPENDED:
			strp = strdup(B_TRANSLATE("Suspended")); break;
		case B_THREAD_WAITING:
			strp = strdup(B_TRANSLATE("Waiting")); break;
		case B_THREAD_RECEIVING:
			strp = strdup(B_TRANSLATE("Receiving")); break;
		case B_THREAD_ASLEEP:
			strp = strdup(B_TRANSLATE("Sleeping")); break;
		default:
			strp = strdup(B_TRANSLATE("Undefined")); break;
		}
		SetColumnContent(TeamListView::state_ndx, strp, false);
		changed |= state_chg;
	}
}

void ThreadItem::DrawItemColumn(BView *owner, BRect itemColumnRect, int32
		columnIndex, bool complete = false)
{
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
	}
}
