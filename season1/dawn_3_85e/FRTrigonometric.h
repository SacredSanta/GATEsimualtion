#ifndef FRTRIGONOMETRIC_TABLE_H
#define FRTRIGONOMETRIC_TABLE_H

#include "FRVector2.h"
#include "FRArray.h"
#include "FRMath.h"

const int NDIV_DEFAULT = 24;

class FRTrigonometricTable
{
	friend class FRSegmentTable;
public:
	FRTrigonometricTable(unsigned div_ = NDIV_DEFAULT) { SetDiv(div_); }

	void SetDiv(unsigned);
	unsigned GetDiv()	 const { return div; }
private:
	FRArray<FRVector2> table;
	unsigned div;
};

class FRSegmentTable
{
public:
	FRSegmentTable(
		const FRTrigonometricTable&,
		double start = 0,
		double sweep = FR_D_PI * 2);

	const FRVector2* GetTable() const { return table; }
	unsigned GetSize() const { return num; }
protected:
	FRArray<FRVector2> buf;
	const FRVector2 *table;
	unsigned num;
};

#endif
