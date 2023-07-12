//////////////////////////
///// FRBoxForBrep.h /////
//////////////////////////

#if !defined FR_BOX_FOR_BREP_H
#define FR_BOX_FOR_BREP_H

#include "FRBox.h"
#include "FRBrepSolid.h"
#include "FRBrepGenerator.h"

class FRBoxForBrep : public FRBox, public FRBrepSolid
{
public:
	FRBoxForBrep(double pdx, double pdy, double pdz) : FRBox(pdx, pdy, pdz) { }
	FRBoxForBrep(const FRBox& box) : FRBox(box) { } 
	~FRBoxForBrep() { } 

		//----- Generation of B-Rep data
	void	GenerateBrepData(FRBrepGenerator*) const;
};

#endif
