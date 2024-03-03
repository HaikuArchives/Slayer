#ifndef _SIZECOLUMN_H_
#define _SIZECOLUMN_H_

#include <ColumnTypes.h>

class SizeColumn : public BSizeColumn {
  public:
	SizeColumn(
		const char *title, float width, float minWidth, float maxWidth,
		alignment align = B_ALIGN_LEFT
	);
	virtual void DrawField(BField *field, BRect rect, BView *parent);
	virtual int CompareFields(BField *field1, BField *field2);
};

#endif
