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
#include "Options.h"
#include <StorageKit.h>

#define SETTINGS_FILE "Slayer_settings"

Options::Options()
{
	refresh = 250;
	workspace_activation = current_workspace;
	workspaces = 1;
	wind_minimized = false;
	wind_rect.Set(1.0, 1.0, 0.0, 0.0);
	//	shown_columns = id_col|priority_col|state_col|cpu_col|name_col;
}

void
Options::Save()
{
	BPath path;

	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path, true) != B_OK) {
		printf("Couldn't save options\n");
		return;
	}
	path.SetTo(path.Path(), SETTINGS_FILE);
	BFile file(path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (file.InitCheck() != B_NO_ERROR) {
		printf("Couldn't open file for saving\n");
		return;
	}
	BMessage message;

	message.AddInt32("refresh", refresh);
	message.AddRect("wind_rect", wind_rect);
	message.AddMessage("columnsState", &columnsState);
	message.AddInt8("workspace_activation", workspace_activation);
	message.AddInt32("workspaces", workspaces);

	message.Flatten(&file);
}

void
Options::Load()
{
	BPath path;

	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path, true)) {
		printf("Couldn't load options (user settings dir not found)\n");
		return;
	}
	path.SetTo(path.Path(), SETTINGS_FILE);

	BFile file(path.Path(), B_READ_ONLY);
	if (file.InitCheck() != B_NO_ERROR)
		return;

	BMessage message;
	message.Unflatten(&file);

	if (message.FindInt32("refresh", &refresh) != B_OK)
		refresh = 250;
	message.FindRect("wind_rect", &wind_rect);
	message.FindMessage("columnsState", &columnsState);
	int8 tmp;
	if (message.FindInt8("workspace_activation", &tmp) == B_OK)
		workspace_activation = static_cast<workspace_type>(tmp);
	else
		workspace_activation = current_workspace;
	if (message.FindInt32("workspaces", &workspaces) != B_OK)
		workspaces = 1;
}

/*
int32 Options::get_refresh()
{
	return refresh;
}
int32 Options::set_refresh(int32 r)
{
	refresh = r;
	return r;
}

bool Options::get_save_wind_pos()
{
	return save_wind_pos;
}
bool Options::set_save_wind_pos(bool s)
{
	save_wind_pos = s;
	return s;
}

bool Options::get_save_workspace()
{
	return save_workspace;
}
bool Options::set_save_workspace(bool s)
{
	save_workspace = s;
	return s;
}

bool Options::get_wind_minimized()
{
	return wind_minimized;
}
bool Options::set_wind_minimized(bool s)
{
	wind_minimized = s;
	return s;
}

BRect Options::get_save_workspace()
{
	return wind_minimized;
}
bool Options::set_save_workspace(bool s)
{
	save_workspace = s;
	return s;
}
*/
