//////////////////////////////////////
///// linesegment.cc  1995/05/15 /////
//////////////////////////////////////

#include "linesegment.h" 

	//----- QuickSort<Polygon3D_p>::compare0()
int  QuickSort<Intersection2D>::compare0(	Intersection2D* a , 
						Intersection2D* b  ) 
{
		//----- compare
	if         ( a->lineParameter > b->lineParameter ) {
		return   1 ;	// *a is nearer to eye than *b
	} else 	if ( a->lineParameter < b->lineParameter ) {
		return   -1 ;	// *b is nearer to eye than *a
	} else {
		return 0 ;
	}

} // QuickSort<Intersection2D>compare0()	


	//----- QuickSort<Polygon3D_p>::compare1()
int  QuickSort<Intersection2D>::compare1(	Intersection2D* a , 
						Intersection2D* b  ) 
{
	return ( a == b ) ; // dummy
}

	//----- LineSegment::LineSegment (void) 
LineSegment::LineSegment() 
{
		//----- boxel
	xmin =  xmax = 0.0 ;
	ymin =  ymax = 0.0 ;
	zmin =  zmax = 0.0 ;
	umin =  umax = 0.0 ;	
	vmin =  vmax = 0.0 ;		

		//----- intersection
	numIntersection2D = 0 ;
	intersection2D    = NULL ;

} // LineSegment::LineSegment(void) 


	//----- LineSegment::LineSegment ( ...) 
LineSegment::LineSegment(	const Vector3& begin_given ,
				const Vector3& end_given   ,
				const LineType& linetype_given ) 
{
		//----- position
	setPosition( begin_given, end_given ) ;

		//----- line type
	setLineType( linetype_given ) ;

		//----- boxel
	xmin =  xmax = 0.0 ;
	ymin =  ymax = 0.0 ;
	zmin =  zmax = 0.0 ;
	umin =  umax = 0.0 ;	
	vmin =  vmax = 0.0 ;		

		//----- intersection
	numIntersection2D = 0 ;
	intersection2D    = NULL ;

} // LineSegment::LineSegment(...)


	//----- LineSegment::setPosition()
void
LineSegment::setPosition(	const Vector3& begin_given ,
				const Vector3& end_given     ) 
{
	begin = begin_given ;
	end   = end_given ;

} // LineSegment::setPosition()


	//----- LineSegment::setLineType()
void
LineSegment::setLineType( const LineType& linetype_given  )
{
	linetype = linetype_given ;
}


	//----- LineSegment::setImage()
int 
LineSegment::setImage(	Perspective*	camera_p  ,
			double		epsilon3d   )
{
		//------ local
	int		status_begin, status_end ;
	Vector3		screen_coord ;
	double		screen_coord_u , screen_coord_v  ;

		//----- screen coordinate of "begin"
	status_begin = camera_p->worldToScreen( begin, screen_coord, epsilon3d ) ;
	screen_coord_u = screen_coord.y + screen_coord.x ;
	screen_coord_v = screen_coord.y - screen_coord.x ;
					
		//----- set begin2D
	begin2D( screen_coord.x, screen_coord.y ) ;

		//-----  initialize boxel
	xmin = xmax = screen_coord.x ;
	ymin = ymax = screen_coord.y ;
	zmin = zmax = screen_coord.z ;
	umin = umax = screen_coord_u ;
	vmin = vmax = screen_coord_v ;


		//----- screen coordinate of "end"
	status_end = camera_p->worldToScreen( end, screen_coord, epsilon3d ) ;
	screen_coord_u = screen_coord.y + screen_coord.x ;
	screen_coord_v = screen_coord.y - screen_coord.x ;

		//----- set end2D
	end2D( screen_coord.x, screen_coord.y ) ;

		//----- update boxel if necessary
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

		//----- return status
	return ( status_begin && status_end ) ;

} // LineSegment::setImage()



	//----- LineSegment::operator = ()
void	
LineSegment::operator = ( const LineSegment& rhs ) 
{
		//----- 3D position
	begin	= rhs.begin ;  
	end	= rhs.end ;

		//----- linetype
	linetype = rhs.linetype ;

		//----- 2D position
	begin2D	= rhs.begin2D ;  
	end2D	= rhs.end2D ;

		//----- boxel
	xmin = rhs.xmin ;	xmax = rhs.xmax ;
	ymin = rhs.ymin ;	ymax = rhs.ymax ;
	zmin = rhs.zmin ;	zmax = rhs.zmax ;
	umin = rhs.umin ;	umax = rhs.umax ;
	vmin = rhs.vmin ;	vmax = rhs.vmax ;

		//----- intersection
	delete [] intersection2D ; // delete old data
	numIntersection2D = rhs.numIntersection2D ;
	if( 0 == numIntersection2D ) {
		intersection2D = NULL ;
	} else {
		intersection2D = new Intersection2D [ numIntersection2D ];
		if( intersection2D == NULL ) {
			cerr << "ERROR (LineSegment::operator = ()) \n";
			exit(1);
		}
		for( int i = 0 ; i < numIntersection2D ; i++ ) {
			intersection2D[i] = (rhs.intersection2D)[i];
		}
	}
} // LineSegment::operator = ()


	//-----Intersection2D::setIntersectionn2D()
void
LineSegment::setIntersection2D( List<Intersection2D>* input_list_p ) 
{
		//----- delete old data
	delete [] intersection2D ;	

		//----- set numIntersection2D
	numIntersection2D	= input_list_p->getNItem();

		//----- set intersection2D
	if( numIntersection2D ==  0 ) {	
		intersection2D	= NULL;
	} else {
		intersection2D		= new Intersection2D [ numIntersection2D ];
		if ( intersection2D == NULL ) {
			cerr << "ERROR (LineSegmetn::setIntersection2D() ) \n";
			exit(1);
		}

		input_list_p->toArray( intersection2D , numIntersection2D );
		this->sortIntersection2D();
	}

} // Intersection2D::setIntersection2D()

	
	//----- LineSegment::sortIntersection2D()
void	LineSegment::sortIntersection2D()
{
		//----- local
	QuickSort<Intersection2D>	qsorter ;

		//----- do quick sort of the pointer array
	qsorter.setMode ( 0 ); // use compare0()
	qsorter.doSort ( intersection2D, 0, (numIntersection2D- 1 ) );
	
} // LineSegment::sortIntersection2D()

	//----- LineSegment::sortIntersection2D()
void	LineSegment::getBoundingDiamond(	double*		xmin_p ,
						double*		xmax_p ,
						double*		ymin_p ,
						double*		ymax_p ,
						double*		umin_p ,
						double*		umax_p ,
						double*		vmin_p ,
						double*		vmax_p  )
{
	*xmin_p = xmin ;	*xmax_p = xmax ;
	*ymin_p = ymin ;	*ymax_p = ymax ;
	*umin_p = umin ;	*umax_p = umax ;
	*vmin_p = vmin ;	*vmax_p = vmax ;
} // LineSegment::sortIntersection2D()

