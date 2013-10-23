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
#include "miscSlayer.h"
#include <string.h>
#include <stdio.h>

void get_app_info(team_id team, BBitmap **icon, char **name)
{
	app_info info;

	if (be_roster->GetRunningAppInfo(team, &info) == B_BAD_TEAM_ID) {
		*icon = NULL;
		*name = NULL;
		return;
	}
	
	*name = strdup(info.ref.name);
	BFile file;
	BAppFileInfo afi;
	
	file.SetTo(&info.ref, B_READ_ONLY);
	afi.SetTo(&file);
	*icon = new BBitmap(BRect(0.0,0.0,15.0,15.0), B_CMAP8);
	if (!*icon) {return;}
	if (afi.GetIcon(*icon, B_MINI_ICON) != B_NO_ERROR)
		{*icon = NULL; return;}
}
