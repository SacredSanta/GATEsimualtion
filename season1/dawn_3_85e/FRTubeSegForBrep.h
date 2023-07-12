//////////////////////////////
///// FRTubeSegForBrep.h /////
//////////////////////////////

#if !defined FR_TUBE_SEG_FOR_BREP_H
#define FR_TUBE_SEG_FOR_BREP_H

#include "FRBrepSolid.h"
#include "FRTubeSeg.h"
#include "FRBrepGenerator.h"

class FRTubeSegForBrep : public FRTubeSeg, public FRBrepSolid
{
public:
	FRTubeSegForBrep(double pRmin, double pRmax, double pDz, double pSPhi, double pDPhi)
	: FRTubeSeg(pRmin, pRmax, pDz, pSPhi, pDPhi) { } 
	FRTubeSegForBrep(const FRTubeSeg& tubeseg) : FRTubeSeg(tubeseg) { } 

	~FRTubeSegForBrep() { } 

		//----- Generation of B-Rep data
	void	GenerateBrepData(FRBrepGenerator*) const ;
};

#endif
