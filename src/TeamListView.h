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

#ifndef _TEAM_LIST_VIEW_H
#define _TEAM_LIST_VIEW_H

#include <ColumnListView.h>
#include <InterfaceKit.h>

#define TEAM_INV 'tein'
#define SELECTION_CHANGED 'sech'
#define SET_PRIORITY 'pset'
#define REFRESH_TEAMS 'rfsh'

class TeamListView : public BColumnListView
{
  public:
	static const int expander_ndx = 0, icon_ndx = 1, name_ndx = 2, id_ndx = 3, priority_ndx = 4,
					 state_ndx = 5, areas_ndx = 6, CPU_ndx = 7;

	TeamListView(const char* name);

	BPopUpMenu* operationMenu;
	BMenu* priorityMenu;

	BPopUpMenu* ActionMenu();
	virtual void MakeFocus(bool focused = true);
	virtual void SelectionChanged(void);
	void FullListDoForEach(bool (*func)(BRow*, void*), void* data);
	void KeyDown(const char* bytes, int32 numBytes);
	void MoveToRow(int RowIndex);
};
#endif
