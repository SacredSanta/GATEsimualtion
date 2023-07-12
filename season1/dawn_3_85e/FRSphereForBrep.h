/////////////////////////////
///// FRSphereForBrep.h /////
/////////////////////////////

#if !defined FRSPHERE_FOR_BREP_H
#define FRSPHERE_FOR_BREP_H

#include "FRSphere.h"
#include "FRBrepSolid.h"

class FRSphereForBrep : public FRSphere, public FRBrepSolid
{
 public:
	 FRSphereForBrep(double pR) : FRSphere(pR)  { } 
	 FRSphereForBrep(const FRSphere& sphere) : FRSphere(sphere) { } 
	~FRSphereForBrep() { } 

		//----- Generation of B-Rep data
	void GenerateBrepData(FRBrepGenerator*) const;
};

#endif
