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
#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <InterfaceKit.h>
#include <SupportKit.h>

struct Options {
	int32 refresh;

	enum workspace_type { current_workspace, all_workspaces, saved_workspace } workspace_activation;

	int32 workspaces;
	bool wind_minimized;
	BRect wind_rect;

	enum {
		id_col = 0x1,
		priority_col = 0x2,
		state_col = 0x4,
		memory_col = 0x8,
		cpu_col = 0x10,
		name_col = 0x20
	};

	//	int32 shown_columns;
	BMessage columnsState;

	/*
	public:
		int32 get_refresh();
		int32 set_refresh(int32);
		bool	get_save_wind_pos();
		bool	set_save_wind_pos(bool);
		bool	get_save_workspace();
		bool	set_save_workspace(bool);
		bool	get_wind_minimized();
		bool	set_wind_minimized(bool);
		BRect	get_wind_rect();
		BRect	set_wind_rect();
	*/
	Options();
	void Save();
	void Load();
};

#endif
