#include <stdio.h>
#include "brepformat.h"
#include "object.h"
#include "polyhedron.h"


	//----- Polyhedron::GenerateBrepData()
void Polyhedron::GenerateOneVertex(	int label                     , 
					const FRString&   string_data ,
					FRBrepGenerator* factory_ptr   ) 
{
		//----- local
	const char* line = string_data.GetCharString(); // dummy_str x y z
	int vlabel = label ;
	char   dummy[256];
	double x, y, z ;

		//----- read label, x, y, z of a vertex
		//.....  Note: Irregular lines, e.g. blak lines
		//.....        are skipped
	if ( sscanf(	line, "%s %lg %lg %lg", \
			       dummy, &x, &y, &z ) != EOF ) 
	{ 		
			//----- add label and vertex to object
		if( vlabel > 0 ) {
			factory_ptr->GenerateVertex( vlabel,  x, y, z ) ;
		} else {
			cerr << "WARNING: Irrelevant vertex label, 1" << vlabel;
			cerr << endl;
		}
	} // if

} // Polyhedron::GenerateOneVertex(label,...)



	//----- Polyhedron::GenerateBrepData()
void Polyhedron::GenerateOneVertex(	const FRString&   string_data ,
					FRBrepGenerator* factory_ptr ) 
{
		//----- local
	const char* line = string_data.GetCharString(); // label x y z
	int vlabel ;
	double x, y, z ;

		//----- read label, x, y, z of a vertex
		//.....  Note: Irregular lines, e.g. blak lines
		//.....        are skipped
	if ( sscanf(	line, "%d %lg %lg %lg", \
			&vlabel, &x, &y, &z ) != EOF ) 
	{ 		
			//----- add label and vertex to object
		if( vlabel > 0 ) {
			factory_ptr->GenerateVertex( vlabel,  x, y, z ) ;
		} else {
			cerr << "WARNING: Irrelevant vertex label, 2" << vlabel;
			cerr << endl;
		}
	} // if

} // Polyhedron::GenerateOneVertex()


	//----- Polyhedron::GenerateOneFacet()
void Polyhedron::GenerateOneFacet(	const FRString&   string_data ,
					FRBrepGenerator* factory_ptr    ) 
{
	
		//----- object
	Object& object \
		= *( ((FRObjectGenerator*)(factory_ptr))->GetObject() ) ;

		//----------  make integer array of vertex labels
	const int VERTEX_MAX  = DEFAULT_VERTEX_MAX  ;
	if( getenv( "DAWN_MAX_FACET_SIZE" ) != NULL ) {
		sscanf( getenv("DAWN_MAX_FACET_SIZE" ), "%d", &VERTEX_MAX ) ;
	} 
	int*  v      = new int  [ VERTEX_MAX ] ; // array of vertex labels
	int   n      = 0 ;                       // num vertex labels
	string_data.Decompose( v, &n, VERTEX_MAX ) ;

		//----- other local variables
	int	i;
	int	edge;					// current edge
	int*	edge_data	= new int [n] ;		// edge list

	int	startEdge = object.getNumEdgeInList();	// start edge
	int	endEdge   = startEdge + n - 1;		// end   edge

		//----- data which constructs half-edge data
	int	vertex;					// vertex
	int	pVertex = v[n-1];			// previous vertex
	int	prev;					// previous edge
	int	next;					// next edge
	int	other;					// other-half edge
	int	facet = object.getNumFacetInList();	// left-facet
	int	rFacet;					// right-facet


		//----- add edge data to object
	for(i=0, edge=startEdge; i < n; i++, edge++)
	{
		vertex = v[i];

		next = (edge == endEdge)   ? startEdge : edge + 1;

		prev = (edge == startEdge) ? endEdge   : edge - 1;

		other = rFacet = NO_ITEM ;

			//----- add Edge to list in Object
		const int INVISIBLE = 0 ;
		if( vertex < 0 ) { 
				//----- add INVISIBLE edge
			object.addEdgeItem(Edge( edge, \
						 abs(vertex), abs(pVertex), \
						 next, prev, other, \
						 rFacet, facet,\
						 INVISIBLE                 ));
		} else {
				//----- add VISIBLE edge
			object.addEdgeItem(Edge( edge, 
						 abs(vertex), abs(pVertex), 
						 next, prev, other, 
						 rFacet, facet             ));
		}

			//----- add edge's label to list in Facet
		edge_data[i] = edge;

		pVertex = vertex;
	}


		//----- add facet data to object
		//----------- declare facet with label
	Facet	facetItem( facet );

		//----------- edge
	for(i=0 ; i<n; i++) { 
		facetItem.addEdge( edge_data[i] ) ; 
	}
	facetItem.setEdgeArray() ; 
		// do nothing in current version of class Facet

		//----------- color
	facetItem.setAttribute( *(factory_ptr->GetAttribute()) );
	facetItem.setColorMode( Facet::SOFT_COLOR );

		//---------- add facet to object
	object.addFacetItem(facetItem);

		//----- delete
	delete [] edge_data ;
	delete [] v         ;

		//----- return
	return ;

} // Polyhedron::GenerateFacet()





