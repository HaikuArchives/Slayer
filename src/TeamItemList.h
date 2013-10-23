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
#ifndef _TEAM_ITEM_LIST_H
#define _TEAM_ITEM_LIST_H

#include "TeamItem.h"
#include "Hashtable.h"

class TeamItemList {
	Hashtable list;
public:
	virtual ~TeamItemList();
	TeamItem *AddItem(TeamItem *item);
	TeamItem *DelItem(TeamItem *item);
	TeamItem *ItemAt(int32 index);
};

/*
class TeamItemList : public BList {
public:
	virtual ~TeamItemList();
	bool AddItem(TeamItem *item);	
	TeamItem *ItemAt(int32 index);
};
*/
#endif
	
