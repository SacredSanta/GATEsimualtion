/////////////////////////////////////
///// FRParallelepipedForBrep.h /////
/////////////////////////////////////

#if !defined FR_PARALLELEPIPED_FOR_BREP_H
#define FR_PARALLELEPIPED_FOR_BREP_H

#include "FRParallelepiped.h"
#include "FRBrepSolid.h"

class FRParallelepipedForBrep : public FRParallelepiped, public FRBrepSolid
{
public:
	 FRParallelepipedForBrep(double pDx, double pDy, double pDz, double pAlp, double pTheta, double pPhi)
	 : FRParallelepiped(pDx, pDy, pDz, pAlp, pTheta, pPhi) { } 
	 FRParallelepipedForBrep(const FRParallelepiped& parallelepiped)
	 : FRParallelepiped(parallelepiped) { } 

	~FRParallelepipedForBrep() { } 

		//----- Generation of B-Rep data
	void GenerateBrepData(FRBrepGenerator*) const;
};

#endif
