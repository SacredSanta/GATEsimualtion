////////////////////////////////
///// object1.cc	////////
/////                   ////////
////////////////////////////////

#include	"object.h"
#include        "facet_list.h"


	//--------------------------// 
	//----- class Vertex  ------//
	//--------------------------// 

	//----- constructor
Vertex::Vertex ( int label_given     , Vector3 position_given  ) 
{ 
	label = label_given ;
	position = position_given  ;
}

	//----- constructor
Vertex::Vertex ( int label_given ) 
{ 
	Vector3 ZERO(0.0, 0.0, 0.0) ;

	label    = label_given ;
	position = ZERO ;
}

	//----- constructor
Vertex::Vertex ( void ) 
{ 
	Vector3 ZERO(0.0, 0.0, 0.0) ;

	label = NO_LABEL ;
	position = ZERO  ;

}

	//---------- operator = ()
int	Vertex::operator = ( const Vertex& rhs )
{
	label = rhs.label ;
	position = rhs.position  ;

		//----- return
	return NORMAL ;

}	// Vertex::operator =()



	//--------------------------// 
	//----- class Edge    ------//
	//--------------------------// 

	//----- constructor
Edge::Edge (	int label_,
		int vertex_  	, int prevVertex_ ,
		int nextEdge_ 	, int prevEdge_   ,
		int otherEdge_	, int rightFacet_  ,
		int leftFacet_	, 
		int visibility_ ,
		LineType* linetype_p                   )
{
		//----- Label
	label = label_ ;	

		//----- vertices
	vertex  	= vertex_	  ; 	prevVertex	= prevVertex_ ;

		//----- edges
	nextEdge	= nextEdge_   ; 	prevEdge	= prevEdge_   ;
	otherEdge	= otherEdge_  ;
	
		//----- facets
	rightFacet	= rightFacet_ ; 	leftFacet	= leftFacet_  ;

		//----- visibility
	visibility = visibility_;

		//----- working variables for drawing
	lineType_p	= linetype_p   ;
	flag		= 0 ;

} // Edge ()


	//---------- operator = ()
int	Edge::operator = ( const Edge& rhs )
{
		//----- Label
	label           = rhs.label ;

		//----- Connected vertices
	vertex		= rhs.vertex  ;		
	prevVertex 	= rhs.prevVertex ;	

		//----- Connected edges
	nextEdge	= rhs.nextEdge	 ;
	prevEdge	= rhs.prevEdge	 ;
	otherEdge	= rhs.otherEdge  ;

		//----- Connected Faces
	rightFacet	= rhs.rightFacet ;
	leftFacet	= rhs.leftFacet  ;

		//----- visibility
	visibility      = rhs.visibility      ;

		//----- working linetype
	lineType_p	= rhs.lineType_p ;

		//----- working flag
	flag            = rhs.flag ;

		//----- return
	return  NORMAL ;

} // Edge::operator =()



	//--------------------------// 
	//----- class Facet   ------//
	//--------------------------// 

	//----- constructor
Facet::Facet ( int label_ )
	: edgeList(), attribute() 
{
		//----- Label
	label = label_ ;

		//----- color
	flag_color_set = OBJECT_COLOR ;
	color.setRGB( 0.0, 0.0, 0.0 ) ;

		//----- normal vector in world coordinate
	normalVector( 0.0 , 0.0 , 0.0 ) ;

		//------ first vertex position in world coordinate
	oneVertexPosition( 0.0 , 0.0 , 0.0 ) ;

		//----- initialize edge arrays
	edgeArray = NULL ;

		//----- initialize working flag
	flag = 0 ;

} // Facet::Facet ( int , char* )


	//----- Facet::clear()
void	Facet::clear()
{
	edgeList.clear() ; 
	delete [] edgeArray ; edgeArray = NULL ; 
}


	//----- Facet::setNormalVector()
int	Facet::setNormalVector( Object* objItem_p )
{
		//----- local variables
	int 		i ;         	// loop variable
	Vector3**	point_vector;	// array of vertex point address
	int 		status;     	// return status
	int 		edge , vertex ;	// edge and vertex labels
	Edge		edgeItem ;    	// item of edge
	int 		num_point = edgeList.getNItem() ; 
				// = number of vertex in this facet

		//----- dynamical memory allocation
	point_vector = new Vector3_p[ num_point ] ;
	if( point_vector == NULL ) { 
		cerr << "ERROR ( Facet::setNormalVector() ) 1\n"; 
		return ERROR ;
	}

		//----- calc vertex points in world coordinate
	edgeList.toTail();	i = 0 ;
	while( edgeList.getItem( edge ) ) {

			//----- get edge item correspoinding to label "edge"
		status = objItem_p->getEdgeItem( edge , edgeItem );
		if( !status ) { 
			delete [] point_vector ;			
			return ERROR ;
		}

			//----- get vertex label connecting to the edge
		vertex = edgeItem.getVertex();

			//-----  get vertex position in world coord 
			//.....   for the obtained vertex label
		if( objItem_p->vertexPosition != NULL ) {
			point_vector[i] \
			= &(objItem_p->vertexPosition[ vertex ]) ;
		} else {
			cerr << "ERROR (Facet::setNormalVector) 2\n";
			delete [] point_vector ;
			return ERROR ;
		}

		//----- increment current node of list
		edgeList.upward();
		i++;
	}

		//----- get a vertex position ( world coordinate ) 
		//.....  of this facet 
	oneVertexPosition = *(point_vector[ 0 ]) ;

		//----- get normal vector in world coordinate
	normalVector = Tool3D::getNormalVector( point_vector , num_point ) ;

		//----- delete dynamical memory
	delete [] point_vector ;

		//----- normal vector successfully obtained
	return NORMAL;

} // Facet::setNormalVector( Object* )


	//----- getEdgeItem ()
int	Facet::getEdgeItem ( int edge_label, int& edge_item ) const 
{
	//----- error checking ( label out of range?)
	if( edge_label < 0 || edge_label >= (getNumEdgeInList()) )
	{
		return ERROR ;
	}

	//----- set return  value
	if( edgeArray != NULL ) {
		edge_item = edgeArray[ edge_label ];
	} else {
		cerr << "ERROR ( Facet::getEdgeItem()): edgeArray = NULL\n";
		return ERROR;
	}

	//----- edge item successfully obtained
	return NORMAL ;

} // Facet::getEdgeItem ()


	//----- setEdgeArray()
int  Facet::setEdgeArray()
{
		//----- local variables
	int		label ;		// label (Facet edgeList)
	int		edge = 0 ;	// edge label (object edgeList)
	int		n = getNumEdgeInList() ; // number of edge items

		//----- clear array if already exists
	if( edgeArray != NULL ) { 
		delete [] edgeArray ; edgeArray = NULL ; 
	}

		//----- allocate dynamical memories
	edgeArray = new int[ n ] ;
	if( edgeArray == NULL ) { 
		cerr << "ERROR ( Facet::setEdgeArray() ) \n"; 
		return ERROR ;
	}


		//----- substitute items to array
	edgeList.toTail() ;
	for(label = 0 ; label < n ; label++ ) {
		edgeList.getItem( edge ) ;
		edgeArray[label] = edge ;
		edgeList.upward();
	}

		//----- array successfully made
	return NORMAL ;

} //Facet::setEdgeArray()


	//---------- operator = ()
int	Facet::operator = ( const Facet& rhs )
{
		//----- local
	int i, num_edge;

		//----- Label
	setLabel( rhs.getLabel() ) ;

		//----- color 
	flag_color_set = rhs.isColorSet() ;
	color          = rhs.getColor();

		//----- Attributes
	this->setAttribute( rhs.getAttribute() );

        	//----- list of edges
	edgeList = rhs.edgeList ;

		//----- normal vector of this facet
	normalVector	= rhs.getNormalVector() ;

		//----- vertex position of this facet
	oneVertexPosition  = rhs.getOneVertexPosition() ;

		//----- edge Array
	if( edgeArray != NULL ) {
		delete [] edgeArray ; edgeArray = NULL ; 
	}
	num_edge = rhs.getNumEdgeInList() ;

	edgeArray = new int [ num_edge ];
	if( edgeArray == NULL ) { 
		cerr << "ERROR ( Facet::operator = () ) \n"; 
		return ERROR ;
	}


	for( i = 0 ; i < num_edge ; i++ ) {
		rhs.getEdgeItem( i, edgeArray[i] ) ;
	}

		//----- working flag
	flag = rhs.getFlag() ;

		//----- return
	return NORMAL ;

} // Facet::operator = ()


	//----- Facet::setColorMode ()
void
Facet::setColorMode ( int mode ) 
{
	if        ( mode  <= OBJECT_COLOR ) {
		flag_color_set = OBJECT_COLOR ;
	} else if ( mode  > HARD_COLOR ) {
		flag_color_set = OBJECT_COLOR ;
	} else {
		flag_color_set = mode ;
	}

} // Facet::setColorMode ()

