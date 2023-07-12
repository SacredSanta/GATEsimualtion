///////////////////////////////
///// object3.cc          /////
/////   drawing fundtions /////
///////////////////////////////


#include "object.h" 
#include "clip_tester.h" 


	//---------- draw edge
int	Edge::draw_PS (	Object&		objItem  , 
			Perspective&	camera   ,
			PostScript&	ps       ,
	                const LineType& linetype , 
			double		epsilon3d )
{
		//----- local variables
	int		status1 = 1, status2 = 1 ;
	Vector2		vertexPoint_screen2d ;
	Vector2		prevVertexPoint_screen2d ;
	Vector3*	vertexPoint_p ;
	Vector3*	prevVertexPoint_p;
	LineType	linetype_tmp ; linetype_tmp = linetype ;
	int		left_facet ;
	ColorType	color ;	

		//----- width of edge ( = real edge width )
	linetype_tmp.setWidth( objItem.lineType[ REAL ].getWidth() ) ;

		//----- set color of this edge to the color of facet
	left_facet = this->getLeftFacet();
	color = objItem.getFacetItem(left_facet).getAttribute().getKd()  ;	
	color.whiteToBlack();
	linetype_tmp.setColor( 	color ) ;	

		//----- get vertex positions in world coordinate
	prevVertexPoint_p	= &(objItem.vertexPosition[ this->prevVertex ]) ;
	vertexPoint_p		= &(objItem.vertexPosition[ this->vertex ]    ) ;

		//----- 2D screen coordinate
	status1 = camera.worldToScreen( *prevVertexPoint_p , prevVertexPoint_screen2d , epsilon3d ) ; 
	status2 = camera.worldToScreen( *vertexPoint_p     , vertexPoint_screen2d     , epsilon3d ) ;    

		//----- draw linesegment
	if( status1 && status2 && (this->getVisibility() != 0) ) 
	{
		ps.drawLineSegment_vp(	prevVertexPoint_screen2d ,
					vertexPoint_screen2d     , 
					&linetype_tmp              ) ;
	}

		//----- successfully drawn
	return NORMAL;

} // Edge::draw_PS ()




	//----- Object::drawWireFrame_PS ()
void  Object::drawWireFrame_PS (	Perspective&	camera ,
					PostScript&	ps     ,
				        const LineType&	linetype ,
					double		threshold_angle ,
					double		epsilon3d ,
					int		clipping_mode  ) 
{	


		//----- error checking
	if( vertexPosition == NULL ) { 
	  cerr << "***** ERROR: vertexPosition = NULL" << endl;
	  return ;
	}

	if( isBoundingBoxClippedOut(		camera    , 
						epsilon3d ,
						clipping_mode  )  )
	{
		return ;
	} else {

	}

		//----- local
	int		i ;
	int             nedge  = this->getNumEdge() ;
	int             nfacet = this->getNumFacet() ; 
	Vector3         N_tmp ;  // normal vector of temp facet 
	Vector3         P0_tmp ; // a vertex position on temp facet 
	Vector3         N_left ; // left-facet  normal vector
	Vector3         N_right; // right-facet normal vector
//	double		cos_threshold_angle  = cos( threshold_angle *180.0 / 3.141592 ) ;
	LineType        linetype_given = linetype    ;
	LineType*       linetype_p = &linetype_given ; // dummy
	
		//----- set working facet flags
	for( i = 0 ; i < nfacet ; i++ ) {
			//----- get i-th facetItem of array
		Facet* facetItem_p = &(facetArray[ i ]) ;

			//----- get plane data of i-th facet
		N_tmp  = facetItem_p->getNormalVector() ;
		P0_tmp = facetItem_p->getOneVertexPosition() ;

			//----- set flag to i-th facet to show its visibility
		if( camera.frontOrBack( N_tmp , P0_tmp )  == 1 ) {
			facetItem_p->setFlag( VISIBLE );
		} else {	// 0 or -1
			facetItem_p->setFlag( INVISIBLE );			
		}

	} // for(i) loop 1

		//----- draw all edges of this object
	for( i = 0 ; i < nedge ; i++ ) {

		//----- facet labels
	  int left_facet_label  = (edgeArray[ i ]).getLeftFacet();
	  int right_facet_label = (edgeArray[ i ]).getRightFacet();

		//----- facet visibilities
	  int left_facet_visibility  \
	    = (facetArray[ left_facet_label  ]).getFlag();
	  int right_facet_visibility  \
	    = (facetArray[ right_facet_label  ]).getFlag();


	  if( right_facet_label == NO_ITEM ) { 
		  linetype_p = &(lineType[REAL]) ; 
	  } else {
	
			//----- normal vectors and their product
		  N_left = (facetArray[ left_facet_label  ]).getNormalVector() ;
		  N_right= (facetArray[ right_facet_label ]).getNormalVector() ;
		  double N_left_N_right_angle_deg \
		         = Tool3D::getAngleDeg( N_left, N_right ); 


			//----- determine linetype of i-th edge
		  // outline?
		  if((  (right_facet_visibility == INVISIBLE) && ( left_facet_visibility == VISIBLE   )   )\
				|| \
		     (  (right_facet_visibility == VISIBLE  ) && ( left_facet_visibility == INVISIBLE )   ) )
		    {
		      linetype_p = &lineType[ OUTLINE ] ; 
	            } 
		  // aux line?
		  else if( N_left_N_right_angle_deg < threshold_angle  ) { 
		    linetype_p = &(lineType[AUXILIARY]) ; 
		  } else {

		    linetype_p = &(lineType[REAL]) ; 
		  }

	  } // if (right_facet_label...) -- else

		//----- draw the i-th edge
	  if( linetype_p->getVisibility() ) {
	    (edgeArray[ i ]).draw_PS( *this, camera, ps, *linetype_p , epsilon3d );
	  }
	} // for
}// Object::drawWireFrame_PS()


/////////////////////////////////////////////////////////////////////////

	//---------- draw edge
int	Edge::draw_landscape_PS (	Object&		objItem  , 
					Perspective&	camera   ,
					PostScript&	ps       ,
	        		        const LineType&	linetype , 
					double		epsilon3d )
{
		//----- local variables
	int		status1 = 1, status2 = 1 ;
	Vector2		vertexPoint_screen2d ;
	Vector2		prevVertexPoint_screen2d ;
	Vector3*	vertexPoint_p ;
	Vector3*	prevVertexPoint_p;
	LineType	linetype_tmp ; linetype_tmp = linetype ;
	int		left_facet ;
	ColorType	color ;	

		//----- width of edge ( = real edge width )
	linetype_tmp.setWidth( objItem.lineType[ REAL ].getWidth() ) ;

		//----- set color of this edge to the color of facet
	left_facet = this->getLeftFacet();
	color = objItem.getFacetItem(left_facet).getAttribute().getKd()  ;	
	color.whiteToBlack();
	linetype_tmp.setColor( 	color ) ;	

		//----- get vertex positions in world coordinate
	prevVertexPoint_p	= &(objItem.vertexPosition[ this->prevVertex ]) ;
	vertexPoint_p		= &(objItem.vertexPosition[ this->vertex ]    ) ;

		//----- 2D screen coordinate
	status1 = camera.worldToScreen( *prevVertexPoint_p , prevVertexPoint_screen2d , epsilon3d ) ; 
	status2 = camera.worldToScreen( *vertexPoint_p     , vertexPoint_screen2d     , epsilon3d ) ;    

		//----- draw linesegment
	if( status1 && status2 && (this->getVisibility() != 0) ) 
	{
		ps.drawLineSegment_landscape_vp(	prevVertexPoint_screen2d ,
							vertexPoint_screen2d     , 
							&linetype_tmp              ) ;
	}

		//----- successfully drawn
	return NORMAL;

} // Edge::draw_landscape_PS ()



	//----- Object::drawWireFrame_landscape_PS ()
void  Object::drawWireFrame_landscape_PS (	Perspective&	camera ,
					PostScript&	ps     ,
				        const LineType&	linetype ,
					double		threshold_angle ,
					double		epsilon3d ,
					int		clipping_mode  ) 
{	
		//----- error checking
	if( vertexPosition == NULL ) { 
	  cerr << "***** ERROR: vertexPosition = NULL" << endl;
	  return ;
	}

	if( isBoundingBoxClippedOut(		camera    , 
						epsilon3d ,
						clipping_mode  )  )
	{
		return ;
	} else {

	}

		//----- local
	int		i ;
	int             nedge  = this->getNumEdge() ;
	int             nfacet = this->getNumFacet() ; 
	Vector3         N_tmp ;  // normal vector of temp facet 
	Vector3         P0_tmp ; // a vertex position on temp facet 
	Vector3         N_left ; // left-facet  normal vector
	Vector3         N_right; // right-facet normal vector
//	double		cos_threshold_angle  = cos( threshold_angle *180.0 / 3.141592 ) ;
	LineType        linetype_given = linetype    ;
	LineType*       linetype_p = &linetype_given ; // dummy
	
		//----- set working facet flags
	for( i = 0 ; i < nfacet ; i++ ) {
			//----- get i-th facetItem of array
		Facet* facetItem_p = &(facetArray[ i ]) ;

			//----- get plane data of i-th facet
		N_tmp  = facetItem_p->getNormalVector() ;
		P0_tmp = facetItem_p->getOneVertexPosition() ;

			//----- set flag to i-th facet to show its visibility
		if( camera.frontOrBack( N_tmp , P0_tmp )  == 1 ) {
			facetItem_p->setFlag( VISIBLE );
		} else {	// 0 or -1
			facetItem_p->setFlag( INVISIBLE );			
		}

	} // for(i) loop 1

		//----- draw all edges of this object
	for( i = 0 ; i < nedge ; i++ ) {

		//----- facet labels
	  int left_facet_label  = (edgeArray[ i ]).getLeftFacet();
	  int right_facet_label = (edgeArray[ i ]).getRightFacet();

		//----- facet visibilities
	  int left_facet_visibility  \
	    = (facetArray[ left_facet_label  ]).getFlag();
	  int right_facet_visibility  \
	    = (facetArray[ right_facet_label  ]).getFlag();

		//----- determine linetype of i-th edge
	  if( right_facet_label == NO_ITEM ) { 
		  linetype_p = &(lineType[REAL]) ; 
	  } else {

		  N_left = (facetArray[ left_facet_label  ]).getNormalVector() ;
		  N_right= (facetArray[ right_facet_label ]).getNormalVector() ;
		  double N_left_N_right_angle_deg \
		         = Tool3D::getAngleDeg( N_left, N_right ); 
	
		  // outline?
		  if((  (right_facet_visibility == INVISIBLE) && ( left_facet_visibility == VISIBLE   )   )\
				|| \
		     (  (right_facet_visibility == VISIBLE  ) && ( left_facet_visibility == INVISIBLE )   ) )
		    {
		      linetype_p = &lineType[ OUTLINE ] ; 
	            } 
		  // aux line?
		  else if( N_left_N_right_angle_deg < threshold_angle  ) { 
		    linetype_p = &(lineType[AUXILIARY]) ; 
		  } else {
	
		    linetype_p = &(lineType[REAL]) ; 
		  }

	  } // if ( right_facet_label...) -- else 

		//----- draw i-th edge
	  if( linetype_p->getVisibility() ) {
	    (edgeArray[ i ]).draw_landscape_PS( *this, camera, ps, *linetype_p , epsilon3d );
	  }
	} // for
}// Object::drawWireFrame_landscape_PS()




