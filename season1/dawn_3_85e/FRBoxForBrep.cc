///////////////////////////
///// FRBoxForBrep.cc /////
///////////////////////////

#include "FRBoxForBrep.h"
#include "FRVector3.h"
#include "FRArray.h"
//#include "fHexahedron.h"

void FRBoxForBrep::GenerateBrepData(FRBrepGenerator* factory_ptr) const
{
	factory_ptr->GenerateHeaderInformation();
	factory_ptr->GenerateVertexBlockHeader();
	factory_ptr->GenerateDummyVertex();
	   
	const int num_box_vertices = 8;
	FRArray<FRVector3> vertex_array(num_box_vertices);

	vertex_array[(size_t)0] = FRVector3( fDx,  fDy,  fDz);
	vertex_array[(size_t)1] = FRVector3(-fDx,  fDy,  fDz);
	vertex_array[(size_t)2] = FRVector3( fDx, -fDy,  fDz);
	vertex_array[(size_t)3] = FRVector3(-fDx, -fDy,  fDz);
	vertex_array[(size_t)4] = FRVector3( fDx,  fDy, -fDz);
	vertex_array[(size_t)5] = FRVector3(-fDx,  fDy, -fDz);
	vertex_array[(size_t)6] = FRVector3( fDx, -fDy, -fDz);
	vertex_array[(size_t)7] = FRVector3(-fDx, -fDy, -fDz);

////////////////////////////////////////////////////
//	vertex_array[0] = FRVector3( fDx,  fDy,  fDz);
//	vertex_array[1] = FRVector3(-fDx,  fDy,  fDz);
//	vertex_array[2] = FRVector3( fDx, -fDy,  fDz);
//	vertex_array[3] = FRVector3(-fDx, -fDy,  fDz);
//	vertex_array[4] = FRVector3( fDx,  fDy, -fDz);
//	vertex_array[5] = FRVector3(-fDx,  fDy, -fDz);
//	vertex_array[6] = FRVector3( fDx, -fDy, -fDz);
//	vertex_array[7] = FRVector3(-fDx, -fDy, -fDz);
//////////////////////////////////////////////////////

	factory_ptr->GenerateHexahedronVertices(vertex_array );
	factory_ptr->GenerateVertexBlockTailer();
	factory_ptr->GenerateAttributeBlock();
	factory_ptr->GenerateFacetBlockHeader();
 	factory_ptr->GenerateHexahedronFacets();
	factory_ptr->GenerateFacetBlockTailer();

	factory_ptr->GenerateOptimizedData();
}
