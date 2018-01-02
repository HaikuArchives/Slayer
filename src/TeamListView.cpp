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
/*#include "TeamListView.h"

TeamListView::TeamListView(BRect frame, const char *name)
	: BOutlineListView(frame, name, B_MULTIPLE_SELECTION_LIST,
	  B_FOLLOW_ALL_SIDES)
{
	SetInvocationMessage(new BMessage(TEAM_INV));

}

void TeamListView::SelectionChanged()
{
	Window()->PostMessage(SELECTION_CHANGED);
} */

#include "TeamListView.h"
#include "CLVColumn.h"
#include "SlayerApp.h"

TeamListView::TeamListView(BRect frame, const char *name, CLVContainerView **s)
	: ColumnListView(frame, s, name, B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_NAVIGABLE|B_FRAME_EVENTS,
	B_MULTIPLE_SELECTION_LIST, true, true, true, false, B_FANCY_BORDER)
{
	// add Columns...
	AddColumn(new CLVColumn(NULL, 20.0, CLV_EXPANDER|CLV_LOCK_AT_BEGINNING|CLV_NOT_MOVABLE));
	AddColumn(new CLVColumn(NULL, 20.0, CLV_LOCK_AT_BEGINNING|CLV_NOT_RESIZABLE|
		CLV_NOT_MOVABLE|CLV_MERGE_WITH_RIGHT));
	AddColumn(new CLVColumn("Name", 220.0, CLV_LOCK_AT_BEGINNING|CLV_NOT_MOVABLE));
	AddColumn(new CLVColumn("Id", 40.0));
	AddColumn(new CLVColumn("Priority", 50.0));
	AddColumn(new CLVColumn("State", 60.0));
	AddColumn(new CLVColumn("Memory", 60.0));
	AddColumn(new CLVColumn("CPU", 60.0));

	SetInvocationMessage(new BMessage(TEAM_INV));

	// create the PopUpMenu
	BMenuItem *inv;

	operationMenu = new BPopUpMenu("operationMenu", false, false);
	operationMenu->AddItem((inv = new BMenuItem("Kill",
		new BMessage(IE_MAINWINDOW_MAINKILL))));
		inv->SetTarget(slayer->mainWindow);
	operationMenu->AddItem((inv = new BMenuItem("Suspend",
		new BMessage(IE_MAINWINDOW_MAINSUSPEND))));
		inv->SetTarget(slayer->mainWindow);
	operationMenu->AddItem((inv = new BMenuItem("Resume",
		new BMessage(IE_MAINWINDOW_MAINRESUME))));
		inv->SetTarget(slayer->mainWindow);
	operationMenu->AddSeparatorItem();
	priorityMenu = new BMenu("PriorityMenu");
	BMenuItem *pr = new BMenuItem(priorityMenu); //, new BMessage('tmpj'));
	pr->SetLabel(B_TRANSLATE("Set priority"));
	operationMenu->AddItem(pr);
	ItemsToPopUpPriorityMenu();

//	MakeFocus(true);
//	 = new BMenu("Set priority");
//	operationMenu->AddItem(setPriorityMenu);

//	operationMenu->AddSeparator
}

void TeamListView::MakeFocus(bool focused)
{
	ColumnListView::MakeFocus(focused);
}

void TeamListView::SelectionChanged()
{
	Window()->PostMessage(SELECTION_CHANGED);
}

void
TeamListView::KeyDown(const char* bytes, int32 numBytes)
{
	TeamItem* item = (TeamItem*) ItemAt(CurrentSelection());

	switch (bytes[0]) {
		case B_LEFT_ARROW:
			if (item != NULL && item->IsSuperItem() && item->IsExpanded())
				Collapse(item);
			break;
		case B_RIGHT_ARROW:
			if (item != NULL && item->IsSuperItem() && !item->IsExpanded())
				Expand(item);
			break;
		default:
			ColumnListView::KeyDown(bytes, numBytes);
	}
}

void TeamListView::MouseDown(BPoint point)
{
	ColumnListView::MouseDown(point);

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

void TeamListView::SetShownColumns(int32 mask)
{
	CLVColumn *col;
	col = ColumnAt(name_ndx);
	col->SetShown(mask & Options::name_col);

	col = ColumnAt(id_ndx);
	col->SetShown(mask & Options::id_col);

	col = ColumnAt(state_ndx);
	col->SetShown(mask & Options::state_col);

	col = ColumnAt(priority_ndx);
	col->SetShown(mask & Options::priority_col);

	col = ColumnAt(areas_ndx);
	col->SetShown(mask & Options::memory_col);

	col = ColumnAt(CPU_ndx);
	col->SetShown(mask & Options::cpu_col);
}

