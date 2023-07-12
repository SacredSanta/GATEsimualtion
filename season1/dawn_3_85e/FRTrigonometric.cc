#include <math.h>
#include "FRTrigonometric.h"
#include <iostream.h>

void FRTrigonometricTable::SetDiv(unsigned div_)
{
	unsigned i;
	double x;

	div = div_;
	table.alloc(2 * div + 1);

	for(i = 0; i < div; i++)
	{
		x = (double)i / (double)div * 2 * FR_D_PI;
		table[i] = FRVector2(cos(x), sin(x));
		table[div + i] = table[i];
	}
//	table[2 * div] = table[0];
	table[(size_t)(2 * div)] = table[(size_t)0];
}

FRSegmentTable::FRSegmentTable(
	const FRTrigonometricTable& X,
	double start,
	double sweep)
{
	if((sweep >= 2 * FR_D_PI) || (sweep <= 0.0))
	{
		table = X.table;
		num = X.div;
	}
	else
	{
		if((start < 0.0) || (start < -2.0 * FR_D_PI))
			start += 2.0 * FR_D_PI;
		else if(start > 2.0 * FR_D_PI)
			start -= 2.0 * FR_D_PI;

		unsigned p0, p1;
		p0 = (int)(start / (2.0 * FR_D_PI) * (double)X.div);
		p1 = X.div - (int)( ((2.0 * FR_D_PI) -(start + sweep)) / (2.0 * FR_D_PI) * (double)X.div )
			 + ((start + sweep > 2 * FR_D_PI) ? 1 : 0);
		num = p1 - p0 + 1;

		buf.alloc(num);

		unsigned i;
		double L;
		L = cos(FR_D_PI / (double)X.div) / cos((2.0 * (double)p0 + 1.0)/(double)X.div * FR_D_PI - start);
		L = fabs(L);
//		buf[0] = FRVector2(cos(start) * L, sin(start) * L);
		buf[(size_t)0] = FRVector2(cos(start) * L, sin(start) * L);

		for(i = 1; i <= num - 2; i++){
			buf[i] = X.table[p0 + i];
		}

		L = cos(FR_D_PI / (double)X.div) / cos((2.0 * (double)(p1-1) + 1.0)/(double)X.div * FR_D_PI - start - sweep);
		L = fabs(L);
		buf[num - 1] = FRVector2(cos(start + sweep) * L, sin(start + sweep) * L);

		table = buf;
	}
}
