#include "PositiveIntegerColumn.h"

PositiveIntegerColumn::PositiveIntegerColumn(
	const char* title, float width, float minWidth, float maxWidth, alignment align
)
	: BIntegerColumn(title, width, minWidth, maxWidth, align)
{
}

void
PositiveIntegerColumn::DrawField(BField* field, BRect rect, BView* parent)
{
	int32 size = ((BIntegerField*)field)->Value();

	if (size < 0)
		DrawString("-", parent, rect);
	else
		BIntegerColumn::DrawField(field, rect, parent);
}
