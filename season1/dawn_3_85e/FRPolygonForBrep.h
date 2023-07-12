//////////////////////////////
///// FRPolygonForBrep.h /////
//////////////////////////////

#if !defined FR_POLYGON_FOR_BREP_H
#define FR_POLYGON_FOR_BREP_H

#include "FRPolygon.h"
#include "FRBrepSolid.h"

class FRPolygonForBrep : public FRPolygon, public FRBrepSolid
{
public:
	FRPolygonForBrep(double pSPhi, double pDPhi, int pNdiv, int pNum,
		const double* pDzP = NULL,
		const double* pRminP = NULL,
		const double* pRmaxP = NULL)
	: FRPolygon(pSPhi, pDPhi, pNdiv, pNum, pDzP, pRminP, pRmaxP) { }

	FRPolygonForBrep(const FRPolygon& polygon) : FRPolygon(polygon) { }
	~FRPolygonForBrep() { } 

	void GenerateBrepData(FRBrepGenerator*) const;
};

#endif
