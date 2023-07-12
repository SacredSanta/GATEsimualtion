//////////////////////////////////
///// tool3d.cc  1995/11/15  /////
//////////////////////////////////

//#define DEBUG_TOOL3D

#include "tool3d.h"
#include "FRMath.h"

//---------- getNormalVector( Vector3* ) 
Vector3 
Tool3D::getNormalVector( Vector3* point_vector, int num_point)
{
	//----- local variables
	int i;		// loop variable
	int i_plus_1;	// mod(i+1,num_point)

	//----- return value
	Vector3 normal_vector(0.0, 0.0, 0.0) ;	

	//----- calc normal_vector 
	//.....  \sum_{i = 0}^{n-1} P[i]^P[i+1]
	//.....  with periodic boundary condition
	for( i = 0 ; i < num_point ; i++) {
		//-----  i+1 with periodic boundary
		i_plus_1 = i + 1 ;
		if( i_plus_1 == num_point ) { i_plus_1 = 0 ;}

		//----- sum
		normal_vector \
		+= ( point_vector[i] ^ point_vector[i_plus_1] );
	}

	//----- normalization ;
	normal_vector = normal_vector.normalize();

	//----- return
	return normal_vector;

} // Vector3 Tool3D::getNormalVector()


//---------- getNormalVector( Vector3** ) 
Vector3 
Tool3D::getNormalVector( Vector3** point_vector, int num_point)
{
	//----- local variables
	int i;		// loop variable
	int i_plus_1;	// mod(i+1,num_point)

	//----- return value
	Vector3 normal_vector(0.0, 0.0, 0.0) ;	

	//----- calc normal_vector 
	//.....  \sum_{i = 0}^{n-1} P[i]^P[i+1]
	//.....  with periodic boundary condition
	for( i = 0 ; i < num_point ; i++) {
		//-----  i+1 with periodic boundary
		i_plus_1 = i + 1 ;
		if( i_plus_1 == num_point ) { i_plus_1 = 0 ;}

		//----- sum
		normal_vector \
		+= (  (*point_vector[i]) ^ (*point_vector[i_plus_1])   );
	}

	//----- normalization ;
	normal_vector = normal_vector.normalize();

	//----- return
	return normal_vector;

} // Vector3 Tool3D::getNormalVector()



//----- Tool3D::planeVsPolygon(..., Vector3* P , ...)
int 
Tool3D::planeVsPolygon( Vector3 normal, Vector3 P_on, Vector3* P, int nvertex, int* nvertex_on_plane_p, double epsilon ) 
{
	//----- local variables
	int		i  ;
	int		side ;
	int		n_plus_side  = 0 ;
	int		n_minus_side = 0 ; 
				// num of vertices in positive and 
				// negative side of plane( normal, P_on )
	int		n_on_plane   = 0 ;  
				// num of vertices just on plane
	double		dummy ;

	//----- error checking 
	if( nvertex < 3 ) {
		cerr << "ERROR: too few vertices for a polygon " ;
		cerr << "( Tool3d::planeVsPolygon(), 1)\n";
		exit(1) ;
	}

	//----- investigate position of each vertex relateive to
	//..... plane ( normal, P_on )
	for( i = 0 ; i < nvertex ; i++ ) {
		side  = planeVsPoint( normal, P_on, P[i], &dummy, epsilon ) ;

		//----- count num of points in each side of plane
		switch ( side ) {
			case 1: 
				n_plus_side++  ;  break ;
			case -1: 
				n_minus_side++ ;  break ;
			case 0: 
				n_on_plane++ ;    break ;
			default:
				//----- error 
				cerr << "ERROR:" ;
				cerr << "( Tool3d::planeVsPolygon(), 2)\n";
				exit(1) ;
		}

		//----- intersect ?
		if( n_plus_side > 0 && n_minus_side > 0 ) {
			break ;	
		}

	} // for(i)


	//-----  decide side of the plane in which thr polygon exists
	if       ( n_plus_side > 0 && n_minus_side > 0 ) {
		//----- polygon intersects with plane
		*nvertex_on_plane_p = 0 ;
		return 0 ;	
	} else if( n_plus_side > 0 && n_minus_side == 0 ) {
		//----- polygon is in positive side of plane
		*nvertex_on_plane_p = n_on_plane ;
 		return 1 ;
	} else if ( n_plus_side == 0 && n_minus_side > 0 ) {
		//----- polygon is in negative side of plane
		*nvertex_on_plane_p = n_on_plane ;
 		return -1;
	} else if( n_plus_side == 0 && n_minus_side == 0 && n_on_plane == nvertex ){
		//----- polygon is just on the plane
		*nvertex_on_plane_p = nvertex ;
		return 0 ;
	} else {
		//----- error 
		cerr << "ERROR: wrong data for a polygon " ;
		cerr << "( Tool3d::planeVsPolygon(), 3)\n";
		exit(1) ;
		return -999 ; // dummy
	}
	

} // Tool3D::planeVsPolygon(..., Vector3* P , ...)




//----- Tool3D::planeVsPolygon(..., Vector3** P , ...)
int 
Tool3D::planeVsPolygon( Vector3 normal, Vector3 P_on, Vector3** P, int nvertex, int* nvertex_on_plane_p, double epsilon ) 
{
	//----- local variables
	int		i  ;
	int		side ;
	int		n_plus_side  = 0 ;
	int		n_minus_side = 0 ; 
				// num of vertices in positive and 
				// negative side of plane( normal, P_on )
	int		n_on_plane   = 0 ;  
				// num of vertices just on plane
	double		dummy ;

	//----- error checking 
	if( nvertex < 3 ) {
		cerr << "ERROR: too few vertices for a polygon " ;
		cerr << "( Tool3d::planeVsPolygon(), 1)\n";
		exit(1) ;
	}

	//----- investigate position of each vertex relateive to
	//..... plane ( normal, P_on )
	for( i = 0 ; i < nvertex ; i++ ) {
		side  = planeVsPoint( normal, P_on, *P[i], &dummy, epsilon ) ;

		//----- count num of points in each side of plane
		switch ( side ) {
			case 1: 
				n_plus_side++  ;  break ;
			case -1: 
				n_minus_side++ ;  break ;
			case 0: 
				n_on_plane++ ;    break ;
			default:
				//----- error 
				cerr << "ERROR:" ;
				cerr << "( Tool3d::planeVsPolygon(), 2)\n";
				exit(1) ;
		}

		//----- intersect ?
		if( n_plus_side > 0 && n_minus_side > 0 ) {
			break ;	
		}

	} // for(i)


	//-----  decide side of the plane in which thr polygon exists
	if       ( n_plus_side > 0 && n_minus_side > 0 ) {
		//----- polygon intersects with plane
		*nvertex_on_plane_p = 0 ;
		return 0 ;	
	} else if( n_plus_side > 0 && n_minus_side == 0 ) {
		//----- polygon is in positive side of plane
		*nvertex_on_plane_p = n_on_plane ;
 		return 1 ;
	} else if ( n_plus_side == 0 && n_minus_side > 0 ) {
		//----- polygon is in negative side of plane
		*nvertex_on_plane_p = n_on_plane ;
 		return -1;
	} else if( n_plus_side == 0 && n_minus_side == 0 && n_on_plane == nvertex ){
		//----- polygon is just on the plane
		*nvertex_on_plane_p = nvertex ;
		return 0 ;
	} else {
		//----- error 
		cerr << "ERROR: wrong data for a polygon " ;
		cerr << "( Tool3d::planeVsPolygon(), 3)\n";
		exit(1) ;
		return -999; //dummy
	}
	

} // Tool3D::planeVsPolygon(..., Vector3** P , ...)




//----- planeVsPoint()
int 
Tool3D::planeVsPoint( Vector3 normal, Vector3 P_on, Vector3 P, double* F_p, double epsilon )
{
	//----- local variables 
	int				side ;
	double			F    ;
	
	//----- calc parameter F, signed distance between point and plane
	F = normal * ( P - P_on ) ;

	//----- decide in which side of plane the given point P is
	if     ( F >   epsilon ) { side = Tool3D::POSITIVE_SIDE ; } // positive side of plane
	else if( F < - epsilon ) { side = Tool3D::NEGATIVE_SIDE ; } // negative side of plane
	else                     { side = Tool3D::ON_PLANE ; } // on plane

	//----- return obtained results
	*F_p = F ;
	return side ;

} // Tool3D::planeVsPoint()	


//----- planeVsLinesegment()
int 
Tool3D::planeVsLinesegment( Vector3 normal, Vector3 P_on,  Vector3 P, Vector3 Q,  Vector3* intersect, double epsilon ) 
{
	//----- local variables
	double		FP, FQ ;
	double		FP_FQ, signFP_FQ ;
	double		t_param ;
	int		sum = 0 ;
	
	//----- position of P and Q relative to plane( normal, P_on )
	sum += planeVsPoint( normal, P_on, P, &FP, epsilon ) ;
	sum += planeVsPoint( normal, P_on, Q, &FQ, epsilon ) ;
	
	//----- position of intersection
	FP_FQ = FP - FQ ;
	if( (FP_FQ > epsilon) || (FP_FQ < -epsilon)  ) {
		t_param = FP / FP_FQ ;
	} else {
		signFP_FQ = ( FP_FQ >= 0 ? 1 : -1 ) ;
		t_param = signFP_FQ * 1.0 / epsilon ;
	}
	*intersect = t_param * ( Q - P ) + P ;

	//----- return 2, 1, 0, -1, or -2
	return sum ;

} // Tool3D::planeVsLinesegment() 



	//----- Tool3D::getZmin()
double	Tool3D::getZmin( Vector3* P, int numP ) 
{
		//----- local variables
	int	i ;
	double	zmin = 0.0 ;

		//----- calc zmin
	for( i = 0 ; i < numP ; i++ ) {
		if( i == 0 ) { 
			zmin = P[i].z ; // initialize
		} else if( P[i].z < zmin ) {
			zmin = P[i].z;  // compare
		}
	}

		//----- return
	return zmin ;

}// Tool3D::getZmin()


Vector3	Tool3D::getCenter( Vector3* P, int numP )
{
		//----- local
	int		i ;
	double		numP_inv = 1.0 / ( (double)(numP) ) ;
	Vector3		center(0.0, 0.0, 0.0) ; // return value


	if( numP <= 0 ) {
			//----- error checking
		cerr << "ERROR ( Tool3D ): numP = " << numP << endl;
		exit(1) ;
	} else {
			//----- calc center of P's
		for( i = 0 ; i < numP ; i++ ) {
			center += P[i] ;
		}
		center = numP_inv * center ;
	}

		//----- return calculatec center
	return center ;

}// Tool3D::getCenter()



	//----- Tool3D::getBoundingBox()
void
Tool3D::getBoundingBox(	const Vector3& A, const Vector3& B , 
			Vector3* min_position_p            , 
			Vector3* max_position_p              )
{
		//----- local 
	double xmin, ymin, zmin ;
	double xmax, ymax, zmax ;

		//----- calc min position
	xmin = (A.x < B.x ? A.x : B.x ) ;
	ymin = (A.y < B.y ? A.y : B.y ) ;
	zmin = (A.z < B.z ? A.z : B.z ) ;
	(*min_position_p)( xmin, ymin, zmin );

		//----- calc max position
	xmax = (A.x > B.x ? A.x : B.x ) ;
	ymax = (A.y > B.y ? A.y : B.y ) ;
	zmax = (A.z > B.z ? A.z : B.z ) ;
	(*max_position_p)( xmax, ymax, zmax );

} // Tool3D::getBoundingBox()



	//----- Tool3D::getBoundingBox()
double
Tool3D::getBoundingBox(	Vector3* P, int numP , 
			Vector3* min_position_p , 
			Vector3* max_position_p )
{
		//----- local
	int	i;
	double	xmin, ymin, zmin ;
	double	xmax, ymax, zmax ;
	double	size ;

		//----- initialization
	xmin = xmax = P[0].x ;
	ymin = ymax = P[0].y ;
	zmin = zmax = P[0].z ;

		//----- calculation of max and min points
	for( i = 1; i < numP ; i++ ) {
		if( P[i].x < xmin  ) { xmin = P[i].x ; }
		if( P[i].x > xmax  ) { xmax = P[i].x ; }

		if( P[i].y < ymin  ) { ymin = P[i].y ; }
		if( P[i].y > ymax  ) { ymax = P[i].y ; }

		if( P[i].z < zmin  ) { zmin = P[i].z ; }
		if( P[i].z > zmax  ) { zmax = P[i].z ; }
	}

		//----- return results
	(*min_position_p)( xmin, ymin, zmin );
	(*max_position_p)( xmax, ymax, zmax );

		//----- calc size of area including given points
	size = ( (*max_position_p) - (*min_position_p) ).getLength() ;

		//----- return 
	return size ;

} // Tool3D::getBoundingBox()


	//----- Tool3D:isPointInsideOrOnEdgeOfPolygon()
int 
Tool3D::isPointInsideOrOnEdgeOfPolygon(	const Vector3&	P               ,
					Vector3_p*	vertex_position ,
					int		num_vertex      ,
					Vector3*	normal_vector_p ,
					double		epsilon         ,
					double		epsilon_nodim      )
{
		//----- local 
	int		flag_inside_or_onedge    ;
	int		i, i_plus ;
	int		num_nonzero_length_edge = num_vertex ;
	double		length_AB, length_AP ;
	Vector3		*A_p, *B_p ;	// starting and ending point 
					// of an edge
	Vector3		vec_AB, vec_AP , vec_AP_X_AB ;
	Vector3		normal_vector ;


		//----- too few vertices?
	if( num_vertex < 3 ) { 
			//----- regarded as "outside"
		flag_inside_or_onedge = 0 ; 
		return flag_inside_or_onedge ;
	}

		//----- calc normal vector, if not given
	if( normal_vector_p == NULL ) {
		normal_vector = \
			Tool3D::getNormalVector( vertex_position, num_vertex );
	} else {
		normal_vector = *normal_vector_p ;
	}

		//----- invectigate each edge 
	flag_inside_or_onedge = 1 ; // Initialize to "inside or onedge".
				    // See the commnent (*) below.
	for( i = 0 ; i < num_vertex ; i++ ) {

			//----- i_plus = i + 1 
		i_plus = i + 1 ; 
		if( i_plus == num_vertex ) { i_plus = 0 ;}

			//----- calc i-th edge vector vec_AB (normalized)
		A_p = vertex_position[ i ]       ;
		B_p = vertex_position[ i_plus ] ;
		vec_AB    = (*B_p) - (*A_p) ;
		length_AB = vec_AB.getLength() ;
		if( length_AB < epsilon ) {
			num_nonzero_length_edge--;
			if( num_nonzero_length_edge < 3 ) {
				flag_inside_or_onedge = 0 ; 
				break ; // Polygon with zero area:
					// Regarded as "outside".
			} else {
				continue ; // skip this edge
			}
		} 
		vec_AB = ( 1.0 / length_AB) * vec_AB ;

			//----- calc vector vec_AP  (normalized)
		vec_AP    = P - (*A_p) ;
		length_AP = vec_AP.getLength() ;
		if( length_AP < epsilon ) {
			flag_inside_or_onedge = 1 ; 
			break ;	// P is just on A.
				// Regarded as "onedge".
		} 
		vec_AP = ( 1.0 / length_AP) * vec_AP ;

			//----- (*) see direction of AP X AB   
			//...... If there exists any edge AB 
			//...... for which AP X AB directs to 
			//...... positive side of the polygon,
			//...... point P is outside the polygon.
			//...... ( Otherwise, P is inside of on a edge of
			//......   the polygon. See initialization 
			//......   just before the for-loop. )
		vec_AP_X_AB = vec_AP ^ vec_AB ;
		if ( vec_AP_X_AB * normal_vector > epsilon_nodim ) {
			flag_inside_or_onedge = 0  ; 
			break ;	// Regarded as "outside".
		}

	} // for(i)

		//----- return flag_inside_or_onedge 
	return flag_inside_or_onedge ;

} // Tool3D:isPointInsideOrOnEdgeOfPolygon()




	//----- Tool3D::getPlaneLineIntersection(), 1
int	Tool3D::getPlaneLineIntersection( const Vector3& normal , 
					  const Vector3& P_on   ,  
					  const Vector3& A      , 
					  const Vector3& B      ,
					  Vector3* X_p          , 
					  double*  t_X_p        , 
					  double epsilon3d      ,
					  double epsilon_nodim     )
{
		//----- local const
	int		status ;	  // return value
	double		AB_normal, AP_normal ;
	double		absAB ;
	Vector3		AB ;

		//----- calc vector AB and its length
	AB    = B - A ;
	absAB = AB.getLength();

		//----- calc  AB * n , AP * n  
	AB_normal = AB * normal ;
	AP_normal = ( P_on - A ) * normal ;

		//----- TEST 1: A == B ?
	if( absAB < epsilon3d ) {
			//----- set status
		status = 0 ;	

		//----- TEST 2: AB is perpendicular to normal vector?		
	} else if ( fabs( AB_normal / absAB ) < epsilon_nodim ) {
			//----- set status
		status = 0 ;	
			
		//----- calc intersection
	} else {
			//----- local to this block
		double	t_X ;

			//----- set status
		status = 1 ;	

			//-----  intersection exists
		t_X    = AP_normal / AB_normal ; 

			//----- return results to argument
		*X_p = t_X * ( B - A ) + A ;
		*t_X_p = t_X ;
	}

		//----- message
	if( !status ) {
#if defined DEBUG_DEBUG_TOOL3D
		cerr << "WARNING ( Tool3D::getPlaneLineIntersection()), 1" << endl;
#endif
	}

		//----- return status
	return status  ;

} // Tool3D::getPlaneLineIntersection() , 1


	//----- Tool3D::getPlaneLineIntersection(), 2
int	Tool3D::getPlaneLineIntersection( const Vector3&  normal , 
					  const Vector3&  P_on   ,  
					  const Vector3&  A      , 
					  const Vector3&  B      , 
					  Vector3*        X_p    , 
					  double          epsilon3d ,
					  double          epsilon_nodim     )
{
		//----- local 
	int		status ;	// return value
	double		t_X ;		// dummy 

		//----- calc status and intersection position if any
	status = \
	Tool3D::getPlaneLineIntersection( normal , 
					  P_on   ,  
					  A      , 
					  B      , 
					  X_p    , 
					  &t_X   ,
					  epsilon3d, epsilon_nodim  );

		//----- return status
	return status  ;

} // Tool3D::getPlaneLineIntersection(), 2


	//----- Tool3D::isLineIntersectedWithPolygon()
int	Tool3D::isLineIntersectedWithPolygon(	const Vector3&  A               , 
					  	const Vector3&  B               , 
						Vector3_p*	vertex_position ,
						int		num_vertex      ,
						Vector3*	normal_vector_p ,
						Vector3*	X_p             , 
					  	double*	t_X_p                   , 
						double	epsilon                 ,
						double	epsilon_nodim            )
{
		//----- local 
	int		status  ;	// return value
	int		flag_line_plane_intersected ;
	Vector3		normal_vector , P_on ;

		//----- plane data
		//---------- (1) one point on plane
	P_on = *vertex_position[0] ;
		//---------- (2) normal vector
	if( normal_vector_p == NULL ) {
		normal_vector = \
			Tool3D::getNormalVector( vertex_position, num_vertex );
	} else {
		normal_vector = *normal_vector_p ;
	}


		//----- calc plane line intersection if any
	flag_line_plane_intersected \
	=  Tool3D::getPlaneLineIntersection(	normal_vector , 
						P_on   ,  
						A      , 
						B      , 
						X_p    , 
						t_X_p  ,
						epsilon  , epsilon_nodim );

		//----- see if line AB intersects with polygon
	if( !flag_line_plane_intersected ) 
	{
			//-----  Plane does not intersect with line.
			//.....  Possible reasons:
			//.....  (1) line AB parallel to plane( n, P_on )
			//.....  (2) A = B, 
			//.....  (3) n = 0 
		status = 0 ; // not intersect 
	} else {

		status \
		= Tool3D::isPointInsideOrOnEdgeOfPolygon(	*X_p             ,
								vertex_position  ,
								num_vertex       ,
								&normal_vector   ,
								epsilon          ,
								epsilon_nodim      );

	} // if--else

		//----- return status 
	return status  ;

} // Tool3D::isLineIntesectedWithPolygon() 


	//----- Tool3D::isLineIntersectedWithBox()
int	Tool3D::isLineIntersectedWithBox(	const Vector3&  A               , 
					  	const Vector3&  B               , 
					  	const Vector3&  box_min         , 
					  	const Vector3&  box_max         , 
						Vector3*	X_array         , 
					  	double*		t_X_array         , 
						double		epsilon         ,
						double		epsilon_nodim      )
{
		//----- local 
	enum{ FRONT_FACE = 0 , BACK_FACE = 1 };
	const	int	FOUR = 4 ;
	const	double	minus_epsilon_nodim = (-1.0) * epsilon_nodim ;
	int		num_intersection = 0 ; // return value
	int		status ;
	double		t_X_tmp ;
	double		prod  ;
	Vector3		X_tmp ;
	Vector3		vertex_position[8];
	Vector3*	vertex_position_p[4];
	Vector3		normal_vector ;
	Vector3		vec_AB ;

		//----- vector AB ( normalized )
	vec_AB = B - A ;
	vec_AB = vec_AB.normalize();

		//------------------------------//
		//----- vertex positions -------//
		//------------------------------//
		//---------- (1) bottom plane
	(vertex_position[0])( box_min.x, box_min.y, box_min.z ) ;
	(vertex_position[1])( box_max.x, box_min.y, box_min.z ) ;
	(vertex_position[2])( box_max.x, box_max.y, box_min.z ) ;
	(vertex_position[3])( box_min.x, box_max.y, box_min.z ) ;

		//---------- (2) top plane
	(vertex_position[4])( box_min.x, box_min.y, box_max.z ) ;
	(vertex_position[5])( box_max.x, box_min.y, box_max.z ) ;
	(vertex_position[6])( box_max.x, box_max.y, box_max.z ) ;
	(vertex_position[7])( box_min.x, box_max.y, box_max.z ) ;

		//-----------------------------------//
		//----- top facet investigation -----//
		//-----------------------------------//
		//---------- (t1) vertex
	vertex_position_p[0] = &vertex_position[4] ;
	vertex_position_p[1] = &vertex_position[5] ;
	vertex_position_p[2] = &vertex_position[6] ;
	vertex_position_p[3] = &vertex_position[7] ;

		//---------- (t2) normal vector
	normal_vector( 0.0, 0.0, 1.0 ) ;

		//---------- (t3) intersection
	status = \
	 Tool3D::isLineIntersectedWithPolygon(	A                 ,  B          , 
						vertex_position_p ,  FOUR       ,
						&normal_vector    ,
						&X_tmp              , 
					  	&t_X_tmp            , 
						epsilon           ,  epsilon_nodim   ) ;
		//---------- (t4) see status
	if( status == 1  )
	{ 
		prod =  normal_vector * vec_AB  ;
		if(         prod < minus_epsilon_nodim ) {
			num_intersection++ ;
			X_array   [ FRONT_FACE ] = X_tmp ;
			t_X_array [ FRONT_FACE ] = t_X_tmp ;

		} else if ( prod > epsilon_nodim        ) {
			num_intersection++ ;
			X_array   [ BACK_FACE ] = X_tmp ;
			t_X_array [ BACK_FACE ] = t_X_tmp ;
		} 

		if( num_intersection >= 2 ) { return num_intersection ; }
	}


		//--------------------------------------//
		//----- bottom facet investigation -----//
		//--------------------------------------//
		//---------- (b1) vertex
	vertex_position_p[0] = &vertex_position[3] ;
	vertex_position_p[1] = &vertex_position[2] ;
	vertex_position_p[2] = &vertex_position[1] ;
	vertex_position_p[3] = &vertex_position[0] ;

		//---------- (b2) normal vector
	normal_vector( 0.0, 0.0, -1.0 ) ;

		//---------- (b3) intersection
	status = \
	 Tool3D::isLineIntersectedWithPolygon(	A                 ,  B          , 
						vertex_position_p ,  FOUR       ,
						&normal_vector    ,
						&X_tmp              , 
					  	&t_X_tmp            , 
						epsilon           ,  epsilon_nodim   ) ;
		//---------- (b4) see status
	if( status == 1  )
	{ 
		prod =  normal_vector * vec_AB  ;
		if(         prod < minus_epsilon_nodim ) {
			num_intersection++ ;
			X_array   [ FRONT_FACE ] = X_tmp ;
			t_X_array [ FRONT_FACE ] = t_X_tmp ;

		} else if ( prod > epsilon_nodim        ) {
			num_intersection++ ;
			X_array   [ BACK_FACE ] = X_tmp ;
			t_X_array [ BACK_FACE ] = t_X_tmp ;
		} 

		if( num_intersection >= 2 ) { return num_intersection ; }
	}





		//---------------------------------------//
		//----- side facet 1  investigation -----//
		//---------------------------------------//
		//---------- (s1_1) vertex
	vertex_position_p[0] = &vertex_position[1] ;
	vertex_position_p[1] = &vertex_position[2] ;
	vertex_position_p[2] = &vertex_position[6] ;
	vertex_position_p[3] = &vertex_position[5] ;

		//---------- (s1_2) normal vector
	normal_vector( 1.0, 0.0, 0.0 ) ;

		//---------- (s1_3) intersection
	status = \
	 Tool3D::isLineIntersectedWithPolygon(	A                 ,  B          , 
						vertex_position_p ,  FOUR       ,
						&normal_vector    ,
						&X_tmp              , 
					  	&t_X_tmp            , 
						epsilon           ,  epsilon_nodim   ) ;
		//---------- (s1_4) see status
	if( status == 1  )
	{ 
		prod =  normal_vector * vec_AB  ;
		if(         prod < minus_epsilon_nodim ) {
			num_intersection++ ;
			X_array   [ FRONT_FACE ] = X_tmp ;
			t_X_array [ FRONT_FACE ] = t_X_tmp ;

		} else if ( prod > epsilon_nodim        ) {
			num_intersection++ ;
			X_array   [ BACK_FACE ] = X_tmp ;
			t_X_array [ BACK_FACE ] = t_X_tmp ;
		} 

		if( num_intersection >= 2 ) { return num_intersection ; }
	}




		//---------------------------------------//
		//----- side facet 2  investigation -----//
		//---------------------------------------//
		//---------- (s2_1) vertex
	vertex_position_p[0] = &vertex_position[3] ;
	vertex_position_p[1] = &vertex_position[0] ;
	vertex_position_p[2] = &vertex_position[4] ;
	vertex_position_p[3] = &vertex_position[7] ;

		//---------- (s2_2) normal vector
	normal_vector( -1.0, 0.0, 0.0 ) ;

		//---------- (s2_3) intersection
	status = \
	 Tool3D::isLineIntersectedWithPolygon(	A                 ,  B          , 
						vertex_position_p ,  FOUR       ,
						&normal_vector    ,
						&X_tmp              , 
					  	&t_X_tmp            , 
						epsilon           ,  epsilon_nodim   ) ;
		//---------- (s2_4) see status
	if( status == 1  )
	{ 
		prod =  normal_vector * vec_AB  ;
		if(         prod < minus_epsilon_nodim ) {
			num_intersection++ ;
			X_array   [ FRONT_FACE ] = X_tmp ;
			t_X_array [ FRONT_FACE ] = t_X_tmp ;

		} else if ( prod > epsilon_nodim        ) {
			num_intersection++ ;
			X_array   [ BACK_FACE ] = X_tmp ;
			t_X_array [ BACK_FACE ] = t_X_tmp ;
		} 

		if( num_intersection >= 2 ) { return num_intersection ; }
	}



		//---------------------------------------//
		//----- side facet 3  investigation -----//
		//---------------------------------------//
		//---------- (s3_1) vertex
	vertex_position_p[0] = &vertex_position[2] ;
	vertex_position_p[1] = &vertex_position[3] ;
	vertex_position_p[2] = &vertex_position[7] ;
	vertex_position_p[3] = &vertex_position[6] ;

		//---------- (s3_2) normal vector
	normal_vector( 0.0, 1.0, 0.0 ) ;

		//---------- (s3_3) intersection
	status = \
	 Tool3D::isLineIntersectedWithPolygon(	A                 ,  B          , 
						vertex_position_p ,  FOUR       ,
						&normal_vector    ,
						&X_tmp              , 
					  	&t_X_tmp            , 
						epsilon           ,  epsilon_nodim   ) ;
		//---------- (s3_4) see status
	if( status == 1  )
	{ 
		prod =  normal_vector * vec_AB  ;
		if(         prod < minus_epsilon_nodim ) {
			num_intersection++ ;
			X_array   [ FRONT_FACE ] = X_tmp ;
			t_X_array [ FRONT_FACE ] = t_X_tmp ;

		} else if ( prod > epsilon_nodim        ) {
			num_intersection++ ;
			X_array   [ BACK_FACE ] = X_tmp ;
			t_X_array [ BACK_FACE ] = t_X_tmp ;
		} 

		if( num_intersection >= 2 ) { return num_intersection ; }
	}




		//---------------------------------------//
		//----- side facet 4  investigation -----//
		//---------------------------------------//
		//---------- (s4_1) vertex
	vertex_position_p[0] = &vertex_position[0] ;
	vertex_position_p[1] = &vertex_position[1] ;
	vertex_position_p[2] = &vertex_position[5] ;
	vertex_position_p[3] = &vertex_position[4] ;

		//---------- (s4_2) normal vector
	normal_vector( 0.0, -1.0, 0.0 ) ;

		//---------- (s4_3) intersection
	status = \
	 Tool3D::isLineIntersectedWithPolygon(	A                 ,  B          , 
						vertex_position_p ,  FOUR       ,
						&normal_vector    ,
						&X_tmp              , 
					  	&t_X_tmp            , 
						epsilon           ,  epsilon_nodim   ) ;
		//---------- (s4_4) see status
	if( status == 1  )
	{ 
		prod =  normal_vector * vec_AB  ;
		if(         prod < minus_epsilon_nodim ) {
			num_intersection++ ;
			X_array   [ FRONT_FACE ] = X_tmp ;
			t_X_array [ FRONT_FACE ] = t_X_tmp ;

		} else if ( prod > epsilon_nodim        ) {
			num_intersection++ ;
			X_array   [ BACK_FACE ] = X_tmp ;
			t_X_array [ BACK_FACE ] = t_X_tmp ;
		} 

		if( num_intersection >= 2 ) { return num_intersection ; }
	}



		//------------------------------------//
		//------ return num_intersection -----//
		//------------------------------------//
	return num_intersection  ;

} // Tool3D::isLineIntersectedWithBox()



	//----- Tool3D::getAngleRad () 
double	Tool3D::getAngleRad ( const Vector3& A,  const Vector3& B) 
{
	
	double theta  =  acos ( (A * B) / (A.getLength() * B.getLength() ) );
	return theta ;
}

	//----- Tool3D::getAngleDeg () 
double	Tool3D::getAngleDeg ( const Vector3& A,  const Vector3& B) 
{
	double theta  =  acos ( (A * B) / (A.getLength() * B.getLength() ) );
	return  ( theta * 180.0 / FR_D_PI );
}


		//----- rotate point P 
		//.....  axis : (u, v, w ) 
		//.....  angle: a
Vector3	Tool3D::rotate(	const Vector3& P , 
			double u, double v, double w ,
			double theta                  )
{
	double  c = cos( theta ) ;
	double  s = sin( theta ) ;

	double  u2    = u * u    ;
	double  v2    = v * v    ;
	double  w2    = w * w    ;

	double  uv    = u * v    ;
	double  vw    = v * w    ;
	double  wu    = w * u    ;

	// Rotation matrix 
	double R[3][3] ;

	// 0th row
	R[0][0] = u2 + (1. - u2) * c; 
	R[0][1] = uv * (1. - c )  - w * s ;
	R[0][2] = wu * (1. - c )  + v * s ;

	// 1st row
	R[1][0] = uv * (1. - c )  + w * s ;
	R[1][1] = v2 + (1. - v2) * c      ;
	R[1][2] = vw * (1. - c )  - u * s ;

	// 2nd row
	R[2][0] = wu * (1. - c )  - v * s ;
	R[2][1] = vw * (1. - c )  + u * s ;
	R[2][2] = w2 + (1. - w2) * c      ;

	// V2 = R * V1  with  V1 = P
	double V1[3] ; V1[0] = P.x ; V1[1] = P.y ; V1[2] = P.z ; 
	double V2[3] ; 	

	for ( int i = 0 ; i < 3 ; i++ ) {
		V2[i] = 0.0 ;	
		for ( int j = 0 ; j < 3 ; j++ ) {
			V2[i] +=  R[i][j] * V1[j]  ;	
		}
	}

	Vector3 P_rotated ( V2[0],   V2[1],  V2[2] ) ;    

	// return rotated point
	return  P_rotated ;

}



	//----- Tool3D::LinesegmentVsPoint ()
int		Tool3D::LinesegmentVsPoint ( const Vector3& A    ,
                                             const Vector3& B    ,
                                             const Vector3& P    ,
                                             double epsilon_3dim ,
                                             double epsilon_nodim )
{
	// return value 
	int isOnLineAB_and_betweenAB = 0 ; 

	// normalized vectors parallel to directed lines PA and PB  
	Vector3 PA = P - A ; 
	Vector3 PB = P - B ; 

	if(( PA.getLength() < epsilon_3dim ) || ( PB.getLength() < epsilon_3dim )){

		isOnLineAB_and_betweenAB = 1 ;		
	}
	else{
		// See if point P is on the line AB

		PA.selfNormalize();
		PB.selfNormalize();	

		// See if point P on line AB is between points A and B
		if( ( PA * PB ) <= epsilon_nodim ){
			//   A ------ P ------ B
			// P=A ------ P ------ B
			//   A ------ P ------ B=P

			double* F_p ;
			double real_F_p ;
			F_p = &real_F_p ;

			Vector3 BA = B - A ; BA.selfNormalize() ;
			Vector3 normal = ( BA ^ ( BA ^ PA )) ; 
			normal.selfNormalize() ;

			planeVsPoint( normal , A , P , F_p , epsilon_3dim ) ;

			if(( *F_p < epsilon_3dim ) && ( -epsilon_3dim < *F_p )){
				isOnLineAB_and_betweenAB = 1 ;
			}
		}
		else{
			//          A ------ B ------ P 
			//  P ----- A ------ B 

			isOnLineAB_and_betweenAB = 0 ;	
		}
	}
	// return boolean
	return isOnLineAB_and_betweenAB ;
}

	//----- Tool3D::LinesegmentVsLinesegment ()
int		Tool3D::LinesegmentVsLinesegment ( const Vector3& A1   ,
                                           const Vector3& B1   ,
                                           const Vector3& A2   ,
                                           const Vector3& B2   ,
                                           double epsilon_3dim ,
                                           double epsilon_nodim )
{
	int isCross = 0 ;

	Vector3* X_p        ;
	Vector3 real_X_p    ;
	X_p = &real_X_p     ;

	Vector3 AB1 = A1 - B1 ; AB1.selfNormalize() ;
	Vector3 AB2 = A2 - B2 ;	AB2.selfNormalize() ;
	Vector3 n = AB1 ^ AB2 ;

	if( n.getLength() > epsilon_nodim ){
		n.selfNormalize();
		Vector3 N = n ^ AB1 ; N.selfNormalize() ; 
		if( Tool3D::getPlaneLineIntersection( N , A1 , A2 , B2 ,\
                                                      X_p              ,\
                                                      epsilon_3dim     ,\
                                                      epsilon_nodim    )){
			if( Tool3D::LinesegmentVsPoint( A1 , B1 , *X_p , epsilon_3dim , epsilon_nodim )){
				isCross = 1 ;
			}
			else{
				isCross = 0 ;
			}
		}
		else{
			isCross = 0 ;
		}
	}
	else{
		isCross = 0 ;
	}

	return isCross ;
}
