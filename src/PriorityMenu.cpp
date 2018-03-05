/*
	ProcessController © 2000, Georges-Edouard Berenger, All Rights Reserved.
	Copyright (C) 2004 beunited.org 

	This library is free software; you can redistribute it and/or 
	modify it under the terms of the GNU Lesser General Public 
	License as published by the Free Software Foundation; either 
	version 2.1 of the License, or (at your option) any later version. 

	This library is distributed in the hope that it will be useful, 
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
	Lesser General Public License for more details. 

	You should have received a copy of the GNU Lesser General Public 
	License along with this library; if not, write to the Free Software 
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA	
*/


#include "PriorityMenu.h"
#include "SlayerApp.h"
#include <Catalog.h>
#include <MenuItem.h>
#include <Window.h>

#include <stdio.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "PriorityMenu"

PriorityMenu::PriorityMenu(TeamListView* teamListView)
	: BMenu(B_TRANSLATE("SetPriority")),
	fTeamListView(teamListView),
	fPriority(-2),
	fEnabled(false)
{
}


void
PriorityMenu::Update()
{
	BRow* selected = fTeamListView->CurrentSelection(NULL);
	int32 priority;
	bool enabled = selected != NULL;
	
	if (enabled && fTeamListView->CurrentSelection(selected) == NULL && !selected->HasLatch()) 
		priority = ((ThreadItem *)selected)->priority;
	else 
		priority = -1;

	if (priority != fPriority || fEnabled != enabled)
	{   
		fPriority = priority;
		fEnabled = enabled;
		if (CountItems() > 0) 
			RemoveItems(0, CountItems(), true);
		if (CountItems() < 1)
			BuildMenu();
	}

}


typedef struct {
	const char*	name;
	long		priority;
} PriorityRec;

static PriorityRec	priorities[] = {
	{ B_TRANSLATE("Idle priority"),	0 },
	{ B_TRANSLATE("Lowest active priority"), 1 },
	{ B_TRANSLATE("Low priority"), 5 },
	{ B_TRANSLATE("Normal priority"), 10 },
	{ B_TRANSLATE("Display priority"), 15 },
	{ B_TRANSLATE("Urgent display priority"), 20 },
	{ B_TRANSLATE("Real-time display priority"), 100 },
	{ B_TRANSLATE("Urgent priority"), 110 },
	{ B_TRANSLATE("Real-time priority"), 120 },
	{ "",	-1 }
};

PriorityRec customPriority = { B_TRANSLATE("Custom priority"), 0 };


void
PriorityMenu::BuildMenu()
{
	BMenuItem* item;
	BMessage* message;
	long found = false;

	for (long index = 0; ; index++) {
		PriorityRec	*priority = &priorities[index];
		if (priority->priority < 0)
			break;
		if (!found && fPriority < priority->priority && fPriority >= 0) {
			priority = &customPriority;
			priority->priority = fPriority;
			index--;
		}
		message = new BMessage(SET_PRIORITY);
		message->AddInt32("priority", priority->priority);
		BString name;
		const size_t size = B_OS_NAME_LENGTH * 4;
		snprintf(name.LockBuffer(size), size,
			"%s [%d]", priority->name, (int)priority->priority);
		name.UnlockBuffer();
		item = new BMenuItem(name.String(), message);
		item->SetTarget(slayer->mainWindow);
		item->SetEnabled(fEnabled);
		if (fPriority == priority->priority)
			found = true, item->SetMarked(true);
		AddItem(item);
	}
}

