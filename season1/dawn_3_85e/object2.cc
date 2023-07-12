///////////////////////
///// object2.cc  /////
///////////////////////

#include "clip_tester.h" 
#include "object.h" 
#include "facet_list.h"


// #define DISPLAY_LOG


const LineType  LINE_TYPE_REAL            ( ColorType(0.0,0.0,0.0) );
const LineType  LINE_TYPE_OUTLINE         ( ColorType(0.0,0.0,0.0) , (2.0 * UNIT_LINE_WIDTH)  );
const LineType  LINE_TYPE_AUXILIARY       ( ColorType(0.0,0.0,0.0) , UNIT_LINE_WIDTH , FR_LINE_ON_OFF_DASH ) ;
const LineType  LINE_TYPE_AUXILIARY_H     ( ColorType(0.0,0.0,0.0) , UNIT_LINE_WIDTH , FR_LINE_ON_OFF_DASH ) ;
const LineType  LINE_TYPE_CRYSTAL         ( ColorType(0.0,0.0,0.0, 0.0) );


	//--------------------------// 
	//----- class Object  ------//
	//--------------------------// 

	//---------- constructor
Object::Object(	int label_, char* name_  ) :
facetList() , edgeList() , vertexList()
{
		//----- local variables

		//----- Label
	setLabel(label_ ) ;

		//----- Name
	setName( name_ );

		//----- flag_att_file_exist
	flag_att_file_exist = 0 ;

		//----- convexity
	setConvexity( 0 ); // not convex

		//----- set default reflection coefficients
		//.....  Constants in rhs's are defined in object.h
	Attribute	att_default ;
	this->setAttribute( att_default ) ;

		//----- initialize facet, edge, and vertex arrays
	facetArray	= NULL	;
	edgeArray	= NULL	;
	vertexArray	= NULL	;

		//----- initialize numbers of facet, edge, and vertex arrays
	numFacet	= 0  ;
	numEdge		= 0  ;
	numVertex	= 0  ;

		//----- origin and base vectors of body coordinates
	origin( 0.0 , 0.0 , 0.0 ) ;
	e1( 1.0 , 0.0 , 0.0 ) ;
	e2( 0.0 , 1.0 , 0.0 ) ;
	e3( 0.0 , 0.0 , 1.0 ) ;

		//----- initialize vertex positions in world coordinate
	vertexPosition = NULL ;

		//----- initialize bounding box
	boundingBox_min (0.0, 0.0, 0.0 ) ;
	boundingBox_max (0.0, 0.0, 0.0 ) ;

		//----- set default edge types
		//.....  Constants in rhs's are defined in object.h
		//.....	lineType[ REAL        ] = LINE_TYPE_REAL        ; 
		//.....	lineType[ OUTLINE     ] = LINE_TYPE_OUTLINE     ; 
		//.....	lineType[ AUXILIARY   ] = LINE_TYPE_AUXILIARY   ;
		//.....	lineType[ AUXILIARY_H ] = LINE_TYPE_AUXILIARY_H ;
		//.....	lineType[ CRYSTAL     ] = LINE_TYPE_CRYSTAL     ;
	this->setDefaultLineType ();

}   // Object::Object( char* )

	//----- Object::getLineType()
LineType 	Object::getLineType( int type ) const
{ 
	if( type >= 0 && type < NUM_LINETYPE ) { 
		return lineType[ type ] ; 
	} else {
		cerr << "ERROR (Object::getLineType() )\n";
		return lineType[ REAL ] ;
	}
} // Object::getLineType()


	//---------- clear list-list
void	Object::clear()
{
	this->clearList();	this->clearArray();
	if( vertexPosition != NULL ) { 
		delete [] vertexPosition ; 
		vertexPosition = NULL ; 
	}
} 

	//----- Object::clearList() 
void	Object::clearList() 
{
		//----- clear edge lists in all facets,
	facetList.toHead() ; // go to head of facet list
	while( !facetList.isCurrentNodeNull() ) {
		//----- clear edge list of this facet
		facetList.clearItem(); // defined is this file
		facetList.downward() ;		
	}

		//----- clear lists
	facetList.clear()	;
	edgeList.clear()	;
	vertexList.clear()	;

}// Object::clearList()


	//----- Object::clearArray_VertexEdgeFacet()
void	Object::clearArray_VertexEdgeFacet()
{
		//----- local
	int	i;

		//----- clear edge lists in all facets,
	if( facetArray != NULL ) { 
		for( i = 0 ; i < numFacet ; i++ ) {
			facetArray[i].clear();
		}
	}
		//----- clear arrays for facets, edges, and vertices
	delete [] vertexArray ; 	vertexArray = NULL ;
	delete [] edgeArray   ; 	edgeArray   = NULL ;  
	delete [] facetArray  ;  	facetArray  = NULL ;

		//----- clear vertex position in world coord 
//	delete [] vertexPosition ; 	vertexPosition = NULL ; 	

}// Object::clearArray_VertexEdgeFacet()


	//----- Object::clearArray() 
void	Object::clearArray() 
{
		//----- local
	int	i;

		//----- clear edge lists in all facets,
	if( facetArray != NULL ) { 
		for( i = 0 ; i < numFacet ; i++ ) {
			facetArray[i].clear();
		}
	}
		//----- clear arrays for facets, edges, and vertices
	delete [] facetArray  ;  	facetArray  = NULL ;
	delete [] edgeArray   ; 	edgeArray   = NULL  ;  
	delete [] vertexArray ; 	vertexArray = NULL ;

		//----- clear vertex position in world coord 
	delete [] vertexPosition ; 	vertexPosition = NULL ; 	

}// Object::clearArray()


	//----- getVertexItem ()
int	Object::getVertexItem ( int vertex_label, Vertex& vertex_item ) const 
{
	//----- error checking ( label out of range?)
	if( vertex_label < 0 || vertex_label >= numVertex ) 
	{
		return ERROR ;
	}

	//----- set return  value
	if( vertexArray != NULL ) {
		vertex_item = vertexArray[ vertex_label ];
	} else {
		cerr << "ERROR ( Object::getVertexItem()) : vertexArray = NULL\n";
		return ERROR;
	}

	//----- vertex item successfully obtained
	return NORMAL ;

} // Object::getVertexItem ()


	//----- getEdgeItem ()
int	Object::getEdgeItem ( int edge_label, Edge& edge_item ) const
{
	//----- error checking ( label out of range?)
	if( edge_label < 0 || edge_label >= numEdge )
	{
		return ERROR ;
	}

	//----- set return  value
	if( edgeArray != NULL ) {
		edge_item = edgeArray[ edge_label ];
	} else {
		cerr << "ERROR ( Object::getEdgeItem()) edgeArray = NULL\n";
		return ERROR;
	}

	//----- edge item successfully obtained
	return NORMAL ;

} // Object::getEdgeItem ()


	//----- getFacetItem ()
int	Object::getFacetItem ( int facet_label, Facet& facet_item ) const 
{
	//----- error checking ( label out of range?)
	if( facet_label < 0 || facet_label >= numFacet )
	{
		return ERROR ;
	}

	//----- set return  value
	if( facetArray != NULL ) {
		facet_item = facetArray[ facet_label ];
	} else {
		cerr << "ERROR (Object::getFacetItem()): facetArray = NULL\n";
		return ERROR;
	}

	//----- facet item successfully obtained
	return NORMAL ;

} // Object::getFacetItem ()



	//----- Object::passFacetToHid ()
void  Object::passFacetToHid (	Perspective&	camera           ,
				Shading&	shading          ,
				Hid*		hid_output_p     , 
				HidLine*	hidline_p        , 
				double		threshold_angle  , 
				double		epsilon3d        , 
				int		double_edge_draw_mode ,
				int		backface_culling_mode,
				int		shading_mode          ,
				int		clipping_mode )
{	

		//----- local
	int		i,j ;
	int		vertex_label, edge_label, other_edge_label ;
	int		right_facet_label ;
	int		nedge_of_facet, nedge, nfacet ;
	int		visibility , right_facet_visibility;
	Vector3**	point3d	= NULL ; 
	Vector3		normal_vector , one_vertex_position ;
	Vector3		right_facet_normal_vector ;
	Edge		edgeItem ;
	Facet*		facetItem_p ;
	Facet		rightFacetItem ;
	LineType**	linetype_array = NULL ;
	ColorType	color ;
	ColorType	line_color ;
	int*		neighboring_facet = NULL ;


		//----- error checking
	if( vertexPosition == NULL ) {
		cerr << "ERROR (Object::passFacetToHid() ) 1\n";
		exit(1);
	}

	if( isBoundingBoxClippedOut(		camera    , 
						epsilon3d ,
						clipping_mode  )  )
	{
		return ;
	} else {
	}

		//----- reset working edge flags to "not drawn" 
	nedge = this->getNumEdge();
	for( i = 0 ; i < nedge ; i++ ) {
		edgeArray[i].setFlag( NOT_DRAWN ) ;
	} // for(i)

		//----- set nfacet ( num of facet in this object)
	nfacet = this->getNumFacet() ; 

		//----- set working facet flags
	for( i = 0 ; i < nfacet ; i++ ) { // for(i) loop 1
			//----- get i-th facetItem of array
		facetItem_p = &(facetArray[ i ]) ;

			//----- get plane data of i-th facet
		normal_vector       = facetItem_p->getNormalVector() ;
		one_vertex_position = facetItem_p->getOneVertexPosition() ;

			//----- set flag to i-th facet to show its visibility
		if( camera.frontOrBack( normal_vector , one_vertex_position )  == 1 ) {
			facetItem_p->setFlag( VISIBLE );
		} else {	// 0 or -1
			facetItem_p->setFlag( INVISIBLE );			
		}

	} // for(i) loop 1


		//----- draw all facets of this object
	for( i = 0 ; i < nfacet ; i++ ) { // for(i) loop 2
			//----- local
		int	flag_color_set = 0 ;

			//----- get i-th facetItem of array
		facetItem_p = &(facetArray[ i ]) ;

			//----- color status
		flag_color_set = facetItem_p->isColorSet() ;

			//---- get normal vector of i-th facet
		normal_vector	= facetItem_p->getNormalVector() ;

			//---- get visibility of i-th facet
		visibility = facetItem_p->getFlag();

			//---- if i-th facet is invisible skip drawing it
		if( backface_culling_mode ) { 
			if( visibility == INVISIBLE && !facetItem_p->getAttribute().getTransparency() ) 
			{	
				continue ; 
					// to next i ( facet )
			} 
		}

			//----- set color of visible edges
		line_color = facetItem_p->getAttribute().getKd()  ;	
		line_color.whiteToBlack();

			//----- set nedge_of_facet of i-th facet
		nedge_of_facet = facetItem_p->getNumEdgeInList() ;

			//----- allocate dynamical mamories 
		point3d         = new Vector3_p  [ nedge_of_facet ] ;
		if( point3d    == NULL ) 	{ 
			cerr << "ERROR ( Object::passFacetToHid() ) 2\n"; 
			exit(1) ;
		}

		linetype_array = new LineType_p [ nedge_of_facet ] ;
		if( linetype_array == NULL ) 	{ 
			cerr << "ERROR ( Object::( Object::passFacetToHid() ) 3\n"; 
			exit(1) ;
		}

		neighboring_facet = new int [ nedge_of_facet ] ;
		if( neighboring_facet == NULL ) 	{ 
			cerr << "ERROR ( Object::( Object::passFacetToHid() ) 4\n"; 
			exit(1) ;
		}

			//----- get screen 2D coordinate of each j-th vertex of i-th facet
		for( j = 0 ; j < nedge_of_facet ; j++ ) {
				//----- get j-th edge of i-th facet
				//.....  ( Pointer facetItem_p was set to &(facetArray[ i ]) above. )
			edge_label	= facetItem_p->getEdgeLabel( j ) ; // TO BE IMPROVED for SPEED UP.
									   // IMPORTANT for LARGE nedge.
			getEdgeItem( edge_label , edgeItem ) ;

				//----- get label of j-th vertex of i-th facet
			vertex_label 	= edgeItem.getPrevVertex() ;

				//----- get world coordinate of the j-th vertex 
				//.....  of i-th facet
			point3d[j] =  &(vertexPosition[ vertex_label ]) ;

				//----- get other edge (in reverse direction)
				//..... and get right facet_label
				//.....   for the j-th edge of i-th facet
			other_edge_label  = edgeItem.getOtherEdge();
			right_facet_label = edgeItem.getRightFacet();


				//----- set neighboring facet label
			neighboring_facet [j] = right_facet_label ;

				//----- if other edge and right facet exist ....
			if( other_edge_label != NO_ITEM && right_facet_label != NO_ITEM ) {

				//----- 1. ALREADY DRAWN?
				//.....  If yes, set crystal line type
				if( !double_edge_draw_mode ) {
					Edge		otherEdgeItem ;
					getEdgeItem( other_edge_label , otherEdgeItem ) ;
					if( otherEdgeItem.getFlag() == DRAWN ) { // already drawn
						linetype_array[ j ] = &(lineType[ CRYSTAL ] ) ;
						edgeArray[ edge_label ].setFlag( DRAWN );
						continue ;
					} 
				}

				//----- 2.INVISIBLE LINE ( minus vertex label for edge )?
				if( 0 == edgeItem.getVisibility() ) { 
					linetype_array[ j ] = &lineType[ CRYSTAL ] ; 
					edgeArray[ edge_label ].setFlag( DRAWN );
					continue ;
				}

				//----- 3. OUTLINE ?
				getFacetItem( right_facet_label, rightFacetItem ) ;
				right_facet_visibility     = rightFacetItem.getFlag();
				if(\
				   (  (right_facet_visibility == INVISIBLE) && ( visibility == VISIBLE   )   )\
					|| \
				   (  (right_facet_visibility == VISIBLE  ) && ( visibility == INVISIBLE )   )\
				  ) 
				{
					linetype_array[ j ] = &lineType[ OUTLINE ] ; 
					edgeArray[ edge_label ].setFlag( DRAWN );
					continue ;	// to next j ( edge ) of i-th facet
				}

				//----- 4. AUXILIARY line or AUXILIARY_H  line?
				//---------- normal vector of right facet
				right_facet_normal_vector = rightFacetItem.getNormalVector() ;

				//---------- auxiliary line?
				double N_left_N_right_angle_deg \
				  = Tool3D::getAngleDeg( normal_vector, right_facet_normal_vector ); 

				if( N_left_N_right_angle_deg < threshold_angle  ) 
				{
					linetype_array[ j ] = &lineType[AUXILIARY] ; 

						//----- reset flag and go to next j ( edge )
						//.....  of i-th facet
					edgeArray[ edge_label ].setFlag( DRAWN );
					continue ;					
				}

			} // if( other_edge_label != NO_ITEM ... )


			//----- 5. ELSE: real or crystal line
			if( 0 == edgeItem.getVisibility() ) { 
				linetype_array[ j ] = &lineType[ CRYSTAL ] ; 
				edgeArray[ edge_label ].setFlag( DRAWN );
			} else {
				linetype_array[ j ] = &lineType[ REAL ] ; 
				edgeArray[ edge_label ].setFlag( DRAWN );
			}
		} // for(j)

				//----- local 
		Attribute	facet_attribute ; 
		facet_attribute = this->attribute ; 

			//----- make polygon3d and pass it to hid_output_p
		if( this->isAttFileExist() ) {

			shading.doShading(	shading_mode, normal_vector, &color, 
						getKd(), getKd() , getKs() ,
						camera, DEFAULT_PHONG_POWER ) ;

		} else  if( flag_color_set == Facet::HARD_COLOR ) {

			color = facetItem_p->getColor();
		} else  if( flag_color_set == Facet::SOFT_COLOR ) {

			facet_attribute = facetItem_p->getAttribute() ;

			shading.doShading(	shading_mode, normal_vector, &color, 
						facet_attribute.getKd(), facet_attribute.getKd(), facet_attribute.getKs() ,
						camera, facet_attribute.getPhongPower()  ) ;

		} else { // flag_color_set == Facet::OBJECT_COLOR 
			shading.doShading(	shading_mode, normal_vector, &color, 
						getKd(), getKd() , getKs() ,
						camera,  getPhongPower()  ) ;

		}



		if( !(facet_attribute.getTransparency()) )
		{
			Polygon3D* polygon3d_p = new Polygon3D[1];

			polygon3d_p->set(	point3d        , 
						linetype_array , 
						nedge_of_facet , 
						color          , 
						&normal_vector , 
						&camera        , 
						epsilon3d      ,
						this->label    , 
						i                 , 
						neighboring_facet ,
						nedge_of_facet     );
			polygon3d_p->setConvexity( (this->getConvexity()) );

			hid_output_p->addItem_Polygon3DList( *polygon3d_p , clipping_mode );

			delete [] polygon3d_p ;  polygon3d_p = NULL ;

		} else { // transpaent facet

				//------ pass edges to hidline_p
			for( j = 0 ; j < nedge_of_facet ; j++ ) {

					//----- local
				int		j_plus = j+1 ; 	if( j_plus == nedge_of_facet ) { j_plus = 0 ; } 
				LineSegment	lineseg_tmp ;
				LineType	linetype_tmp ;

				if( linetype_array[j]->getVisibility() ) 
				{
						//----- set position
					lineseg_tmp.setPosition( *(point3d[ j ]) ,*(point3d[ j_plus ]) );

						//----- set linetype
					linetype_tmp = *( linetype_array[j] );
					linetype_tmp.setColor( line_color );
					lineseg_tmp.setLineType( linetype_tmp );

						//----- calc 2D image
					lineseg_tmp.setImage( &camera, epsilon3d );

						//----- pass linesegment to hidline_p
					hidline_p->addItem_lineSegmentList( lineseg_tmp ) ;
				}
			} // for(j)

		} // if--else

			//----- delete dynamical memories
		delete [] point3d           ;  point3d           = NULL ;
		delete [] linetype_array    ;  linetype_array    = NULL ; 
		delete [] neighboring_facet ;  neighboring_facet = NULL ;

	} //for( i ) loop 2


}// Object::passFacetToHid()


	//----- setOtherEdgeAndRightFacet()
void Object::setOtherEdgeAndRightFacet()
{
		//----- local variables
	List<Edge>	otherEdgeList ;
	Edge		edgeItem ;
	Edge		otherEdgeItem ;
	int 		vertex ;
	int 		prevVertex ;

		//----- otherEdgeList
		//.....  Make copy of edgeList,
		//.....  in which we search for otherEdge's
	otherEdgeList = edgeList ;

	//----- edgeList loop
	edgeList.toHead();
	while( edgeList.getItem( edgeItem ) )
	{
			//----- vertex and prevVertex of current edgeItem
		vertex     = edgeItem.getVertex();
		prevVertex = edgeItem.getPrevVertex();

			//----- otherEdgeList loop
		otherEdgeList.toHead();
		while( otherEdgeList.getItem( otherEdgeItem ) )
		{
			if( vertex == ( otherEdgeItem.getPrevVertex() )
					&&
			    prevVertex == ( otherEdgeItem.getVertex() ) )
			 {
				//----- set other edge half to edgeItem
				edgeItem.setOtherEdge( otherEdgeItem.getLabel() );

				//----- set right facet to edgeItem
				edgeItem.setRightFacet( otherEdgeItem.getLeftFacet() );

				//----- set revised edgeItem to edgeList
				edgeList.overwriteItem( edgeItem );
				
				//----- Delete the node of otherEdgeItem
				//.....  from otherEdgeList.
				//.....  since this node is no longer
				//.....  necessary.
				//.....  Note that current node moves to
				//.....  downward by 1 step
				//.....  after the deletion.
				otherEdgeList.deleteItem() ;

				//----- to next edgeItem
				break ; // --> *** label 1

			} else {
				//----- to next otherEdgeItem
				otherEdgeList.downward();
			} // if -- else

		} // otherEdgeList_loop
		
//%%% label 1
			//----- next EdgeItem
		edgeList.downward();

	} // edgeList_loop

} // Object::setOtherEdgeAndRightFacet()


	//----- setFacetArray()
int  Object::setFacetArray()
{
		//----- local variables
	int 	facet ;		// facet label
	Facet	facetItem ;	// facet item
	int 	n = facetList.getNItem() ; // number of facet items

		//----- clear array if already exists
	if( facetArray != NULL ) { 
		delete [] facetArray ; 
		facetArray = NULL ; 
	}

		//----- no facets?
	if( n == 0 ) {
		delete [] facetArray ; 
		facetArray = NULL ;
		return NORMAL      ;
	}

		//----- allocate dynamical memories
	facetArray = new Facet[ n ] ;
	if( facetArray == NULL ) { 
		cerr << "ERROR ( Object::setFacetArray() ) \n";
		exit(1);
	}

		//----- substitute items to array
	facetList.toTail() ;
	while( facetList.getItem( facetItem ) ){
		facet = facetItem.getLabel();	
		facetArray[facet] = facetItem ;
		facetList.upward();
	}

		//----- set number of items in array
	numFacet = n ;

		//----- clear list
//	facetList.clear();


		//----- array successfully made
	return NORMAL ;

} // Object::setFacetArray()


	//----- setEdgeArray()
int  Object::setEdgeArray()
{
		//----- local variables
	int 	edge ;		// edge label
	Edge	edgeItem ;	// edge item
	int 	n = edgeList.getNItem() ; // number of edge items

		//----- clear array if already exists
	if( edgeArray != NULL ) { 
		delete [] edgeArray ; 
		edgeArray = NULL ; 
	}

		//----- no edges?
	if( n == 0 ) {
		delete [] edgeArray ; 
		edgeArray = NULL ;
		return NORMAL      ;
	}

		//----- allocate dynamical memories
	edgeArray = new Edge[ n ] ;
	if( edgeArray == NULL ) { 
		cerr << "ERROR ( Object::setEdgeArray() ) \n";
		exit(1);
	}

		//----- substitute items to array
	edgeList.toTail() ;
	while( edgeList.getItem( edgeItem ) ){
		edge = edgeItem.getLabel();	
		edgeArray[edge] = edgeItem ;
		edgeList.upward();
	}

		//----- set number of items in array
	numEdge = n ;

		//----- clear list
//	edgeList.clear();

	//----- array successfully made
	return NORMAL ;

} // Object::setEdgeArray()


	//----- setVertexArray()
int  Object::setVertexArray()
{
		//----- local variables
	int 	vertex ;	// vertex label
	Vertex	vertexItem ;	// vertex item
	int 	n = vertexList.getNItem() ; // number of vertex items

		//----- clear array if alreapdy exists
	if( vertexArray != NULL ) { 
		delete [] vertexArray ; 
		vertexArray = NULL ; 
	}

		//----- no vertices?
	if( n == 0 ) {
		delete [] vertexArray ; 
		vertexArray = NULL ;
		return NORMAL      ;
	}

		//----- allocate dynamical memories
	vertexArray = new Vertex[ n ] ;
	if( vertexArray == NULL ) { 
		cerr << "ERROR ( Object::setVertexArray() ) \n";
		exit(1);
	}

		//----- substitute items to array
	vertexList.toTail() ;
	while( vertexList.getItem( vertexItem ) ){
		vertex = vertexItem.getLabel();	
		vertexArray[vertex] = vertexItem ;
		vertexList.upward();
	}

		//----- set number of items in array
	numVertex = n ;

		//----- clear list
//	vertexList.clear();

	//----- array successfully made
	return NORMAL ;

} //  Object::setVertexArray()


	//----- setAllArray()
int  Object::setAllArray()
{
		//----- return value
	int 	status = 0 ;	

		//----- set vertex, edge, and facet arrays
	status |= this->setVertexArray();
	status |= this->setEdgeArray();
	status |= this->setFacetArray();

		//----- return status
	return status ;

} // Object::setAllArray()


	//----- Object::install()
int  Object::install( int mode_winged_edge )
{
		//----- return value
	int 	status = 0 ;	

		//----- install data to half-edge structure
	if( mode_winged_edge ) { 
#if defined DISPLAY_LOG
		cerr << " ****** Making winged-Edge structure ..." << endl;
#endif
		setOtherEdgeAndRightFacet() ; 
	}

#if defined DISPLAY_LOG
	cerr << " ****** Converting lists to arrays ..." << endl;
#endif
	status |= setAllArray() ;

#if defined DISPLAY_LOG
	cerr << " ****** Calculating world coordinate data ..." << endl;
#endif
	status |= setWorldCoordData();

		//----- clear list
#if defined DISPLAY_LOG
	cerr << " ****** Clearing lists ..." << endl;
#endif
	clearList();

		//----- return status
	return status ;

} // Object::install()


	//----- setWorldCoordData()
int 	Object::setWorldCoordData( void )
{
		//----- local variables
	int		i ;
	int		num_vertex = this->getNumVertex() ;
	int		num_facet  = this->getNumFacet () ;

		//----- clear array of vertex positions if it alreapdy exists
	delete [] vertexPosition ; vertexPosition = NULL ; 

		//----- allocate dynamical memories for vertex positions
	vertexPosition = new Vector3[ num_vertex ];
	if( vertexPosition == NULL ) { 
		cerr << "ERROR ( Object::setWorldCoordData() ) \n";
		return ERROR ;
	}

		//------ set vertex positions in world coordinate
	for( i = 0 ; i < num_vertex ; i++ ) {
		this->bodyToWorld(	vertexArray[i].getPosition() , 
					vertexPosition[i] ) ;
	}

		//----- calc normal vector and first vertex position
		//.....  of each facet belonging to this object
	for( i = 0 ; i < num_facet; i++ ) {
		facetArray[i].setNormalVector( this );
	}


		//----- set bounding box
	this->setBoundingBox();

		//----- return
	return	NORMAL ;	

} //Object::setWorldCoordData()


	//----- setName()
void	Object::setName( const char* name_ )
{
	if( strlen( name_ ) <= NAME_LENGTH - 1 ) {
		strcpy( name, name_ ) ;
	} else {	// truncation
		strncpy( name, name_, NAME_LENGTH -1 ) ;
		name[ NAME_LENGTH - 1 ] = '\0' ;
	}

} // Object::setName()

	//----- setBaseVector()
void Object::setBaseVector(Vector3 e1_ , Vector3 e2_ )
{
	e1 = e1_     ;  e1.selfNormalize();
	e2 = e2_     ;  e2.selfNormalize();
	e3 = e1 ^ e2 ;  e3.selfNormalize();

} // Object::setBaseVectors()


	//----- Object::setLineType ( int type , LineType linetype )	
void	Object::setLineType ( int type , LineType& linetype )	
{ 
	if( type >= 0 && type < NUM_LINETYPE ) { lineType[ type ] = linetype; }
}


	//----- Object::setDefaultLineType ()
void	Object::setDefaultLineType ()
{ 
	lineType[ REAL        ] = LINE_TYPE_REAL        ; 
	lineType[ OUTLINE     ] = LINE_TYPE_OUTLINE     ; 
	lineType[ AUXILIARY   ] = LINE_TYPE_AUXILIARY   ;
	lineType[ AUXILIARY_H ] = LINE_TYPE_AUXILIARY_H ;
	lineType[ CRYSTAL     ] = LINE_TYPE_CRYSTAL     ;
}


	//---------- operator = ()
int	Object::operator = ( const Object& rhs )
{
		//----- local
	int	i, num_vertex  ;

		//----- Label and Name
	setLabel( rhs.getLabel() ) ;
	setName( rhs.getName() );

		//----- flags
	flag_att_file_exist = rhs.isAttFileExist();
	flag_convex = rhs.getConvexity();

		//----- Attributes
	this->setAttribute( rhs.getAttribute() );

	        //----- Lists of facets, edges, and vertices composing object
	this->facetList   = rhs.facetList  ;
	this->edgeList    = rhs.edgeList   ;
	this->vertexList  = rhs.vertexList ;

		//----- number of items in Arrays
	numFacet	= rhs.getNumFacet  ()  ;
	numEdge		= rhs.getNumEdge   ()  ;
	numVertex	= rhs.getNumVertex ()  ;

		//----- Arrays
	delete [] facetArray ;	delete [] edgeArray ;	delete [] vertexArray ;
	facetArray  = NULL   ;	edgeArray   = NULL  ;	vertexArray = NULL ;

	facetArray	= new Facet  [ numFacet  ];
	edgeArray	= new Edge   [ numEdge   ];
	vertexArray	= new Vertex [ numVertex ];
	if( !facetArray || !edgeArray || !vertexArray ) {
		cerr << "ERROR ( Object::operator = () ) 1 \n";
		exit(1);
	}
	for( i = 0 ; i < numFacet; i++ ) { 
		 rhs.getFacetItem( i, facetArray[i] );
	}
	for( i = 0 ; i < numEdge; i++ ) { 
		 rhs.getEdgeItem( i, edgeArray[i] );
	}
	for( i = 0 ; i < numVertex; i++ ) { 
		 rhs.getVertexItem( i, vertexArray[i] );
	}


		//----- body coordinate ( difined with world coordinate)
	origin		= rhs.getOrigin() ;
	e1		= rhs.getE1() ;		// base vector for x-direction
	e2		= rhs.getE2() ;		// base vector for y-direction
	e3		= rhs.getE3() ;		// base vector for z-direction


		//----- vertex position in world coordinate
	delete [] vertexPosition ;   vertexPosition = NULL ; 
	num_vertex = rhs.getNumVertex();
	vertexPosition = new Vector3 [ num_vertex ];
	if( vertexPosition == NULL ) { 
		cerr << "ERROR ( Object::operator = () ) 2 \n";
		exit(1);
	}
	for( i = 0 ; i < num_vertex ; i++ ) {
		vertexPosition[i] = rhs.vertexPosition[i] ;
	}


		//----- bounding box
	this->boundingBox_min = rhs.getBoundingBoxMin();
	this->boundingBox_max = rhs.getBoundingBoxMax();


		//----- type of edge
	for( i = 0 ; i < NUM_LINETYPE ; i++ ) {
		lineType[i]  = rhs.getLineType( i ) ;		
	}

		//----- return
	return NORMAL ;

} // Object::operator =


	//----- bodyToWorld()
void  Object::bodyToWorld( const Vector3 body_coord, Vector3& world_coord )
{
	//----- local variables
	double x, y, z ;

	//----- components of body coordinate
	x = body_coord.x ;	
	y = body_coord.y ;
	z = body_coord.z ;

	//----- world coordinate (output)
	world_coord = origin + (x * e1) + (y * e2) + ( z * e3 ) ;

} // Object::bodyToWorld( const Vector3& body )


	//-----	worldToBody()
void  Object::worldToBody ( const Vector3 world_coord,	Vector3& body_coord )
{
	//----- local variables
	Vector3  world = world_coord ;

	//----- shift origin of given world coordinate
	world -= origin ;

	//----- object coordinate ( output )
	body_coord.x = world * e1 ;
	body_coord.y = world * e2 ;
	body_coord.z = world * e3 ;

} // Object::worldToBody()



	//----- Object::setBoundingBox()
void 
Object::setBoundingBox()
{
	if( vertexPosition == NULL ) { 
		cerr << "ERROR ( Object::setBoundingBox() ) \n";
	} else {

			//----- set boundingBox_min and boundingBox_max
		Tool3D::getBoundingBox(	vertexPosition,  
					numVertex , 
					&(this->boundingBox_min )    ,
					&(this->boundingBox_max )    );
	}

} // Object::setBoundingBox()



	//----- Object::getBoundingBoxMinAddress()
const Vector3* 
Object::getBoundingBoxMinAddress() const
{
	if( vertexPosition == NULL ) { 
		cerr << "ERROR ( Object::getBoundingBoxMinAddress() ) \n";
		return NULL ;
	} else {

		return  &(this->boundingBox_min) ;
	}

} // Object::getBoundingBoxMinAdddress()


	//----- Object::getBoundingBoxMaxAddress()
const Vector3* 
Object::getBoundingBoxMaxAddress() const 
{
	if( vertexPosition == NULL ) { 
		cerr << "ERROR ( Object::getBoundingBoxMaxAddress() ) \n";
		return NULL ;
	} else {

		return  &(this->boundingBox_max) ;
	}

} // Object::getBoundingBoxMaxAddress()




	//----- Object::getNumEdgeOfFacet() 
int
Object::getNumEdgeOfFacet( int index ) const 
{
		//----- local 
	int	num_edge = 0 ;
	Facet*	facetItem_p ;

		//------ error ?
	if( index < 0 || index >= numFacet || facetArray == NULL ) 
	{ 
		cerr << "ERROR (Object::getNumEdgeOfFacet()\n";
		cerr << "      index = " << index << endl;
		exit(1);
	} else {
		facetItem_p = &(facetArray[ index ]) ;
		num_edge = facetItem_p->getNumEdgeInList() ;
	} 

		//----- return number of edge 
	return num_edge ;

} // Object::getNumEdgeOfFacet()




	//----- Object::getFacet()
int
Object::getFacet(	int		index, 
			Vector3_p*	vertex_position_p , 
			Vector3*	normal_vector_p    ) const
{
		//----- local
	int	j ;
	int	vertex_label ;
	int	edge_label ;
	int	nedge_of_facet ;
	Edge*	edgeItem_p ;
	Facet*	facetItem_p ;

		//------ error ?
	if( index < 0 || index >= numFacet ) { 
		return  ERROR ;
	}

		//----- get addres of the index-th facetItem in array
	if( facetArray != NULL ) {
		facetItem_p = &(facetArray[ index ]) ;
	} else {
		return  ERROR ;
	}

		//----- set nedge_of_facet of i-th facet
	nedge_of_facet = facetItem_p->getNumEdgeInList() ;

		//----- get positions of j-th vertex of index-th facet
	for( j = 0 ; j < nedge_of_facet ; j++ ) {

			//----- get j-th edge of i-th facet
			//.....  ( Pointer facetItem_p was set to &(facetArray[ i ]) above. )
		edge_label	= facetItem_p->getEdgeLabel( j ) ; // TO BE IMPROVED for SPEED UP.
								   // IMPORTANT for LARGE nedge.
		if( edgeArray != NULL ) {
			edgeItem_p = &(edgeArray[ edge_label ]) ;
		} else {
			return  ERROR ;
		}

			//----- get label of j-th vertex of index-th facet
		vertex_label 	= edgeItem_p->getPrevVertex() ;

			//----- get world-coordinate postion of the j-th vertex 
			//.....  of index-th facet
		vertex_position_p[j] =  &(vertexPosition[ vertex_label ]) ;

	} // for(j)

		//----- get normal vector
	*normal_vector_p = facetItem_p->getNormalVector() ;

		//----- normal return 
	return  NORMAL ;

} // Object::getFacet()





	//----- Object::passEdgeToHidLine
void  Object::passEdgeToHidLine (	Perspective&	camera                ,
					HidLine*	hidline_p             ,
					double		threshold_angle       ,
					double		epsilon3d	      ,
					int		double_edge_draw_mode ,
					int		clipping_mode         ,
					int		back_edge_dashed        )
{	
		//----- local
	int		i,j ;
	int		vertex_label, edge_label, other_edge_label ;
	int		right_facet_label = -1 ;
	int		nedge_of_facet, nedge, nfacet ;
	int		visibility , right_facet_visibility;

	Vector3		normal_vector , one_vertex_position ;
	Vector3		right_facet_normal_vector ;
	Edge		edgeItem ;
	Facet*		facetItem_p    = NULL;
	Facet		rightFacetItem ;

	ColorType	line_color ;
	Vector3**	point3d	          = NULL ; 
	LineType**	linetype_array    = NULL ;
	int*		is_frontface_edge = NULL ;

	int flag_backedge_dashed = back_edge_dashed ;

		//----- error checking
	if( vertexPosition == NULL ) {
		cerr << "ERROR (Object::passFacetToHid() ) 1\n";
		exit(1);
	}


	if( isBoundingBoxClippedOut(		camera    , 
						epsilon3d ,
						clipping_mode  )  )
	{
		return ;
	} else {

	}

		//----- reset working edge flags to "not drawn" 
	nedge = this->getNumEdge();
	for( i = 0 ; i < nedge ; i++ ) {
		edgeArray[i].setFlag( NOT_DRAWN ) ;
	} // for(i)

		//----- set nfacet ( num of facet in this object)
	nfacet = this->getNumFacet() ; 

		//----- set working facet flags
	for( i = 0 ; i < nfacet ; i++ ) { // for(i) loop 1
			//----- get i-th facetItem of array
		facetItem_p = &(facetArray[ i ]) ;

			//----- get plane data of i-th facet
		normal_vector       = facetItem_p->getNormalVector() ;
		one_vertex_position = facetItem_p->getOneVertexPosition() ;

			//----- set flag to i-th facet to show its visibility
		if( camera.frontOrBack( normal_vector , one_vertex_position )  == 1 ) {
			facetItem_p->setFlag( VISIBLE );
		} else {	// 0 or -1
			facetItem_p->setFlag( INVISIBLE );			
		}

	} // for(i) loop 1


		//----- draw all facets of this object
	for( i = 0 ; i < nfacet ; i++ ) { // for(i) loop 2
			//----- local

			//----- get i-th facetItem of array
		facetItem_p = &(facetArray[ i ]) ;

			//---- get normal vector of i-th facet
		normal_vector	= facetItem_p->getNormalVector() ;

			//---- get visibility of i-th facet
		visibility = facetItem_p->getFlag();


			//----- set nedge_of_facet of i-th facet
		nedge_of_facet = facetItem_p->getNumEdgeInList() ;

			//----- set color of visible edges
		line_color = facetItem_p->getAttribute().getKd()  ;	
		line_color.whiteToBlack();


			//----- allocate dynamical mamories 
		point3d           = new Vector3_p  [ nedge_of_facet ] ;
		linetype_array    = new LineType_p [ nedge_of_facet ] ;
		is_frontface_edge = new int        [ nedge_of_facet ] ;


			//----- get screen 2D coordinate of each j-th vertex of i-th facet
		for( j = 0 ; j < nedge_of_facet ; j++ ) {
				//----- get j-th edge of i-th facet
				//.....  ( Pointer facetItem_p was set to &(facetArray[ i ]) above. )
			edge_label	= facetItem_p->getEdgeLabel( j ) ; // TO BE IMPROVED for SPEED UP.
									   // IMPORTANT for LARGE nedge.
			getEdgeItem( edge_label , edgeItem ) ;

				//----- get label of j-th vertex of i-th facet
			vertex_label 	= edgeItem.getPrevVertex() ;

				//----- get world coordinate of the j-th vertex 
				//.....  of i-th facet
			point3d[j] =  &(vertexPosition[ vertex_label ]) ;

				//----- get other edge (in reverse direction)
				//..... and get right facet_label
				//.....   for the j-th edge of i-th facet
			other_edge_label  = edgeItem.getOtherEdge();
			right_facet_label = edgeItem.getRightFacet();

				//----- current edge belongs to frontface, bacface, or outline
			getFacetItem( right_facet_label, rightFacetItem ) ;
			right_facet_visibility     = rightFacetItem.getFlag();
			if        ( right_facet_visibility == VISIBLE   && visibility == VISIBLE   ) 
			{
				is_frontface_edge [j] =  1 ;   // edge of frontface	
			} else if ( right_facet_visibility == INVISIBLE && visibility == INVISIBLE   ) 
			{
				is_frontface_edge [j] = -1 ;  // edge of backface
			} else {
				is_frontface_edge [j] =  0  ; // outline
			}

				//----- if other edge and right facet exist ....
			if( other_edge_label != NO_ITEM && right_facet_label != NO_ITEM ) {

				//----- 1. ALREADY DRAWN?
				//.....  If yes, set crystal line type
				if( !double_edge_draw_mode ) {
					Edge		otherEdgeItem ;
					getEdgeItem( other_edge_label , otherEdgeItem ) ;
					if( otherEdgeItem.getFlag() == DRAWN ) { // already drawn
						linetype_array[ j ] = &(lineType[ CRYSTAL ] ) ;
						edgeArray[ edge_label ].setFlag( DRAWN );
						continue ;
					} 
				}

				//----- 2.INVISIBLE LINE ( minus vertex label for edge )?
				if( 0 == edgeItem.getVisibility() ) { 
					linetype_array[ j ] = &lineType[ CRYSTAL ] ; 
					edgeArray[ edge_label ].setFlag( DRAWN );
					continue ;
				}

				//----- 3. OUTLINE ?
				if ( is_frontface_edge [j] ==  0  )
				{
					linetype_array[ j ] = &lineType[ OUTLINE ] ; 
					edgeArray[ edge_label ].setFlag( DRAWN );
					continue ;	// to next j ( edge ) of i-th facet
				}

				//----- 4. AUXILIARY line or AUXILIARY_H  line?
				//---------- normal vector of right facet
				right_facet_normal_vector = rightFacetItem.getNormalVector() ;

				//---------- auxiliary line?
				double N_left_N_right_angle_deg \
				   = Tool3D::getAngleDeg( normal_vector, right_facet_normal_vector ); 

				if( N_left_N_right_angle_deg < threshold_angle  ) 
				{
					linetype_array[ j ] = &lineType[AUXILIARY] ; 

						//----- reset flag and go to next j ( edge )
						//.....  of i-th facet
					edgeArray[ edge_label ].setFlag( DRAWN );
					continue ;					
				}

			} // if( other_edge_label != NO_ITEM ... )


			//----- 5. ELSE: real line
			if( 0 == edgeItem.getVisibility() ) { 
				linetype_array[ j ] = &lineType[ CRYSTAL ] ; 
				edgeArray[ edge_label ].setFlag( DRAWN );
			} else {
				linetype_array[ j ] = &lineType[ REAL ] ; 
				edgeArray[ edge_label ].setFlag( DRAWN );
			}
		} // for(j) ... part 1


			//------ pass edges to hidline_p
		for( j = 0 ; j < nedge_of_facet ; j++ ) {

				//----- local
			int		j_plus = j+1 ; 	if( j_plus == nedge_of_facet ) { j_plus = 0 ; } 
			LineSegment	lineseg_tmp ;
			LineType	linetype_tmp ;


			if( linetype_array[j]->getVisibility() ) 
			{
					//----- set position
				lineseg_tmp.setPosition( *(point3d[ j ]) ,*(point3d[ j_plus ]) );

					//----- set linetype
				linetype_tmp = *( linetype_array[j] );
				linetype_tmp.setColor( line_color );
/////////////////////////////////////////////////////////////////////////////////////////////////

				if(	flag_backedge_dashed && is_frontface_edge[j] == -1 ) 
				{
					linetype_tmp.setStyle( FR_LINE_ON_OFF_DASH );
				}

/////////////////////////////////////////////////////////////////////////////////////////////////
				lineseg_tmp.setLineType( linetype_tmp );

					//----- calc 2D image
				lineseg_tmp.setImage( &camera, epsilon3d );

					//----- pass linesegment to hidline_p
				hidline_p->addItem_lineSegmentList( lineseg_tmp ) ;
			}
		} // for(j)... part 2

			//----- delete dynamical memories
		delete [] point3d            ; point3d           = NULL   ; 
		delete [] linetype_array     ; linetype_array    = NULL   ; 
		delete [] is_frontface_edge  ; is_frontface_edge = NULL ; 

	} //for( i ) loop 2


}// Object::passEdgeToHidLine()



	//----- Object::isBoundingBoxClippedOut()
int	Object::isBoundingBoxClippedOut(	Perspective&	camera,
						double		epsilon3d,        
						int		clipping_mode ) 
{
		//-----  local
	int		status      ;
	ClipTester	clip_tester ;


		//----- clipped out or not
	status = \
	clip_tester.isBox3DClippedOut(	Box3D (boundingBox_min, boundingBox_max ) ,  
					camera    , 
					epsilon3d ,
					clipping_mode                               ) ;

		//----- return status
		//.....  1: clipped out 
		//.....  0: not clipped out 
	return status  ;

} // Object::isBoundingBoxClippedOut()
