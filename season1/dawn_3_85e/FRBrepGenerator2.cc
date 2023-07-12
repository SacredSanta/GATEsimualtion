#include "FRBrepGenerator.h"

	//----- FRBrepGenerator::GenerateHexahedronVertices()
void FRBrepGenerator::GenerateHexahedronVertices( const FRArray<FRVector3>& vec_array )
{
//	int    i ;
	size_t i ;

	for(i = 0; i < 8; i++) {
		GenerateVertex(i+1, vec_array[i]);
	}
}

	//----- FRBrepGenerator::GenerateHexahedronFacets()
void FRBrepGenerator::GenerateHexahedronFacets()
{
	GenerateSquare(1, 2, 4, 3, 0);
	GenerateSquare(5, 6, 2, 1, 0);
	GenerateSquare(6, 8, 4, 2, 0);
	GenerateSquare(8, 7, 3, 4, 0);
	GenerateSquare(7, 5, 1, 3, 0);
	GenerateSquare(7, 8, 6, 5, 0);
}


	//----- FRBrepGenerator::GenerateCylindricalVertices()
void FRBrepGenerator::GenerateCylindricalVertices(	const FRSegmentTable& seg,
							double pRmin1, double pRmax1, 
							double pRmin2, double pRmax2, 
							double pDz)
{
	const int Number = seg.GetSize();
	const FRVector2* vertex_table = seg.GetTable();

	int	i;

	for(i = 0; i < Number; i++)
	{
		FRVector3 temp;	
		double XX = vertex_table[i].x();
		double YY = vertex_table[i].y();

			//----- vertex on top plane (outside)
		temp = FRVector3(pRmax2 * XX, pRmax2 * YY, pDz);
		GenerateVertex(i + 1, temp);
		
			//----- vertex on bottom plane (outside)
		temp = FRVector3(pRmax1 * XX, pRmax1 * YY, -pDz);
		GenerateVertex(i + 1 + Number, temp);
		
			//----- vertex on top plane (inside)
		temp = FRVector3(pRmin2 * XX, pRmin2 * YY, pDz);
		GenerateVertex(i + 1 + 2 * Number, temp);

			//----- vertex on bottom plane (inside)
		temp = FRVector3(pRmin1 * XX, pRmin1 * YY, -pDz);
		GenerateVertex(i + 1 + 3 * Number, temp);
	}
}

	//----- GenerateColumnFacets()
void FRBrepGenerator::GenerateColumnFacets( int N )
{
		//----- local
	int i;
	int*	top_vertex    = new int [ N ]; // top    vertices
	int*	bottom_vertex = new int [ N ]; // bottom vertices

		//----- generate top and bottom vertices
	for(i = 0; i < N; i++)
	{
		top_vertex   [i] = i + 1 ;
		bottom_vertex[i] = 2 * N - i ;
	}
		//----- top
	GenerateFacet(top_vertex , N , 0);

		//----- bottom
	GenerateFacet(bottom_vertex , N , 0);

		//----- outside facets
	for(i = 1; i < N; i++)								
		GenerateSquare(i + 1, i, N + i, N + i + 1, 0);
	GenerateSquare(1, N, 2 * N, N + 1, 0); // last side facet

		//----- clean up
	delete [] top_vertex; 	
	delete [] bottom_vertex; 
}

	//----- GenerateColumnSegFacets()
void FRBrepGenerator::GenerateColumnSegFacets( int N )
{
	int i;
	int*	top_vertex    = new int [ N + 1 ]; // top    vertices
	int*	bottom_vertex = new int [ N + 1 ]; // bottom vertices

		//----- top facet
	top_vertex[0] = 2 * N + 1 ;	// top center
	for(i = 0; i < N; i++)
		top_vertex[i+1] = i + 1 ;
	GenerateFacet(top_vertex , N + 1  , 0);

		//----- bottom facet
	bottom_vertex[0] = 3 * N + 1 ;	// top center
	for(i = 0; i < N; i++)
		bottom_vertex[i+1] = 2 * N - i;
	GenerateFacet(bottom_vertex , N + 1  , 0);

		//----- side facets
	for(i = 1; i < N; i++)								
		GenerateSquare(i + 1, i, N + i, N + i + 1, 0);

		//----- segment facets (start and end)
	GenerateSquare(1, 2 * N + 1, 3 * N + 1, N + 1, 0); // start
	GenerateSquare(2 * N + 1, N, 2 * N, 3 * N + 1, 0); // end

		//----- clean up
	delete [] top_vertex; 	
	delete [] bottom_vertex; 
}

	//----- FRBrepGenerator::GenerateCylindricalFacets()
void FRBrepGenerator::GenerateCylindricalFacets( int N )
{
	int	i;
	int	NDIV = GetNdiv();

		
	for(i = 1; i < NDIV; i++)
	{
			//----- top 
		GenerateSquare(-(i), i + 1, -(2 * N + i+1), 2 * N + i, 0);
		 	//----- bottom
		GenerateSquare(N + i, -(3 * N + i), 3 * N + i + 1, -(N + i + 1), 0);
			//----- outside
		GenerateSquare(i + 1, i, N + i, N + i + 1, 0);	
			//----- inside
		GenerateSquare(2 * N + i, 2 * N + i + 1, 3 * N + i + 1, 3 * N + i, 0);	
	}

		//----- last facets (top, bottom, outside, inside)
		//---------- top 
	GenerateSquare(-(NDIV), 1, -(2 * N + 1), 2 * N + NDIV , 0);
		//---------- bottom
	GenerateSquare( N + NDIV, -(3 * N + NDIV), 3 * N + 1, -(N + 1), 0);
		//---------- outside
	GenerateSquare(1, NDIV, N + NDIV, N + 1, 0);
		//---------- inside
	GenerateSquare(2 * N + NDIV , 2 * N + 1, 3 * N + 1, 3 * N + NDIV, 0);
}


	//----- FRBrepGenerator::GenerateCylindricalSegFacets()
void FRBrepGenerator::GenerateCylindricalSegFacets( int N )
{
	int i;
		//----- first division
	GenerateSquare( 1 , 2, -(2 * N + 2), 2 * N + 1, 0);
	GenerateSquare(N + 1, 3 * N + 1, 3 * N + 2, -(N + 2), 0);
	GenerateSquare(-(2), 1, N + 1, N + 2, 0);
	GenerateSquare(2 * N + 1, 2 * N + 2, 3 * N + 2, 3 * N + 1, 0);	

	for(i = 2; i < N - 1; i++)
	{
		GenerateSquare(-(i), i + 1, -(2 * N + i + 1), 2 * N + i, 0);		//----- top
		GenerateSquare(N + i, -(3 * N + i), 3 * N + i+1, -(N + i + 1), 0);	//-----bottom
		GenerateSquare(i + 1, i, N + i, N + i + 1, 0);			//----- outside
		GenerateSquare(2 * N + i, 2 * N + i+1, 3 * N + i + 1, 3 * N + i, 0);	//-----inside	
	}
		//----- last division
	GenerateSquare(-(N - 1), N, 3 * N, 3 * N - 1, 0);		//----- top
	GenerateSquare(2 * N -1, -(4 * N -1), 4 * N, 2 * N, 0);	//-----bottom
	GenerateSquare(N, N - 1, 2 * N - 1, 2 * N, 0);			//----- outside
	GenerateSquare(3 * N - 1, 3 * N, 4 * N, 4 * N - 1, 0);	//-----inside	

		//----- segment facets (start and end)
	GenerateSquare(1, 2 * N + 1, 3 * N + 1, N + 1, 0);
	GenerateSquare(4 * N, 3 * N, N, 2 * N, 0);
}
