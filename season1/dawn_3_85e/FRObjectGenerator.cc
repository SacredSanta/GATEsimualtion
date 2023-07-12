/////////////////////////////////
///// FRObjectGenerator.cc  /////
/////////////////////////////////

#include<stdlib.h>
#include "FRObjectGenerator.h"
#include "object.h"

	//----- FRObjectGenerator::GenerateHeaderInformation()
void	FRObjectGenerator::GenerateHeaderInformation()
{
	this->GenerateHeaderComment();    // do nothing
	this->GenerateCoordBlock();
	this->GenerateBoundingBoxBlock(); // do nothing
}

	//----- FRObjectGenerator::GenerateCoordBlock()
void	FRObjectGenerator::GenerateCoordBlock()
{
	Vector3 the_origin ( origin.x(),  origin.y(),  origin.z() );  
	Vector3 the_e1     ( e1.x(),  e1.y(),  e1.z() );  
	Vector3 the_e2     ( e2.x(),  e2.y(),  e2.z() );  

	object_p->setOrigin     ( the_origin );
	object_p->setBaseVector ( the_e1, the_e2 ) ; 
}

	//----- FRObjectGenerator::GenerateAttributeBlock() 
void FRObjectGenerator::GenerateAttributeBlock() 
{
	object_p->setAttribute( *att_p ) ;
} 

	//----- FRObjectGenerator::GenerateDummyVertex()
void FRObjectGenerator::GenerateDummyVertex()
{
	Vertex	dummy_vertex(0, Vector3(0.0, 0.0, 0.0) );
	object_p->addVertexItem( dummy_vertex ); 
}


	//----- FRObjectGenerator::GenerateVertex( label , x, y, z )
void	FRObjectGenerator::GenerateVertex( int label, double body_x, double body_y, double body_z ) 
{
	FRVector3 Body ( body_x, body_y, body_z );
	this->GenerateVertex( label, Body );
}

	//----- FRObjectGenerator::GenerateVertex( label , Body )
void	FRObjectGenerator::GenerateVertex( int label, const FRVector3& Body ) 
{
	// Note: Body->world transformation is done inside 
	//       Object class

	Vector3 P( Body.x(), Body.y(), Body.z() ); 
	Vertex	vertex_item ( label, P ); 
	object_p->addVertexItem( vertex_item );

}


	//----- FRObjectGenerator::GenerateFacet ()
void	FRObjectGenerator::GenerateFacet ( int* v, int n, int dummy ) 
{
	int	i;					// loop variable
	int	edge;					// current edge label
	int*	edge_data	= new int [n] ;		// edge label list

	dummy = 0 ;

	int	startEdge = object_p->getNumEdgeInList(); // starting edge label
	int	endEdge = startEdge + n - 1;		   // ending  edge label

		//----- data which constructs half-edge data
	const int INVISIBLE = 0 ;
	int	vertex;					
	int	pVertex = v[n-1];			//  initialized to the last label
	int	prev;					// previous edge
	int	next;					// next edge
	int	other;					// other-half edge
	int	facet = object_p->getNumFacetInList();	// left-facet: current facet label
	int	rFacet;					// right-facet

		//----- dummy routine
	dummy = 0 ;

		//----- too few vertices for a facet?
	if( n < 3 ) {
		cerr << "Too few vertices for a facet\n" ;
		return  ;
	}

		//----- add edge data to object
	for( i=0, edge=startEdge; i < n; i++, edge++ )
	{
		vertex = v[i]; // ending point of current edge
		next = (edge == endEdge) ? startEdge : edge + 1; // next edge
		prev = (edge == startEdge) ? endEdge : edge - 1; // prev edge
		other = rFacet = NO_ITEM ;

			//----- add Edge to list in Object
		if( vertex < 0 ) { 
				//----- add INVISIBLE edge
			object_p->addEdgeItem(Edge(	edge, \
							abs(vertex), abs(pVertex), \
							next, prev, other, \
							rFacet, facet,\
							INVISIBLE                 ));
		} else {
				//----- add VISIBLE edge
			object_p->addEdgeItem(Edge( edge, 
						 abs(vertex), abs(pVertex), 
						 next, prev, other, 
						 rFacet, facet             ));
		}

			//----- prepare to add edge's label to list in Facet
		edge_data[i] = edge;

			//----- update prev vertex label
		pVertex = vertex;
	}// for

		//----- add facet data to object
		//----------- declare facet 
	Facet	facetItem( facet );

		//----------- edge
	for(i=0 ; i<n; i++) { 
		facetItem.addEdge( edge_data[i] ) ; 
	}
	facetItem.setEdgeArray() ; 
		// do nothing in current version of class Facet

		//----------- color
	facetItem.setAttribute( *att_p );
	facetItem.setColorMode( Facet::SOFT_COLOR );

		//---------- add facet to object
	object_p->addFacetItem( facetItem );

		//----- delete 
	delete [] edge_data ;
}


	//----- FRObjectGenerator::GenerateSquare ()
void	FRObjectGenerator::GenerateSquare   ( int v0, int v1, int v2, int v3, int dummy ) 
{
	int	num_vertex = 4 ;
	int	vertex[4] ;
	dummy = 0 ;

	vertex[0] = v0 ;	vertex[1] = v1 ; 
	vertex[2] = v2 ;	vertex[3] = v3 ; 

	this->GenerateFacet( vertex, num_vertex , dummy );
}


	//----- FRObjectGenerator::GenerateTriangle ()
void	FRObjectGenerator::GenerateTriangle ( int v0, int v1, int v2,  int dummy ) 
{
	int	num_vertex = 3 ;
	int	vertex[3] ;
	dummy = 0 ;

	vertex[0] = v0 ;	vertex[1] = v1 ; 
	vertex[2] = v2 ;

	this->GenerateFacet( vertex, num_vertex, dummy );
}

	//----- FRObjectGenerator::GenerateLineseg ()
void	FRObjectGenerator::GenerateLineseg ( int v0, int v1, int att_label ) 
{
		//----- it must be via file
	FRBrepGenerator::GenerateLineseg (v0, v1, att_label ) ;
}


	//----- FRObjectGenerator::GenerateOptimizedData () 
void	FRObjectGenerator::GenerateOptimizedData () 
{
	object_p->install( winged_edge_mode );
}

