////////////////////////////////////
///// tool2d.cc   1995/08/06   /////
////////////////////////////////////

// #define DEBUG_TOOL2D
#define SKIP_COUNTING_IDENTICAL_INTERSECTION


#include "tool2d.h"



	//----- lineVsPolygon()
	//..... Investigate positioon of polygon {P[i]} relative to directed line AB
	//.....  Return:	 1	if positive (left)  side
	//.....			 0	if on line ( polygon is flat )	
	//.....			-1	if negative (right) side 	
	//.....			 2	if intersected
	//.....  This function can be applied to both convex and concave polygons
int 
Tool2D::lineVsPolygon(	Vector2& A , Vector2& B, 
			Vector2* P, int num_point, 
			double epsilon ) 
{
		//----- local variables
	int		i  ;		// loop variable
	int		status = 0 ;	// return value of this function
	int		side ;		// return value of orientation()
	int		num_plus_side  = 0 ;
	int		num_minus_side = 0 ; 
					// num of vertices in positive (left) and 
					// negative (minus) side of line AB
	int		num_online     = 0 ;  
					// num of vertices just on line

		//----- error checking 
	if( num_point < 3 ) {
		cerr << "ERROR: too few vertices for a polygon " ;
		cerr << "( Tool2d::lineVsPolygon(), 1)\n";
		exit(1) ;
	}


		//----- investigate position of vertices relateive to
		//..... given line
	for( i = 0 ; i < num_point ; i++ ) {
			//----- see position of P[i] relative to line AB
			//.....  +1: plus  side ( left  side )
			//.....   0: on line 
			//.....  -1: minus side ( right side )
		side  = orientation( P[i], A, B, epsilon ) ;

			//----- count num of vertices existing in each side of line
		switch ( side ) {
			case 1: 
				num_plus_side++  ;  break ;
			case 0: 
				num_online++     ;  break ;
			case -1: 
				num_minus_side++ ;  break ;
			default:
					//----- error 
				cerr << "ERROR:" ;
				cerr << "( Tool2d::lineVsPolygon() ) 3 \n";
				exit(1);
		}

			//----- intersect ?
		if( num_plus_side > 0 && num_minus_side > 0 ) {
			break ;	
		}

	} // for(i)


		//-----  decide in which side of line AB, the given polygon exists
	if       ( num_plus_side > 0 && num_minus_side > 0 ) {
			//----- polygon intersects with line
		status = 2 ;
	} else if( num_plus_side > 0 && num_minus_side == 0 ) {
			//----- polygon is in positive side of plane
 		status =  1 ;
	} else if ( num_plus_side == 0 && num_minus_side > 0 ) {
			//----- polygon is in negative side of plane
 		status =  -1;
	} else if( num_plus_side == 0 && num_minus_side == 0 && num_online == num_point ){
			//----- polygon is just on the plane
		status =  0 ;

#if defined DEBUG_TOOL2D
		cerr << "WARNING: All vertices are on a line or zero-length line? " ;
		cerr << "( Tool2d::lineVsPolygon(), 4)\n";
#endif

	} else {
			//----- error 
		cerr << "ERROR: wrong data for a polygon " ;
		cerr << "( Tool3d::planeVsPolygon(), 4)\n";
		exit(1);
	}
	
		//----- return
	return status ;

} // Tool2D::lineVsPolygon()


	//-----Tool2D::polygonVsPolygon_sub()
	//.....  Investigate position of polygon {P[i]} 
	//.....  relative to polygon {Q[i]}
	//.....	 Return:	 2	P totally includes Q or P = Q
	//.....  		 1	P is independent of Q
	//.....			 0	P is intersected with Q, 
	//.....                         or P is totally inluded by Q
	//.....				or P is independent of Q.
	//.....  This function can be applied only to convex polygons
	//.....  Note that independency may exist for return value 0.
int Tool2D::polygonVsPolygon_sub( Vector2* P,  int numP, Vector2* Q,  int numQ, double epsilon )
{
		//----- local constants
		//---------- status of Tool2D::lineVsPolygon()
	enum { LEFT = 1, RIGHT = -1, ONLINE = 0, LINE_POLY_INTERSECT = 2 };

		//---------- return value
	enum {	INTERSECT_OR_Q_INCLUDE_P_OR_INDEPENDENT = 0,\
		INDEPENDENT                             = 1, \
		P_INCLUED_OR_EQUAL_Q                    = 2 } ;

		//---------- value of flag_lineVsPolygon
	enum { NO = 0 , YES = 1 };

		//----- local variables
	int	i, i_plus ;	// loop_variable , loop_variable + 1
	int	flag_lineVsPolygon ;
	int	flag_P_include_or_equal_Q ;
	int	num_zero_length_edge_P ;
	int	ort ;
	double	x, y ;
	Vector2	T ;


		//----- see position of polygon Q 
		//..... relative to each line including each edge of polygon P
	flag_P_include_or_equal_Q = YES; // initialization
	num_zero_length_edge_P = 0 ;	 // initialization
	for( i = 0 ; i < numP ; i++ ) { // for each edge of polygon P ...
			//----- i+1 in cyclic way
		i_plus = i + 1 ;  if( i_plus == numP ) { i_plus = 0 ;}

			//----- position of polygon Q relative to edge P[i]P[i+1] 
			//.....  of polygon P
		flag_lineVsPolygon = lineVsPolygon( P[i], P[i_plus], Q , numQ, epsilon );

			//----- investivate position of polygon Q
			//.....  relative to polygon P
		if( flag_lineVsPolygon == RIGHT ) {
				//----- Polygon Q is totally 
				//.....  in right side of line P[i]P[i+1],
			return INDEPENDENT; 

		} else if( flag_lineVsPolygon == ONLINE    ) {
				//----- see if P = Q 
			x = P[ i_plus ].x - P[ i ].x ;
			y = P[ i_plus ].y - P[ i ].y ;
			T( -y, x );		// vector normal to line PQ
			T = P[i] + T ;		// a point not on line PQ
						// "if P != Q" .

			ort = Tool2D::orientation( P[i], P[i_plus ], T , epsilon );

			if( ort == 0 ) {
					//-----  P = Q 
					//......  flag_P gives no information.
				num_zero_length_edge_P++ ;
				continue ;								
			} else {
				return INDEPENDENT; 
				//  flat polygon Q is totally on line P[i]P[i+1].
				// --> Polygons P and Q  are independent
				// --> return 1 
			}

		} else if ( flag_lineVsPolygon == LINE_POLY_INTERSECT ) {
				//----- Line P[i]P[i+1] intersects with polygon Q.
				//.....  Now P does not include or is equal to Q.
			flag_P_include_or_equal_Q = NO; 

		} else if ( flag_lineVsPolygon == LEFT ) {
				//----- Polygon Q is totally 
				//..... in left side of line P[i]P[i+1],
				//..... If this part is executed for all i,
				//..... it means P include or is equal to Q.
			continue ; // do nothing
		} else {
			//----- error 
			cerr << "ERROR: " ;
			cerr << "( Tool3d::polygonVsPolygon_sub() )\n";
			exit(1);
		}
			
	} // for(i)
		

		//----- all edges of polygon P have zero length?
	if( num_zero_length_edge_P == numP ) {
		return INDEPENDENT ;
	}

		//----- If polygon Q is totally in left side of line P[i]P[i+1]
		//.....  for all i, 
		//.....  it means P include or is equal to Q.
	if( flag_P_include_or_equal_Q == YES ) { 
		return  P_INCLUED_OR_EQUAL_Q ;
	} else { // if flag_P_include_or_equal_Q == NO
		return  INTERSECT_OR_Q_INCLUDE_P_OR_INDEPENDENT ;
	}

} // Tool2D::polygonVsPolygon_sub()



	//----- Tool2D::halfLineVsPolygon(.. Vector3* vertex ...)
int Tool2D::halfLineVsPolygon(	Vector2& P,  Vector2& Q,
				Vector2*  vertex , int num_vertex ,
				Vector2*  intersection_p,
				double*   t_hl_p, 
				double*   t_edge_p, 
				int*      edge_label_p ,
				double    epsilon )
{
		//----- local variables
	int  		i , i_plus ;
	int		flag_intersection_found ;
	int		status_tmp ;	 // result of linesegVsLineseg()
	Vector2		solution_tmp ;   // intersection between
	double		t_hl_tmp ;		 //  half line and a vertex
	double		t_edge_tmp ;


		//----- initialize flag to "no intersection"
	flag_intersection_found = 0 ;

		//----- search an intersection for each edge
	for( i = 0 ; i < num_vertex ; i++ ) {
		i_plus = i + 1 ; 
		if( i_plus == num_vertex ) { i_plus = 0 ;}

		status_tmp \
		= linesegVsLineseg( P, Q, vertex[i], vertex[i_plus] , &solution_tmp, &t_hl_tmp, &t_edge_tmp , epsilon );

			//----- Return an intersection between
			//..... halfline PQ and polygon
			//..... In case that plural intersections exist,
			//..... the one for mininum t is returned.
		if (	status_tmp !=0            && \
			t_hl_tmp   > (-epsilon)   && \
			t_edge_tmp > (-epsilon)   && \
			t_edge_tmp < (1.0 + epsilon)   )
		{
			if( flag_intersection_found == 0 || t_hl_tmp < *t_hl_p ){
					//----- reset flag:
					//.....  An intersection is found.
				flag_intersection_found = 1 ;	

					//----- reset output arguments
				*intersection_p = solution_tmp ;
				*t_hl_p   = t_hl_tmp ;
				*t_edge_p = t_edge_tmp ;
				*edge_label_p = i ;
			}
		}
	} // for

		//----- return
		//.....  1  if halfline PQ intersects with polygon
		//.....  0  if halfline PQ does not intersects with polygon
	return		flag_intersection_found  ;

} // Tool2D::halfLineVsPolygon(.. Vector3* vertex ...)



	//----- Tool2D::halfLineVsPolygon(.. Vector3** vertex ...)
int Tool2D::halfLineVsPolygon(	Vector2& P,  Vector2& Q,
				Vector2**  vertex , int num_vertex ,
				Vector2*  intersection_p,
				double*   t_hl_p, 
				double*   t_edge_p, 
				int*      edge_label_p ,
				double    epsilon )
{

		//----- local variables
	int  		i , i_plus ;
	int		flag_intersection_found ;
	int		status_tmp ;	 // result of linesegVsLineseg()
	Vector2		solution_tmp ;   // intersection between
	double		t_hl_tmp ;		 //  half line and a vertex
	double		t_edge_tmp ;


		//----- initialize flag to "no intersection"
	flag_intersection_found = 0 ;

		//----- search an intersection for each edge
	for( i = 0 ; i < num_vertex ; i++ ) {
		i_plus = i + 1 ; 
		if( i_plus == num_vertex ) { i_plus = 0 ;}

		status_tmp \
		= linesegVsLineseg( P, Q, *(vertex[i]), *(vertex[i_plus]) , &solution_tmp, &t_hl_tmp, &t_edge_tmp , epsilon );

			//----- Return an intersection between
			//..... halfline PQ and polygon
			//..... In case that plural intersections exist,
			//..... the one for mininum t is returned.
		if (	status_tmp !=0            && \
			t_hl_tmp   > (-epsilon)   && \
			t_edge_tmp > (-epsilon)   && \
			t_edge_tmp < (1.0 + epsilon)   )
		{
			if( flag_intersection_found == 0 || t_hl_tmp < *t_hl_p ){
					//----- reset flag:
					//.....  An intersection is found.
				flag_intersection_found = 1 ;	

					//----- reset output arguments
				*intersection_p = solution_tmp ;
				*t_hl_p   = t_hl_tmp ;
				*t_edge_p = t_edge_tmp ;
				*edge_label_p = i ;
			}
		}
	} // for

		//----- return
		//.....  1  if halfline PQ intersects with polygon
		//.....  0  if halfline PQ does not intersects with polygon
	return		flag_intersection_found  ;

} // Tool2D::halfLineVsPolygon(.. Vector3** vertex ...)



	//----- Tool2D::halfLineVsLineseg()
	//.....  See if halfline PQ intersects with linesegment AB
int	Tool2D::halfLineVsLineseg(	Vector2& P,  Vector2& Q,
					Vector2& A,  Vector2& B,
					Vector2*     intersection_p,
					double*      t_PQ_p, 
					double*      t_AB_p, 
					double       epsilon )
{
		//----- local variables
	int		flag_intersection_found ;
	int		status ;	 // result of linesegVsLineseg()

		//----- initialize flag to "no intersection"
	flag_intersection_found = 0 ;

		//----- see if halfline PQ intersects with linesegment AB
	status = linesegVsLineseg( P, Q, A, B , intersection_p, t_PQ_p, t_AB_p , epsilon );

		//----- return value
	if (	status !=0             && \
		*t_PQ_p  > (-epsilon)  && \
		*t_AB_p  > (-epsilon)  && \
		*t_AB_p  < (1.0 + epsilon)   )
	{
			//----- reset flag: Intersection is found.
		flag_intersection_found = 1 ;	
	}

		//----- return
		//.....  1  if halfline PQ intersects with polygon
		//.....  0  if halfline PQ does not intersects with polygon
	return		flag_intersection_found  ;

} // Tool2D::halfLineVsLineseg()



	//----- Tool2D::getPolygonIntersection()
int  
Tool2D::getPolygonIntersection( Vector2* P,  		int numP, 
				Vector2* Q,  		int numQ, 
				Vector2* X_array , 
				int*	edgeP_array,	int* edgeQ_array, 
				double* tP_array,	double* tQ_array,  
				int	max_intersection, 
				double epsilon )
{
		//----- local variables
	int		i, j, i_plus, j_plus ;
	int		num_intersection        = 0 ;
	int		flag_intersection_found ;
	double		tP,	tQ ;
	Vector2		X;

		//----- search intersection
	for( i = 0 ; i < numP ; i++ ) { // for each edge of polygon P ...
			//----- i+1 in cyclic way
		i_plus = i + 1 ;  if( i_plus == numP ) { i_plus = 0 ;}

		for( j = 0 ; j < numQ ; j++ ) { // for each edge of polygon Q ...
				//----- j+1 in cyclic way
			j_plus = j + 1 ;   if( j_plus == numQ ) { j_plus = 0 ;}

				//----- calc intersection between
				//..... P[i]P[i+1] and Q[j]Q[j+1] if any 
			flag_intersection_found \
			= linesegVsLineseg( P[i], P[i_plus], Q[j] , Q[j_plus] ,&X, &tP, &tQ, epsilon );

				//----- return intersection to argument
			if( flag_intersection_found == 1 ) {
				X_array [ num_intersection ] = X ;				

				edgeP_array[ num_intersection ] = i ;
				edgeQ_array[ num_intersection ] = j ;

				tP_array[ num_intersection ] = tP ;
				tQ_array[ num_intersection ] = tQ ;

				num_intersection++ ;
			} 

			if( num_intersection >= max_intersection ) { break ; }

		} // for(j)

			//----- all intersection needed are already found?
		if( num_intersection >= max_intersection ) { break ; }
			
	} // for( i )
		

		//----- return number of intersections
	return		num_intersection ;

} // Tool2D::getPolygonIntersection()



	//----- Tool2D::getLineParameter()
double  Tool2D::getLineParameter(	const Vector2& R, 
					const Vector2& P, 
					const Vector2& Q, 
					double epsilon )
{
		//----- local variables
	double  t ;	// return value
	double  dx, dy ;

		//----- calc dx, dy
	dx = fabs( Q.x - P.x ) ;
	dy = fabs( Q.y - P.y ) ;

		//-----
	if ( dx > dy ) {
		if( dx < epsilon ) { 
#if defined DEBUG_TOOL2D
			cerr << "WARNING (Tool2D::getLineParameter() 1 ";
			cerr << "       Return -1.0e6 , anyway\n";
#endif
			return -1.0e-6;

		} else {
			t = ( R.x - P.x ) /  ( Q.x - P.x ) ;
		}
	} else {
		if( dy < epsilon ) { 
#if defined DEBUG_TOOL2D
			cerr << "WARNING (Tool2D::getLineParameter() 2 \n";
			cerr << "       Return -1.0e6 , anyway\n";
#endif
			return -1.0e-6;

		} else {
			t = ( R.y - P.y ) / ( Q.y - P.y ) ;
		}
	}

		//------ return line parameter t 
	return t ;

} // Tool2D::getLineParameter()


	//-----Tool2D::orientation ()
int Tool2D::orientation ( Vector2& P, Vector2& Q, Vector2& R, double epsilon )
{
	//----- local variables
	Vector2		PQ ;	// vector PQ
	Vector2		PR ;	// vector PR
	double		absPQ ;	// |PQ|
	double		absPR ;	// |PR|
	double		D ;	// determinant
	int		ort;	// orientation of P, Q, R ( return value )

	//----- calc vectors PQ and PR
	PQ = Q - P ;
	PR = R - P ;

	//----- Is P very near to Q?
	absPQ = PQ.getLength();	
	if( absPQ < epsilon ) {
	  	ort = 0 ;
		return ort ;	// colinear
	} 

	//----- Is P very near to R?
	absPR = PR.getLength();	
	if( absPR < epsilon ) {
		ort = 0 ;
		return ort ;	// colinear
	}

	//----- calc determinant ( normalized )
	D = ( PQ.x * PR.y - PQ.y * PR.x ) / absPQ /absPR ;

	//----- determine orientation
	if      ( D  >   epsilon )  { ort =  1; } // counterclock-wise
	else if ( D  < - epsilon )  { ort = -1; } // clock-wise
	else                        { ort =  0; } // colinear  

 	//----- return orientation 
	return ort;

} // Tool2D::orientation ()


		//-----Tool2D::polygonVsPolygon()
		//.....  Relative position between polygon {P[i]} 
		//.....  and polygon {Q[i]}
		//.....  Return:	  0	P is independent of Q
		//.....			  1	P is totally included by Q
		//.....			  2	P = Q 
		//.....			  3	P totally includes Q
		//.....                   4     P and Q intersect
		//.....  This function can be applied only to convex polygons
int Tool2D::polygonVsPolygon( Vector2* P,  int numP, Vector2* Q,  int numQ, double epsilon )
{
		//---- local variables
	int flagP, flagQ ;

		//----- position of P relative to Q
	flagP = polygonVsPolygon_sub( P, numP, Q, numQ, epsilon ) ;

		//----- Is P independent of Q?
	if( flagP == 1 ) {	//  polygon P is independent of polygon Q:
				//   decided!
		return 0 ;
	} 

		//----- position of Q relative to P
	flagQ = polygonVsPolygon_sub( Q, numQ, P, numP, epsilon ) ;

		//----- Is Q independent of P?
		//....... May become true when flag_P = 2.
	if( flagQ == 1 ) {	//  polygon Q is independent of polygon P:
				//   decided!
		return 0 ;
	} 

		//----- Decide relative positions of P and Q to each other.
		//.....  Here P and Q are not independent.
	if     ( flagP == 0 && flagQ == 2 ) { return 1 ; } // P is totaly includED by Q
	else if( flagP == 2 && flagQ == 2 ) { return 2 ; } // P = Q
	else if( flagP == 2 && flagQ == 0 ) { return 3 ; } // P totaly includes Q
	else if( flagP == 0 && flagQ == 0 ) { return 4 ; } // P and Q intersect
	else { 
		cerr << "ERROR (Tool2D::polygonVsPolygon()\n";
		cerr << "     flagP = " << flagP << endl;
		cerr << "     flagQ = " << flagQ << endl;
		exit(1);
	}

		//----- return dummy ( this part is never executed )
	return -1 ;

} // Tool2D::polygonVsPolygon()


	//----- Tool2D::linearEq2()
int
Tool2D::linearEq2(	double a, double b, double e, \
			double c, double d, double f, \
			Vector2*	solution, \
			double		epsilon )
{
		//----- local variables
	double det;
	int status ;

		//----- 2 x 2 determinant 
	det = a * d - b * c ;

		//----- calc solution and status
	if ( -epsilon < det && det < epsilon ) {// if det = 0 ..
		solution = NULL ;
		status   = 0;	// no solution, 
	} else {				// if det != 0 ...
		solution->x = ( d * e - b * f ) / det;
		solution->y = ( a * f - c * e ) / det;
		status = 1;  	// A solution (solution.x, solution.y ) 
				//  exists.
	}

		//----- return
	return status ;

} // Tool2D::linearEq2()


	//----- Tool2D::linesegVsLineseg ()
int 
Tool2D::linesegVsLineseg (	Vector2& P1,  Vector2& P2, 
				Vector2& PP1, Vector2& PP2, 
				Vector2*  solution_p, 
				double*   t_P1P2, double* t_PP1PP2, 
				double epsilon )
{
		//----- local constants
		//---------- return value
	int   status ;

		//---------- line parameters
	double 	t ;  // for line P1--P2 ;
	double 	tt ; // for line PP1--PP2 ;

		//---------- coefficients of linear equations
		//.....  a x + by = e 
		//.....  c x + dy = f 
	double  a, b, e;	
	double  c, d, f;	

		//----- calc coefficiants for equation of line P1--P2,
		//..... P1 = ( x1, y1 ) , P2 = ( x2, y2 )
		//.....  (y1 - y2)x + (x2 - x1)y = x2 y1 - x1 y2  
		//.....      a            b              e
	a =  P1.y - P2.y ;
	b =  P2.x - P1.x ;
	e =  P2.x * P1.y - P1.x * P2.y ;

		//----- calc coefficiants for equation of line PP1--PP2
		//.....  PP1 = ( xx1, yy1 ) , PP2 = ( xx2, yy2 )
		//.....  (yy1 - yy2)x + (xx2 - xx1)y = xx2 yy1 - xx1 yy2
		//.....       c            d              f
	c =  PP1.y - PP2.y ;
	d =  PP2.x - PP1.x ;
	f =  PP2.x * PP1.y - PP1.x * PP2.y ;

		//----- TEST 1: Are two linesegments are parallel, 
		//..... |P1--P2| == 0 , or |PP1--PP2| == 0 ?
	if ( !linearEq2 ( a, b, e,  c, d, f,  solution_p ) ) { 
					
			//----- no solution 
		status     = 0 ;

	} else { // in case that solution_p != NULL ...

			//----- get line parameters of intersection
		t  = getLineParameter( *solution_p ,  P1 , P2  ) ;
					// for line P1--P2
		tt = getLineParameter( *solution_p , PP1,  PP2 ) ;
					// for line PP1--PP2

			//----- return line parameters
		*t_P1P2 = t ;		*t_PP1PP2 = tt ; 

			//----- Where is the intersection, on a line or outside lines?
		if(        ( t  > ( 0.0 - epsilon ) ) 
			&& ( t  < ( 1.0 + epsilon ) )
			&& ( tt > ( 0.0 - epsilon ) )
			&& ( tt < ( 1.0 + epsilon ) )
		) {
			status  = 1 ;	// intersection on a line
		} else {
			status = -1 ;	// intersection outside lines
		}

	} 

		//----- return status
	return status ;		


} // Tool2D::linesegVsLineseg()


	//----- Tool2D::isPointInsideOrOnEdgeOfPolygon()
int  
Tool2D::isPointInsideOrOnEdgeOfPolygon( Vector2& P,  Vector2* vertex, int num_point, double epsilon )
{

		//----- local variables		
	int		status ;	// return value
	int		flag_right , flag_left, flag_up, flag_down ;
	int		edge_label;
	double		t_hl,  t_edge ; 
	Vector2		X ;
	Vector2		right , left, up, down ;

		//----- max-min test
	double		P_u = P.y + P.x ;
	double		P_v = P.y - P.x ;
	double		xmin, xmax, ymin, ymax, umin, umax, vmin, vmax ; 
	getBoundingDiamond(	vertex , num_point , 
				&xmin, &xmax, &ymin, &ymax, &umin, &umax, &vmin, &vmax );
	if( P.x  < ( xmin - epsilon ) ) { return 0 ; } // -x 
	if( P.x  > ( xmax + epsilon ) ) { return 0 ; } // +x 
	if( P.y  < ( ymin - epsilon ) ) { return 0 ; } // -y 
	if( P.y  > ( ymax + epsilon ) ) { return 0 ; } // +y 	
	if( P_u  < ( umin - epsilon ) ) { return 0 ; } // -u 
	if( P_u  > ( umax + epsilon ) ) { return 0 ; } // +u 	
	if( P_v  < ( vmin - epsilon ) ) { return 0 ; } // -v 
	if( P_v  > ( vmax + epsilon ) ) { return 0 ; } // +v 	


		//----- right, left, upward, downward point relative to P
	right.x = P.x + 1.0 ;		right.y = P.y ;
	left.x  = P.x - 1.0 ;		left.y  = P.y ;

	up.x     = P.x ;		up.y     = P.y + 1.0 ;
	down.x   = P.x ;		down.y   = P.y - 1.0 ;


		//----- half line to right direction and polygon
	flag_right   = halfLineVsPolygon(	P, right, vertex, num_point, 
						&X,  &t_hl,  &t_edge, &edge_label,  epsilon ) ;

		//----- half line to left direction and polygon
	flag_left    = halfLineVsPolygon(	P, left, vertex, num_point, 
						&X,  &t_hl,  &t_edge, &edge_label,  epsilon ) ;

		//----- half line to upward direction and polygon
	flag_up     = halfLineVsPolygon(	P, up, vertex, num_point, 
						&X,  &t_hl,  &t_edge, &edge_label,  epsilon ) ;

		//----- half line to downward direction and polygon
	flag_down  = halfLineVsPolygon(		P, down, vertex, num_point, 
						&X,  &t_hl,  &t_edge, &edge_label,  epsilon ) ;


		//----- calc status
	if( flag_right == 1 && flag_left   == 1 ) { 
		status = 1 ; 
	} else if ( flag_up   == 1 && flag_down == 1 ) { 
		status = 1 ; 
	} else {
		status = 0 ;
	}


		//----- return status 
		//.....  1 if P inside or on a edge of polygon
		//.....	 0 if P outside polygon
	return status ;

} // Tool2D::isPointInsideOrOnEdgeOfPolygon()


	//----- Tool2D::isPointInsidePolygon()
int  
Tool2D::isPointInsidePolygon( Vector2& P,  Vector2* vertex, int num_point, double epsilon )
{

		//----- local variables		
	int		status ;	// return value
	int		flag_right , flag_left, flag_up, flag_down ;
	int		edge_label;
	double		t_hl,  t_edge ; 
	Vector2		X ;
	Vector2		right , left, up, down ;

		//----- max-min test
	double		P_u = P.y + P.x ;
	double		P_v = P.y - P.x ;
	double		xmin, xmax, ymin, ymax, umin, umax, vmin, vmax ; 
	getBoundingDiamond(	vertex , num_point , 
				&xmin, &xmax, &ymin, &ymax, &umin, &umax, &vmin, &vmax );
	if( P.x  < ( xmin - epsilon ) ) { return 0 ; } // -x 
	if( P.x  > ( xmax + epsilon ) ) { return 0 ; } // +x 
	if( P.y  < ( ymin - epsilon ) ) { return 0 ; } // -y 
	if( P.y  > ( ymax + epsilon ) ) { return 0 ; } // +y 	
	if( P_u  < ( umin - epsilon ) ) { return 0 ; } // -u 
	if( P_u  > ( umax + epsilon ) ) { return 0 ; } // +u 	
	if( P_v  < ( vmin - epsilon ) ) { return 0 ; } // -v 
	if( P_v  > ( vmax + epsilon ) ) { return 0 ; } // +v 	

		//----- right, left, upward, downward point relative to P
	right.x = P.x + 1.0 ;		right.y = P.y ;
	left.x  = P.x - 1.0 ;		left.y  = P.y ;

	up.x     = P.x ;		up.y     = P.y + 1.0 ;
	down.x   = P.x ;		down.y   = P.y - 1.0 ;


		//----- half line to right direction and polygon
	flag_right   = halfLineVsPolygon(	P, right, vertex, num_point, 
						&X,  &t_hl,  &t_edge, &edge_label,  epsilon ) ;
	if( flag_right == 1 && t_hl < epsilon ) { flag_right = 0 ; }

		//----- half line to left direction and polygon
	flag_left    = halfLineVsPolygon(	P, left, vertex, num_point, 
						&X,  &t_hl,  &t_edge, &edge_label,  epsilon ) ;
	if( flag_left == 1 && t_hl < epsilon ) { flag_left = 0 ; }

		//----- half line to upward direction and polygon
	flag_up     = halfLineVsPolygon(	P, up, vertex, num_point, 
						&X,  &t_hl,  &t_edge, &edge_label,  epsilon ) ;
	if( flag_up == 1 && t_hl < epsilon ) { flag_up = 0 ; }

		//----- half line to downward direction and polygon
	flag_down  = halfLineVsPolygon(		P, down, vertex, num_point, 
						&X,  &t_hl,  &t_edge, &edge_label,  epsilon ) ;
	if( flag_down == 1 && t_hl < epsilon ) { flag_down = 0 ; }


		//----- calc status
	if( flag_right == 1 && flag_left   == 1 ) { 
		status = 1 ; 
	} else if ( flag_up   == 1 && flag_down == 1 ) { 
		status = 1 ; 
	} else {
		status = 0 ;
	}

		//----- return status 
		//.....  1 if P inside polygon
		//.....	 0 if P outside or on edge of polygon
	return status ;

} // Tool2D::isPointInsidePolygon()




	//----- Tool2D::getNumZeroLengthEdge( double epsilon )
int
Tool2D::getNumZeroLengthEdge( Vector2* vertex2D, int num_vertex, double epsilon )
{
		//----- local constants
	int	num_zero_length_edge ;
	int	k, k_plus ;
	double	x, y ;	

			//----- search edge with zero length
	num_zero_length_edge = 0 ;
	for( k = 0 ; k < num_vertex ; k++ ) {
			//----- next vertex label
		k_plus = k + 1 ;
		if( k_plus == num_vertex ) { k_plus = 0  ; }

			//----- calc edge length
		x = vertex2D[ k_plus ].x - vertex2D[ k ].x ;  
		y = vertex2D[ k_plus ].y - vertex2D[ k ].y ;  
		x = fabs(x); y = fabs(y);

			//----- length of edge
		if( x < epsilon && y < epsilon ) {
			num_zero_length_edge++ ;
		}
	} // for(k)

		//----- return number of vertices for the revised polygon
	return num_zero_length_edge ;

} // Tool2D::getNumZeroLengthEdge



	//----- Tool2D::getNumVertexOfGivenOrt( double epsilon )
int
Tool2D::getNumVertexOfGivenOrt(	Vector2* vertex2D  , 
				int num_vertex     , 
				int	ort_type   ,			
				double epsilon )
{
		//----- local constants
	int	num_given_ort ; // return value
	int	k, k_plus , k_minus ;
	int	ort ;

		//----- count vertices with orientation() = 0
	num_given_ort = 0 ; // initialization of return value
	for( k = 0 ; k < num_vertex ; k++ ) {
			//----- next and previous vertex labels
		k_plus  = k + 1 ;
		if( k_plus  == num_vertex ) { k_plus = 0  ; }
		k_minus = k - 1 ;
		if( k_minus == -1 )         { k_minus = num_vertex - 1  ; }

			//----- calc orientation
		ort = orientation(	vertex2D[ k_minus ], 
					vertex2D[ k       ], 
					vertex2D[ k_plus  ], 
					epsilon             );

			//----- increment counter
		if         ( ort_type > 0 ) {
			if(  1 == ort  ) { num_given_ort++ ;} 
		} else if  ( ort_type < 0 ) {
			if( -1 == ort  ) { num_given_ort++ ;} 	
		} else {
			if(  0 == ort  ) { num_given_ort++ ;} 			
		}

	} // for(k)

		//----- return number of vertices for the revised polygon
	return num_given_ort ;

} // Tool2D::getNumVertexOfGivenOrt()



	//----- Tool2D::getNumConvexVertex()
int	
Tool2D::getNumConvexVertex(	Vector2* P, int numP, 
				double epsilon         )
{
	return	getNumVertexOfGivenOrt( P, numP, 1, epsilon );
}


	//----- Tool2D::getNumOrtZeroVertex()
int	
Tool2D::getNumOrtZeroVertex(	Vector2* P, int numP, 
				double epsilon        )
{
	return getNumVertexOfGivenOrt( P, numP, 0, epsilon );
}


	//----- Tool2D::getNumConcaveVertex()
int	
Tool2D::getNumConcaveVertex(	Vector2* P, int numP, 
				double epsilon        )
{
	return getNumVertexOfGivenOrt( P, numP, -1, epsilon );
}


	//----- Tool2D:getRoughPolygonCenter()
Vector2
Tool2D::getRoughPolygonCenter( Vector2* vertex2d, int num_vertex ) 
{
		//----- local variables
	int	middle_label;
	Vector2  center;

		//----- calc rough center 
	if( num_vertex < 3 ) {
		center(0.0, 0.0 ) ;
	} else if( num_vertex == 3 ) {
			//---- averaging vertex positions
		center = 0.333333 * ( vertex2d[0] + vertex2d[1] + vertex2d[2] ); 
	} else {
		middle_label = num_vertex / 2 ;
		center = 0.5 * ( vertex2d[0] + vertex2d[ middle_label ] ) ;
	}

		//----- return obtained rough center
	return center ;

} // Tool2D::getRoughPolygonCenter()



	//----- Tool2D::directedLineVsConvexPolygon()
int	
Tool2D::directedLineVsConvexPolygon(	Vector2&	P,  
					Vector2&	Q,
					Vector2*	vertex , 
					int		num_vertex ,
					Vector2*	intersection_array,
					double*		t_line_array, 
					double*		t_edge_array, 
					int*		edge_label_array,
					double		epsilon  )
{
		//----- local variables
	int		num_intersection = 0 ;
	int		flag_left_to_right_intersection_found =  0 ;
	int		flag_right_to_left_intersection_found =  0 ;
	int  		i , i_plus ;
	int		flag_intersect ;
	int		edge_direction ; 
	const	int	RIGHT_TO_LEFT = -1 ;
	const	int	LEFT_TO_RIGHT =  1 ;
	double		t_line_tmp ;	
	double		t_edge_tmp ;
	Vector2		intersection_tmp ;


		//----- search an intersection for each edge
	for( i = 0 ; i < num_vertex ; i++ ) {
		i_plus = i + 1 ; 
		if( i_plus == num_vertex ) { i_plus = 0 ;}

		flag_intersect \
		= linesegVsLineseg(	P        ,       Q        , 
					vertex[i], vertex[i_plus] , 
					&intersection_tmp, &t_line_tmp, &t_edge_tmp , epsilon );

			//----- store calculated intersection 
		if (	flag_intersect != 0       &&   
			t_edge_tmp > (-epsilon)   && \
			t_edge_tmp < (1.0 + epsilon)   ) 
		{

			edge_direction \
			= Tool2D::isLeftToRight( P, Q, vertex[i], vertex[i_plus] );

				//----- store left-to-right intersection for minumus t_edge
				//...... ( For t_edte = 1 and t_edge = 0, the latter is selected )
			if( LEFT_TO_RIGHT == edge_direction ) {
				if(	( !flag_left_to_right_intersection_found ) || \
					(  flag_left_to_right_intersection_found &&   \
					   t_edge_tmp < t_edge_array[0]          )    \
				) {
					intersection_array[ 0 ] = intersection_tmp ;
					t_line_array      [ 0 ] = t_line_tmp ;
					t_edge_array      [ 0 ] = t_edge_tmp ;
					edge_label_array  [ 0 ] = i ;
					flag_left_to_right_intersection_found =  1 ;
				}


				//----- store right_to_left intersection for minumus t_edge
				//...... ( For t_edte = 1 and t_edge = 0, the latter is selected )
			} else if( RIGHT_TO_LEFT == edge_direction ) {
				if(	( !flag_right_to_left_intersection_found ) || \
					(  flag_right_to_left_intersection_found &&   \
					   t_edge_tmp < t_edge_array[1]          )    \
				) {

					intersection_array[ 1 ] = intersection_tmp ;
					t_line_array      [ 1 ] = t_line_tmp ;
					t_edge_array      [ 1 ] = t_edge_tmp ;
					edge_label_array  [ 1 ] = i ;
					flag_right_to_left_intersection_found =  1 ;
				}

			}


		} // if( flag_intersect ...)


	} // for(i)


#if defined SKIP_COUNTING_IDENTICAL_INTERSECTION
	int		edge_label_array_0_plus ;
	int		edge_label_array_1_plus ;

		//----- error checking for the obtained two intersections
	if (	flag_left_to_right_intersection_found \
		&& \
		flag_right_to_left_intersection_found  ) 
	{
			//----- calc edge labels
		edge_label_array_0_plus = edge_label_array[0] + 1 ; 
		if( edge_label_array_0_plus == num_vertex ) { 
			edge_label_array_0_plus = 0 ;
		}
		edge_label_array_1_plus = edge_label_array[1] + 1 ; 
		if( edge_label_array_1_plus == num_vertex ) { 
			edge_label_array_1_plus = 0 ;
		}


			//----- check if the two intersections are identical
		if(	( edge_label_array[1] == edge_label_array_0_plus ) \
			&&                                                 \
			( t_edge_array [ 0 ] > ( 1.0 - epsilon ) )         \
			&&                                                 \
			( t_edge_array [ 1 ] <  epsilon                  )  ) \
		{
				//----- make only left-to-right intersection remain
			flag_right_to_left_intersection_found = 0 ;
		} 
		else 	if(	( edge_label_array[0] == edge_label_array_1_plus ) \
				&&                                                 \
				( t_edge_array [ 1 ] > ( 1.0 - epsilon ) )         \
				&&                                                 \
				( t_edge_array [ 0 ] <  epsilon                  )  ) \
		{
				//----- make only left-to-right intersection remain
			flag_right_to_left_intersection_found = 0 ;
		}

	}
#endif

		//----- count num_intersection
	num_intersection \
	=   flag_left_to_right_intersection_found + \
	  + flag_right_to_left_intersection_found ;

		//----- return number of intersection
	return		num_intersection  ;

} // Tool2D::directedLineVsConvexPolygon()


		//----- max &  min of components
void	Tool2D::getBoundingBox( Vector2*  P, int num_point, 
				double*		left ,
				double*		bottom ,
				double*		right ,
				double*		top )
{
		//----- initialization
	*left = *right = P[0].x ;		*bottom = *top = P[0].y ;


		//----- calculation
	for( int i = 1; i < num_point ; i++ ) {
		if( P[i].x < *left    ) { *left   = P[i].x ; }
		if( P[i].x > *right   ) { *right  = P[i].x ; }
		if( P[i].y < *bottom  ) { *bottom = P[i].y ; }
		if( P[i].y > *top     ) { *top    = P[i].y ; }
	}

} // Tool2D::getBoundingBox()
	

	//----- Tool2D::getLeftNormalVector()
Vector2	Tool2D::getLeftNormalVector( Vector2  P, Vector2  Q )
{
		//----- local
	Vector2 PQ, normal ;

		//----- vector PQ
	PQ = Q - P ;

		//----- calc normal vector
	normal.x = -1.0 * PQ.y ;
	normal.y = PQ.x ;
	normal = normal.normalize() ;

		//----- return normal vector
	return normal ;

} // Tool2D::getLeftNormalVector()


	//----- Tool2D::isLeftToRight()
int	Tool2D::isLeftToRight(		Vector2&  P, Vector2&  Q , 
					Vector2&  A, Vector2&  B  )
{
		//----- local
	int	status ;
	int	side_of_A, side_of_B ;
	const	int	RIGHT_TO_LEFT = -1 ;
	const	int	NOT_INTERSECT =  0 ;
	const	int	LEFT_TO_RIGHT =  1 ;
	const	int	LEFT	= -1 ;
	const	int	ONLINE	=  0 ;
	const	int	RIGHT	=  1 ;

		//----- calc side 
	side_of_A = orientation( P, A , Q ) ;
	side_of_B = orientation( P, B , Q ) ;

		//----- 
	if         ( side_of_A == LEFT   && side_of_B >= ONLINE  ) {
		status = LEFT_TO_RIGHT ;
	} else 	if ( side_of_A == ONLINE && side_of_B == RIGHT ) {
		status = LEFT_TO_RIGHT ;
	} else if  ( side_of_A == RIGHT  && side_of_B <= ONLINE  ) {
		status = RIGHT_TO_LEFT ;
	} else 	if ( side_of_A == ONLINE && side_of_B == LEFT ) {
		status = RIGHT_TO_LEFT ;
	} else {
		status = NOT_INTERSECT ;
	}

		//----- return normal vector
	return status ;

} // Tool2D::isLeftToRight()



	//----- PostScript::drawLineSegment_vp()
Vector2	
Tool2D::getPosition(	const	Vector2&	R, 
			double			distance ,
			const	Vector2&	tangent_vector )
{
		//----- local
	Vector2		position   ;
	Vector2		t ;

		//----- calc tangent_vector 
		//..... tangent vector in direction of line PQ
	t = tangent_vector.normalize(); 

		//----- end point
	position = R + ( distance * t ) ;

		//----- return calculated position
	return position ;

} // Tool2D::getPosition()



		//----- Tool2D::getBoundingDiamond()
void	
Tool2D::getBoundingDiamond(	Vector2*  P, int num_point, 
				double*		xmin ,
				double*		xmax ,
				double*		ymin ,
				double*		ymax ,
				double*		umin ,
				double*		umax ,
				double*		vmin ,
				double*		vmax  )
{
		//----- local
	double	P_i_u , P_i_v ;

		//----- initialization
	*xmin = *xmax = P[0].x ;	*ymin = *ymax = P[0].y ;
	*umin = *umax = P[0].y + P[0].x ; 
	*vmin = *vmax = P[0].y - P[0].x ; 

		//----- calculation
	for( int i = 1; i < num_point ; i++ ) {
		P_i_u = P[i].y + P[i].x ; 
		P_i_v = P[i].y - P[i].x ; 

		if( P[i].x < *xmin ) { *xmin = P[i].x ; }
		if( P[i].x > *xmax ) { *xmax = P[i].x ; }

		if( P[i].y < *ymin ) { *ymin = P[i].y ; }
		if( P[i].y > *ymax ) { *ymax = P[i].y ; }

		if( P_i_u  < *umin ) { *umin = P_i_u ; }
		if( P_i_u  > *umax ) { *umax = P_i_u ; }

		if( P_i_v  < *vmin ) { *vmin = P_i_v ; }
		if( P_i_v  > *vmax ) { *vmax = P_i_v ; }

	}


} // Tool2D::getBoundingDiamond()
