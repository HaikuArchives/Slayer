#include "SizeColumn.h"

SizeColumn::SizeColumn(
	const char* title, float width, float minWidth, float maxWidth, alignment align
)
	: BSizeColumn(title, width, minWidth, maxWidth, align)
{
}

void
SizeColumn::DrawField(BField* field, BRect rect, BView* parent)
{
	off_t size = ((BSizeField*)field)->Size();

	if (size <= 0)
		DrawString("-", parent, rect);
	else
		BSizeColumn::DrawField(field, rect, parent);
}

int
SizeColumn::CompareFields(BField* field1, BField* field2)
{
	off_t diff = ((BSizeField*)field1)->Size() - ((BSizeField*)field2)->Size();
	if (diff > 0)
		return 1;
	else if (diff < 0)
		return -1;
	return 0;
}
