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
#include <Catalog.h>

#define SETTINGS_FILE "Slayer_settings"
#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "Options"

Options::Options()
{
	refresh = 200;
	save_wind_pos = true;
	save_workspace = false;
	workspace_activation = current_workspace;
	workspaces = B_CURRENT_WORKSPACE;
	wind_minimized = false;
	wind_rect.Set(1.0, 1.0, 0.0, 0.0);
	shown_columns = id_col|priority_col|state_col|cpu_col|name_col;
}

void Options::Save()
{
	BPath path;

	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path, true) != B_OK) {
		printf(B_TRANSLATE("Couldn't save options\n"));
		return;
	}

	path.SetTo(path.Path(), SETTINGS_FILE);

	BFile node(path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	if (node.InitCheck() != B_NO_ERROR) {
		printf(B_TRANSLATE("Couldn't open file for saving\n"));
		return;
	}

	char tmp;
	node.WriteAttr("refresh", B_INT32_TYPE, 0, &refresh, sizeof(int32));
	node.WriteAttr("save_wind_pos", B_CHAR_TYPE, 0, &(tmp = save_wind_pos),
		sizeof(tmp));
	node.WriteAttr("save_workspace", B_CHAR_TYPE, 0, &(tmp = save_workspace),
		sizeof(tmp));
	node.WriteAttr("workspace_activation", B_INT32_TYPE, 0, &workspace_activation,
		sizeof(workspace_activation));
	node.WriteAttr("workspaces", B_INT32_TYPE, 0, &workspaces,
		sizeof(workspaces));
	node.WriteAttr("wind_minimized", B_CHAR_TYPE, 0, &(tmp = wind_minimized),
		sizeof(wind_minimized));
	node.WriteAttr("wind_rect", B_RECT_TYPE, 0, &wind_rect,
		sizeof(wind_rect));
	node.WriteAttr("shown_columns", B_INT32_TYPE, 0, &shown_columns,
		sizeof(shown_columns));
}

void Options::Load()
{
	BPath path;

	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path, true)) {
		printf(B_TRANSLATE("Couldn't load options (user settings dir not found)\n"));
		return;
	}
	path.SetTo(path.Path(), SETTINGS_FILE);

	BNode node(path.Path());
	if (node.InitCheck() != B_NO_ERROR)
		return;

	char tmp;

	node.ReadAttr("refresh", B_INT32_TYPE, 0, &refresh, sizeof(int32));
	node.ReadAttr("save_wind_pos", B_CHAR_TYPE, 0, &tmp,
		sizeof(tmp)); save_wind_pos = tmp;
	node.ReadAttr("save_workspace", B_CHAR_TYPE, 0, &tmp,
		sizeof(tmp)); save_workspace = tmp;
	node.ReadAttr("workspace_activation", B_INT32_TYPE, 0, &workspace_activation,
		sizeof(workspace_activation));
	node.ReadAttr("workspaces", B_INT32_TYPE, 0, &workspaces,
		sizeof(workspaces));
	node.ReadAttr("wind_minimized", B_CHAR_TYPE, 0, &tmp,
		sizeof(wind_minimized)); wind_minimized = tmp;
	node.ReadAttr("wind_rect", B_RECT_TYPE, 0, &wind_rect,
		sizeof(wind_rect));

	node.ReadAttr("shown_columns", B_INT32_TYPE, 0, &shown_columns, sizeof(shown_columns));
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
