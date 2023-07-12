///////////////////////////////////////////
//////////  hid1.cc  1995/01/23 b //////////
///////////////////////////////////////////

	//-------------------------------//
	//-----  DEBUG macro        -----//
	//-------------------------------//
// #define DEBUG_HID1

	//-------------------------------//
	//-----  include files      -----//
	//-------------------------------//
#include "hid.h"
#include "toolio.h" 



///////////////////////////////////////////////////////
///// member functions of QuickSort<Polygon3D_p> //////
///////////////////////////////////////////////////////

	//----- QuickSort<Polygon3D_p>::compare0()
int  QuickSort<Polygon3D_p>::compare0( Polygon3D_p *a , Polygon3D_p *b )
{
		//----- compare
	if         ( (*a)->zmin < (*b)->zmin ) {
		return  -1 ;	// *a is nearer to eye than *b
	} else 	if ( (*b)->zmin < (*a)->zmin ) {
		return   1 ;	// *b is nearer to eye than *a
	} else {
		return 0 ;
	}

} // QuickSort<Polygon3d_p>compare0()	

	//----- QuickSort<Polygon3D_p>::compare1()
	//.....  EMPTY
int  QuickSort<Polygon3D_p>::compare1( Polygon3D_p *a , Polygon3D_p *b )
{ 

	return ( a == b ) ; // dummy
}


//-------------------------------------------------//
//-----  member functions of class Polygon2D  -----//
//-------------------------------------------------//

		//----- Polygon2D::Polygon2D ( void ), constructor
Polygon2D::Polygon2D ( void )
{
		//----- set data members
	color		= NULL ;
	numVertex	= 0    ;
	vertex2D	= NULL ;
	edgeType	= NULL ;
	objectLabel	= -1   ;
	parent3D_p 	= NULL ;

} // Polygon2D::Polygon2D(void)


	//----- Polygon2D::Polygon2D (....) , constructor
Polygon2D::Polygon2D (	Vector2*	position     , 
			LineType**	linetype     , 
			int		num          , 
			ColorType*	color_p      , 
			int		object_label ,
			Polygon3D* 	parent_polygon3D_p )
{
	numVertex	= 0;
	vertex2D	= NULL ;
	edgeType	= NULL ;
	this->set( position, linetype, num , color_p , object_label , parent_polygon3D_p );

} // Polygon2D::Polygon2D (...)


	//----- set data
void
Polygon2D::set (	Vector2*	vertex2D_given  , 
			LineType**	edgeType_given  ,
			int		numVertex_given ,
			ColorType*	color_p         ,
			int		object_label    ,
			Polygon3D* 	parent_polygon3D_p )
{
		//----- local variables
	int		i ;
	Vector2*	vertex2D_old = this->vertex2D ; // backup old data
	LineType**	edgeType_old = this->edgeType ; // backup old data

		//----- set number of vertices
	numVertex	= numVertex_given ;

		//----- to few vertices?
	if( numVertex < 3 ){
		cerr << "ERROR ( Polygon2D::set () ) 1\n";
		exit(1);
	}


		//----- color
	color = color_p ;

		//----- set new vertex2D
	vertex2D	= new Vector2  [ numVertex ] ;
	if( vertex2D == NULL ){
		cerr << "ERROR ( Polygon2D::set () ) 2\n";
		exit(1);
	}
	for( i = 0 ; i < numVertex ; i++ ) {
		vertex2D[i] = vertex2D_given[i] ;
	}

		//---------- set types of edges
	if( edgeType_given == NULL ) {
		edgeType = NULL ;
	} else {
		edgeType	= new LineType_p [ numVertex ];
		if( edgeType == NULL ) {
			cerr << "ERROR ( Polygon2D::set () ) 2\n";
			cerr << "    Cannot allocate memory  sizeof(LineType * ) x " << numVertex_given << endl;
			exit(1);
		}

		for( i = 0 ; i < numVertex ; i++ ) 
			edgeType[i] = edgeType_given[i];
	}	


		//----- set  object label 
	this->objectLabel = object_label ;

		//----- set  parent Polygon3D
	this->parent3D_p = parent_polygon3D_p ;

		//----- delete dynamical memories for old data
	if( edgeType_old != NULL ) { delete [] edgeType_old ; }

	if( vertex2D_old != NULL ) { delete [] vertex2D_old ; }

} // Polygon2D::set()


	//----- Polygon2D::set ( list )
void Polygon2D::set (	List<Vertex2WithLineType>&	list_vlt         , 
			ColorType*			color_p           , 
			int				object_label      ,
			Polygon3D* 			parent_polygon3D_p )
{
		//----- local variables
	int		i ;
	Vector2*	vertex2D_old = this->vertex2D ; // backup old data
	LineType**	edgeType_old = this->edgeType ; // backup old data
	Vertex2WithLineType	item_vlt ;

		//----- set number of vertices
	numVertex	= list_vlt.getNItem();

		//----- to few vertices?
	if( numVertex < 3 ){
		cerr << "ERROR ( Polygon2D::set ( list ) ) 1\n";
		exit(1);
	}

		//----- color
	color = color_p ;

		//----- dnamical memory
	vertex2D	= new Vector2  [ numVertex ] ;
	edgeType	= new LineType_p [ numVertex ];
	if( vertex2D == NULL || edgeType == NULL ){
		cerr << "ERROR ( Polygon2D::set ( list ) ) 2 \n";
		exit(1);
	}

		//----- set data
	list_vlt.toTail();
	for( i = 0 ; i < numVertex ; i++ ) {
			//----- get i-th vertex with linetype
		list_vlt.getItem( item_vlt );		

			//----- set i-th vertex data
		vertex2D[i] = item_vlt.position ;
		edgeType[i] = item_vlt.linetype ;

			//----- updation
		list_vlt.upward();
	}

		//----- set  object label 
	this->objectLabel = object_label ;

		//----- set  parent Polygon3D
	this->parent3D_p = parent_polygon3D_p ;

		//----- delete dynamical memories for old data
	if( edgeType_old != NULL ) { delete [] edgeType_old ; }
	if( vertex2D_old != NULL ) { delete [] vertex2D_old ; }

} // Polygon2D::set ( list )


	//----- Polygon2D::~Polygon2D ()
Polygon2D::~Polygon2D (){ clear() ; }


		//----- Polygon2D::clear()
void
Polygon2D::clear(void)
{
	color = NULL;
	delete [] edgeType ;	edgeType	= NULL ;
	numVertex	= 0;
	delete [] vertex2D ;	vertex2D	= NULL ;
	objectLabel = -1 ;
	parent3D_p  = NULL;

} // Polygon2D::clear()



	//----- Polygon2D::opeartor = ()
void
Polygon2D::operator = ( const Polygon2D& rhs )
{
		//----- copy rhs stored in buffer to lhs
	this->set(	rhs.vertex2D    , 
			rhs.edgeType    , 
			rhs.numVertex   , 
			rhs.color       , 
			rhs.objectLabel , 
			rhs.parent3D_p   );

} // Polygon2D::operator =()


	//----- Polygon2D::opeartor = ()
void
Polygon2D::operator = ( Polygon2D& rhs )
{
		//----- copy rhs stored in buffer to lhs
	this->set(	rhs.vertex2D    , 
			rhs.edgeType    , 
			rhs.numVertex   , 
			rhs.color       , 
			rhs.objectLabel , 
			rhs.parent3D_p   );

} // Polygon2D::operator =()


	//----- Polygon2D::removeZeroLengthEdge( double epsilon )
int
Polygon2D::removeZeroLengthEdge( double epsilon )
{
		//----- local constants
	int	i ;
	int	flag_polygon_modified ;
	double	length_x, length_y ;
	ColorType*			color_backup = this->color;	
	List<Vertex2WithLineType>	list_vlt ;	
	Vertex2WithLineType		P_vlt, Q_vlt ;		
	Vector2				P, Q ;

		//----- too few vertices of polygon?
	if( numVertex < 3 ) {
		cerr << "WARNING  (Polygon2D::removeZeroLengthEdge()) 1\n";
		return numVertex ;
	}

		//----- make initial polygon list 
	for ( i = 0 ; i < numVertex ; i++ ) {
		list_vlt.addItem( Vertex2WithLineType( vertex2D[i], edgeType[i] ) );
	}

			//----- initialization for while loop
	list_vlt.toTail();	flag_polygon_modified = 0 ;

			//----- delete edges with zero length
	while( !list_vlt.isCurrentNodeNull() ) {
			//----- starting point of edge PQ
		list_vlt.getItem( P_vlt );	
		P = P_vlt.position ;

			//----- ending point of edge PQ
		list_vlt.getUpwardItemCyclic( Q_vlt );
		Q = Q_vlt.position ;	

			//----- length of edge PQ
		length_x = fabs( Q.x - P.x );
		length_y = fabs( Q.y - P.y );

			//----- if edge PQ has zero length, delete P
		if( length_x < epsilon && length_y < epsilon ) {
				//----- delete P , and
				//.....  reset set flag to show "some P is removed"
			list_vlt.deleteItem();		flag_polygon_modified = 1 ;

				//----- too few vertices of polygon after deletion?
			if( list_vlt.getNItem() < 3 ) { break; }

				//----- P at tail of list_vlt was deleted:
				//..... reset current node to new tail.
			if( list_vlt.isCurrentNodeNull() ) {
				list_vlt.toTail();
			}
		} else { 

				//----- uptdation
			list_vlt.upward();
		}

	}// while


		//----- reset data members
	if ( flag_polygon_modified ) {
		if( list_vlt.getNItem() >= 3 ) {
			this->set( list_vlt, color_backup , this->objectLabel , this->parent3D_p );
		} else {
			this->clear() ;
		}
	}
		
		//----- return number of vertices for the revised polygon
	return ( this->numVertex );

} // Polygon2D::removeZeroLengthEdge


//-------------------------------------------------//
//-----  member functions of class Polygon3D  -----//
//-------------------------------------------------//
	//----- Polygon3D::Polygon3D(), constructor
Polygon3D::Polygon3D( void ) : color(), subPolygonList()
{
		//----- set DATA
	label		= -1 ;
	objectLabel	= -1 ;
	facetLabel	= -1 ;
	neighboringFacet = NULL ;
	numNeighboringFacet = 0 ;
	setConvexity( 0 );           // not decided to be convex

	edgeType	= NULL ;	// linetype of edge
	numVertex	= 0 ;		//  number of vertices
	vertex		= NULL ;	// vertices in world coord (address)
	vertex3D	= NULL ;	// vertices in world coord
	normalVector(0.0, 0.0, 0.0 );
					// normal vector in world coordinate
	vertex2D	= NULL ;	// vertices in 2D screen coordinate
	xmin = xmax = 0.0 ;		// boxel
	ymin = ymax = 0.0 ;
	zmin = zmax = 0.0 ;
	umin = umax = 0.0 ;
	vmin = vmax = 0.0 ;

	vertex3D_org  = NULL ;
	edgeType_org  = NULL ;
	numVertex_org = 0    ;

} // Polygon3D::Polygon3D()


	//----- Polygon3D::set()
int
Polygon3D::set(		Vector3**		vertex_given    , 
			LineType**		edgeType_given  ,
			int			numVertex_given , 
			ColorType		color_given     , 
			Vector3*		normal_vector_p ,
			Perspective*		camera_p        ,
			double			epsilon         ,
			int			object_label    ,
			int			facet_label      ,
			int*			neighboring_facet_given ,
			int			num_neighboring_facet_given  , 
			Vector3*		vertex3D_given               )
{
		//----- local variables
	int		i ;	
	int		status                = 0 ; // initialization
	int		flag_image_calculated = 0 ; // initialization
	int*		neighboringFacet_old = NULL ; // old data
	LineType**	edgeType_old         = NULL ; // old data
	Vector3**	vertex_old           = NULL ; // old data
	Vector3*	vertex3D_old         = NULL ; // old data
	Vector2*	vertex2D_old         = NULL ; // old data

		//----- object label 
	this->objectLabel = object_label ;

		//----- facet label
	this->facetLabel = facet_label ;

		//----- set color
	color = color_given ;

		//----- set DATA: num of vertices and num of neighboring facet
	numVertex		= numVertex_given	;
	numNeighboringFacet	= num_neighboring_facet_given ;

		//----- error checking  1
	if( numVertex < 3 ) {
		cerr << "WARNING ( Polygon3D::set() ) 1\n";
		cerr << "numVertex = " << numVertex << endl;
		status = 0 ;
		return  status ;
	}

		//----- set DATA: neighboring facet labels
	if(	num_neighboring_facet_given == 0 || \
		neighboring_facet_given     == NULL   )
	{
		delete [] neighboringFacet ;	neighboringFacet = NULL ;
	} else {
			//----- backup old data
		neighboringFacet_old = neighboringFacet ; 

			//----- dynamical memory allocation
		neighboringFacet	= new int [ numNeighboringFacet ];
		if( neighboringFacet == NULL ) {
			cerr << "ERROR ( Polygon3D::set() ) 2\n";
			cerr << "  Cannot allocate memory  sizeof(LineType * ) x " << numVertex_given << endl;
			exit(1);
		}

			//----- set new data
		for( i = 0 ; i < numNeighboringFacet ; i++ ) {
			neighboringFacet[i] = neighboring_facet_given[i];
		}

			//----- delete old data
		delete [] neighboringFacet_old ; 
	}

		//----- set DATA: set types of edges
	edgeType_old	= edgeType ;
	edgeType	= new LineType_p [ numVertex ];
	if( edgeType == NULL ) {
		cerr << "ERROR ( Polygon3D::set() ) 3\n";
		cerr << "    Cannot allocate memory  sizeof(LineType * ) x " << numVertex_given << endl;
		exit(1);
	}
	for( i = 0 ; i < numVertex ; i++ ) {
		edgeType[i] = edgeType_given[i];
	}
	delete [] edgeType_old ; 

		//----- set DATA: vertex[]
	vertex_old = vertex ;
	vertex	= new Vector3_p [ numVertex ] ;	
	if( vertex == NULL ) {
		cerr << "ERROR ( Polygon3D::set() ) 4 : insufficient memory.\n";
		exit(1);
	}
	for( i = 0 ; i < numVertex ; i++ ) {
		vertex[i] = vertex_given[i] ;
	}
	delete [] vertex_old ;

		//----- set DATA: vertex3D[] (and reset vertex[])
	if( vertex3D_given == NULL ) {
		delete [] vertex3D ; vertex3D = NULL ;
	} else {
			//----- backup old data
		vertex3D_old = vertex3D ;

			//----- dynamical memory allocation
		vertex3D = new Vector3 [ numVertex ] ;	
		if( vertex3D == NULL ) {
			cerr << "ERROR ( Polygon3D::set() ) 5 : insufficient memory.\n";
			exit(1);
		}

			//----- set vertex3D[] and  reset vertex[]
		for( i = 0 ; i < numVertex ; i++ ) {
			vertex3D[i] = vertex3D_given[i] ;
			vertex  [i] = &(vertex3D[i]) ;
		}

			//----- delete old data
		delete [] vertex3D_old ;
	} 

		//----- set DATA: normalVector
	if( normal_vector_p != NULL ) {
		normalVector = *normal_vector_p;
	} else {
		normalVector = Tool3D::getNormalVector( vertex , numVertex );
	}

		//-----  set DATA: vertex2D and boxel
	if( camera_p != NULL ) {
			//----- backup old data
		vertex2D_old = vertex2D ;

			//----- memory allocation
		vertex2D		= new Vector2[ numVertex ] ;	
		if( vertex2D == NULL ) {
			cerr << "ERROR ( Polygon3D::set() ) 6 : insufficient memory.\n";
			exit(1);
		}
			//----- set 2d data
			//.....  (1) vertex2D
			//.....  (2) boxel: xmax, xmin, ..., vmax, vmin 
		flag_image_calculated = this->setImage( camera_p , epsilon );

			//----- delete old_data
		delete [] vertex2D_old ;

	} else {
		flag_image_calculated = 0 ;
		delete [] vertex2D ;  vertex2D = NULL ;
	}


		//----- set DATA: initialize boxel and set "status"
	if( !flag_image_calculated ) {
			//----- set status
		status = 0 ;

			//------ message
#if defined DEBUG_HID1
		cerr << "WARNING ( Polygon3D::set() ) 7 : failed to calc screen coordinate.\n";
#endif
			//----- initialize boxel
		xmin = xmax = 0.0 ;
		ymin = ymax = 0.0 ;
		zmin = zmax = 0.0 ;
		umin = umax = 0.0 ;
		vmin = vmax = 0.0 ;
	} else {
		status = 1 ;
	}

		//----- set DATA: clear sub_polygon_list
	subPolygonList.clear();

		//----- return status
	return status ;

} // Polygon3D::set()


	//----- Polygon3D::set()
int
Polygon3D::set(		List<Vertex2WithLineType>&	list_vlt       ,
			ColorType			color_given     , 
			Vector3*			normal_vector_p ,
			Perspective*			camera_p        ,
			double				epsilon         ,
			int				object_label    ,
			int				facet_label      ,
			int*				neighboring_facet ,
			int				num_neighboring_facet_given ,
			Vector3*			vertex3D_given               ) 
{
		//----- local
	int			status = 0 ;
	int			i;
	int			num_vertex_given ; 
	int			cur_backup = list_vlt.whereIsCurrentNode() ;
	Vector3**		vertex_given ;
	LineType**		edgetype_given ;
	Vertex2WithLineType	item_vlt ; 

		//----- set num_vertex_given
	num_vertex_given =  list_vlt.getNItem();

		//----- error checking
	if( num_vertex_given < 3 ) {
		cerr << "WARNING ( Polygon3D::set(list) ) 1\n";
		cerr << "numVertex = " << num_vertex_given << endl;
		status = 0 ;
		return  status ;
	}

		//----- dynamical memory
	vertex_given   = new Vector3_p  [ num_vertex_given ];
	edgetype_given = new LineType_p [ num_vertex_given ];

		//----- error checking
	if( NULL == vertex_given || NULL == edgetype_given ) { 
		cerr << "Polygon3D::set(list), 2\n";
		exit(1);
	} 
		
		//----- set arrays of vertex_given and lineType_given
	list_vlt.toTail(); i = 0 ;
	while( list_vlt.getItem( item_vlt ) ) {
		edgetype_given[i] = item_vlt.linetype ;
		vertex_given  [i] = item_vlt.position3D_p ;
		list_vlt.upward(); i++ ;
	}

		//----- call set( Vector3**, ...)
	status = \
	this->set (	vertex_given     , 
			edgetype_given   , 
			num_vertex_given ,
			color_given      , 
			normal_vector_p  ,
			camera_p         ,
			epsilon          ,
			object_label     , 
			facet_label      ,
			neighboring_facet , 
			num_neighboring_facet_given ,
			vertex3D_given              ) ;

		//----- delete dynamical memories
	delete [] vertex_given ; 
	delete [] edgetype_given ;

		//----- reset current node
	list_vlt.goTo( cur_backup ) ;

		//----- return status
	return status ;

} // Polygon3D::set( list )


//DDD

	//-----	Polygon3D::setVertex3D()
void	Polygon3D::setVertex3D()
{
		//----- local
	int		i;
	Vector3*	vertex3D_old    = vertex3D ; 
				// backup old data

		//----- error checking
	if( vertex == NULL || numVertex < 3 ) {
		cerr << "ERROR ( Polygon3D::setVertex3D() ) 1\n";
		exit(1);
	}

		//----- allocate dynamical memory 
	vertex3D = new Vector3 [ numVertex ] ;	
	if( vertex3D == NULL ) {
		cerr << "ERROR ( Polygon3D::setVertex3D() ) 2\n";
		exit(1);
	}

		//-----  set vertex3D[] and reset vertex[]
	for( i = 0 ; i < numVertex ; i++ ) {
		vertex3D[i] = *(vertex[i]) ;
		vertex  [i] = &(vertex3D[i]) ;
	}

		//----- delete old data if any
	delete [] vertex3D_old ; 

} // Polygon3D::setVertex3D()


	//----- Polygon3D::clear()
void
Polygon3D::clear()
{
	label		= -1 ;
	flag_convex     = 0 ;          // not decided to be convex
	objectLabel	= -1 ;
	facetLabel	= -1 ;
	delete [] neighboringFacet ;	neighboringFacet = NULL ;
	numNeighboringFacet = 0 ;
	color = ColorType();
//	color.ColorType();
//	color = THE_DEFAULT_COLORTYPE ;

	delete [] edgeType         ;	edgeType         = NULL ;
	numVertex           = 0 ;
	delete [] vertex           ;    vertex           = NULL ;
	delete [] vertex3D         ;    vertex3D         = NULL ;
	delete [] vertex2D         ;    vertex2D         = NULL ; 

	xmin =  xmax = 0.0 ;
	ymin =  ymax = 0.0 ;
	zmin =  zmax = 0.0 ;
	umin =  umax = 0.0 ;
	vmin =  vmax = 0.0 ;	

	subPolygonList.clear(); 


	delete [] vertex3D_org ;	vertex3D_org  = NULL ;
	delete [] edgeType_org ;	edgeType_org  = NULL ;
	numVertex_org = 0    ;

}  // Polygon3D::clear()


	//----- Polygon3D::operator = ()
void
Polygon3D::operator = ( const Polygon3D& rhs ) 
{
		//----- local variables
	int		i ;		// working variable
	int*		neighboringFacet_old = NULL ; // old data
	LineType**	edgeType_old         = NULL ; // old data
	Vector3**	vertex_old           = NULL ; // old data
	Vector2*	vertex2D_old         = NULL ; // old data
	Polygon2D*	polygon2d_p = NULL ;

		//----- backup old data
	neighboringFacet_old	= this->neighboringFacet ;
	edgeType_old		= this->edgeType ;
	vertex_old 		= this->vertex   ;
	vertex2D_old		= this->vertex2D ;

		//----- copy DATA: label
	label = rhs.getLabel();
	
		//----- copy DATA: flag_convex
	flag_convex = rhs.getConvexity();

		//----- copy DATA: objectLabel
	objectLabel = rhs.objectLabel ;

		//----- copy DATA: facetLabel
	facetLabel = rhs.facetLabel ;

		//----- copy DATA: numVertex and numNeighboringFacet
	numVertex		= rhs.numVertex ;
	numNeighboringFacet	= rhs.numNeighboringFacet ;

		//----- copy DATA: color
	color		= rhs.color ;

		//----- copy DATA: neighboringFacet
	if( numNeighboringFacet == 0 || rhs.neighboringFacet == NULL ) {
		neighboringFacet = NULL ;
	} else 	{ 
		neighboringFacet = new int [ numNeighboringFacet ];
		if( neighboringFacet == NULL ) {
			cerr << "ERROR ( Polygon3D:: operator=() ) 1a\n";
			exit(1);
		}
		for( i = 0 ; i < numNeighboringFacet ; i++ ) {
			neighboringFacet[i] = rhs.neighboringFacet[i];
		}
	} 

		//----- copy DATA: edgeType
	if( numVertex == 0 || rhs.edgeType == NULL ) {
		edgeType = NULL ;
 	} else {
		edgeType	= new LineType_p [ numVertex ];
		if( edgeType == NULL ) {
			cerr << "ERROR ( Polygon3D:: operator=() ) 2\n";
			exit(1);
		}
		for( i = 0 ; i < numVertex ; i++ ) {
			edgeType[i] = rhs.edgeType[i];
		}
	}

		//----- copy DATA: vertex
	if( numVertex == 0 ) {
		delete [] vertex ;	vertex = NULL ;
	} else {
		vertex	= new Vector3_p [ numVertex ] ;	
		if( vertex == NULL ){
			cerr << "ERROR ( Polygon3D:: operator=() ) 3\n";
			exit(1);
		}
		for( i = 0 ; i < numVertex ; i++ ) {
			vertex[i] = rhs.vertex[i] ;
		}
	}

		//----- copy DATA: vertex3D 
	if( numVertex == 0 || rhs.vertex3D == NULL ) {
		delete [] vertex3D ; 
		vertex3D = NULL ;
	} else {
		vertex3D = new Vector3 [ numVertex ] ;	
		if( vertex3D == NULL ){
			cerr << "ERROR ( Polygon3D:: operator=() ) 4\n";
			exit(1);
		}
		for( i = 0 ; i < numVertex ; i++ ) {
			vertex3D[i] = rhs.vertex3D[i] ;
			vertex  [i] = &(vertex3D[i]) ;
		}
	}

		//----- copy DATA: normalVector
	normalVector = rhs.normalVector ;

		//----- copy DATA: vertex2D
	if( numVertex == 0 || rhs.vertex2D == NULL ) {
		vertex2D	= NULL ;
	} else {
		vertex2D	= new Vector2  [ numVertex ] ;
		if( vertex2D == NULL ){
			cerr << "ERROR ( Polygon3D:: operator=() ) 5\n";
			exit(1);
		}
		for( i = 0 ; i < numVertex ; i++ ) {
			vertex2D[i] = rhs.vertex2D[i] ;
		}
	}

		//----- copy DATA: boxel
	xmin = rhs.xmin ;	xmax = rhs.xmax ;
	ymin = rhs.ymin ;	ymax = rhs.ymax ;
	zmin = rhs.zmin ;	zmax = rhs.zmax ;
	umin = rhs.umin ;	umax = rhs.umax ;
	vmin = rhs.vmin ;	vmax = rhs.vmax ;

		//----- copy DATA: subPolygonList
	subPolygonList = rhs.subPolygonList;

		//----- reset color pointer of polygon2d in list
		//..... to &(this->color)
	subPolygonList.toTail();
	while( (polygon2d_p = subPolygonList.getItemAddress() ) ) 
	{
		polygon2d_p->color = &(this->color) ;
		subPolygonList.upward();
	}


		//----- copy Data: vertex3D_org and numVertex_org
	delete [] vertex3D_org ;
	delete [] edgeType_org ;
	
	if( rhs.numVertex_org == 0 || rhs.vertex3D_org == NULL || rhs.edgeType_org == NULL ) {
		numVertex_org	= 0    ;
		vertex3D_org	= NULL ;
		edgeType_org	= NULL ;
	} else {
		numVertex_org	= rhs.numVertex_org ;
		vertex3D_org	= new Vector3    [ numVertex_org ] ;
		edgeType_org	= new LineType_p [ numVertex_org ] ;
		if( vertex3D_org == NULL || edgeType_org == NULL ){
			cerr << "ERROR ( Polygon3D:: operator=() ) 6\n";
			exit(1);
		}
		for( i = 0 ; i < numVertex_org ; i++ ) {
			vertex3D_org[i] = rhs.vertex3D_org[i] ;
			edgeType_org[i] = rhs.edgeType_org[i] ;
		}
	}



		//----- delete dynamical memories for old data
	delete [] neighboringFacet_old ; 
	delete [] edgeType_old ; 
	delete [] vertex_old   ; 
	delete [] vertex2D_old ; 

} // Polygon3D::operator = ()


	//----- Polygon3D::setImage()
int
Polygon3D::setImage( Perspective* camera_p, double epsilon )
{
		//----- local variables
	int		i ;
	int		status = 0 ;
	Vector3		screen_coord ;	// (x,y) of this 3D screen coordinate
					// makes 2D screen coordinate, i.e.
					// viewport coordinate
	double		screen_coord_u ;	// y + x
	double		screen_coord_v ;	// y - x

		//----- set DATA: xmin, xmax, ymin, ymax, zmin, zmax, umin, umax, vmin, vmax
		//.....  vertex2D[] also calculated.
	for( i = 0 ; i < numVertex ; i++ ) {

			//----- vertex2D[i]
		status = \
		camera_p->worldToScreen( *vertex[i], screen_coord, epsilon ) ;
						// get screen coord

		if( !status ) { break ; }

		vertex2D[i]( screen_coord.x, screen_coord.y ) ;	
						// get vertex2D[i]
		screen_coord_u = screen_coord.y + screen_coord.x ;
		screen_coord_v = screen_coord.y - screen_coord.x ;

			//-----  xmin, xmax, ymin, ymax, zmin, zmax
		if( i == 0 ) {	// initialize boxel
			xmin = xmax = screen_coord.x ;
			ymin = ymax = screen_coord.y ;
			zmin = zmax = screen_coord.z ;
			umin = umax = screen_coord_u ;
			vmin = vmax = screen_coord_v ;
		} else {	// update boxel
			if ( screen_coord.x  < xmin ) { xmin  = screen_coord.x ;}
			if ( screen_coord.x  > xmax ) { xmax  = screen_coord.x ;}

			if ( screen_coord.y  < ymin ) { ymin  = screen_coord.y ;}
			if ( screen_coord.y  > ymax ) { ymax  = screen_coord.y ;}

			if ( screen_coord.z  < zmin ) { zmin  = screen_coord.z ;}
			if ( screen_coord.z  > zmax ) { zmax  = screen_coord.z ;}

			if ( screen_coord_u  < umin ) { umin  = screen_coord_u ;}
			if ( screen_coord_u  > umax ) { umax  = screen_coord_u ;}

			if ( screen_coord_v  < vmin ) { vmin  = screen_coord_v ;}
			if ( screen_coord_v  > vmax ) { vmax  = screen_coord_v ;}
		}

	} // for( i )

		//----- return status 
	return status ;

} // Polygon3D::setImage()


	//----- set subPolygonList
void	Polygon3D::setSubPolygonList()
{
		//----- set DATA:subPolygonList
	subPolygonList.clear();
	subPolygonList.addItem( Polygon2D( vertex2D, this->edgeType, this->numVertex, &color , objectLabel , this ) )  ;

} // Polygon3D::setSubPolygonList()


	//----- int  Polygon3D::isNeighborOf()
int  
Polygon3D::isNeighborOf( const Polygon3D& polygon3d ) 
{
		//----- local
	int	status = 0 ; // initialize to "not neighbor"
	int	i ;


		//----- 
	if( this->objectLabel == polygon3d.objectLabel ) {
			//----- 
		for( i = 0 ; i < numNeighboringFacet ; i++ ) {

			if( neighboringFacet[i] == polygon3d.facetLabel ) {
				status = 1 ;	// neighbor (decided)
				break ;
			} 
		} // i
	} // if

		//----- return status 
	return status ;

} // Polygon3D::isNeighborOf()


	//----- Polygon3D::removeZeroLengthImageEdge()
int
Polygon3D::removeZeroLengthImageEdge( double epsilon2d , double epsilon3d , Perspective* camera_p )
{
		//----- local constants
	int				i ;
	int				num_vertex = this->numVertex ;
	int				flag_polygon_modified ;
	double				length_x, length_y ;
	List<Vertex2WithLineType>	list_vlt ;	
	Vertex2WithLineType		P_vlt, Q_vlt ;		
	Vector2				P, Q ;

		//----- too few vertices of polygon?
	if( num_vertex < 3 ) {
		cerr << "WARNING  (Polygon3D::removeZeroLengthImageEdge()) 1\n";
		return num_vertex ;
	}

		//----- make initial polygon list 
	for ( i = 0 ; i < num_vertex ; i++ ) {
		list_vlt.addItem( Vertex2WithLineType( vertex2D[i], edgeType[i], vertex[i] ) );
	}

		//----- delete edges with zero length
	list_vlt.toTail();	flag_polygon_modified = 0 ;

	while( !list_vlt.isCurrentNodeNull() ) {
			//----- starting point of edge PQ
		list_vlt.getItem( P_vlt );	
		P = P_vlt.position ;

			//----- ending point of edge PQ
		list_vlt.getUpwardItemCyclic( Q_vlt );
		Q = Q_vlt.position ;	

			//----- length of edge PQ
		length_x = fabs( Q.x - P.x );
		length_y = fabs( Q.y - P.y );

			//----- if edge PQ has zero length, delete P
		if( length_x < epsilon2d && length_y < epsilon2d ) {
				//----- delete P , and
				//.....  reset set flag to show "some P is removed"
			list_vlt.deleteItem();		flag_polygon_modified = 1 ;

				//----- too few vertices of polygon after deletion?
			if( list_vlt.getNItem() < 3 ) { break; }

				//----- P at tail of list_vlt was deleted:
				//..... reset current node to new tail.
			if( list_vlt.isCurrentNodeNull() ) {
				list_vlt.toTail();
			}
		} else { 
				//----- uptdation
			list_vlt.upward();
		}

	}// while

		//----- new number of vertices
	num_vertex = list_vlt.getNItem() ;

		//----- reset data members
	if( num_vertex < 3 ) {
		this->clear() ;
		num_vertex = 0 ;
	} else if ( flag_polygon_modified ) {

			//----- backup original polygon
		Polygon3D	poly3D_bak ;
		poly3D_bak = *this ;

			//----- warning
		if( camera_p == NULL ) {
				//-----	message
			cerr << "WARNING ( Polygon3D::removeZeroLengthImageEdge() ) 2\n";
			cerr << "         Polygon image is not calculated\n";
		}

			//----- set data members

			//---------- (1) label
		this->setLabel( poly3D_bak.label) ;

			//---------- (2) others except for subPolygonList
		this->set(	list_vlt                   , 
				poly3D_bak.color            , 
				&(poly3D_bak.normalVector)  ,
				camera_p                    ,
				epsilon3d                   , 
				poly3D_bak.objectLabel      ,
				poly3D_bak.facetLabel       ,
				poly3D_bak.neighboringFacet ,
				poly3D_bak.numNeighboringFacet ) ;

			//---------- (3) subPolygonList
		if( camera_p != NULL && poly3D_bak.subPolygonList.getNItem() != 0 ) {
			this->setSubPolygonList() ;
		} else {
			subPolygonList.clear() ;
		}

	} // if(num_vertex...)--else
		
		//----- return number of vertices for the revised polygon
	return num_vertex ;

} // Polygon3D::removeZeroLengthImageEdge()


	//----- Polygon3D::from2DTo3D()
int  
Polygon3D::from2DTo3D( const Polygon2D& polygon2d , Hid* hid_p ,  double epsilon3d )
{
		//----- local variables
	int		status = 0 ;
	int		i ;
	int		num_vertex = polygon2d.numVertex ;
	Polygon3D*	parent3D_p = polygon2d.parent3D_p ;	
	Perspective*	camera_p = hid_p->getCamera();
	ColorType*	color_p  = polygon2d.color  ;
	Vector3_p*	vertex_position_address_array ;
	Vector3*	position3D ;
	Polygon2D	poly2d_tmp ; poly2d_tmp = polygon2d ; 
					// used as tmporary arrays for vertex2D
					// and edgeType.

		//----- make empty arrays position3D (local)
	position3D = new Vector3[ num_vertex ] ;	
	if( position3D == NULL ) {
		ToolIO::errorMessage("Polygon3D::from2DTo3D()", "1" ,"insufficient memory" ) ; 
	} 

		//----- vertex_position_address_array (local)
	vertex_position_address_array = new Vector3_p[ num_vertex ] ;	
	if( vertex_position_address_array == NULL ) {
		ToolIO::errorMessage("Polygon3D::from2DTo3D()", "2" ,"insufficient memory" ) ; 
	} 
	
		//----- calc position3D 
	for( i = 0 ; i < num_vertex ; i++) {

			//----- local
		int status_tmp, j, j_plus ;

			//----- vertex: 2D-->3D
		status_tmp \
		= hid_p->point2DToPoint3D(	(poly2d_tmp.vertex2D)[i]  , 
						parent3D_p->normalVector ,
						*(parent3D_p->vertex[0])  ,
						&(position3D[i]) ,
						epsilon3d ) ;
		
			//----- err cheking
		if( !status_tmp ) {

				//----- skip this vertex				
			ToolIO::warningMessage("Polygon3D::from2DTo3D()", "3" ,"2D-->3D failed" ) ; 
			for( j = i ; j <= (num_vertex - 2) ; j++ ) {
				j_plus = j+1 ; if( j_plus == num_vertex ) { j_plus = 0 ; }
				poly2d_tmp.edgeType[j] = poly2d_tmp.edgeType[j_plus ] ;
				poly2d_tmp.vertex2D[j] = poly2d_tmp.vertex2D[j_plus ] ;
			}

			i--; num_vertex--;
		} 
	}// i 

		//----- calc vertex_position_address_array
	for( i = 0 ; i < num_vertex ; i++) {
		vertex_position_address_array[i] = &(position3D[i]) ;
	}// i 


		//----- set Polygon3D
	if( num_vertex >= 3 ) {

		status = \
		this->set(	vertex_position_address_array	, 
				poly2d_tmp.edgeType		,
				num_vertex			, 
				*color_p			,
				&(parent3D_p->normalVector)	,
				camera_p 		        ,
				epsilon3d        		, 
				parent3D_p->objectLabel		,
				parent3D_p->facetLabel		,
				parent3D_p->neighboringFacet	,
				parent3D_p->numNeighboringFacet ,
				position3D			) ;


	} else {
		status = 0 ;
		this->clear();	
	}

		//----- set data which were not set through set()
		//---------- (1) label
	this->setLabel( parent3D_p->getLabel() ) ;

		//---------- (2) subPolygonList
	this->subPolygonList.clear();

		//---------- (3) vertex3D_org , edgeType_org, and numVertex_org
	if( parent3D_p->isOriginalDataSet() ) {
			//----- parent itself is not the original polygon3D
		setOriginalData(	( parent3D_p->vertex3D_org  ) , 
					( parent3D_p->edgeType_org  ) ,
					( parent3D_p->numVertex_org ) );

	} else {
			//----- parent itself is the original polygon3D
		setOriginalData( 	( parent3D_p->vertex    ) , 
					( parent3D_p->edgeType  ) ,
					( parent3D_p->numVertex )  ) ; 
	}

		//----- delete dynamical memory
	delete [] vertex_position_address_array ;
	delete [] position3D ;


		//----- return status
	return status ;

} // Polygon3D::from2DTo3D()


	//-----	Polygon3D::setOriginalData( Vector3* )
void	Polygon3D::setOriginalData( Vector3*  vertex3D_given , LineType** edgeType_given, int num_vertex_given )
{
		//----- local
	int	i ;

		//----- set numVertex_org
	numVertex_org = num_vertex_given ;

		//----- set vertex3D_org and edgeType_org
	delete [] vertex3D_org ;
	delete [] edgeType_org ;

	if( numVertex_org <= 0 ) {
		numVertex_org	= 0    ;
		vertex3D_org	= NULL ;
		edgeType_org	= NULL ;
	} else {
		vertex3D_org	= new Vector3     [ numVertex_org ] ;
		edgeType_org	= new LineType_p  [ numVertex_org ] ;
		if( vertex3D_org == NULL || edgeType_org == NULL ){
			cerr << "ERROR ( Polygon3D:: setOriginalData(Vector3*) ) 1\n";
			exit(1);
		} 

		for( i =  0 ; i < numVertex_org ; i++ ) {
			vertex3D_org[i] = vertex3D_given[i] ;
			edgeType_org[i] = edgeType_given[i] ;
		}

	} // if--else

} // Polygon3D::setOriginalData( Vector3* )




	//-----	Polygon3D::setOriginalData( Vector3** )
void	Polygon3D::setOriginalData( Vector3**  vertex_given , LineType** edgeType_given, int num_vertex_given )
{
		//----- local
	int	i ;

		//----- set numVertex_org
	numVertex_org = num_vertex_given ;

		//----- set vertex3D_org and edgeType_org
	delete [] vertex3D_org ;
	delete [] edgeType_org ;

	if( numVertex_org <= 0 ) {
		numVertex_org	= 0    ;
		vertex3D_org	= NULL ;
		edgeType_org	= NULL ;
	} else {
		vertex3D_org	= new Vector3     [ numVertex_org ] ;
		edgeType_org	= new LineType_p  [ numVertex_org ] ;
		if( vertex3D_org == NULL || edgeType_org == NULL ){
			cerr << "ERROR ( Polygon3D:: setOriginalData(Vector3*) ) 1\n";
			exit(1);
		} 

		for( i =  0 ; i < numVertex_org ; i++ ) {
			vertex3D_org[i] = *(vertex_given[i]) ;
			edgeType_org[i] = edgeType_given[i] ;
		}

	} // if--else

} // Polygon3D::setOriginalData( Vector3** )


	//----- Polygon3D::isOriginalDataSet() 
int	Polygon3D::isOriginalDataSet() const
{
	int status ;

	if( vertex3D_org == NULL || numVertex_org == 0 ) { 
		status = 0 ; 
	} else {
		status = 1 ; 
	}

	return status ;

} // Polygon3D::isOriginalDataSet() 


	//----- Hid::setPolygon3dToArray()
int
Hid::setPolygon3DToArray( List<Polygon3D>& polygon3d_list_given )
{
		//----- local
	int			i ;
	Polygon3D_p* 		polygon3d_pp ;  
	Polygon3D* 		polygon3d_old ; 
	QuickSort<Polygon3D_p>	qsort_polygon3d_p ;

		//-----  delete old data if any
	this->clearPolygon3DArray();

		//----- number of items in list
		//...... set Hid::numPolygon3D
	numPolygon3D = polygon3d_list_given.getNItem() ;

		//----- list --> array
		//...... set Hid::polygon3d
	polygon3d = new Polygon3D [ numPolygon3D ];
	if( polygon3d == NULL ) { 
		cerr << "ERROR: (set::setPolygon3dToArray() ) 1 \n";
		exit(1);
	}
	polygon3d_list_given.toArray( polygon3d, numPolygon3D ) ;

		//----- make pointer array
	polygon3d_pp = new Polygon3D_p [ numPolygon3D ] ;
	if( polygon3d_pp == NULL ) { 
		cerr << "ERROR: (Hid::setPolygon3dToArray() ) 2 \n";
		exit(1);
	}
	for( i = 0 ; i < numPolygon3D ; i++ ) {
		polygon3d_pp[i] = &polygon3d[i] ;
	}		

		//----- do quick sort of the pointer array
	qsort_polygon3d_p.setMode ( 0 ); // use compare0()
	qsort_polygon3d_p.doSort ( polygon3d_pp, 0, (numPolygon3D - 1 ) );

		//----- set sorted  result to array
	polygon3d_old = polygon3d ;  // back up data before sort
	polygon3d = new Polygon3D [ numPolygon3D ];
	if( polygon3d == NULL ) { 
		cerr << "ERROR: (Hid::setPolygon3dToArray() ) 3 \n";
		exit(1);
	}
	for( i = 0 ; i < numPolygon3D ; i++ ) {
		polygon3d[i] = *polygon3d_pp[i];
	}

		//----- delete dynamical memories
	delete [] polygon3d_old ; // data before sort
	delete [] polygon3d_pp  ; // pointer array

		//----- return
	return  	numPolygon3D ;

} // Hid::setPolygon3dToArray()
