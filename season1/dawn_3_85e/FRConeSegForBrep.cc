///////////////////////////////
///// FRConeSegForBrep.cc /////
///////////////////////////////

#include <math.h>
#include "FRConeSegForBrep.h"
#include "FRTrigonometric.h"
#include "FRMath.h"

const double epsilon_radius = 0.0;
const double epsilon_radian = 0.0001;

void FRConeSegForBrep::GenerateBrepData(FRBrepGenerator* factory_ptr) const
{
	factory_ptr->GenerateHeaderInformation();
	factory_ptr->GenerateVertexBlockHeader();
	factory_ptr->GenerateDummyVertex();

	const FRTrigonometricTable trigon = factory_ptr->GetTrigonometricTable();
	const FRSegmentTable segment(trigon, fSPhi, fDPhi);
	const int Number = segment.GetSize();

	factory_ptr->GenerateCylindricalVertices( segment, fRmin1, fRmax1, fRmin2, fRmax2, fDz );

	factory_ptr->GenerateVertexBlockTailer();
	factory_ptr->GenerateBlankLine();
	factory_ptr->GenerateAttributeBlock() ;
	factory_ptr->GenerateFacetBlockHeader ();	

	if((fRmin1 <= epsilon_radius) && (fRmin2 <= epsilon_radius))
	{
		if(fabs(fDPhi - 2.0 * FR_D_PI) <= epsilon_radian)
			factory_ptr->GenerateColumnFacets(Number);
		else
			factory_ptr->GenerateColumnSegFacets(Number);
	}
	else
	{
		if(fabs(fDPhi - 2.0 * FR_D_PI) < epsilon_radian)
			factory_ptr->GenerateCylindricalFacets(Number);
		else
			factory_ptr->GenerateCylindricalSegFacets(Number);
	}
	factory_ptr->GenerateFacetBlockTailer();


	factory_ptr->GenerateOptimizedData();

} // FRConeSegForBrep::GenerateBrepData()

