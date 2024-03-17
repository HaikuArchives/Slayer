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
#define _INIT_SLAYER_APP_H
#include "SlayerApp.h"
#undef _INIT_SLAYER_APP_H
#include "MiniSlayer.h"

const char* slayer_signature = "application/x-vnd.AJ-Slayer2";

int
main(int argc, char* argv[])
{
	bool dock = false;

	for (argv++; argc > 1; argc--, argv++) {
		if (!strcmp(argv[0], "--deskbar"))
			dock = true;
	}
	SlayerApp* me = new SlayerApp(dock);
	me->Run();
	delete me;
	return 0;
}

SlayerApp::SlayerApp(bool dock) : BApplication(slayer_signature)
{
	slayer = this;
	docked = false;
	mainWindow = NULL;

	if (!dock) {
		options.Load();
		new MainWindow;
	}
	else {
		MiniSlayer* replicant = new MiniSlayer();
		BMessage archiveMsg(B_ARCHIVED_OBJECT);
		replicant->Archive(&archiveMsg);
		BMessenger messenger("application/x-vnd.Be-TSKB", -1, NULL);
		messenger.SendMessage(&archiveMsg);
		docked = true;
		be_app->PostMessage(B_QUIT_REQUESTED);
	}
}

SlayerApp::~SlayerApp()
{
	if (!docked)
		options.Save();
}

BWindow*
SlayerOpt::FindWindow(const char* name)
{
	BWindow *window, *fwindow = NULL;
	int32 i = 0;

	while ((window = be_app->WindowAt(i++)) && !fwindow)
		if (window->Lock()) {
			if (!strcmp(name, window->Name()))
				fwindow = window;
			window->Unlock();
		}

	return fwindow;
}
