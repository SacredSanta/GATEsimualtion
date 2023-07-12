////////////////////////////////
///// FRTrdForBrep.h /////
////////////////////////////////

#if !defined FR_TRD_FOR_BREP_H
#define FR_TRD_FOR_BREP_H

#include "FRTrd.h"
#include "FRBrepSolid.h"
#include "FRBrepGenerator.h"

class FRTrdForBrep : public FRTrd, public FRBrepSolid
{
public:
	FRTrdForBrep(double pdx1, double pdx2, double pdy1, double pdy2, double pdz)
	: FRTrd(pdx1, pdx2, pdy1, pdy2, pdz) { }
	FRTrdForBrep(const FRTrd& trd) : FRTrd(trd) { } 
	~FRTrdForBrep() { } 

		//----- Generation of B-Rep data
	void	GenerateBrepData(FRBrepGenerator*) const ;
};

#endif
