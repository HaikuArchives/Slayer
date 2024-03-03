#ifndef _POSITIVEINTEGERCOLUMN_H_
#define _POSITIVEINTEGERCOLUMN_H_

#include <ColumnTypes.h>

class PositiveIntegerColumn : public BIntegerColumn {
  public:
	PositiveIntegerColumn(
		const char *title, float width, float minWidth, float maxWidth,
		alignment align = B_ALIGN_LEFT
	);
	virtual void
	DrawField(BField *field, BRect rect, BView *parent);
};

#endif
