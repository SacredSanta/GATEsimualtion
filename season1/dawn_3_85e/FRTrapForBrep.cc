////////////////////////////
///// FRTrapForBrep.cc /////
////////////////////////////

#include "FRTrapForBrep.h"
#include "FRVector3.h"
#include "FRArray.h"
#include <math.h>

void FRTrapForBrep::GenerateBrepData(FRBrepGenerator* factory_ptr) const
{
	factory_ptr->GenerateHeaderInformation();
	factory_ptr->GenerateVertexBlockHeader();
	factory_ptr->GenerateDummyVertex();
	   
	const int num_trap_vertices = 8;
	FRArray<FRVector3> vertex_array(num_trap_vertices);

	double cx = fDz * tan(fTheta) * cos(fPhi);
	double cy = fDz * tan(fTheta) * sin(fPhi);
	double dx1 = fH1 * tan(fAlpha1);
	double dx2 = fH2 * tan(fAlpha2);

	vertex_array[(size_t)0] = FRVector3(  fTL2 - dx2 + cx,  fH2 + cy,  fDz);
	vertex_array[(size_t)1] = FRVector3( -fTL2 - dx2 + cx,  fH2 + cy,  fDz);
	vertex_array[(size_t)2] = FRVector3(  fBL2 + dx2 + cx, -fH2 + cy,  fDz);		
	vertex_array[(size_t)3] = FRVector3( -fBL2 + dx2 + cx, -fH2 + cy,  fDz);
	vertex_array[(size_t)4] = FRVector3(  fTL1 - dx1 - cx,  fH1 - cy, -fDz);
	vertex_array[(size_t)5] = FRVector3( -fTL1 - dx1 - cx,  fH1 - cy, -fDz);
	vertex_array[(size_t)6] = FRVector3(  fBL1 + dx1 - cx, -fH1 - cy, -fDz);		
	vertex_array[(size_t)7] = FRVector3( -fBL1 + dx1 - cx, -fH1 - cy, -fDz);

//////////////////////////
//	vertex_array[0] = FRVector3(  fTL2 - dx2 + cx,  fH2 + cy,  fDz);
//	vertex_array[1] = FRVector3( -fTL2 - dx2 + cx,  fH2 + cy,  fDz);
//	vertex_array[2] = FRVector3(  fBL2 + dx2 + cx, -fH2 + cy,  fDz);		
//	vertex_array[3] = FRVector3( -fBL2 + dx2 + cx, -fH2 + cy,  fDz);
//	vertex_array[4] = FRVector3(  fTL1 - dx1 - cx,  fH1 - cy, -fDz);
//	vertex_array[5] = FRVector3( -fTL1 - dx1 - cx,  fH1 - cy, -fDz);
//	vertex_array[6] = FRVector3(  fBL1 + dx1 - cx, -fH1 - cy, -fDz);		
//	vertex_array[7] = FRVector3( -fBL1 + dx1 - cx, -fH1 - cy, -fDz);
///////////////////////

	factory_ptr->GenerateHexahedronVertices(vertex_array);
	
	factory_ptr->GenerateVertexBlockTailer();
	factory_ptr->GenerateBlankLine();
	factory_ptr->GenerateAttributeBlock() ;
	factory_ptr->GenerateFacetBlockHeader();

	factory_ptr->GenerateHexahedronFacets();

	factory_ptr->GenerateFacetBlockTailer();
	factory_ptr->GenerateOptimizedData();

}
