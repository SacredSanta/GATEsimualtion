//////////////////////////////////////
///// FRParallelepipedForBrep.cc /////
//////////////////////////////////////

#include "FRParallelepipedForBrep.h"
#include "FRBrepGenerator.h"
#include "FRVector3.h"
#include "FRArray.h"
#include <math.h>

void FRParallelepipedForBrep::GenerateBrepData(FRBrepGenerator* factory_ptr) const
{
	factory_ptr->GenerateHeaderInformation();
	factory_ptr->GenerateVertexBlockHeader();
	factory_ptr->GenerateDummyVertex();

	const int num_parallelepiped_vertices = 8;
	FRArray<FRVector3> vertex_array(num_parallelepiped_vertices);

	double cx = fDz * fTthetaCphi;			//	X coord of center at +z plane
	double cy = fDz * fTthetaSphi;			//	Y coord of center at +z plane
	double alpha = atan(fTalpha);			//  angle along y-axis	
	double dx = fDy * tan(alpha);
	
	vertex_array[(size_t)0] = FRVector3( fDx + dx + cx,  fDy + cy,  fDz);		
	vertex_array[(size_t)1] = FRVector3(-fDx + dx + cx,  fDy + cy,  fDz);
	vertex_array[(size_t)2] = FRVector3( fDx - dx + cx, -fDy + cy,  fDz);
	vertex_array[(size_t)3] = FRVector3(-fDx - dx + cx, -fDy + cy,  fDz);
	vertex_array[(size_t)4] = FRVector3( fDx + dx - cx,  fDy - cy, -fDz);
	vertex_array[(size_t)5] = FRVector3(-fDx + dx - cx,  fDy - cy, -fDz);
	vertex_array[(size_t)6] = FRVector3( fDx - dx - cx, -fDy - cy, -fDz);
	vertex_array[(size_t)7] = FRVector3(-fDx - dx - cx, -fDy - cy, -fDz);

////////////////////
//	vertex_array[0] = FRVector3( fDx + dx + cx,  fDy + cy,  fDz);		
//	vertex_array[1] = FRVector3(-fDx + dx + cx,  fDy + cy,  fDz);
//	vertex_array[2] = FRVector3( fDx - dx + cx, -fDy + cy,  fDz);
//	vertex_array[3] = FRVector3(-fDx - dx + cx, -fDy + cy,  fDz);
//	vertex_array[4] = FRVector3( fDx + dx - cx,  fDy - cy, -fDz);
//	vertex_array[5] = FRVector3(-fDx + dx - cx,  fDy - cy, -fDz);
//	vertex_array[6] = FRVector3( fDx - dx - cx, -fDy - cy, -fDz);
//	vertex_array[7] = FRVector3(-fDx - dx - cx, -fDy - cy, -fDz);
//////////////////////

	factory_ptr->GenerateHexahedronVertices(vertex_array);

	factory_ptr->GenerateVertexBlockTailer();
	factory_ptr->GenerateBlankLine();
	factory_ptr->GenerateAttributeBlock();
	factory_ptr->GenerateFacetBlockHeader();

	factory_ptr->GenerateHexahedronFacets();

	factory_ptr->GenerateFacetBlockTailer();
	factory_ptr->GenerateOptimizedData();


}
