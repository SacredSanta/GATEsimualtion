////////////////////////////////
///// FRTrapForBrep.h /////
////////////////////////////////

#if !defined FR_TRAP_FOR_BREP_H
#define FR_TRAP_FOR_BREP_H

#include "FRTrap.h"
#include "FRBrepSolid.h"
#include "FRBrepGenerator.h"

class FRTrapForBrep : public FRTrap, public FRBrepSolid
{
public:
	FRTrapForBrep(double dz, double theta, double phi,
			double h1, double bl1, double tl1, double alp1,
			double h2, double bl2, double tl2, double alp2)
	: FRTrap(dz, theta, phi, h1, bl1, tl1, alp1, h2, bl2, tl2, alp2) { } 

	FRTrapForBrep(const FRTrap& trap) : FRTrap(trap) { } 
	~FRTrapForBrep() { } 

		//----- Generation of B-Rep data
	void	GenerateBrepData(FRBrepGenerator*) const ;
};

#endif
