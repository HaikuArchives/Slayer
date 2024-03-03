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
#ifndef _MINI_SLAYER_H
#define _MINI_SLAYER_H

#include <InterfaceKit.h>

class _EXPORT MiniSlayer;

class MiniSlayer : public BView {
	BBitmap *miniIcon;
	const char *signature; // the loaded apps signature (should be equal to
						   // slayer_signature usually)
  public:
	MiniSlayer();
	MiniSlayer(BMessage *);
	virtual ~MiniSlayer();

	// archiving overrides
	static MiniSlayer *
	Instantiate(BMessage *data);
	virtual status_t
	Archive(BMessage *data, bool deep = true) const;

	virtual void
	AttachedToWindow();
	virtual void MouseDown(BPoint);
	virtual void Draw(BRect);
};

#endif
