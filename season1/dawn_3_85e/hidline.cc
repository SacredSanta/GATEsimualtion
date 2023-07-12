//////////////////////////////////////////////
//////////  hidline.cc  1996/01/09  //////////
//////////////////////////////////////////////

// #define DEBUG_HIDLINE

#include "hidline.h" 

	//----- constant parameters
const	int	DIAMOND_INTERVAL = 100 ;
const	int	MESSAGE_INTERVAL = 250 ;


///////////////////////////////////////////////////////
///// member functions of QuickSort<LineSegment> //////
///////////////////////////////////////////////////////

	//----- QuickSort<LineSegment>::compare0()
int  QuickSort<LineSegment>::compare0( LineSegment *a , LineSegment *b )
{
		//----- compare
	if         ( a->zmin < b->zmin ) {
		return   1 ;	// *a is nearer to eye than *b
	} else 	if ( b->zmin < a->zmin ) {
		return  -1 ;	// *b is nearer to eye than *a
	} else {
		return 0 ;
	}

} // QuickSort<LineSegment>compare0()	


	//----- QuickSort<LineSegment>::compare1()
	//.....  EMPTY
int  QuickSort<LineSegment>::compare1( LineSegment *a , LineSegment *b )
{ 

	return ( a == b ) ; // dummy
}



	//----- HidLine::HidLine()
HidLine::HidLine( Hid*  hid_p_given )
{
	numLineSegment	= 0 ;
	lineSegment	= NULL ;

	numVisibleLineSegment	= 0 ;
	visibleLineSegment 	= NULL ;

	hid_p = hid_p_given ;
}


	//----- HidLine::setArray()
void
HidLine::setArray()
{
		//----- clear old data
	delete [] lineSegment ;
	lineSegment	= NULL ;
	numLineSegment	= 0 ;

		//----- set numLineSegment 
	numLineSegment = lineSegmentList.getNItem();

		//----- set lineSegment 
	if( numLineSegment == 0 ) { 
		lineSegment = NULL ;
	} else {

		lineSegment = new LineSegment [ numLineSegment ] ; 
		if( NULL == lineSegment ) {
			cerr << "ERROR (HidLine::setArray()) \n";
			exit(1);
		}
		lineSegmentList.toArray( lineSegment , numLineSegment );
	} 
		
} // HidLine::setArray()



	//----- HidLine::zsortLineSegment()
void
HidLine::zsortLineSegment()
{
	if( numLineSegment > 1 ) {
			//----- do quick sort of the pointer array
		QuickSort<LineSegment>	ls_qsort ;
		ls_qsort.setMode ( 0 ); // use compare0()
		ls_qsort.doSort  ( lineSegment, 0, (numLineSegment - 1 ) );
	}
}


	//----- HidLine::install()
void
HidLine::install()
{
		//----- set numLineSegment and lineSegment, 
	setArray();	

		//----- z-sort (far_from_eye ---> near_from_eye)
	zsortLineSegment();


		//----- 
	if( getenv( "DAWN_DATA_ACCUMULATION" ) != NULL ) {
		if( !strcmp( getenv( "DAWN_DATA_ACCUMULATION" ),"0" ) ) 
		{ 
				//----- clear temporary list
			lineSegmentList.clear();
		} 
	} else { // No environmental variable
			//----- clear temporary list
		lineSegmentList.clear();
	}

} // HidLine::install()


	//----- HidLine::~HidLine()
HidLine::~HidLine()
{
	this->clear();
}  // HidLine::~HidLine()

	//----- HidLine::clear()
void
HidLine::clear()
{
	lineSegmentList.clear() ;

	numLineSegment = 0 ;
	delete [] lineSegment        ;  lineSegment        = NULL ;

	numVisibleLineSegment = 0 ;
	delete [] visibleLineSegment ;  visibleLineSegment = NULL ;

	hid_p = NULL ;

}  // HidLine::clear()

	//----- HidLine::addItem_lineSegmentList
int
HidLine::addItem_lineSegmentList  ( LineSegment& lineseg )
{
	return lineSegmentList.addItem( lineseg ); 
}

	//----- HidLine::maxMinTest( lineseg_p, bounding_diamond) 
int	HidLine::maxMinTest(	LineSegment* lineseg_p , 
				double xmin, double xmax,
				double ymin, double ymax,
				double umin, double umax,
				double vmin, double vmax,
				double epsilon2d ) 
{
		//----- local variables
	int	status = 0 ;

		//----- max-min test
	if(	lineseg_p->xmax < xmin - epsilon2d ||\
		lineseg_p->xmin > xmax + epsilon2d ||\
		lineseg_p->ymax < ymin - epsilon2d ||\
		lineseg_p->ymin > ymax + epsilon2d ||\
		lineseg_p->umax < umin - epsilon2d ||\
		lineseg_p->umin > umax + epsilon2d ||\
		lineseg_p->vmax < vmin - epsilon2d ||\
		lineseg_p->vmin > vmax + epsilon2d   ) 
	{
		status = 0 ;	// independent

	} else {
		status = 1 ;	// else or unknown
	}

		//----- RETURN
	return status ;

} // HidLine::maxMinTest(lineseg , bounding_diamond) 


	//----- HidLine::maxMinTest()
	//..... max min test for 8 direction
	//.....  Return
	//.....		0:  if *lineseg_p is independent of given *polygon_p
	//.....		    on screen.
	//.....			--> *lineseg_p is never hidden by *polygon_p
	//.....		1:  else
	//.....			--> *lineseg_p may be hidden by polygon_p
int	HidLine::maxMinTest( LineSegment* lineseg_p , Polygon3D*  polygon_p  , double epsilon ) 
{
		//----- local variables
	int	status ;

		//----- max-min test
	if(	lineseg_p->xmax < polygon_p->xmin - epsilon ||\
		lineseg_p->xmin > polygon_p->xmax + epsilon ||\
		lineseg_p->ymax < polygon_p->ymin - epsilon ||\
		lineseg_p->ymin > polygon_p->ymax + epsilon ||\
		lineseg_p->umax < polygon_p->umin - epsilon ||\
		lineseg_p->umin > polygon_p->umax + epsilon ||\
		lineseg_p->vmax < polygon_p->vmin - epsilon ||\
		lineseg_p->vmin > polygon_p->vmax + epsilon   ) 
	{
		status = 0 ;	// independent

	} else {
		status = 1 ;	// else or unknown
	}

		//----- RETURN
	return status ;

} // HidLine::maxMinTest(lineseg , polygon )


	//-----	HidLine::maxMinTest()
int	HidLine::maxMinTest(	LineSegment* linesegA_p , 
				LineSegment* linesegB_p , 
				double epsilon             )
{
		//----- local variables
	int	status ;

		//----- max-min test
	if(	linesegA_p->xmax < linesegB_p->xmin + epsilon ||\
		linesegA_p->xmin > linesegB_p->xmax - epsilon ||\
		linesegA_p->ymax < linesegB_p->ymin + epsilon ||\
		linesegA_p->ymin > linesegB_p->ymax - epsilon ||\
		linesegA_p->umax < linesegB_p->umin + epsilon ||\
		linesegA_p->umin > linesegB_p->umax - epsilon ||\
		linesegA_p->vmax < linesegB_p->vmin + epsilon ||\
		linesegA_p->vmin > linesegB_p->vmax - epsilon   ) 
	{
		status = 0 ;	// independent

	} else {
		status = 1 ;	// else or unknown
	}

		//----- RETURN
	return status ;

} // HidLine::maxMinTest(lineseg, lineseg)


		//----- Polygon3D::depthOverlapTest()
		//.....  Depth overlapping test
		//.....  Return
		//.....		 1: if (*a)->zmax < (*b)->zmin.
		//.....		-1: if (*b)->zmax < (*a)->zmin.
		//.....		 0: else (ovrlaps in z direction)
		//..... (zmax, zmin are eye coordinate.)
int	HidLine::depthOverlapTest(	LineSegment* lineseg_p , 
					Polygon3D*   polygon_p  , 
					double epsilon3d ) 
{
		//----- local variables
	int	status ; // return value

		//----- check if overlapping exists in z_eye direction
	if        ( lineseg_p->zmax    < polygon_p->zmin + epsilon3d ) {
		status = 1 ;	// linesegment is nearer to eye

	} else if ( polygon_p->zmax  < lineseg_p->zmin + epsilon3d ) {
		status = -1 ;	// polygon is nearer to eye

	} else {
		status = 0 ;	// overlap in eye_z-direction 
	}

		//----- return status
	return status ;

} // HidLine::depthOverlapTest( lineseg, polygon)


int	
HidLine::depthOverlapTest(	LineSegment* linesegA_p , 
				LineSegment* linesegB_p , 
				double epsilon3d          ) 
{
		//----- local variables
	int	status ; // return value

		//----- check if overlapping exists in z_eye direction
	if        ( linesegA_p->zmax    < linesegB_p->zmin + epsilon3d ) {
		status = 1 ;	// lineseg A is nearer to eye

	} else if ( linesegB_p->zmax  < linesegA_p->zmin + epsilon3d ) {
		status = -1 ;	// lineseg B is nearer to eye

	} else {
		status = 0 ;	// overlap in eye_z-direction 
	}

		//----- return status
	return status ;

} // HidLine::depthOverlapTest( lineseg, lineseg )


	//----- HidLine::isHidden()
int	HidLine::isHidden(	LineSegment*	lineseg_p    ,    
				Polygon3D*	polygon_p    ,    
				int*		relation_2d  ,    
				Vector3*	X3D_array    ,    
				double*		t_line_X3D_array ,
				int*		X3D_side_p     , 
			  	double		epsilon3d        ,    
				double		epsilon2d            )
{
		//----- local const
	const double ONE_MINUS_EPSILON2D = 1.0 - epsilon2d ;	

		//----- local var
	int		status ;
	int		side_begin ;
	int		side_end ;
	int		num_vertex ;
	int		num_intersection ;
	int		flag_err ;
	int		side_X3D[2] ;
	int		edge_label_array[2];
	Vector2		X_array[2];
	double		t_line_X2D_array[2] ;
	double		t_edge_2D_array[2] ;
	double		dummy ;
	Vector2*	vertex2D;
	Vector2*	begin2D_p ;
	Vector2*	end2D_p ;
	Vector3*	P_on_p ;
	Vector3*	normal_vector_p ;
	Vector3*	begin3D_p ;
	Vector3*	end3D_p ;

		//----- preliminaries
		//----------- data of linesegment
	begin3D_p	= &(lineseg_p->begin ) ;
	end3D_p		= &(lineseg_p->end   ) ;
	begin2D_p	= &(lineseg_p->begin2D ) ;
	end2D_p		= &(lineseg_p->end2D   ) ;
		//----------- data of polygon
	P_on_p 			= (polygon_p->vertex)[0] ;
	normal_vector_p		= &(polygon_p->normalVector) ;
	num_vertex		= polygon_p->numVertex ;
	vertex2D		= polygon_p->vertex2D   ;

		//-------------------------------------------------------------//
		//----- TEST 1: depth overlapping test in z_eye direction -----//
		//-------------------------------------------------------------//
	status = depthOverlapTest( lineseg_p, polygon_p, epsilon3d ) ;

	if        ( status ==  1 ) { // if *lineseg_p is nearer to eye...
		*relation_2d = Tool2D::UNKNOWN ;
		return  NOT_HIDDEN2  ; 
	} 

		//------------------------------------------------//
		//----- TEST 2: max-min test for 8 direction -----//
		//------------------------------------------------//
	status	=  maxMinTest( lineseg_p, polygon_p , epsilon2d ) ;
	if( status == HidLine::INDEPENDENT ) { 
		*relation_2d = Tool2D::INDEPENDENT ;
		return NOT_HIDDEN ; 
	}

		//--------------------------------------------//
		//----- TEST 3: Is the whole linesegment -----//
		//-----         in positive side of the  -----//
		//-----         plane of the polygon?    -----//      
		//--------------------------------------------//
	side_begin = Tool3D::planeVsPoint(	*normal_vector_p, *P_on_p , 
						*begin3D_p , &dummy , epsilon3d );

	side_end   = Tool3D::planeVsPoint(	*normal_vector_p, *P_on_p , 
						*end3D_p , &dummy , epsilon3d );

	if(	( side_begin == Tool3D::POSITIVE_SIDE || side_begin == Tool3D::ON_PLANE )  \
		&&                                                                         \
		( side_end   == Tool3D::POSITIVE_SIDE || side_end   == Tool3D::ON_PLANE )    )
	{ 
		*relation_2d = Tool2D::UNKNOWN ;
		return NOT_HIDDEN ; 
	}

		//-------------------------------------//
		//----- Calc 2-dim intersections  -----//
		//-------------------------------------//
	num_intersection = \
	Tool2D::directedLineVsConvexPolygon(	*begin2D_p , 
						*end2D_p , 
						vertex2D , 
						num_vertex ,
						X_array ,
						t_line_X2D_array,
						t_edge_2D_array, 
						edge_label_array,
						epsilon2d );


		//-----------------------------------------------------//
		//----- Test 4: line vs polygon independensy test -----//
		//-----------------------------------------------------//
	if(	num_intersection < 2      ) { 
		*relation_2d = Tool2D::INDEPENDENT ;
		return NOT_HIDDEN ; 
	} 

		//----- In the following num_intersection = 2

		//--------------------------------------------//
		//----- TEST 5: consistensy test of      -----//
		//-----         the 2-dim intersections  -----//
		//--------------------------------------------//
	flag_err = 0 ;	// initialization
	if(		edge_label_array[0] <  0          || 
			edge_label_array[0] >= num_vertex || 
			edge_label_array[1] < 0 	  || 
			edge_label_array[1] >= num_vertex     ) 
	{
			//----- ERROR 1A
#if defined DEBUG_HIDLINE
		cerr << "WARNING (HidLine::isHidden() )  1A:\n";
#endif
		flag_err = 1 ;
	}
	else if(    edge_label_array[0] == edge_label_array[1] ) { 
			//----- ERROR 1B
#if defined DEBUG_HIDLINE
		cerr << "WARNING (HidLine::isHidden() )  1B:\n";
#endif
		flag_err = 1 ;
	}
	else if( t_line_X2D_array[0] > t_line_X2D_array[1] ) {  
			//----- ERROR 1C
#if defined DEBUG_HIDLINE
		cerr << "WARNING (HidLine::isHidden() )  1C:\n";
#endif
		flag_err = 1 ;
	}
		//----- if ERROR, skip this polygon
	if( 1 == flag_err ) {
		*relation_2d = Tool2D::INDEPENDENT ;
		return NOT_HIDDEN ; 
	}


		//--------------------------------------------------------//
		//----- TEST 6: lineseg vs polygon independensy test -----//
		//--------------------------------------------------------//
	else 	if(	( t_line_X2D_array[ 0 ] > ONE_MINUS_EPSILON2D ) \
			&&                                        \
			( t_line_X2D_array[ 1 ] > ONE_MINUS_EPSILON2D )   )
	{
			//----- linesegment is outside polygon (1)
		*relation_2d = Tool2D::INDEPENDENT ;
		return NOT_HIDDEN ; 
	}
	else if(	 t_line_X2D_array[ 0 ] <  epsilon2d    && 
			 t_line_X2D_array[ 1 ] <  epsilon2d         ) 
	{
			//----- linesegment is outside polygon (2)
		*relation_2d = Tool2D::INDEPENDENT ;
		return NOT_HIDDEN ; 
	}


		//-----------------------------------------------------//
		//----- TEST 7: lineseg vs polygon inclusion test -----//
		//-----------------------------------------------------//
	if(	( side_begin == Tool3D::NEGATIVE_SIDE    )  \
		&&                                          \
		( side_end   == Tool3D::NEGATIVE_SIDE    )  \
		&&                                          \
		( t_line_X2D_array[ 0 ] <  epsilon2d         )  \
		&& 		                            \
		( t_line_X2D_array[ 1 ] >  ONE_MINUS_EPSILON2D  )    )
	{ 
		*relation_2d = Tool2D::INCLUDED;
		return HIDDEN ; 
	}
	

		//------------------------------------------------------------------//
		//----- TEST 8: Is included part in positive side of polygon ? -----//
		//------------------------------------------------------------------//
	{
			//----- local to this block
		int		i ;
		Perspective*	camera_p	= hid_p->getCamera();
		double		z0, z1, t ;
		z0 = camera_p->eyeZ( *begin3D_p ) ;
		z1 = camera_p->eyeZ( *end3D_p ) ;

			//----- calc X3D_array and side_X3D
		for( i = 0 ; i < 2 ; i++ ) {

			t  = t_line_X2D_array[i] ;
			t_line_X3D_array[i] = ( z0 * t ) / ( z1 * (1.0 - t ) + z0 * t ) ;
			X3D_array[i] \
			= Tool3D::getPositionOnLine( *begin3D_p, *end3D_p , t_line_X3D_array[i] );

				//----- calc position of X3D_array[] relative to plane of polygon
			side_X3D[i] = Tool3D::planeVsPoint(	*normal_vector_p, *P_on_p , 
								X3D_array[i] , &dummy , epsilon3d );
		} // for

	}


		//----- 4 cases of side_X3D[]
	if      ( side_X3D[0] >  0 && side_X3D[1] >  0 ) { *X3D_side_p = PLUS_PLUS   ; }
	else if ( side_X3D[0] >  0 && side_X3D[1] == 0 ) { *X3D_side_p = PLUS_ZERO   ; }
	else if ( side_X3D[0] >  0 && side_X3D[1] <  0 ) { *X3D_side_p = PLUS_MINUS  ; }

	else if ( side_X3D[0] == 0 && side_X3D[1] >  0 ) { *X3D_side_p = ZERO_PLUS   ; }
	else if ( side_X3D[0] == 0 && side_X3D[1] == 0 ) { *X3D_side_p = ZERO_ZERO   ; }
	else if ( side_X3D[0] == 0 && side_X3D[1] <  0 ) { *X3D_side_p = ZERO_MINUS  ; }

	else if ( side_X3D[0] <  0 && side_X3D[1] >  0 ) { *X3D_side_p = MINUS_PLUS  ; }
	else if ( side_X3D[0] <  0 && side_X3D[1] == 0 ) { *X3D_side_p = MINUS_ZERO  ; }
	else if ( side_X3D[0] <  0 && side_X3D[1] <  0 ) { *X3D_side_p = MINUS_MINUS ; }

		//----- no MINUS ? 
	if(	*X3D_side_p == PLUS_PLUS  || \
		*X3D_side_p == PLUS_ZERO  || \
		*X3D_side_p == ZERO_PLUS  || \
		*X3D_side_p == ZERO_ZERO       )
	{
		*relation_2d = Tool2D::CROSS ;
		return NOT_HIDDEN ; 		
	} else {
		*relation_2d = Tool2D::CROSS ;
		return HIDDEN ;
	}


} // HidLine::isHidden()


	//----- HidLine::extractVisiblePart()
int	HidLine::extractVisiblePart( double epsilon3d , double epsilon2d )
{
		//----- local
	int	j ;
	int	num_polygon3D ;
	int	relation_2d ;
	int	isHidden_status ;
	int	flag_hidden ;
	int	X3D_side  ;
	double			t_line_X3D_array[2] ;
	Vector3			X3D_array[2] ;
	LineSegment		lineseg_popd ;
	List<LineSegment>	lineseg_stack ;
	List<LineSegment>	visible_lineseg_list ;
	double		xPmin, yPmin, uPmin, vPmin ; // min of polygon2d[] in hid_p
	double		xPmax, yPmax, uPmax, vPmax ; // max of polygon2d[] in hid_p
	double		xLmin, yLmin, uLmin, vLmin ; // min of polygon2d[] in hid_p
	double		xLmax, yLmax, uLmax, vLmax ; // max of polygon2d[] in hid_p
	int		flag_total_P_bounding_diamond_exist ;
	int		flag_total_L_bounding_diamond_exist ;
	int		ncount ;
	Polygon3D*	polygon3d_subset = NULL ;
	int		nitem_polygon3d_subset;


		//----- clear old data
	numVisibleLineSegment = 0 ;
	delete [] visibleLineSegment ;	visibleLineSegment = NULL ;

		//----- error checking no linesegment ?
	if( numLineSegment == 0 ) { 
		return numVisibleLineSegment ;
	}

		//----- num Polygon3D items stored in *hid_p
	num_polygon3D = hid_p->getNumPolygon3D();


		//----- no polygons in hid? (e.g. wireframe mode)
	if( num_polygon3D <= 0 ) 
	{
			//----- regard lineSegment with visibleLineSegment
			//..... without copying
		numVisibleLineSegment = numLineSegment ;
		visibleLineSegment    = lineSegment    ;

		numLineSegment = 0    ;
		lineSegment    = NULL ;

#if defined DEBUG_HIDLINE
		cerr << " ***** No polygon in hid.  visibleLineSegment = lineSegment[] " << endl;
#endif

		return numVisibleLineSegment ;
	}


		//----- initialize stack
	lineseg_stack.clear() ;
	lineseg_stack.addArray( lineSegment, numLineSegment ) ;

		//----- calc bounding diamond of TOTAL hid_p->polygon2d[]
	flag_total_P_bounding_diamond_exist = \
	hid_p->getBoundingDiamond(	&xPmin , &xPmax ,
					&yPmin , &yPmax ,
					&uPmin , &uPmax ,
					&vPmin , &vPmax  ) ;

		//----- calc bounding diamond of TOTAL this->lineSegment[]
	flag_total_L_bounding_diamond_exist = \
	this->getBoundingDiamond(	&xLmin , &xLmax ,
					&yLmin , &yLmax ,
					&uLmin , &uLmax ,
					&vLmin , &vLmax  ) ;

		//----- select valid subset of polygons, which may hide items in linesegmnet[]
	if ( flag_total_L_bounding_diamond_exist )
	{ 
			//----- local
		List<Polygon3D>	polygon3d_list_tmp ;

			//----- selection loop
		for( j = 0 ; j < num_polygon3D ; j++ ) {

				//----- original Polygon3D
			Polygon3D	polygon3D_j_original ;
			hid_p->recoverOriginalPolygon3D(	(hid_p->getPolygon3D(j)) , 
								&polygon3D_j_original      ,
								epsilon3d                   );
						// recover original polygon for j-th Polygon3D
			polygon3D_j_original.setImage( (hid_p->getCamera()), epsilon3d )  ;

			if(	xLmax < polygon3D_j_original.xmin + epsilon2d ||\
				xLmin > polygon3D_j_original.xmax - epsilon2d ||\
				yLmax < polygon3D_j_original.ymin + epsilon2d ||\
				yLmin > polygon3D_j_original.ymax - epsilon2d ||\
				uLmax < polygon3D_j_original.umin + epsilon2d ||\
				uLmin > polygon3D_j_original.umax - epsilon2d ||\
				vLmax < polygon3D_j_original.vmin + epsilon2d ||\
				vLmin > polygon3D_j_original.vmax - epsilon2d   ) 
			{
				; // this polygon is independent of all linesegment
			} else {
				 	//----- this polygon may hide linesegments
				polygon3d_list_tmp.push ( polygon3D_j_original) ;
			}
		} // j

			//----- list --> array
		nitem_polygon3d_subset = polygon3d_list_tmp.getNItem();
		polygon3d_subset = new Polygon3D [ nitem_polygon3d_subset ];
		if( polygon3d_subset == NULL ) {
			cerr << "ERROR (HidLine::extractVisiblePart()()), 1A\n";
			exit(1);
		}
		polygon3d_list_tmp.toArray( polygon3d_subset, nitem_polygon3d_subset );
		polygon3d_list_tmp.clear();
	} else {
		nitem_polygon3d_subset = num_polygon3D ;
		polygon3d_subset = new Polygon3D [ nitem_polygon3d_subset ];
		if( polygon3d_subset == NULL ) {
			cerr << "ERROR (HidLine::extractVisiblePart()()), 1B\n";
			exit(1);
		}
		for( j = 0 ; j < num_polygon3D ; j++ ) {

				//----- original Polygon3D
			Polygon3D	polygon3D_j_original ;
			hid_p->recoverOriginalPolygon3D(	(hid_p->getPolygon3D(j)) , 
								&polygon3D_j_original      ,
								epsilon3d                   );
						// recover original polygon for j-th Polygon3D
			polygon3D_j_original.setImage( (hid_p->getCamera()), epsilon3d )  ;

				//----- store result
			polygon3d_subset [j] = polygon3D_j_original ;
		} // j

	} // if--else


		//----- extract visible parts of each item in this->lineSegment[]
	ncount = 0 ;
	while( lineseg_stack.pop( lineseg_popd ) ) {

			//----- local
		int		div_val = ncount / MESSAGE_INTERVAL ;

			//----- message
		if( (div_val * MESSAGE_INTERVAL) == ncount && ncount > 0 ) {
#if defined DEBUG_HIDLINE
			cerr << "  ****** Remaining linesegments: " << lineseg_stack.getNItem() << endl ;
			cerr << "\n";
#endif
		}
		ncount++ ;

			//----- initialize flag
		flag_hidden = NOT_HIDDEN ; // initialization

			//----- max-min test with TOTAL polygon2d[]
		if( flag_total_P_bounding_diamond_exist )
		{
			int status_tmp =  \
			this->maxMinTest(	&lineseg_popd, 
						xPmin, xPmax, yPmin, yPmax, 
						uPmin, uPmax, vPmin, vPmax , epsilon2d ) ;
			
			if( status_tmp == 0 ) 
			{
					//----- independent
				flag_hidden = NOT_HIDDEN ;
				visible_lineseg_list.push( lineseg_popd );
				continue ; // pop next lineseg
			}
		}


			//----- for each polygon3d[i]...
		for( j = 0 ; j < nitem_polygon3d_subset ; j++ ) {

				//-----  isHidden_status becomes
				//.....  
				//.....  		  NOT_HIDDEN  	if  lineseg is not hidden by polygon
				//.....  		  NOT_HIDDEN2 	if  lineseg is not hidden by polygon
				//.....	                                 and lineseg.zmax_eye < polygon.zmin_eye
				//.....			  HIDDEN      	if  lineseg is hidden by polygon
			isHidden_status = \
			isHidden(	&lineseg_popd          , \
					&(polygon3d_subset[j])  , \
					&relation_2d           , \
					X3D_array              , \
					t_line_X3D_array       , \
					&X3D_side              , \
					epsilon3d, epsilon2d       );


				//----- decide if lineseg_popd should be output or not
			if ( isHidden_status == NOT_HIDDEN  ) { 
					//----- NOT_HIDDEN 
				flag_hidden = NOT_HIDDEN ; 

					//----- skip to j+1 
				continue ; 
			}
			else if  ( isHidden_status == NOT_HIDDEN2 ) { 
					//----- NOT_HIDDEN2 
				flag_hidden = NOT_HIDDEN ; 
				break ; // Lineseg is not hidden by the j-th polygon,    
					// and the remaining set of polygons.
					// Pop next linesegment.
			} else if ( isHidden_status == HIDDEN ) { 


					//----- HIDDEN 
				if( relation_2d == Tool2D::INDEPENDENT ) { 
						//----- independent: not hidden
					flag_hidden = NOT_HIDDEN ;
					continue ; // to next j (polygon)
				} else if (	relation_2d == Tool2D::INCLUDED   ) {
						//----- Lineseg is totally hidden by polygon.
					flag_hidden = HIDDEN ;
					break ; // Abandon this linesegment
				        	// and pop next one.
				} else if (	relation_2d == Tool2D::CROSS ) 	{
						//----- Lineseg is partially hidden by polygon
					flag_hidden = HIDDEN ;

						//----- devide out visible parts of linesegment
					hideLineSegmentByPolygon(	&lineseg_popd               ,    
									&(polygon3d_subset[j] )    ,
									X3D_array                   ,    
									t_line_X3D_array            , 
									X3D_side                    ,
									&lineseg_stack              ,
				  					epsilon3d                   , 
									epsilon2d    			) ;

					break ;
				} else {
#if defined DEBUG_HIDLINE
					cerr << "WARNING (HidLine::extractVisiblePart() ) 2\n";
					cerr << "   Unexpected relation between linesegment and polygon\n";
					cerr << "   Regarded as 'NOT_HIDDEN'\n";
#endif
					flag_hidden = NOT_HIDDEN ; 
					continue ;
				}

			} else { 
#if defined DEBUG_HIDLINE
				cerr << "WARNING (HidLine::extractVisiblePart() ) 3\n";
				cerr << "   Unexpected relation between linesegment and polygon\n";
				cerr << "   Regarded as 'NOT_HIDDEN'\n";
#endif
				flag_hidden = NOT_HIDDEN ; 
				continue ;
			} 

		} // for(j)

		if ( flag_hidden == NOT_HIDDEN ) { 
			visible_lineseg_list.push( lineseg_popd );
		}

	} // while

		//----- set visible line segments to array
//////////////////////////////////////////////////////////////////
//	numVisibleLineSegment = visible_lineseg_list.getNItem();
//	visibleLineSegment = new LineSegment [numVisibleLineSegment] ; 
//	if( NULL == visibleLineSegment ) {
//		cerr << "ERROR (HidLine::extractVisiblePart()()), 4\n";
//		exit(1);
//	}
//
//	visible_lineseg_list.toArray(	visibleLineSegment, numVisibleLineSegment );
////////////////////////////////////////////////////////////////////
	setVisibleLineSegment( visible_lineseg_list );
	visible_lineseg_list.clear();


		//-----clear mamory
#if defined DISPLAY_LOG
			cerr << endl ;
			cerr << "  *** Clearing unnesessary data from memory (Type 2, Phase 2) ..." << endl;
			cerr << endl ;
#endif
	numLineSegment = 0 ;
	delete [] lineSegment        ;  lineSegment        = NULL ;
	delete [] polygon3d_subset ;

		//----- return number of visible line segment
	return numVisibleLineSegment ;

} // HidLine::extractVisiblePart()


	//----- HidLine::hideLineSegment()
int	HidLine::hideLineSegmentByPolygon(	LineSegment*		lineseg_p    ,    
						Polygon3D*		polygon_p    ,    
						Vector3*		X3D_array    ,    
						double*			t_line_X3D_array, 
						int			X3D_side     , 
						List<LineSegment>*	output_lineseg_list_p ,
			  			double			epsilon3d , 
						double			epsilon2d   ) 
{


		//----- local variables and constants
	int			num_output_lineseg = 0 ;
	int			status ;
	const double		EPSILON           = epsilon2d ; // used as dimesionless small value
	const double		ONE_MINUS_EPSILON = 1.0 - EPSILON ;	
						      // used as dimesionless small value
	Vector3			Y3D ;	// 3D intersection between lineseg 
					// and plane of polygon
	const	Vector3*	begin3D_p	= &( lineseg_p->begin    ) ;
	const	Vector3*	end3D_p		= &( lineseg_p->end      ) ;
	const	LineType*	linetype_p	= &( lineseg_p->linetype ) ;
	const	Vector3*	normal_vector_p	= &( polygon_p->normalVector ) ;
	const	Vector3*	P_on_p		= (polygon_p->vertex)[0] ;
	Perspective*		camera_p	= hid_p->getCamera() ;
	LineSegment		lineseg_tmp ;



		//----- calc Y3D if necessary
		//.....  (3D intersection between lineseg and plane of polygon )
	if( PLUS_MINUS == X3D_side || MINUS_PLUS == X3D_side ) 
	{
			//----- calc intersection between line and plane
		status = Tool3D::getPlaneLineIntersection(	*normal_vector_p, *P_on_p , 
								*begin3D_p , 
								*end3D_p , 
								&Y3D  ,
								epsilon3d );
		if( !status ) {
			cerr << "ERROR (HidLine::HideLineSegment()) 1\n";
			return num_output_lineseg ; // abandon this linesegment
		}

	} // if (PLUS_MINUS ...)


		//----- 
	switch ( X3D_side ) {
	 case ZERO_MINUS  : 
	 case MINUS_ZERO  : 
	 case MINUS_MINUS : {
			//----- linesegment *begin_3D_p--X3D_array[0]
		if( t_line_X3D_array[0] > EPSILON ) {
			num_output_lineseg++ ;
			lineseg_tmp.setPosition( *begin3D_p   , X3D_array[0] ) ;
			lineseg_tmp.setLineType( *linetype_p ) ;
			lineseg_tmp.setImage( camera_p, epsilon3d ) ;
			output_lineseg_list_p->push( lineseg_tmp ) ;
		}

			//----- linesegment X3D_array[1]--*end_3D_p
		if( t_line_X3D_array[1] < ONE_MINUS_EPSILON ) {
			num_output_lineseg++ ;
			lineseg_tmp.setPosition( X3D_array[1] , *end3D_p ) ;
			lineseg_tmp.setLineType( *linetype_p ) ;
			lineseg_tmp.setImage( camera_p, epsilon3d ) ;
			output_lineseg_list_p->push( lineseg_tmp ) ;
		}
		break ;
	 } // MINUS_MINUS
	 case PLUS_MINUS : {
			//----- linesegment *begin3D_p--Y3D (visible)
		num_output_lineseg++ ;
		lineseg_tmp.setPosition( *begin3D_p   , Y3D ) ;
		lineseg_tmp.setLineType( *linetype_p ) ;
		lineseg_tmp.setImage( camera_p, epsilon3d ) ;
		output_lineseg_list_p->push( lineseg_tmp ) ;

			//----- linesegment X3D_array[1]--*end_3D_p
		if( t_line_X3D_array[1] < ONE_MINUS_EPSILON ) {
			num_output_lineseg++ ;
			lineseg_tmp.setPosition( X3D_array[1] , *end3D_p ) ;
			lineseg_tmp.setLineType( *linetype_p ) ;
			lineseg_tmp.setImage( camera_p, epsilon3d ) ;
			output_lineseg_list_p->push( lineseg_tmp ) ;
		}
		break ;
	 } // PLUS_MINUS : 
	 case MINUS_PLUS : {

			//----- linesegment *begin_3D_p--X3D_array[0]
		if( t_line_X3D_array[0] > EPSILON ) {
			num_output_lineseg++ ;
			lineseg_tmp.setPosition( *begin3D_p   , X3D_array[0] ) ;
			lineseg_tmp.setLineType( *linetype_p ) ;
			lineseg_tmp.setImage( camera_p, epsilon3d ) ;
			output_lineseg_list_p->push( lineseg_tmp ) ;
		}

			//----- linesegment Y_3D--*end3D_p (visible)
		num_output_lineseg++ ;
		lineseg_tmp.setPosition( Y3D, *end3D_p ) ;
		lineseg_tmp.setLineType( *linetype_p ) ;
		lineseg_tmp.setImage( camera_p, epsilon3d ) ;
		output_lineseg_list_p->push( lineseg_tmp ) ;
		break ;
	 } // MINUS_PLUS : 
	 default: {
#if defined DEBUG_HIDLINE
		cerr << "WARNING (HidLine::HideLineSegment()) 2\n";
		cerr << "     Exceptional case of X3D_SIDE\n";
#endif
		break ;
	 }

	} // switch( X3D_SIDE )
		//-----
	return num_output_lineseg ;

} // HidLine::hideLineSegmentByPolygon()




		//----- HidLine::doHiddenPartElimination()
int	
HidLine::doHiddenPartElimination( double epsilon3d , double epsilon2d )
{
	extractVisiblePart( epsilon3d, epsilon2d );
	setIntersection2DToVisibleLineSegment( epsilon3d, epsilon2d );
	return numVisibleLineSegment ;
}

		//----- HidLine::doHiddenPartElimination()
int	
HidLine::doNotHiddenPartElimination( double epsilon3d , double epsilon2d )
{
	extractVisiblePart( epsilon3d, epsilon2d );
//	setIntersection2DToVisibleLineSegment( epsilon3d, epsilon2d );
	return numVisibleLineSegment ;
}

		//----- HidLine::doHiddenPartElimination2()
int	
HidLine::doNotHiddenPartElimination2( double epsilon3d , double epsilon2d )
{
	makeAllLinesVisible( epsilon3d, epsilon2d );
	return numVisibleLineSegment ;
}


	//----- HidLine::getBegin2D()
Vector2&	HidLine::getBegin2D(int  index )
{
	if( index >= numLineSegment || index < 0 ) {
		cerr << "ERROR: (HidLine::getBegin2D() ): ";
		cerr <<	"Irregal item index. \n";
		exit(1) ;
	}

		//----- return index-th item
	return (lineSegment[ index ].begin2D) ;
}


	//----- HidLine::getEnd2D()
Vector2&	HidLine::getEnd2D  (int  index )
{
	if( index >= numLineSegment || index < 0 ) {
		cerr << "ERROR: (HidLine::getEnd2D() ): ";
		cerr <<	"Irregal item index. \n";
		exit(1) ;
	}

		//----- return index-th item
	return (lineSegment[ index ].end2D) ;
}


	//----- HidLine::getLineType()
LineType&	HidLine::getLineType( int index )
{
	if( index >= numLineSegment || index < 0 ) {
		cerr << "ERROR: (HidLine::getLineType() ): ";
		cerr <<	"Irregal item index. \n";
		exit(1) ;
	}

		//----- return index-th polygon
	return (lineSegment[ index ].linetype) ;

}


	//----- HidLine::getBegin2DVisible()
Vector2&	HidLine::getBegin2DVisible(int	index )
{
	if( index >= numVisibleLineSegment || index < 0 ) {
		cerr << "ERROR: (HidLine::getBegin2DVisible() ): ";
		cerr <<	"Irregal item index. \n";
		exit(1) ;
	}

		//----- return index-th item
	return (visibleLineSegment[ index ].begin2D) ;
}


	//----- HidLine::getEnd2DVisible()
Vector2&	HidLine::getEnd2DVisible  (int	index )
{
	if( index >= numVisibleLineSegment || index < 0 ) {
		cerr << "ERROR: (HidLine::getEnd2DVisible() ): ";
		cerr <<	"Irregal item index. \n";
		exit(1) ;
	}

		//----- return index-th item
	return (visibleLineSegment[ index ].end2D) ;
}


	//----- HidLine::getLineTypeVisible()
LineType&	HidLine::getLineTypeVisible( int index )
{
	if( index >= numVisibleLineSegment || index < 0 ) {
		cerr << "ERROR: (HidLine::getLineTypeVisible() ): ";
		cerr <<	"Irregal item index. \n";
		exit(1) ;
	}

		//----- return index-th polygon
	return (visibleLineSegment[ index ].linetype) ;

}


	//----- HidLine::getNumIntersection2DVisible ()
int
HidLine::getNumIntersection2DVisible  (int index )
{
	if( index >= numVisibleLineSegment || index < 0 ) {
		cerr << "ERROR: (HidLine::getNumIntersection2DVisible() ): ";
		cerr <<	"Irregal item index. \n";
		exit(1) ;
	}

		//----- return index-th item
	return ( visibleLineSegment[ index ].numIntersection2D ) ;

} 


	//----- HidLine::getIntersection2DVisible ()
int	
HidLine::getIntersection2DVisible (	int index_visible_lineseg , 
					int index_intersection2D  ,
					Intersection2D* intersection2D_p )
{
		//----- local
	int	num_intersection2D ;


		//----- valid index_visible_lineseg?
	if(	index_visible_lineseg >= numVisibleLineSegment \
		|| \
		index_visible_lineseg < 0                           ) 
	{
		return 0 ;
	}

		//----- number of intersection2D for this visible linesegment
	num_intersection2D = \
	this->getNumIntersection2DVisible ( index_visible_lineseg ) ;


		//-----
	if( 	index_intersection2D >= num_intersection2D \
		|| \
		index_intersection2D < 0                        ) 
	{
		return 0 ;
	}

		//----- return intersection2D_p
	*intersection2D_p = \
	visibleLineSegment[ index_visible_lineseg ].intersection2D[ index_intersection2D ];

		//----- return status
	return 1 ;

} // HidLine::getIntersection2DVisible ()


	//----- :HidLine::getHiddenIntersection()
int	
HidLine::getHiddenIntersection(	LineSegment*	linesegA_p   ,    
				LineSegment*	linesegB_p   ,    
				Intersection2D*	intersection_p ,
				double		epsilon3d    ,    
				double		epsilon2d       )
{
		//----- local variables
	int		status ;
	double		z0, z1 ;
	double		t_A, t_B ;
	double		t_A_3D, t_B_3D ;
	double		X3D_A_eyeZ , X3D_B_eyeZ ;

	Vector2		X2D ;
	Vector2*	A_begin2D_p ;
	Vector2*	A_end2D_p ;
	Vector2*	B_begin2D_p ;
	Vector2*	B_end2D_p ;

	Vector3		X3D_A, X3D_B ;
	Vector3*	A_begin3D_p ;
	Vector3*	A_end3D_p ;
	Vector3*	B_begin3D_p ;
	Vector3*	B_end3D_p ;
	Perspective*	camera_p  ;


		//----- preliminaries
		//----------- data of lineseg A 
	A_begin3D_p	= &(linesegA_p->begin ) ;
	A_end3D_p	= &(linesegA_p->end   ) ;
	A_begin2D_p	= &(linesegA_p->begin2D ) ;
	A_end2D_p	= &(linesegA_p->end2D   ) ;

		//----------- data of lineseg A 
	B_begin3D_p	= &(linesegB_p->begin ) ;
	B_end3D_p	= &(linesegB_p->end   ) ;
	B_begin2D_p	= &(linesegB_p->begin2D ) ;
	B_end2D_p	= &(linesegB_p->end2D   ) ;
		//---------- other data
	camera_p	= hid_p->getCamera();

		//-------------------------------------------------------------//
		//----- TEST 1: depth overlapping test in z_eye direction -----//
		//-------------------------------------------------------------//
	status = depthOverlapTest( linesegA_p, linesegB_p, epsilon3d ) ;

	if        ( status ==  1 ) { // if *linesegA_p is nearer to eye...
		return	NOT_HIDDEN ; // No hidden intersection for linesegA.
	} 

		//------------------------------------------------//
		//----- TEST 2: max-min test for 8 direction -----//
		//------------------------------------------------//
	status	=  maxMinTest( linesegA_p, linesegB_p , epsilon2d ) ;
	if( status == 0 ) { 
		return	NOT_HIDDEN ;  // No hidden intersection for linesegA.
	}


		//-----------------------------------------------------//
		//----- Test 3: line vs polygon independensy test -----//
		//-----------------------------------------------------//
		//----- calc intersection
	status = \
	Tool2D::linesegVsLineseg(	*A_begin2D_p ,	*A_end2D_p , 
					*B_begin2D_p ,	*B_end2D_p , 
					&X2D ,
					&t_A , &t_B , 
					epsilon2d                    );
		//----- no intersection?
	if( status != 1 ) { 
		return	NOT_HIDDEN ; // No hidden intersection for linesegA.
	}


		//-----------------------------------------------------//
		//----- TEST4:  compare X3D_A and X_3D_B               //
		//-----------------------------------------------------//

		//----- calc X3D_A 
		//---------- (1) z-depth of linesegment A
	z0 = camera_p->eyeZ( *A_begin3D_p ) ;
	z1 = camera_p->eyeZ( *A_end3D_p ) ;

		//---------- (2) 3-d lineparameter of intersection 
	t_A_3D = ( z0 * t_A ) / ( z1 * ( 1.0 - t_A ) + z0 * t_A ) ;

		//---------- (3) calc X3D_A and X3D_A_eyeZ
	X3D_A = \
	Tool3D::getPositionOnLine( *A_begin3D_p, *A_end3D_p , t_A_3D );
	X3D_A_eyeZ = camera_p->eyeZ( X3D_A ) ;

		//----- calc X3D_B 
		//---------- (1) z-depth of linesegment B
	z0 = camera_p->eyeZ( *B_begin3D_p ) ;
	z1 = camera_p->eyeZ( *B_end3D_p ) ;

		//---------- (2) 3-d lineparameter of intersection
	t_B_3D = ( z0 * t_B ) / ( z1 * ( 1.0 - t_B ) + z0 * t_B ) ;

		//---------- (3) calc X3D_B and X3D_B_eyeZ
	X3D_B = \
	Tool3D::getPositionOnLine( *B_begin3D_p, *B_end3D_p , t_B_3D );
	X3D_B_eyeZ = camera_p->eyeZ( X3D_B ) ;

		//----- compare z-depth of X3D_A and X3D_B
	if( X3D_A_eyeZ < X3D_B_eyeZ + epsilon3d ) {
		return 	NOT_HIDDEN ;
	} else {
			//----- X3D_A is farther from eye
			//..... Return 2-dim lineparameter of intersection
			//..... on linesegment A and width of linesegment B
		intersection_p->lineParameter	= t_A  ;
		intersection_p->width 	= (linesegB_p->linetype).getWidth();
		return 	HIDDEN ;
	}

} // HidLine::getHiddenIntersection()



	//----- HidLine::setIntersection2DToVisibleLineSegment()
void
HidLine::setIntersection2DToVisibleLineSegment( double epsilon3d, double epsilon2d )
{
		//----- local
	int		i, j ;
	int		status ;
	int		num_hidden_intersection ;
	Intersection2D	X2D ;
	LineSegment	*linesegA_p , *linesegB_p ;
	List<Intersection2D>	list_intersection2D ;

		//----- set Intersection2D's 
		//....  to  each i-th visibleLineSegment
	for( i = 0 ; i < numVisibleLineSegment ; i++ ) {

			//----- initilization
		list_intersection2D.clear() ;
		linesegA_p  = &(visibleLineSegment[i]) ;

			//----- calc Intersection2D's of visibleLineSegment[i] 
			//.....  hidden by visibleLineSegment[ j ] 
		for( j = 0 ; j < numVisibleLineSegment ; j++ ) {
			linesegB_p  = &(visibleLineSegment[j]) ;

			if( i != j ) {

					//----- calc X2D if any
				status = \
				getHiddenIntersection(	linesegA_p   ,    
							linesegB_p   ,    
							&X2D         ,
							epsilon3d    ,    
							epsilon2d       );

					//----- set X2D (if any) to list
				if( HIDDEN == status ) {
					list_intersection2D.push( X2D ) ;			
				}
			}

		} // for(j)

			//----- set intersection2D to visibleLineSegment[i] 
			//.....  from list 
			//.....  ( with sort in order of line parameter)
		num_hidden_intersection = list_intersection2D.getNItem() ;
		if( num_hidden_intersection > 0  ) {
			linesegA_p->setIntersection2D( &list_intersection2D );
		}

	} // for(i)


} // HidLine::setIntersection2DToVisibleLineSegment()



		//----- HidLine::getBoundingDiamond()
int
HidLine::getBoundingDiamond(	double*		xmin ,
				double*		xmax ,
				double*		ymin ,
				double*		ymax ,
				double*		umin ,
				double*		umax ,
				double*		vmin ,
				double*		vmax )
{
		//----- local
	int	status ;
	int	i  ;
	double	x, y, u, v ;

		//----- error checking
	if( numVisibleLineSegment <= 0 ) {
		status = 0 ; // error
		return status ;
	}

		//----- initialization
	(*xmin) = (*xmax) = visibleLineSegment[0].begin2D.x ;
	(*ymin) = (*ymax) = visibleLineSegment[0].begin2D.y ;
	(*umin) = (*umax) = (*ymin) + (*xmin);
	(*vmin) = (*vmax) = (*ymin) - (*xmin);

		//----- for each polygon2d[i]
	for( i = 0; i < numVisibleLineSegment ; i++ ) {

			//----- candidate 1 (beginning position)
		x = visibleLineSegment[i].begin2D.x ;
		y = visibleLineSegment[i].begin2D.y ;
		u = y + x ;
		v = y - x ;

			//----- updation
		if( x < (*xmin) ) { (*xmin) = x ; }
		if( x > (*xmax) ) { (*xmax) = x ; }
		if( y < (*ymin) ) { (*ymin) = y ; }
		if( y > (*ymax) ) { (*ymax) = y ; }
		if( u < (*umin) ) { (*umin) = u ; }
		if( u > (*umax) ) { (*umax) = u ; }
		if( v < (*vmin) ) { (*vmin) = v ; }
		if( v > (*vmax) ) { (*vmax) = v ; }

			//----- candidate 2 (ending position)
		x = visibleLineSegment[i].end2D.x ;
		y = visibleLineSegment[i].end2D.y ;
		u = y + x ;
		v = y - x ;

			//----- updation
		if( x < (*xmin) ) { (*xmin) = x ; }
		if( x > (*xmax) ) { (*xmax) = x ; }
		if( y < (*ymin) ) { (*ymin) = y ; }
		if( y > (*ymax) ) { (*ymax) = y ; }
		if( u < (*umin) ) { (*umin) = u ; }
		if( u > (*umax) ) { (*umax) = u ; }
		if( v < (*vmin) ) { (*vmin) = v ; }
		if( v > (*vmax) ) { (*vmax) = v ; }

	} // i


		//----- return status
	status = 1 ; // normal
	return status ;

} // Hid::getBoundingDiamond()



		//----- push linesegment to vislbleLineSegment
void	HidLine::setVisibleLineSegment( List<LineSegment>& lineseg_list ) 
{

		//----- set visible line segments to array
	delete [] visibleLineSegment ;	visibleLineSegment = NULL ;
	numVisibleLineSegment = lineseg_list.getNItem();
	visibleLineSegment = new LineSegment [numVisibleLineSegment] ; 
	lineseg_list.toArray(	visibleLineSegment, \
				numVisibleLineSegment );
}



		//----- push linesegment to vislbleLineSegment
void	HidLine::moveLineSegmentListToVisibleLineSegmentArray()
{
	setVisibleLineSegment( lineSegmentList ) ;
	lineSegmentList.clear();
}


//----- 
int	HidLine::makeAllLinesVisible( double epsilon3d , double epsilon2d )
{
		//----- clear old data
	numVisibleLineSegment = 0 ;
	delete [] visibleLineSegment ;	visibleLineSegment = NULL ;

		//----- error checking no linesegment ?
	if( numLineSegment == 0 ) { 
		return numVisibleLineSegment ;
	}

		//----- regard lineSegment as visibleLineSegment
		//..... without copying
	numVisibleLineSegment = numLineSegment ;
	visibleLineSegment    = lineSegment    ;

	numLineSegment = 0    ;
	lineSegment    = NULL ;

#if defined DEBUG_HIDLINE
	cerr << " ***** visibleLineSegment = lineSegment[] " << endl;
#endif

	return numVisibleLineSegment ;

} 



