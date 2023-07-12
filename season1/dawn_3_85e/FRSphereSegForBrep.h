////////////////////////////////
///// FRSphereSegForBrep.h /////
////////////////////////////////

#if !defined FR_SPHERESEG_FOR_BREP_H
#define FR_SPHERESEG_FOR_BREP_H

#include "FRSphereSeg.h"
#include "FRBrepGenerator.h"

class FRSphereSegForBrep : public FRSphereSeg, public FRBrepSolid
{
public:
	 FRSphereSegForBrep(double R, double r, double phi, double dphi,
	 	double theta, double dtheta)
	 : FRSphereSeg(R, r, phi, dphi, theta, dtheta) { } 
	 FRSphereSegForBrep(const FRSphereSeg& sphereseg) : FRSphereSeg(sphereseg) { } 
	~FRSphereSegForBrep() { } 

		//----- Generation of B-Rep data
	void	GenerateBrepData(FRBrepGenerator*) const;
};

#endif
