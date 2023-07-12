//////////////////////////////
///// FRPolyconeForBrep.h /////
//////////////////////////////

#if !defined FR_POLYCONE_FOR_BREP_H
#define FR_POLYCONE_FOR_BREP_H

#include "FRPolycone.h"
#include "FRBrepSolid.h"

class FRPolyconeForBrep : public FRPolycone, public FRBrepSolid
{
public:
	FRPolyconeForBrep(double pSPhi, double pDPhi, int pNum,
		const double* pDzP = NULL,
		const double* pRminP = NULL,
		const double* pRmaxP = NULL)
	: FRPolycone(pSPhi, pDPhi, pNum, pDzP, pRminP, pRmaxP) { }

	FRPolyconeForBrep(const FRPolycone& polycone) : FRPolycone(polycone) { }
	~FRPolyconeForBrep() { } 

	void GenerateBrepData(FRBrepGenerator*) const;
};

#endif
