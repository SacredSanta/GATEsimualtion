//////////////////////////
///// FRTorusForBrep.h /////
//////////////////////////

#if !defined FR_TORUS_FOR_BREP_H
#define FR_TORUS_FOR_BREP_H

#include "FRTorus.h"
#include "FRBrepSolid.h"
#include "FRBrepGenerator.h"

class FRTorusForBrep : public FRTorus, public FRBrepSolid
{
public:
	FRTorusForBrep(	double rmin, 
			double rmax, 
			double rtor, 
			double sphi, 
			double dphi  ) : \
		FRTorus( rmin, rmax, rtor, sphi, dphi  ) {}

	FRTorusForBrep(const FRTorus& torus) : FRTorus(torus) { } 

	~FRTorusForBrep() { } 

		//----- Generation of B-Rep data
	void	GenerateBrepData( FRBrepGenerator* ) const;
};

#endif
