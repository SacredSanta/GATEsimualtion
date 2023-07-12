///////////////////////////////////////////
//////////  hid2.cc  1996/01/09  //////////
///////////////////////////////////////////

	//-------------------------------//
	//-----  DEBUG macro        -----//
	//-------------------------------//
// #define DEBUG_HID2
// #define DEBUG_PERFORATION_LINE
// #define ABANDON_EXEPTIONAL_FACET

	//-------------------------------//
	//-----  include files      -----//
	//-------------------------------//
#include "hid.h"
#include "parameter.h" 
#include "message.h"
#include "clip_tester.h" 

	//----- message interval
const	int		MESSAGE_INTERVAL = 500 ;

//-------------------------------------------------//
//-----  member functions of class Hid -----//
//-------------------------------------------------//

	//----- Hid::Hid(), constructor 1
Hid::Hid( Perspective*  camera_p_given ) : polygon3DList(), \
					   convexPolygon3DList(), \
					   auxiliaryVertexPosition(),\
					   polygon2DList()
{
		//----- initializa arrays
	numPolygon3D	= 0 ;
	polygon3d	= NULL ; 

	numPolygon2D	= 0 ;
	polygon2d	= NULL ; 

		//----- set camera
	setCamera( camera_p_given ) ;

		//----- set line type for perforation line
	ColorType	color_tmp(1.0,0.0,0.0) ;

#if !defined DEBUG_PERFORATION_LINE
	color_tmp.invisible() ;
#endif

	perforationLineType.setColor ( color_tmp );
	perforationLineType.setStyle ( FR_LINE_ON_OFF_DASH ) ;

		//----- reset polygon counter
	polygon_counter =  0 ;

		//----- bounding diamonds for parts of polygon3d[]
	boundingDiamondInterval = 0 ;	
	boundingDiamondArray = NULL ; 

		//-----
	initializeIncrementalHid();

} // Hid::Hid()



	//----- Hid::clearPolygon()
void
Hid::clearPolygon()
{
		//----- clear polygon lists
	clearPolygon3DList()        ;
	clearConvexPolygon3DList()  ;
	clearAuxiliaryVertexPositionList() ;
	clearPolygon2DList()        ;

		//----- clear polygon arrays
	clearPolygon2DArray() ;
	clearPolygon3DArray() ;

		//----- reset polygon counter
	polygon_counter =  0 ;

		//-----
	clearBoundingDiamondArray(); 


} // Hid::clearPolygon()



	//----- Hid::clear()
void
Hid::clear()
{
		//----- 
	clearPolygon();

		//----- reset camera info
	camera_p     = NULL ;
	eyePosition  = Vector3();

		//----- 
	initializeIncrementalHid();


} // Hid::clear()


	//----- Hid::initializeIncrementalHid()
void
Hid::initializeIncrementalHid()
{
	beginning_object_label_per_session     = 0 ;
	num_non_transparent_facet_per_session  = 0 ;
}


		//----- Hid::addItem_Polygon3DList()
int	Hid::addItem_Polygon3DList( Polygon3D& poly3d , int clipping_mode )
{ 
		//----- local vairables
	int			status ; 
	int			flag_is_clipped_out ;
	static	ClipTester	clip_tester ;

		//----- clipped out or not
	flag_is_clipped_out = \
	clip_tester.isBox2DClippedOut(	Box2D (poly3d.xmin, poly3d.ymin, poly3d.xmax,poly3d.ymax ),  
					clipping_mode                                              ) ;

		//----- add or abandon this polygon 
	if        ( flag_is_clipped_out == CLIPPED_OUT )
	{
		status = 0 ; // not added
	} else {
			//----- add polygon to Hid class 
		poly3d.setLabel( polygon_counter++ );
		status = polygon3DList.addItem( poly3d ); 
	}

		//----- return status 
	return status ;

} // Hid::addItem_Polygon3DList( Polygon3D& poly3d ) 


		//----- manipulation of polygon3DList
int	Hid::getItem_Polygon3DList    ( Polygon3D& poly3d ) 
{ 
	return polygon3DList.getItem( poly3d ); 
}


		//----- manipulation of polygon3DList
int	Hid::getNItem_Polygon3DList () 
{ 
	return polygon3DList.getNItem(); 
}


		//----- manipulation of polygon3DList
void	Hid::toTail_Polygon3DList   ()
{ 
	polygon3DList.toTail(); 
}


		//----- manipulation of polygon3DList
int	Hid::upward_Polygon3DList    ()
{ 
	return polygon3DList.upward(); 
}


	//----- Hid::maxMinTest(), 1
	//..... max min test for 8 direction
	//.....  Return
	//.....		0:  if *a is independent of given polygon
	//.....		    on screen.
	//.....			--> *a is never hidden by polygon
	//.....		1:  else
	//.....			--> *a may be hidden by polygon
int	Hid::maxMinTest( Polygon3D* a , Polygon3D* b  , double epsilon2d )
{
		//----- local variables
	int	status ;

		//----- max-min test
	if(		a->xmax < b->xmin + epsilon2d ||\
			a->xmin > b->xmax - epsilon2d ||\
			a->ymax < b->ymin + epsilon2d ||\
			a->ymin > b->ymax - epsilon2d ||\
			a->umax < b->umin + epsilon2d ||\
			a->umin > b->umax - epsilon2d ||\
			a->vmax < b->vmin + epsilon2d ||\
			a->vmin > b->vmax - epsilon2d   ) {
		status = 0 ;	// independent

	} else {
		status = 1 ;	// else or unknown
	}

		//----- RETURN
	return status ;

} //  Hid::maxMinTest()


	//----- Hid::maxMinTest()
	//..... max min test for 8 direction
	//.....  Return
	//.....		0:  if *(*a) is independent of given polygon
	//.....		    on screen.
	//.....			--> *(*a) is never hidden by polygon
	//.....		1:  else
	//.....			--> *(*a) may be hidden by polygon
int	Hid::maxMinTest( Polygon3D_p* a , Polygon3D_p* b  , double epsilon )
{
		//----- local variables
	int	status ;

		//----- max-min test
	if(		(*a)->xmax < (*b)->xmin + epsilon ||\
			(*a)->xmin > (*b)->xmax - epsilon ||\
			(*a)->ymax < (*b)->ymin + epsilon ||\
			(*a)->ymin > (*b)->ymax - epsilon ||\
			(*a)->umax < (*b)->umin + epsilon ||\
			(*a)->umin > (*b)->umax - epsilon ||\
			(*a)->vmax < (*b)->vmin + epsilon ||\
			(*a)->vmin > (*b)->vmax - epsilon   ) {
		status = 0 ;	// independent

	} else {
		status = 1 ;	// else or unknown
	}

		//----- RETURN
	return status ;

} //  Hid::maxMinTest()


	//----- Polygon3D::depthOverlapTest()
	//.....  Depth overlapping test
	//.....  Return
	//.....		1:  if (*a)->zmax < (*b)->zmin.
	//.....		-1: if (*b)->zmax < (*a)->zmin.
	//.....		0: else (overlaps in z-direction)
int	Hid::depthOverlapTest( Polygon3D_p* a , Polygon3D_p* b  , double epsilon3d )
{
		//----- local variables
	int	status ; // return value

		//----- check if overlapping exists in z_eye direction
	if        ( (*a)->zmax    < (*b)->zmin - epsilon3d ) { 
		status = 1 ;	// **a is nearer to eye
				// **a > **b
	} else if ( (*b)->zmax    < (*a)->zmin - epsilon3d ) { 
		status = -1 ;	// **b is nearer to eye
				// **a < **b
	} else {
		status = 0 ;	// overlap in eye_z-direction 
				// **a = **b
	}

		//----- return status
	return status ;

} // Hid::depthOverlapTest()



	//----- Hid::includedPolygonVsPlaneTest()
int
Hid::includedPolygonVsPlaneTest( Vector3* vertex3d_a,  Vector2* vertex2d_a , 
				 const Vector3& normal_a , int num_vertex_a , 
				 const Vector3& normal_b, const Vector3& P_on_b , double epsilon3d )
{
		//----- local variables
	int	i ;
	int	i_plus ;
	int	side = 0 ;
	int	retValue ;
	double	dummy ;
	Vector2	center2d_a ;
	Vector3	center3d_a ;


		//-------------------------------------------------//
		//----- Part 1investigate center of polygon a -----//
		//-------------------------------------------------//

		//----- get center of polygon
	center2d_a = Tool2D::getRoughPolygonCenter( vertex2d_a , num_vertex_a ) ;

		//----- calc return value and return
	if( point2DToPoint3D( center2d_a , normal_a , vertex3d_a[0], &center3d_a, epsilon3d ) ) 
	{ 
		side = Tool3D::planeVsPoint(	normal_b, P_on_b , 
						center3d_a , &dummy , epsilon3d );
		if( side == Tool3D::POSITIVE_SIDE || side == Tool3D::NEGATIVE_SIDE ) { 
			retValue = side ;
		} else {
			retValue = Tool3D::UNKNOWN ;
		}
			//----- return 
		return	retValue ;

	} else {
		; // go on to part 2
	}



		//---------------------------------------------------------//
		//----- Part 2: investigate each vertex  of polygon a -----//
		//---------------------------------------------------------//

		//----- investigate vertices of  polygon a
	for( i = 0 ; i < num_vertex_a ; i++ ) {

		i_plus = i + 1 ;	if( i_plus == num_vertex_a ) { i_plus = 0 ; }

		side = Tool3D::planeVsPoint(	normal_b, P_on_b , 
						vertex3d_a[ i_plus ] , &dummy , epsilon3d );

		if( side == Tool3D::POSITIVE_SIDE || side == Tool3D::NEGATIVE_SIDE ) { 
			break ;
		} // else: side is Tool3D::ON_PLANE ( = 0 )
	}

		//----- calc return value
	if( side == Tool3D::POSITIVE_SIDE || side == Tool3D::NEGATIVE_SIDE ) { 
		retValue = side ;
	} else {
		retValue = Tool3D::UNKNOWN ;
	}

		//----- return for part 2
	return retValue ;

} //  Hid::includedPolygonVsPlaneTest()



	//----- Hid::crossedPolygonVsPlaneTest()
int
Hid::crossedPolygonVsPlaneTest(	Vector3*       vertex3d_a,  Vector2*       vertex2d_a , 
				const Vector3& normal_a ,   int            num_vertex_a , 
				const Vector3& normal_b ,   const Vector3& P_on_b, 
				Vector2*       vertex2d_b,  int            num_vertex_b, 
				double         epsilon3d ,  double         epsilon )
{
		//----- local variables
	int	i ;
	int	i_plus ;
	int	side ;
	int	flag_included ;
	int	retValue = 0 ;
	double	dummy ;
	Vector2 mid_edge_2d ; // mid edge point
	Vector3 mid_edge_3d ; // mid edge point
	Vector2	center2d_a ;
	Vector3	center3d_a ;


		//-------------------------------------------------//
		//----- Part 1: investigate center of polygon a -----//
		//-------------------------------------------------//

		//----- initialization
	side = Tool3D::UNKNOWN ; 

		//----- get center2d_a
	center2d_a      = Tool2D::getRoughPolygonCenter( vertex2d_a , num_vertex_a ) ;

		//----- Is center2d_a included by polygon b?
	flag_included = Tool2D::isPointInsidePolygon( center2d_a, 
							vertex2d_b, num_vertex_b, 
							epsilon );

		//----- calc and investigate position of center3d_a relative to plane b
	if(	flag_included == 1 && \
		point2DToPoint3D( center2d_a , normal_a , vertex3d_a[0], &center3d_a, epsilon3d ) == 1 )  
	{
			//----- calc return value
		side = Tool3D::planeVsPoint(	normal_b, P_on_b , 
						center3d_a , &dummy , epsilon3d );
		if( side == Tool3D::POSITIVE_SIDE || side == Tool3D::NEGATIVE_SIDE ) { 
			; // do nothing 
		} else {
			side = Tool3D::UNKNOWN ;
		}
	} 
	if( side != Tool3D::UNKNOWN ) { return side ; }		


		//------------------------------------------------------------//
		//----- Part 2: investigate mid edge points of polygon a -----//
		//------------------------------------------------------------//

		//----- investigate vertices of polygon a
	side = Tool3D::UNKNOWN ; // initialize
	for( i = 0 ; i < num_vertex_a ; i++ ) {

		i_plus = i + 1 ;	if( i_plus == num_vertex_a ) { i_plus = 0 ; }
		mid_edge_2d = 0.5 * ( vertex2d_a[ i ] +  vertex2d_a[ i_plus ] ); 

		flag_included \
		= Tool2D::isPointInsidePolygon( mid_edge_2d , vertex2d_b, num_vertex_b,  epsilon );

		if( flag_included == 1 ) {
			mid_edge_3d = 0.5 * ( vertex3d_a[ i ] +  vertex3d_a[ i_plus ] ); 
			side = Tool3D::planeVsPoint(	normal_b ,  P_on_b  ,
							mid_edge_3d, &dummy , epsilon3d );

			if( side == Tool3D::POSITIVE_SIDE || side == Tool3D::NEGATIVE_SIDE ) { 
				break ; // decided!

			} else { 
				side = Tool3D::UNKNOWN ; 
				continue ;
			}

		} // if( flag...)

	} // for

	if( side != Tool3D::UNKNOWN ) { return side ; }		


		//---------------------------------------------------------//
		//----- Part 3: investigate each vertex  of polygon a -----//
		//---------------------------------------------------------//

		//----- investigate vertices of polygon a
	side = Tool3D::UNKNOWN ; // initialize
	for( i = 0 ; i < num_vertex_a ; i++ ) {

		i_plus = i + 1 ;	if( i_plus == num_vertex_a ) { i_plus = 0 ; }

		flag_included \
		= Tool2D::isPointInsidePolygon( vertex2d_a[ i_plus ], 
						vertex2d_b, num_vertex_b, 
						epsilon );

		if( flag_included == 1 ) {

			side = Tool3D::planeVsPoint(	normal_b ,  P_on_b  ,
							vertex3d_a[ i_plus ] , &dummy , epsilon3d );

			if( side == Tool3D::POSITIVE_SIDE || side == Tool3D::NEGATIVE_SIDE ) { 
				break ; // decided!

			} else { 
				side = Tool3D::UNKNOWN ; 
				continue ;
			}

		} // if( flag...)

	} // for

		//----- return value
	return  retValue ;

} //  Hid::crossedPolygonVsPlaneTest()



	//----- Hid::polygonPolygonTest()
int
Hid::polygonPolygonTest(	Vector3* vertex3d_a, Vector2* vertex2d_a, int num_vertex_a, 
				Polygon3D* b , 
				int max_intersection, double epsilon3d , double epsilon )
{
		//----- local variables
	int		i ;
	int		status ;	// return value
	int		status_ab, status_ba ;
	int		num_intersection ;
	int		*edgeA, *edgeB ;
	int		flag_intersected ;
	double		dummy ;
	double		*tA, *tB ;
	Vector2		*X ;		// intersection on screen
	Vector3		XA , XB ;	// intersection in plane a and b
	Vector3		normal_a , normal_b; // normal vectors of planes
	Vector3		P_on_a   , P_on_b ;  // points inside planes


		//----- dynamical memories
	X	= new Vector2 [ max_intersection ] ;
	edgeA	= new int     [ max_intersection ] ;
	edgeB	= new int     [ max_intersection ] ;
	tA	= new double  [ max_intersection ] ;
	tB	= new double  [ max_intersection ] ;

		//----- error checking		
	if( !X || !edgeA || !edgeB || !tA || !tB ) {
		cerr << "ERROR (Hid::polygonPolygonTest()) 1\n";
		exit(1);
	}

		//----- planes of polygons a and b
	normal_a = Tool3D::getNormalVector( vertex3d_a , num_vertex_a );
	P_on_a	 = vertex3d_a[0];
	normal_b = b->normalVector ;
	P_on_b   = *( (b->vertex)[0] );

		//----- search intersection ( <= max_intersection )
	num_intersection \
	= Tool2D::getPolygonIntersection(	vertex2d_a,  num_vertex_a ,
						b->vertex2D, b->numVertex ,
						X, 
						edgeA, edgeB, 
						tA, tB, 
						max_intersection, epsilon );

		//----- error checking: consistensy of num_intersection
	if( num_intersection > max_intersection || num_intersection < 0 ) {
		cerr << "ERROR (Hid::polygonPolygonTest()) 2 \n";
		exit(1);		
	}
			
		//----- no intersection?
	if( num_intersection == 0 ) {
		delete [] X	;
		delete [] edgeA	;
		delete [] edgeB	;
		delete [] tA	;
		delete [] tB	;
		return -2 ;
	} 


		//----- plane a Vs polygon b test
	status =  0 ; // initialize to "intersection is on same plane"
	for( i = 0 ; i < num_intersection ; i++ ) {
			//---------- get XB ( a point on plane b, whose image on screen is X[i] )
		flag_intersected = point2DToPoint3D( X[i] , normal_b , P_on_b, &XB, epsilon3d );
		if( flag_intersected ) {

				//---------- side of point XB relative to plane a
			status_ab = Tool3D::planeVsPoint( normal_a , P_on_a, XB, &dummy, epsilon3d );

				//---------- calc status_ab
			if        ( status_ab == -1 ) {
					//----- polygon b is in negative side of plane a
				status = 1 ;	break ;	// decided!
			} else if ( status_ab == +1 ) {
					//----- polygon b is in positive side of plane a
				status = -1 ;	break ; // decided!
			} else {
				status = 0 ;		// not decided
			}
		} // if

	} // for(i)


		//----- plane b Vs polygon a test
	if( status == 0 ) { // if not decided yet ...

		for( i = 0 ; i < num_intersection ; i++ ) {
				//---------- get XA ( a point on plane a, whose image on screen is X[i] )
			flag_intersected = point2DToPoint3D( X[i] , normal_a , P_on_a, &XA, epsilon3d );
	
			if( flag_intersected ) {
					//---------- side of point XA relative to plane b
				status_ba = Tool3D::planeVsPoint( normal_b , P_on_b, XA, &dummy, epsilon3d );

					//---------- calc status_ba
				if        ( status_ba == +1 ) {
						//----- polygon a is in positive side of plane b
					status = 1 ;	break ;
				} else if ( status_ba == -1 ) {
						//----- polygon a is in negative side of plane b
					status = -1 ;	break ;
				} else {
					status = 0 ; // not decided
				}

			} // if

		} // for

	} // if( status == 0 )


		//----- delete dynamical memories
	delete [] X	;
	delete [] edgeA	;
	delete [] edgeB	;
	delete [] tA	;
	delete [] tB	;

		//----- return status :
		//.....  -1	if A is further from eye
		//.....   1	if B is further from eye
		//.....   0	no intersection or undecided
	return status ;

} // Hid::polygonPolygonTest()



	//----- set camera and eye position
void	Hid::setCamera( Perspective* camera_p_given ) 
{	
		//----- camera
	camera_p = camera_p_given; 

		//----- eye position 
	if( camera_p != NULL ) {
		eyePosition = camera_p->getCameraPosition() ;
	}
} // Hid::setCamera()


	//----- Hid::getPolygon3D()
Polygon3D&
Hid::getPolygon3D( int label ) 
{
		//----- check if given "label" is in the right range
		//..... i.e. 0, 1, 2, ... numPolygon3D -1
	if( label >= numPolygon3D || label < 0 ) {
		cerr << "ERROR: (Hid::getPolygon3D() ): ";
		cerr <<	"Irregal item label. \n";
		exit(1) ;
	}

		//----- return label-th polygon
	return polygon3d[ label ] ;

} // Hid::getPolygon3D()



	//----- Hid::getPolygon2D()
Polygon2D&
Hid::getPolygon2D ( int label )
{
		//----- check if given "label" is in the right range
		//..... i.e. 0, 1, 2, ... numPolygon2D -1
	if( label >= numPolygon2D || label < 0 ) {
		cerr << "ERROR: (Hid::getPolygon2D() ): ";
		cerr <<	"Irregal item label. \n";
		exit(1) ;
	}

		//----- return label-th polygon
	return polygon2d[ label ] ;

} // Hid::getPolygon2D ()


	//----- Hid::operator [ ]
Polygon2D&
Hid::operator [] ( int label )
{
	return this->getPolygon2D( label );
}


	//----- Hid::point2DToPoint3D ()
int
Hid::point2DToPoint3D ( const Vector2&	point2d, 
			const Vector3&	normal ,
			const Vector3&	P_on ,
			Vector3*	point3d_p ,
			double		epsilon3d )
{
		//----- local  variables
	int		status ;
	Vector3		point_image_3d_screen, point_image_3d ;

		//----- 3D screen coordinate of point2d on screen
	point_image_3d_screen( point2d.x, point2d.y, camera_p->getFocalDistance() );

		//----- world coordinate of point2d on screen
	camera_p->screenToWorld( point_image_3d_screen, point_image_3d );

		//----- Intersection between plane( normal, P_on ) and
		//...... line(eyePosition, point_image_3d).
		//...... Intersection point is returned to *point3d_p.
	status = Tool3D::getPlaneLineIntersection( normal, P_on , 
						   eyePosition, 
						   point_image_3d, 
						   point3d_p , 
						   epsilon3d );

		//----- return status (1: normal, 0 error )
	return status ;

} // Hid::point2DToPoint3D (): single point



	//----- Hid::extractVisiblePart()
void
Hid::extractVisiblePart(	int beginning_object_label , 
				int ending_object_label , 
				double epsilon3d, double epsilon )
{
		//----- local variables
	int			i,j, j_initial ; // loop variables 
	int			num_output ;
	int			flag_hidden ;
	int			isHidden_status ;
	int			polygonVsPolygon_status ;
	Polygon2D		sub_polygon ;
	List<int>		list_label ;

		//----- get visible part of polygon in each polygon3d[i]
	for( i = 0 ; i < numPolygon3D ; i++ ) {

			//----- local to this block
		int	object_label_i	= polygon3d[i].objectLabel ;
		int	facet_label_i	= polygon3d[i].facetLabel  ;
		int	convexity_i     = polygon3d[i].getConvexity();
		int	div_val ;

			//----- error checking
		if (	object_label_i < 0 || facet_label_i < 0 ) {
			cerr << "ERROR (Hid::extractVisiblePart()) 1";
			exit(1) ;
		}

			//----- message
		div_val = i / MESSAGE_INTERVAL ;
		if( (div_val * MESSAGE_INTERVAL) == i && i > 0 ) {
			cerr << "  ****** upto OBJECT " << ending_object_label << ": " << i << " polygons are processed" ;
			if ( numPolygon3D > 0 ) { 
				cerr << "  ( " << ( 100 * i / numPolygon3D ) << " % )" ;
			}
			cerr << endl;
		}

			//----- set j_initial
		list_label.clear() ;
		j_initial = 0 ;
		list_label.push( j_initial ) ;


////////////////////////////////////////////
SUBPOLYGON_INVESTIGATION:; // GOTO label 
////////////////////////////////////////////

			//----- for each sub_polygon of polygon3d[i] ...
		while( polygon3d[i].subPolygonList.pop( sub_polygon ) ) {
				//----- pop j_initial
			if( list_label.getNItem() == 0 ) {
				cerr << "ERROR (Hid::extractVisiblePart() ) 2:\n";
				exit(1);
			}
			list_label.pop( j_initial ) ;

				//----- check if poped sub_polygon
				//..... is a valid one
			sub_polygon.removeZeroLengthEdge( epsilon );
			if( sub_polygon.numVertex < 3 ) {
				continue ;	// Abondon this sub_polygon
						// and pop next one.
			}

				//----- see if polygon in polygon3d[j] hides "sub_polygon" 
			flag_hidden = NOT_HIDDEN ; // initialize to "not hidden"
			for( j = j_initial ; j < numPolygon3D ; j++ ) {

					//----- local to this block
				int	object_label_j	= polygon3d[j].objectLabel ;
				int	facet_label_j	= polygon3d[j].facetLabel  ;
				int	convexity_j     = polygon3d[j].getConvexity();

					//----- error checking
				if (	object_label_j < 0 || facet_label_j < 0 ) {
					cerr << "ERROR (Hid::extractVisiblePart()) 3";
					cerr << "  object_label = " << object_label_j << endl;
					cerr << "  facet_label  = " << facet_label_j << endl;
					exit(1) ;
				}

					//----- Test 0: skip comparing polygons in a convex object
				if(	object_label_i == object_label_j    && \
					convexity_i                         && \
					convexity_j                                )
				{
					flag_hidden = NOT_HIDDEN ;
					continue ; // to next j
				}

					//----- Test 1: skip comparing the sampe polygons
				if( i == j ) { 
					flag_hidden = NOT_HIDDEN ;
					continue ; // to next j
				}
					
					//----- Test 2: skip comparing polygons already processed
				if (	object_label_i < beginning_object_label &&
					object_label_j < beginning_object_label   )
				{
					flag_hidden = NOT_HIDDEN ;
					continue ; // to next j
				}

					//----- Test 3: skip comparing polygons from the same facet
				if(	object_label_i == object_label_j && 
					facet_label_i  == facet_label_j      )	
				{
					flag_hidden = NOT_HIDDEN ;
					continue ; // to next j
				}


					//-----  isHidden_status becomes
					//.....  
					//.....  		  NOT_HIDDEN    = 0	if  P is not hidden by Q
					//.....  		  NOT_HIDDEN2  = -1	if  P is not hidden by Q,
					//.....	                                        and P.zmax < Q.zmin
					//.....			  HIDDEN = 1	if  P may be hidden by Q
				isHidden_status	 = isHidden(	&sub_polygon , 
								&polygon3d[i], &polygon3d[j], 
								&polygonVsPolygon_status , epsilon3d, epsilon );

				if ( isHidden_status == NOT_HIDDEN  ) { 
					flag_hidden = NOT_HIDDEN ; 
					continue ;  // to next j 
				}
				else if  ( isHidden_status == NOT_HIDDEN2 ) { 
					flag_hidden = NOT_HIDDEN ; 
					break ; 
					// continue ;  // to next j 
				}


				//----- The latter part is executed only if isHidden_satus == HIDDEN


					//----- updation or exit of for(j)-loop
				if( polygonVsPolygon_status == Tool2D::INDEPENDENT ) { 
						//----- independent: not hidden
					flag_hidden = NOT_HIDDEN ;
					continue ; // to next j
				} else if (	polygonVsPolygon_status == Tool2D::INCLUDED ||  
						polygonVsPolygon_status == Tool2D::EQUAL ) 
				{
						// included or coincide: totally hidden
					flag_hidden = HIDDEN ;
					break ; // --> exit from for(j)-loop:
					        //     Abandon this sub_polygon
					        //     and pop next one 
						//     from polygon3d[i].subPolygonList.
				} else if (	polygonVsPolygon_status == Tool2D::INCLUDE || 
						polygonVsPolygon_status == Tool2D::CROSS ) 
				{
						//----- set flag
					flag_hidden = HIDDEN ;

						//----- devide visible part of sub_polygon into
						//.....  convex smaller polygons and push them  
						//.....  to polygon3d[i].subPolygonList.
					num_output = this->hide_A(	sub_polygon, 
									Polygon2D(	polygon3d[j].vertex2D   ,
											polygon3d[j].edgeType   ,
											polygon3d[j].numVertex  , 
											&(polygon3d[j].color)   , 
											-1, NULL  ) , 
									j , 
									&(polygon3d[i].subPolygonList), 
									&list_label, 
									epsilon ); 

					
						//-----error checking
					if( num_output == 0 ) {
							//----- totally hidden
						flag_hidden = HIDDEN ; 
						break;	// Exit from j-loop, 
							// and abandon this sub polygon.
					} else {
							//----- Items added to polygon3d[i].subPolygonList.
							//..... Investigste it again from the beginning  
						///////////////////////////////
						goto SUBPOLYGON_INVESTIGATION ;
						///////////////////////////////
					}


				} else {  
					cerr << "ERROR (Hid::extractVisiblePart()) 4";
					cerr << "polygonVsPolygon_status = " << polygonVsPolygon_status << endl;
					cerr << endl ;
					exit(1) ;
				} // if--else

			} // for(j)

				//----- if sub_polygon is not hidden by any other polygon 
				//.....  at all, push it to output list 
				//.....  (this->polygon2DList).
				//.....  Else (see previous comment), abanon it.
			if ( flag_hidden == NOT_HIDDEN ) { 
				polygon2DList.push( sub_polygon ) ; 
			}

		} // while (sub_polygon)
	} // for(i)

} // Hid::extractVisiblePart()


	//----- Hid::doHiddenPartElimination()
void
Hid::doHiddenPartElimination(	int beginning_object_label, 
				int ending_object_label, 
				double epsilon3d , double epsilon )
{
		//----- local variables
	int		i;
	int		flag_2DTo3D_success     =   1 ;
	Polygon2D	polygon2d_tmp ;

		//----- Note
		//.....  It is assumed that facets of the objects with given label
		//.....  are already passed to Polygon3DList

		//----- Preparation: add remaining polygon to polygon3DList
	while( polygon2DList.pop( polygon2d_tmp ) ) {

			//----- local to this block
		Polygon3D	polygon3d_tmp ;

			//----- calc polygon3d_tmp
		flag_2DTo3D_success = \
		polygon3d_tmp.from2DTo3D( polygon2d_tmp , this , epsilon3d );
		polygon3d_tmp.setConvexity(1) ; 

			//----- push polygon3d to polygon3DList
		if( flag_2DTo3D_success ) {
			polygon3DList.push( polygon3d_tmp );
		} else {
#if defined DEBUG_HID2
			cerr << "WARNING (Hid::doHiddenPartElimination() 1" << endl;
#endif
		}
	} // while

		//----- Error checking: no Polygon3D exist in list?
	if( polygon3DList.getNItem() <= 0 ) {
		return ;
	}

		//----- STEP 1: Convexation
		//..... Data is moved from polygon3DList to convexPolygon3DList.
		//..... The polygon3DList is cleared.
		//----- STEP 2: Set polygon3d to array
		//..... Data is moved from convexPolygon3DList to polygon3d[]
		//..... The convexPolygon3DListThe is cleared.
		//.....  Old data in polygon3d[] is abandoned.
		//----- STEP 3: Set subPolygonList to each Polygon3D item
	movePolygon3DListToArray( epsilon3d , epsilon );


		//----- STEP 4: Extract visible parts of polygon stored in polygon3d[], 
		//.....         and add them to polygon2DList.
		//.....         The polygon3d[] is not cleared, 
		//.....         since it keeps some data referenced by Polygon2D in polygon2D list.
#if defined DISPLAY_LOG
	cerr << "\n";
//	cerr << "  *** STEP 4: Calculating visible parts of polygons ... " << endl;
	cerr << "------------------------------------------\n";
	cerr << " Removing invisible parts of polygons ... " << endl;
	cerr << "------------------------------------------\n";
#endif

	extractVisiblePart( beginning_object_label, ending_object_label, epsilon3d , epsilon ); 

		//----- step 5: Copy vertex position data from Object to Polygon3D
	for( i = 0 ; i < numPolygon3D ; i++ ) {
		polygon3d[i].setVertex3D();
	}		

} // Hid::doHiddenPartElimination()


	//----- Hid::movePolygon2DListToArray()
int
Hid::movePolygon2DListToArray(void)
{
		//-----  clear array (initialization)
	clearPolygon2DArray();

		//----- allocate memory
	numPolygon2D = this->polygon2DList.getNItem() ;
	polygon2d = new Polygon2D [ numPolygon2D ];
	if( polygon2d == NULL ) {
		cerr << "ERROR (Hid::movePolygon2DListToArray ) 1\n";
		exit(1);		
	}

		//----- set polygon2D to  Array
	this->polygon2DList.toArray( polygon2d, numPolygon2D ) ;


		//----- 
	if( getenv( "DAWN_DATA_ACCUMULATION" ) != NULL ) {
		if( !strcmp( getenv( "DAWN_DATA_ACCUMULATION" ),"0" ) ) 
		{ 
				//----- clear polygon2DList
			this->polygon2DList.clear(); // clear unnecessary memory
		} 

	} else { // No environmental variable
			//----- clear polygon2DList
		this->polygon2DList.clear(); // clear unnecessary memory
	}

		//----- return
	return  	numPolygon2D ;

} // Hid::movePolygon2DListToArray(void)


	//----- Hid::isHidden()
int  Hid::isHidden(	Polygon2D* sub_a , Polygon3D_p a , Polygon3D_p b , int* relation_2d, 
			double  epsilon3d, double epsilon )
{
		//----- local variables
	int		i ;
	int		status  ;
	int		status2 ;
	int		retValue = 0;
	int		num_vertex_sub_a ;
	int		num_vertex_b ;

	int		object_label_a = a->objectLabel ;
	int		object_label_b = b->objectLabel ;

	Vector3*	vertex3d_sub_a ;
	Vector3*	vertex3d_b ;



///////////////////////////////////////
/////  PART 1:  Preliminary Tests /////
///////////////////////////////////////

		//--------------------------//
		//----- TEST 0: a = b? -----//
		//--------------------------//
	if( a == b ) {
		*relation_2d = Tool2D::EQUAL ;
		return NOT_HIDDEN ;
	}


////////////////////////////////////////
/////  PART 2:  2-dimensional Test /////
////////////////////////////////////////

		//-------------------------------------------------------------//
		//----- TEST 1: depth overlapping test in z_eye direction -----//
		//-------------------------------------------------------------//
	status = Hid::depthOverlapTest( &a, &b, epsilon3d ) ;
	if        ( status ==  1 ) { // if a is nearer to eye...
		*relation_2d = Tool2D::UNKNOWN ;
		return  NOT_HIDDEN2  ; 
	} 

		//------------------------------------------------//
		//----- TEST 2: max-min test for 8 direction -----//
		//------------------------------------------------//
	status	=  Hid::maxMinTest( &a, &b , epsilon ) ;
	if( status == Hid::INDEPENDENT ) { 
		*relation_2d = Tool2D::INDEPENDENT ;
		return NOT_HIDDEN ; 
	}
		

		//--------------------------------------------------//
		//----- TEST 3A: b is a neiboring facet of a ? -----//
		//--------------------------------------------------//
	status  = b->isNeighborOf( *a ) ;
	status2 = a->isNeighborOf( *b ) ;

	if( status && status2 ) {
		*relation_2d = Tool2D::INDEPENDENT;
		return NOT_HIDDEN ;
	}


		//----------------------------------------------------------------------------------//
		//----- TEST 3B: exact independency test between sub_polygon of a and polygon b -----//
		//----------------------------------------------------------------------------------//
	status = Tool2D::polygonVsPolygon( sub_a->vertex2D, sub_a->numVertex, b->vertex2D, b->numVertex, epsilon );
	*relation_2d = status ;
	if( status == Tool2D::INDEPENDENT ) {
			//----- independent
		return NOT_HIDDEN ;
	}


////////////////////////////////////////
/////  PART 3:  3-dimensional Test /////
////////////////////////////////////////

		//----------------------------------------------//
		//----- Make data used in 3 dim test below -----//
		//----------------------------------------------//

		//----- make 3D vertex data for sub_polygon_a
	num_vertex_sub_a = sub_a->numVertex ;
	vertex3d_sub_a	 = new Vector3 [ num_vertex_sub_a ] ;
	if( vertex3d_sub_a == NULL ) {
		cerr << "ERROR ( Hid::isHidden() ) 1\n";
		exit(1);
	} 

	for( i = 0 ; i < num_vertex_sub_a ; i++ ) {
		status = point2DToPoint3D ( (sub_a->vertex2D)[i], 
				   a->normalVector, *((a->vertex)[0])   ,
				   &(vertex3d_sub_a[i]) , epsilon3d        );

			//----- normal_a is perpendicular to eye line: 
			//.....  abandon this sub polygon
		if( status == 0 ) {
#if defined DEBUG_HID2
			cerr << "WARNING ( Hid::isHidden() ) 2A\n";
#endif
			*relation_2d = Tool2D::INCLUDED ;
			delete [] vertex3d_sub_a ;	

			retValue = HIDDEN  ; 
			return retValue ;
		}
	}

		//----- make 3D vertex data for polygon_b
	num_vertex_b = b->numVertex ;
	vertex3d_b  = new Vector3 [ num_vertex_b ] ;
	if( vertex3d_b == NULL ) {
		cerr << "ERROR ( Hid::isHidden() ) 2\n";
		exit(1);
	} 
	for( i = 0 ; i< num_vertex_b ; i++) {
		vertex3d_b[i] = *( (b->vertex)[i] );
	}

		//-----------------------------------------------------------//
		//----- TEST 4A : included polygon vs plane test (a, b) -----//
		//-----------------------------------------------------------//
		// Note: If a is included in b, and they are on the same plane,
		//       it is regarded that a is "not hidden" by b.
		//       It makes the included a visible.

	if( *relation_2d == Tool2D::INCLUDED || *relation_2d == Tool2D::EQUAL ) {
		status = Hid::includedPolygonVsPlaneTest(	vertex3d_sub_a, sub_a->vertex2D , 
								a->normalVector , num_vertex_sub_a , 
								b->normalVector , *((b->vertex)[0]), 
								epsilon3d );
		if        ( status == Tool3D::POSITIVE_SIDE ) {
				//----- included subpolygon is in positive side of plane b
			retValue = NOT_HIDDEN  ; 
		} else if ( status == Tool3D::NEGATIVE_SIDE ) {
				//----- included subpolygon is in negative side of plane b
			retValue = HIDDEN  ; 
		} else {
				//----- included subpolygon is just on plane b
//			retValue = HIDDEN      ;  
			retValue = NOT_HIDDEN  ;  

// AAAAAAAAAAAAAAAA
			if ( a->objectLabel < b->objectLabel ) {
				retValue = HIDDEN      ;  				
			}
// BBBBBBBBBBBBBBBBBB

		}

		delete [] vertex3d_sub_a ;	delete [] vertex3d_b ;
		return retValue ;
	}

		//-------------------------------------------------------------//
		//----- TEST 4B : included polygon vs plane test ( b, a ) -----//
		//-------------------------------------------------------------//
		// Note: If b is included in a, and they are on the same plane,
		//       it is regarded that b hides a.
		//       It makes the included b visible.

	if( *relation_2d == Tool2D::INCLUDE ) {
		status = Hid::includedPolygonVsPlaneTest(	vertex3d_b, b->vertex2D, 
								b->normalVector , num_vertex_b , 
								a->normalVector, vertex3d_sub_a[0], 
								epsilon3d );

		if        ( status == Tool3D::NEGATIVE_SIDE ) {
				//----- included polygon b is in negative side of plane a
			retValue = NOT_HIDDEN  ; 
		} else if ( status == Tool3D::POSITIVE_SIDE ) {
				//----- included polygon b is in negative side of plane a
			retValue = HIDDEN  ; 
		} else {
				//----- included polygon b is just on plane a
			retValue = HIDDEN      ;  
//			retValue = NOT_HIDDEN  ;  

			if ( a->objectLabel < b->objectLabel ) {
				retValue = HIDDEN      ;  				
			}

		}
		delete [] vertex3d_sub_a ;	delete [] vertex3d_b ;
		return retValue ;
	}


	//----------------------------------------------//
	//   Tests below are performed only if          //
	//   *realation_2d = Tool2D::CROSS              //
	//----------------------------------------------//

		//---------------------------------------------------------------//
		//----- TEST 5A: included vertex point vs plane test (a, b) -----//
		//---------------------------------------------------------------//
	status = crossedPolygonVsPlaneTest(	vertex3d_sub_a, sub_a->vertex2D , 
							a->normalVector, num_vertex_sub_a , 
							b->normalVector, *((b->vertex)[0]) , 
							b->vertex2D,     b->numVertex  , 
							epsilon3d , epsilon );
	if        ( status == Tool3D::POSITIVE_SIDE ) {
		retValue = NOT_HIDDEN  ; 
	} else if ( status == Tool3D::NEGATIVE_SIDE ) { 
		retValue = HIDDEN  ; 
	} // else: status = Tool3D::UNKNOWN

	if( status == Tool3D::POSITIVE_SIDE || status == Tool3D::NEGATIVE_SIDE ) {
		delete [] vertex3d_sub_a ;	delete [] vertex3d_b ;
		return retValue ;		
	}

		//---------------------------------------------------------------//
		//----- TEST 5B: included vertex point vs plane test (b, a) -----//
		//---------------------------------------------------------------//
	status = crossedPolygonVsPlaneTest(	vertex3d_b, b->vertex2D , 
							b->normalVector, num_vertex_b , 
							a->normalVector,     vertex3d_sub_a[0] , 
							sub_a->vertex2D,     num_vertex_sub_a , 
							epsilon3d , epsilon );
	if        ( status == Tool3D::NEGATIVE_SIDE ) {
		retValue = NOT_HIDDEN  ; 
	} else if ( status == Tool3D::POSITIVE_SIDE  ) {
		retValue = HIDDEN  ; 
	} // else: status = Tool3D::UNKNOWN

	if( status == Tool3D::POSITIVE_SIDE || status == Tool3D::NEGATIVE_SIDE ) {
		delete [] vertex3d_sub_a ;	delete [] vertex3d_b ;
		return retValue ;		
	}


		//----------------------------------------//
		//----- TEST 6: polygon-polygon test -----//
		//----------------------------------------//
		//
		//.....  Note:	Function polygonPolygonTest() returns -2 if no intersections are found.
		//.....         In this case, a and b are  (1) independent , (2) a include2 b,
		//.....         or (3) b inclues a, and we cannot decide anything for hiding from this fact.
		//.....         For return value 0, a and b shares same points in real (3-dim) space.
		//.....         as well as on screen, and we cannot decide anything for hiding from this fact.
	status	=  this->polygonPolygonTest(	vertex3d_sub_a, sub_a->vertex2D, num_vertex_sub_a , 
						b , 8 ,  epsilon3d, epsilon );

	if        ( status ==  1 ){ 
			//------ polygon b is further from eye
		retValue = NOT_HIDDEN  ; 
	} else if ( status ==  -1 ) {
			//------ sub_polygon a is further from eye
		retValue = HIDDEN  ; 
	} else if ( status == 0 ) {
///////////////// DDDDDDD
			//----- a and b are on same plane
			//.....  We assume that outer object comes first
		if( object_label_a < object_label_b ) {
			retValue = HIDDEN ;
			
		} else {
			retValue = NOT_HIDDEN  ; 
		}
///////////////// EEEEEE
	} 
	if( status == 1 || status == -1 || status == 0 ) {
		delete [] vertex3d_sub_a ;	delete [] vertex3d_b ;
		return retValue ;		
	}


		//-----------------//
		//-----  ELSE -----//
		//-----------------//
		//..... Unexpected case:
		//.....  If result of polygonPolygon test is -2 
		//..... ( no intersection for crossed polygons)
	// beginning of exception block
	{ 
#if defined DEBUG_HID2
			//----- warning message
		cerr << "WARINIG (Hid::isHidden()) 3" << endl;
#endif
			//----- local to this block
		Vector2		center_a  ;
		int		flag_inside ;

			//----- get  rough center of a_polygon
		center_a = Tool2D::getRoughPolygonCenter( a->vertex2D , a->numVertex );

			//----- see if the rough center is inside b_polygon
		flag_inside \
		= Tool2D::isPointInsidePolygon(	center_a , 
					 	b->vertex2D , 
						b->numVertex , 
						epsilon ) ;

			//----- If center_a is inside or on a edge of b_polygon, 
			//..... we regard that the a_polygon is INCLUDED and abandon a_polygon.
			//..... Else we regard that a_polygon is INDEPENDENT.
		if( flag_inside ) {
			*relation_2d = Tool2D::INCLUDED ;
			status = HIDDEN ;
#if defined DEBUG_HID2
			cerr << "     Unexpected polygon: regarded as INCLUDED & HIDDEN" << endl;
#endif
		} else {
			*relation_2d = Tool2D::INDEPENDENT ;
			status = NOT_HIDDEN ;
#if defined DEBUG_HID2
			cerr << "     Unexpected polygon: regarded as INDEPNEDENT & NOT_HIDDEN" << endl;
#endif
		}

			//----- delete dynamical memories
		delete [] vertex3d_sub_a ;	delete [] vertex3d_b ;

			//----- return 
		return	status ;

	} // end of EXCEPTION block

} // Hid::isHidden();


	//----- Hid::removeZeroLengthEdge()
int
Hid::removeZeroLengthEdge( List<Vertex2WithLineType>& list_vlt , double epsilon )
{
		//----- local constants
	double				length_x, length_y ;
	Vector2				P, Q ;
	Vertex2WithLineType		P_vlt, Q_vlt ;		

		//----- too few vertices of polygon?
	if( (list_vlt.getNItem()) < 3 ) {
		cerr << "WARNING  (Hid::removeZeroLengthEdge()) 1\n";
		return (list_vlt.getNItem());
	}

			//----- delete edges with zero length
	list_vlt.toTail();	
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
				//----- delete P 
			list_vlt.deleteItem();	

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

		//----- return number of vertices for the revised polygon
	return (list_vlt.getNItem());

} // Hid::removeZeroLengthEdge



	//----- Hid::getFisrtConcaveVertex()
int
Hid::getFirstConcaveVertex(	List<Vertex2WithLineType>& list_vlt ,
				double epsilon )
{
		//----- local 
	const	int		NONE = -1 ;
	int			cur_backup = list_vlt.whereIsCurrentNode() ;
	int			vertex_label ;
	int			ort ;
	int			flag_found ;
	Vector2			A, B, C ;
	Vertex2WithLineType	item_vlt ;

		//----- count vertices with orientation() = 0
	flag_found   = 0 ;  // initialize
	vertex_label = 0 ;  // initialize
	list_vlt.toTail() ;
	while ( list_vlt.getItem( item_vlt ) ) {

//
//               C         B ( current vertex )
//   ... --------*--------*
//                        |
//                        |
//   ... --------*--------* A
//
			//----- current vertex position
		B = item_vlt.position ;

			//----- previsous vertex position
		list_vlt.getDownwardItemCyclic( item_vlt ) ;
		A = item_vlt.position ;

			//----- next vertex position
		list_vlt.getUpwardItemCyclic( item_vlt ) ;
		C = item_vlt.position ;

			//----- calc orientation
		ort = Tool2D::orientation( A, B, C , epsilon );

			//----- concave?
		if( ort < 0 ) { 
			flag_found = 1 ;
			break ; 
		} else {
			list_vlt.upward() ;
			vertex_label++ ;
		}

	} // while

		//----- not found?
	if( 0 == flag_found ) { vertex_label = NONE ; }

		//----- reset current node
	list_vlt.goTo( cur_backup ) ;

		//----- return number of vertices for the revised polygon
	return vertex_label ;

} // Hid::getFirstConcaveVertex()


	//----- Hid::getNumVertexOfGivenOrt()
int
Hid::getNumVertexOfGivenOrt(	List<Vertex2WithLineType>& list_vlt ,
				int	ort_type ,
				double epsilon                          )
{
		//----- local constants
	int			cur_backup = list_vlt.whereIsCurrentNode() ;
	int			num_given_ort_vertex = 0 ; // initialization
	int			ort ;
	Vector2			A, B, C ;
	Vertex2WithLineType	item_vlt ;

		//----- count vertices with orientation() = 0
	list_vlt.toTail() ;
	while ( list_vlt.getItem( item_vlt ) ) {

//
//               C         B ( current vertex )
//   ... --------*--------*
//                        |
//                        |
//   ... --------*--------* A
//
			//----- current vertex position
		B = item_vlt.position ;

			//----- previsous vertex position
		list_vlt.getDownwardItemCyclic( item_vlt ) ;
		A = item_vlt.position ;

			//----- next vertex position
		list_vlt.getUpwardItemCyclic( item_vlt ) ;
		C = item_vlt.position ;

			//----- calc orientation
		ort = Tool2D::orientation( A, B, C , epsilon );

			//----- ort = 0? 
			//..... (i.e. vertex angle is zero or flat, 
			//.....   or its connected edge has no length)
		if         ( ort_type > 0 ) {
			if(  1 == ort  ) { num_given_ort_vertex++ ;} 
		} else if  ( ort_type < 0 ) {
			if( -1 == ort  ) { num_given_ort_vertex++ ;} 	
		} else {
			if(  0 == ort  ) { num_given_ort_vertex++ ;} 			
		}
			
			//----- updation
		list_vlt.upward() ;

	} // while

		//----- reset current node
	list_vlt.goTo( cur_backup ) ;

		//----- return number of vertices for the revised polygon
	return num_given_ort_vertex ;

} // Hid::getNumVertexOfGivenOrt ()


	//----- Hid::getNumOrtZeroVertex()
int	
Hid::getNumOrtZeroVertex(	List<Vertex2WithLineType>& list_vlt ,
				double epsilon                          )
{
	return  getNumVertexOfGivenOrt(  list_vlt, 0, epsilon );
}

	//----- Hid::getNumConcaveVertex()
int	
Hid::getNumConcaveVertex(	List<Vertex2WithLineType>& list_vlt ,
				double epsilon                          )
{
	return  getNumVertexOfGivenOrt(  list_vlt, -1, epsilon );
}

	//----- Hid::getNumConvexVertex()
int	
Hid::getNumConvexVertex(	List<Vertex2WithLineType>& list_vlt ,
				double epsilon                          )
{
	return  getNumVertexOfGivenOrt(  list_vlt, 1, epsilon );
}


	//----- Hid::halfLineVsPolygon()
int
Hid::halfLineVsPolygon(	Vector2&  A,      Vector2&  B           ,
			List<Vertex2WithLineType>& list_vlt   ,
			int  starting_label, int     max_search , 
			Vector2*  X_p                           ,  
			double*   t_AB_p                        ,
			double*   t_edge_p                      ,
			int*      edge_label_p                  ,
			double    epsilon                         )
{
		//----- local constants
	int			cur_backup = list_vlt.whereIsCurrentNode() ;
	int			status_tmp ;
	int			num_vertex = list_vlt.getNItem();
	int			num_intersection ;
	int			num_search_done ;
	int			flag_X_found ;
	double			t_AB_tmp , t_edge_tmp ;
	Vector2			X_tmp ;
	Vector2			E_begin, E_end ;  // edge position
	Vertex2WithLineType	item_vlt ;


		//----- error checking
	if( starting_label < 0 || starting_label >= num_vertex ) {
		return  0 ;
	}
	if( max_search <= 0 ) {	return  0 ; }
	if( max_search > num_vertex   ) {
		max_search = num_vertex ;
	}

		//----- search intersection with minimum t_AB
	list_vlt.goTo( starting_label ) ; // initialize
	num_search_done  = 0 ; // initialize
	num_intersection = 0 ; // initialize	
	flag_X_found     = 0 ; // initialize
	while( num_search_done < max_search ) 
	{ 
			//----- begining point of edge
		list_vlt.getItem( item_vlt ) ;
		E_begin = item_vlt.position ;

			//----- ending point of edge
		list_vlt.getUpwardItemCyclic( item_vlt ) ;
		E_end = item_vlt.position ;

			//----- get intersections between 
			//..... directed half line AB and polygon
		status_tmp = \
		Tool2D::halfLineVsLineseg(	A, B , 
						E_begin, E_end , 
						&X_tmp ,
						&t_AB_tmp, 
						&t_edge_tmp ,
						epsilon );		

			//----- store line parameter and vertex label
		if( status_tmp == 1 ) {
			flag_X_found = 1 ;
			num_intersection++ ;
			if( 1 ==  num_intersection || t_AB_tmp < *t_AB_p ) {
				*X_p          = X_tmp ;
				*t_AB_p       = t_AB_tmp ;
				*t_edge_p     = t_edge_tmp ;
				*edge_label_p = list_vlt.whereIsCurrentNode() ;
			}
		}

			//----- updation
		list_vlt.toCounterClockWise();
		num_search_done++ ;

	} // while( list_vlt)


		//----- reset current node
	list_vlt.goTo( cur_backup ) ;

		//----- return number of vertices for the revised polygon
	return flag_X_found ;

} // Hid::halfLineVsPolygon()


	//----- Hid::doHiddenPartElimination()
void
Hid::doNotHiddenPartElimination( double epsilon3d )
{
		//----- local variables
	int		i;
	int		num_polygon3d ;
	int		flag_2DTo3D_success     =   1 ;
	Polygon2D	polygon2d_tmp ;

		//----- Note
		//.....  It is assumed that facets of the objects with given label
		//.....  are already passed to Polygon3DList

		//----- Preparation: add remaining polygon to polygon3DList
	while( polygon2DList.pop( polygon2d_tmp ) ) {

			//----- local to this block
		Polygon3D	polygon3d_tmp ;

			//----- calc polygon3d_tmp
		flag_2DTo3D_success = \
		polygon3d_tmp.from2DTo3D( polygon2d_tmp , this , epsilon3d );
		polygon3d_tmp.setConvexity(1) ; 

			//----- push polygon3d to polygon3DList
		if( flag_2DTo3D_success ) {
			polygon3DList.push( polygon3d_tmp );
		} else {
#if defined DEBUG_HID2
			cerr << "WARNING (Hid::doHiddenPartElimination() 1" << endl;
#endif
		}
	} // while

		//----- Error checking: no Polygon3D exist in list?
	if( polygon3DList.getNItem() <= 0 ) {
		return ;
	}

		//----- STEP 1: Convexation
		//..... Data is moved from polygon3DList to convexPolygon3DList.
		//..... The polygon3DList is cleared.
#if defined DEBUG_HID2
	cerr << "\n";
	cerr << "\n";
	cerr << "  *** STEP 1: Convexating polygons  ...  " << endl;
#endif
	convexPolygon3DList = polygon3DList ;
//	convexate(	polygon3DList, 
//			&convexPolygon3DList, 
//			beginning_object_label         ,
//			epsilon3d, epsilon   );
	this->clearPolygon3DList(); // clear unnecessary memory

		//----- STEP 2: Set polygon3d to array
		//..... Data is moved from convexPolygon3DList to polygon3d[]
		//..... The convexPolygon3DListThe is cleared.
		//.....  Old data in polygon3d[] is abandoned.
#if defined DEBUG_HID2
	cerr << "\n";
	cerr << "  *** STEP 2: Setting sorted convex polygons to array ...  " << endl;
#endif

	this->clearPolygon3DArray(); // Old data in polygon3d[] is abandoned.
	num_polygon3d = setPolygon3DToArray( (this->convexPolygon3DList) ) ;
	if( num_polygon3d != numPolygon3D ) {
		cerr << "ERROR (Hid::doHiddenPartElimination() ) 2\n";
		exit(1);
	}
	this->clearConvexPolygon3DList(); // clear unnecessary memory

		//----- STEP 3: Set subPolygonList to each Polygon3D item
#if defined DEBUG_HID2
	cerr << "\n";
	cerr << "  *** STEP 3: Initializing stack for polygon images ...  " << endl;
#endif

	for( i = 0 ; i < numPolygon3D ; i++ ) {
		polygon3d[i].setSubPolygonList();
	}		

		//----- STEP 4: Extract visible parts of polygon stored in polygon3d[], 
		//.....         and add them to polygon2DList.
		//.....         The polygon3d[] is not cleared, 
		//.....         since it keeps some data referenced by Polygon2D in polygon2D list.
#if defined DEBUG_HID2
	cerr << "\n";
	cerr << "  *** STEP 4: Calculating visible parts of polygons ... " << endl;
#endif
//	extractVisiblePart( beginning_object_label, ending_object_label, epsilon3d , epsilon ); 
	for( i = 0 ; i < numPolygon3D ; i++ ) {
		Polygon2D	sub_polygon ;
		while( (polygon3d[i].subPolygonList).pop( sub_polygon ) ) {
			polygon2DList.push( sub_polygon ) ; 
		}
	}		

		//----- step 5: Copy vertex position data from Object to Polygon3D
	for( i = 0 ; i < numPolygon3D ; i++ ) {
		polygon3d[i].setVertex3D();
	}		

} // Hid::doNotHiddenPartElimination()


		//----- Tool2D::getBoundingDiamond()
int
Hid::getBoundingDiamond(	double*		xmin ,
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
	int	i ,j  ;
	int	num_vertex ;
	double	x, y, u, v ;

		//----- error checking
	if( numPolygon2D <= 0 ) {
		status = 0 ; // error
		return status ;
	}

		//----- for each polygon2d[i]
	for( i = 0; i < numPolygon2D ; i++ ) {

		num_vertex = polygon2d[i].numVertex ;

			//----- for each polygon2d[i].vertex[j]
		for( j = 0; j < num_vertex ; j++ ) {
			x = polygon2d[i].vertex2D[j].x ;
			y = polygon2d[i].vertex2D[j].y ;
			u = y + x ;
			v = y - x ;

			if( i == 0 && j == 0 ) {
					//----- initialization
				(*xmin) = (*xmax) = x ;
				(*ymin) = (*ymax) = y ;
				(*umin) = (*umax) = u ;
				(*vmin) = (*vmax) = v ;
			} else {
					//----- updation
				if( x < (*xmin) ) { (*xmin) = x ; }
				if( x > (*xmax) ) { (*xmax) = x ; }
				if( y < (*ymin) ) { (*ymin) = y ; }
				if( y > (*ymax) ) { (*ymax) = y ; }
				if( u < (*umin) ) { (*umin) = u ; }
				if( u > (*umax) ) { (*umax) = u ; }
				if( v < (*vmin) ) { (*vmin) = v ; }
				if( v > (*vmax) ) { (*vmax) = v ; }

			} // if--else
		} // j
	} // i


		//----- return status
	status = 1 ; // normal
	return status ;

} // Hid::getBoundingDiamond()

	//-----	setBoundingDiamondArray()
int	Hid::setBoundingDiamondArray( int diamond_interval ) 
{
		//----- local
	int	status = 0 ; // initialization
	int	num_item ;
	int	i ;

		//----- error checking
	if( diamond_interval <= 0 || numPolygon3D == 0 ) {
#if defined DEBUG_HID2
		cerr << "WARNING (Hid::setBoundingDiamondArray) 1\n";
#endif
		status = 0 ;
		return status ;
	}

		//----- clear old data
		//.....  delete [] boundingDiamondArray;
		//.....  boundingDiamondArray = NULL ;
		//.....  boundingDiamondInterval = 0 ;
	clearBoundingDiamondArray();

		//----- set boundingDiamondInterval
	boundingDiamondInterval  = diamond_interval ;

		//----- calc boundingDiamondArray
	num_item = numPolygon3D/boundingDiamondInterval + 1 ;
	boundingDiamondArray = new Diamond [ num_item ] ;
	if( boundingDiamondArray == NULL ) {
		cerr << "ERROR (Hid::setBoundingDiamondArray)  2\n";
		exit(1);
	}

		//----- set boundingDiamondArray[]
	for( i = 0 ; i < numPolygon3D ; i++) {

			//----- index of boundingDiamondArray
		int index = i / boundingDiamondInterval ;
		if( index >= num_item ) {
			cerr << "ERROR (Hid::setBoundingDiamondArray)  3\n";
			exit(1);
		}

			//----- set an item to boundingDiamondArray
			//..... or update it
		boundingDiamondArray[index].set(	polygon3d[i].xmin,  polygon3d[i].xmax,  
							polygon3d[i].ymin,  polygon3d[i].ymax,  
							polygon3d[i].umin,  polygon3d[i].umax,  
							polygon3d[i].vmin,  polygon3d[i].vmax,  
							i 					) ;
	}

		//----- return status 
	status = 1 ;
	return status ;

} // Hid::setBoundingDiamondArray()


	//----- Hid::multiMaxMinTest()
int	Hid::multiMaxMinTest(	double xmin_given , double xmax_given ,
				double ymin_given , double ymax_given ,
				double umin_given , double umax_given ,
				double vmin_given , double vmax_given ,
				int index_given   , double epsilon2d, 
				int* index_end_p     ) 
{
	int	status = 0 ; // initialization
	double	xmin,	xmax ;
	double	ymin,	ymax ;
	double	umin,	umax ; // u = y + x  ;
	double	vmin,	vmax ; // v = y - x  ;
	int	index0 , index1 ;
	int	label = index_given / boundingDiamondInterval ;
			// index for boundingDiamond item to be used for test
	Diamond*	diamond_p = &(boundingDiamondArray[ label ]) ;
			// boundingDiamond item to be used for test

		//----- error checking
	if( boundingDiamondArray == NULL || boundingDiamondInterval <= 0 ) {
		cerr << "ERROR (Hid::multiMaxMinTest) 1\n";
		exit(1);
	}

		//----- select a valid diamond
	diamond_p->getDiamond( &xmin, &xmax, &ymin, &ymax, &umin, &umax, &vmin, &vmax ) ;
	diamond_p->getIndex  ( &index0, &index1 ) ; 

		//----- error checking
	if( index_given < index0 ) {
		cerr << "ERROR (Hid::multiMaxMinTest) 2\n";
		exit(1);
	}
			
	if(	xmax_given < xmin + epsilon2d ||\
		xmin_given > xmax - epsilon2d ||\
		ymax_given < ymin + epsilon2d ||\
		ymin_given > ymax - epsilon2d ||\
		umax_given < umin + epsilon2d ||\
		umin_given > umax - epsilon2d ||\
		vmax_given < vmin + epsilon2d ||\
		vmin_given > vmax - epsilon2d   ) 
	{
		status = 0 ;	// independent
		*index_end_p = index1 ; // skip 
	} else {
		status = 1 ;	// else or unknown
		*index_end_p = index_given ; // do not skip
	}

		//----- RETURN
	return status ;

} // Hid::multiMaxMinTest()
				

	//-----	Hid::recoverOriginalPolygon3D()
void	Hid::recoverOriginalPolygon3D( Polygon3D& polygon3D_input,  Polygon3D* polygon3D_output_p , double epsilon3d )
{


	if( !polygon3D_input.isOriginalDataSet() ) { 
			//----- output (in case that input polygon is original)
		*polygon3D_output_p = polygon3D_input ;
	} else {

			//----- output (in case that input polygon is not original)

			//----- local
		int		i ;
		Vector3_p*	dummy = new Vector3_p[ polygon3D_input.numVertex_org ] ;	

			//----- set dummy
		if( dummy == NULL ) {
			cerr << "Error (Hid::recoverOriginalPolygon3D()\n"  << endl;
			exit(1);
		} 
		for( i = 0 ; i < (polygon3D_input.numVertex_org) ; i++ ) {
			dummy[i] = NULL ;
		}


			//----- output
		polygon3D_output_p->clear();
		polygon3D_output_p->set(	dummy                              , 
						polygon3D_input.edgeType_org    , 
						polygon3D_input.numVertex_org   , 
						polygon3D_input.color           ,
						&(polygon3D_input.normalVector)  , 
						camera_p                           ,
						epsilon3d                          , 
						polygon3D_input.objectLabel      ,
						polygon3D_input.facetLabel       ,
						polygon3D_input.neighboringFacet ,
						polygon3D_input.numNeighboringFacet ,
						polygon3D_input.vertex3D_org         ) ;

			//----- delete 
		delete [] dummy ;

	} // if--else

} // Hid::recoverOriginalPolygon3D()



	//----- Hid::movePolygon3DListToArray()
void
Hid::movePolygon3DListToArray( double epsilon3d , double epsilon2d )
{
		//----- local variables
	int		i;
	int		num_polygon3d ;

		//----- Note
		//.....  It is assumed that facets of the objects with given label
		//.....  are already passed to Polygon3DList

		//----- Error checking: no Polygon3D exist in list?
	if( polygon3DList.getNItem() <= 0 ) {
		return ;
	}

		//----- STEP 1: Convexation
		//..... Data is moved from polygon3DList to convexPolygon3DList.
		//..... The polygon3DList is cleared.
#if defined DEBUG_HID2
	cerr << "\n";
	cerr << "\n";
	cerr << "  *** STEP 1: Convexating polygons  ...  " << endl;
#endif
	convexate(	polygon3DList, 
			&convexPolygon3DList, 
			epsilon3d, epsilon2d   );
	this->clearPolygon3DList(); // clear unnecessary memory

		//----- STEP 2: Set polygon3d to array
		//..... Data is moved from convexPolygon3DList to polygon3d[]
		//..... The convexPolygon3DListThe is cleared.
		//.....  Old data in polygon3d[] is abandoned.
#if defined DEBUG_HID2
	cerr << "\n";
	cerr << "  *** STEP 2: Setting sorted convex polygons to array ...  " << endl;
#endif
	this->clearPolygon3DArray(); // Old data in polygon3d[] is abandoned.
	num_polygon3d = setPolygon3DToArray( (this->convexPolygon3DList) ) ;
	if( num_polygon3d != numPolygon3D ) {
		cerr << "ERROR (Hid::doHiddenPartElimination() ) 2\n";
		exit(1);
	}
	this->clearConvexPolygon3DList(); // clear unnecessary memory

		//----- STEP 3: Set subPolygonList to each Polygon3D item
#if defined DEBUG_HID2
	cerr << "\n";
	cerr << "  *** STEP 3: Initializing stack for polygon images ...  " << endl;
#endif
	for( i = 0 ; i < numPolygon3D ; i++ ) {
		polygon3d[i].setSubPolygonList();
	}		


} // Hid::movePolygon3DListToArray()

