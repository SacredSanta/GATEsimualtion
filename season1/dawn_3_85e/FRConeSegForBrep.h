/////////////////////////////
///// FRConeSegForBrep.h /////
/////////////////////////////

#if !defined FR_CONESEG_FOR_BREP_H
#define FR_CONESEG_FOR_BREP_H

#include "FRBrepSolid.h"
#include "FRConeSeg.h"
#include "FRBrepGenerator.h"

class FRConeSegForBrep : public FRConeSeg, public FRBrepSolid
{
public:
	FRConeSegForBrep(double pRmin1, double pRmax1, double pRmin2, double pRmax2, double pDz, double pSPhi, double pDPhi)
	: FRConeSeg(pRmin1, pRmax1, pRmin2, pRmax2, pDz, pSPhi, pDPhi) { } 
	FRConeSegForBrep(const FRConeSeg& coneseg) : FRConeSeg(coneseg) { } 
	~FRConeSegForBrep() { } 

		//----- Generation of B-Rep data
	void	GenerateBrepData(FRBrepGenerator*) const;

};

#endif
