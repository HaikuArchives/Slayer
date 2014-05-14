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

#include <InterfaceKit.h>
#include "ColumnListView.h"
#include "BetterScrollView.h"

#define TEAM_INV	'tein'
#define SELECTION_CHANGED 'sech'

class TeamListView : public ColumnListView {
public:
	static const int
		expander_ndx = 0,
		icon_ndx = 1,
		name_ndx = 2,
		id_ndx = 3,
		priority_ndx = 4,
		state_ndx = 5,
		areas_ndx = 6,
		CPU_ndx = 7;
	
	TeamListView(BRect frame, const char *name, CLVContainerView **s);
	
	BPopUpMenu *operationMenu;
	BMenu *priorityMenu;
	
	void ItemsToPopUpPriorityMenu();
	void UpdatePopUpPriorityMenu();
	virtual void MakeFocus(bool focused = true);
	
	virtual void SelectionChanged(void);
	virtual void MouseDown(BPoint point);
	
	void SetShownColumns(int32 mask);
};
#endif
