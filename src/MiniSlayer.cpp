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

#include "MiniSlayer.h"
#include "SlayerApp.h"
#include <AppKit.h>
#include <StorageKit.h>

extern const char* slayer_signature;

MiniSlayer::MiniSlayer()
	: BView(BRect(0, 0, 15, 15), "MiniSlayer", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW)
{
	SetViewColor(255, 255, 0);
	//	AddChild(new BDragger(BRect(0,0,16,16), this, B_FOLLOW_RIGHT |
	// B_FOLLOW_BOTTOM));
}

MiniSlayer::MiniSlayer(BMessage* msg) : BView(msg)
{
	// Oh well, we're instantiated from Deskbar.
	// Find the mini icon for Slayer.

	miniIcon = new BBitmap(BRect(0.0, 0.0, 15.0, 15.0), B_RGBA32);
	if (!miniIcon)
		return;

	BAppFileInfo afi;
	entry_ref ref;
	BFile file;

	if (msg->FindString("add_on", &signature) != B_OK)
		signature = slayer_signature;

	be_roster->FindApp(signature, &ref);
	file.SetTo(&ref, B_READ_ONLY);
	afi.SetTo(&file);
	if (afi.GetIcon(miniIcon, B_MINI_ICON) != B_NO_ERROR) {
		miniIcon = NULL;
		return;
	}
}

MiniSlayer::~MiniSlayer()
{
	if (miniIcon != NULL)
		delete miniIcon;
}

MiniSlayer*
MiniSlayer::Instantiate(BMessage* data)
{
	if (!validate_instantiation(data, "MiniSlayer"))
		return NULL;
	return new MiniSlayer(data);
}

status_t
MiniSlayer::Archive(BMessage* data, bool deep) const
{
	BView::Archive(data, deep);
	data->AddString("add_on", slayer_signature);

	return B_NO_ERROR;
}

void
MiniSlayer::AttachedToWindow()
{
	// Set background color to match parent's

	if (Parent())
		SetViewColor(Parent()->ViewColor());

	BView::AttachedToWindow();

	slayer = new SlayerOpt;
	slayer->docked = true;
	slayer->options.Load();

	slayer->mainWindow = NULL;
}

void
MiniSlayer::MouseDown(BPoint point)
{
	if (slayer->mainWindow == NULL) {
		new MainWindow;
		if (slayer->mainWindow->Lock())
			if (slayer->mainWindow->minimized)
				slayer->mainWindow->Minimize(false);
		slayer->mainWindow->Unlock();
	}
	else {
		if (slayer->mainWindow->Lock()) {
			uint32 saved_flags = slayer->mainWindow->Flags();

			// open up in current workspace
			if (slayer->options.workspace_activation == Options::current_workspace)
				slayer->mainWindow->SetFlags(
					slayer->mainWindow->Flags() | B_NOT_ANCHORED_ON_ACTIVATE |
					B_NO_WORKSPACE_ACTIVATION
				);

			if (slayer->mainWindow->minimized)
				slayer->mainWindow->Minimize(false);
			else
				slayer->mainWindow->Activate(true);

			slayer->mainWindow->SetFlags(saved_flags);
		}
		slayer->mainWindow->Unlock();
	}
}

void
MiniSlayer::Draw(BRect rect)
{
	BView::Draw(rect);

	SetDrawingMode(B_OP_OVER);
	if (miniIcon != NULL)
		DrawBitmap(miniIcon);
	else {
		SetHighColor(255, 0, 0);
		FillEllipse(Bounds());
	}

	// else
	// BView::Draw(rect);

	SetDrawingMode(B_OP_COPY);
}
