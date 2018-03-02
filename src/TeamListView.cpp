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

#include "TeamListView.h"
#include <Catalog.h>
#include <ColumnListView.h>
#include <ColumnTypes.h>
#include "SlayerApp.h"
#include "SizeColumn.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "TeamListView"

TeamListView::TeamListView(const char *name)
	: BColumnListView("fileListView", B_FRAME_EVENTS|B_NAVIGABLE)
{
	// add Columns...
	BColumn* column;
	int32 i = 0;
	AddColumn(new BBitmapColumn(B_TRANSLATE("Icon"), 16, 16, 16, B_ALIGN_CENTER), i++);
	AddColumn(new BStringColumn(B_TRANSLATE("Name"), 180, 10, 600, 0), i++);
	AddColumn(new BIntegerColumn(B_TRANSLATE("Id"), 50, 10, 100/*, B_ALIGN_RIGHT*/), i++);
	AddColumn(new BStringColumn(B_TRANSLATE("Priority"), 60, 10, 600, 0), i++);
	AddColumn(new BStringColumn(B_TRANSLATE("State"), 70, 10, 600, 0), i++);
	AddColumn(new SizeColumn(B_TRANSLATE("Memory"), 80, 10, 600), i++);
	AddColumn(new GraphColumn(B_TRANSLATE("CPU"), 80.0, 10, 100.0), i++);
	AddColumn(column = new BStringColumn(B_TRANSLATE("Full name"), 180, 10, 600, 0), i++);
	column->SetVisible(false);

	SetInvocationMessage(new BMessage(TEAM_INV));

	// create the PopUpMenu
	BMenuItem *inv;
	operationMenu = new BPopUpMenu("operationMenu", false, false);
	operationMenu->AddItem((inv = new BMenuItem(B_TRANSLATE("Kill"),
		new BMessage(IE_MAINWINDOW_MAINKILL))));
		inv->SetTarget(slayer->mainWindow);
	operationMenu->AddItem((inv = new BMenuItem(B_TRANSLATE("Suspend"),
		new BMessage(IE_MAINWINDOW_MAINSUSPEND))));
		inv->SetTarget(slayer->mainWindow);
	operationMenu->AddItem((inv = new BMenuItem(B_TRANSLATE("Resume"),
		new BMessage(IE_MAINWINDOW_MAINRESUME))));
		inv->SetTarget(slayer->mainWindow);
	// TODO add priority to the menu

	//operationMenu->AddSeparatorItem();
	//priorityMenu = new BMenu("PriorityMenu");
	//BMenuItem *pr = new BMenuItem(priorityMenu); //, new BMessage('tmpj'));
	//pr->SetLabel("Set priority");
	//operationMenu->AddItem(pr);
	//ItemsToPopUpPriorityMenu();

	//SetSelectionMessage(new BMessage(TEAM_INV));
	BMessage* selected = new BMessage(SELECTION_CHANGED);
	selected->AddInt32("buttons",0);
	SetSelectionMessage(selected);

	MakeFocus(true);
//	 = new BMenu("Set priority");
//	operationMenu->AddItem(setPriorityMenu);

//	operationMenu->AddSeparator
}

void TeamListView::MakeFocus(bool focused)
{
	BColumnListView::MakeFocus(focused);
}

void TeamListView::SelectionChanged()
{
	uint32 buttons;

	BMessage *msg = Window()->CurrentMessage();

	if(msg)
		msg->FindInt32("buttons", (int32 *)&buttons);

	SelectionMessage()->ReplaceInt32("buttons",buttons);

	BColumnListView::SelectionChanged();
}


void
TeamListView::KeyDown(const char* bytes, int32 numBytes)
{
	BColumnListView::KeyDown(bytes, numBytes);
}

// TODO this is never called
void TeamListView::MouseDown(BPoint point)
{
	BColumnListView::MouseDown(point);

	int32 buttons = 0;
	Window()->CurrentMessage()->FindInt32("buttons", &buttons);
	if (buttons & B_SECONDARY_MOUSE_BUTTON) {
		// let's show the pop-up menu
		uint32 buttons = 0;
		GetMouse(&point, &buttons);
		ConvertToScreen(&point);
		// gotta set the priority menu
		// Select priority has to be updated
		if (slayer->mainWindow->Lock()) {
			slayer->mainWindow->SetPriorityState();
			UpdatePopUpPriorityMenu();
			slayer->mainWindow->Unlock();
		}
/*		if (slayer->mainWindow->Lock()) {

			BMenuField *Priority = (BMenuField *)slayer->mainWindow->FindView("MainPriorityField");
			printf("priorityfield: %o\n", Priority);
			BMessage arc;
			Priority->Menu()->Archive(&arc);
			BMenu *PriorityMenu = (BMenu *)instantiate_object(&arc);
			PriorityMenu->MoveTo(-1, -1);
			// remove "Select priority" & separator
			PriorityMenu->RemoveItem((int32)0);
			PriorityMenu->RemoveItem((int32)0);
			BMenuItem *pr = new BMenuItem(PriorityMenu, new BMessage('tmpj'));
			pr->SetLabel("Set priority");
			operationMenu->AddItem(pr);
			slayer->mainWindow->Unlock();
		} */
		operationMenu->Go(point, true, true, true);
	/*	BMenuItem *jack = operationMenu->Go(point);
		if (jack) {
			jack->Message()->PrintToStream();
			BLooper *looper;
			BHandler *target = jack->Target(&looper);
			Window()->PostMessage(jack->Message());
		} */
	}
}

BPopUpMenu *
TeamListView::ActionMenu()
{
	return operationMenu;
};

void TeamListView::ItemsToPopUpPriorityMenu()
{
	BMenuField *Priority = (BMenuField *)slayer->mainWindow->FindView("MainPriorityField");
	BMenu *menu = Priority->Menu();
	BMenuItem *add;
	int32 i;
	for (i = 2; (add = menu->ItemAt(i)); i++) {
		BMenuItem *newItem;
		if (add->Label() && add->Label()[0])
			newItem = new BMenuItem(add->Label(), new BMessage(
				add->Command()));
		else
			newItem = new BSeparatorItem();

		newItem->SetTarget(slayer->mainWindow);
		priorityMenu->AddItem(newItem);
	}
		// priorityMenu->AddItem(add);
}

void TeamListView::UpdatePopUpPriorityMenu()
{
	BMenuField *Priority = (BMenuField *)slayer->mainWindow->FindView("MainPriorityField");
	BMenu *menu = Priority->Menu();
	BMenuItem *item;
	int32 i;
	for (i = 2; (item = menu->ItemAt(i)); i++) {
		priorityMenu->ItemAt(i-2)->SetMarked(item->IsMarked());
		priorityMenu->ItemAt(i-2)->SetEnabled(item->IsEnabled());
	}
}

void TeamListView::FullListDoForEach(bool (*func)(BRow*, void*), void* data)
{
	for(int i = 0; i < CountRows(); i++) {
		func(RowAt(i), data);
		//printf("%s\n", ((BStringField*)(RowAt(i)->GetField(1)))->String());
//		if (RowAt(i)->IsExpanded())
		for (int j = 0; j < CountRows(RowAt(i)); j++) {
			//printf("%s\n", ((BStringField*)(RowAt(j, RowAt(i))->GetField(1)))->String());
			func(RowAt(j, RowAt(i)), data);
		}
	}
}
