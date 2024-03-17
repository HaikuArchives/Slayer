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
#include "PositiveIntegerColumn.h"
#include "PriorityMenu.h"
#include "SizeColumn.h"
#include "SlayerApp.h"
#include <Catalog.h>
#include <ColumnListView.h>
#include <ColumnTypes.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "TeamListView"

TeamListView::TeamListView(const char *name)
	: BColumnListView("fileListView", B_FRAME_EVENTS | B_NAVIGABLE) {
	// add Columns
	BColumn *column;
	int32 i = 0;
	AddColumn(new BBitmapColumn(B_TRANSLATE("Icon"), 16, 16, 16, B_ALIGN_CENTER), i++);
	AddColumn(new BStringColumn(B_TRANSLATE("Name"), 180, 10, 600, 0), i++);
	AddColumn(new BIntegerColumn(B_TRANSLATE("ID"), 50, 10, 100, B_ALIGN_RIGHT), i++);
	AddColumn(new PositiveIntegerColumn(B_TRANSLATE("Priority"), 60, 10, 600), i++);
	AddColumn(new BStringColumn(B_TRANSLATE("State"), 70, 10, 600, 0), i++);
	AddColumn(new SizeColumn(B_TRANSLATE("Memory"), 80, 10, 600), i++);
	AddColumn(new GraphColumn(B_TRANSLATE("CPU"), 80.0, 10, 100.0), i++);
	AddColumn(column = new BStringColumn(B_TRANSLATE("Path"), 180, 10, 600, 0), i++);
	column->SetVisible(false);

	SetInvocationMessage(new BMessage(TEAM_INV));

	// create the PopUpMenu
	BMenuItem *inv;
	operationMenu = new BPopUpMenu("operationMenu", false, false);
	operationMenu->AddItem(
		(inv = new BMenuItem(B_TRANSLATE("Kill"), new BMessage(IE_MAINWINDOW_MAINKILL)))
	);
	inv->SetTarget(slayer->mainWindow);
	operationMenu->AddItem(
		(inv = new BMenuItem(B_TRANSLATE("Suspend"), new BMessage(IE_MAINWINDOW_MAINSUSPEND)))
	);
	inv->SetTarget(slayer->mainWindow);
	operationMenu->AddItem(
		(inv = new BMenuItem(B_TRANSLATE("Resume"), new BMessage(IE_MAINWINDOW_MAINRESUME)))
	);
	inv->SetTarget(slayer->mainWindow);

	operationMenu->AddSeparatorItem();
	priorityMenu = new PriorityMenu(this);
	operationMenu->AddItem(priorityMenu);
	((PriorityMenu *)priorityMenu)->BuildMenu();

	BMessage *selected = new BMessage(SELECTION_CHANGED);
	selected->AddInt32("buttons", 0);
	SetSelectionMessage(selected);

	MakeFocus(true);
}

void
TeamListView::MakeFocus(bool focused) {
	BColumnListView::MakeFocus(focused);
}

void
TeamListView::SelectionChanged() {
	uint32 buttons;

	BMessage *msg = Window()->CurrentMessage();

	if (msg)
		msg->FindInt32("buttons", (int32 *)&buttons);

	SelectionMessage()->ReplaceInt32("buttons", buttons);

	BColumnListView::SelectionChanged();
}

BPopUpMenu *
TeamListView::ActionMenu() {
	((PriorityMenu *)priorityMenu)->Update();
	return operationMenu;
}

void
TeamListView::FullListDoForEach(bool (*func)(BRow *, void *), void *data) {
	for (int i = 0; i < CountRows(); i++) {
		func(RowAt(i), data);

		for (int j = 0; j < CountRows(RowAt(i)); j++) {
			func(RowAt(j, RowAt(i)), data);
		}
	}
}

void
TeamListView::KeyDown(const char *bytes, int32 numBytes) {
	if (numBytes <= 0)
		return;

	int indexOfLastRow = this->CountRows() - 1;

	switch (bytes[0]) {
	case B_HOME:
		MoveToRow(0);
		break;
	case B_END:
		MoveToRow(indexOfLastRow);
		break;
	default: {
		BColumnListView::KeyDown(bytes, numBytes);
	}
	}
}

void
TeamListView::MoveToRow(int RowIndex) {
	DeselectAll();
	ScrollTo(this->RowAt(RowIndex));
	SetFocusRow(this->RowAt(RowIndex), true);
}
