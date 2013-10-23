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
#ifndef _ITEM_LIST_H
#define _ITEM_LIST_H

struct item_info {
	item_info(team_id team_, thread_id thread_)
		: thread(thread_), team(team_) {}
	thread_id thread;
	team_id team;
};

class ItemList : public BList {
public:
	~ItemList()
	{
		item_info *item;
	
		for (int32 i = 0; (item = (item_info *)ItemAt(i)); i++)
			delete item;
	}

	bool AddItem(item_info *item)
	{
		return BList::AddItem((void *)item);
	}

	item_info *ItemAt(int32 index)
	{
		return (item_info *)BList::ItemAt(index);
	}
	
	item_info *HasItem(item_info *item)
	{
		int32 i = 0;
		item_info *info;
		for (; (info = ItemAt(i)); i++) {
	//		printf("team/thread: %d/%d\n", info->team, info->thread);
			if (info->thread == item->thread &&
			    info->team   == item->team)
			    	return info;
		}
		return 0;
	}
};

#endif
