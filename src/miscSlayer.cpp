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
#include <AppMisc.h>
#include <stdio.h>
#include <string.h>

void
get_app_info(team_id team, BBitmap** a_icon, char** name, char** fullName)
{
	// code from the debugger
	app_info appInfo;
	*name = NULL;
	*fullName = NULL;
	status_t status = be_roster->GetRunningAppInfo(team, &appInfo);
	if (status != B_OK) {
		// Not an application known to be_roster

		if (team == B_SYSTEM_TEAM) {
			// Get icon and name from kernel image
			system_info systemInfo;
			get_system_info(&systemInfo);

			BPath kernelPath;
			find_directory(B_BEOS_SYSTEM_DIRECTORY, &kernelPath);
			kernelPath.Append(systemInfo.kernel_name);

			get_ref_for_path(kernelPath.Path(), &appInfo.ref);
		} else
			BPrivate::get_app_ref(team, &appInfo.ref);
	}

	BBitmap* icon = new BBitmap(BRect(0, 0, B_MINI_ICON - 1, B_MINI_ICON - 1), B_RGBA32);

	status = BNodeInfo::GetTrackerIcon(&appInfo.ref, icon, B_MINI_ICON);
	if (status != B_OK) {
		BMimeType genericAppType(B_APP_MIME_TYPE);
		status = genericAppType.GetIcon(icon, B_MINI_ICON);
	}

	if (status != B_OK) {
		delete icon;
		icon = NULL;
	} else {
		*name = strdup(appInfo.ref.name);
		BPath appFullName(&appInfo.ref);
		if (appFullName.InitCheck() == B_OK) {
			*fullName = strdup(appFullName.Path());
		}
	}
	*a_icon = icon;
}
