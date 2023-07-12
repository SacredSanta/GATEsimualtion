///////////////////////////////////
///// tool2d.h   1995/08/06   /////
///////////////////////////////////

#if !defined _TOOL2D_H
#define _TOOL2D_H

#include <stdlib.h>	
#include <math.h>	
#include "vector.h"

//-------------------------//
//-----  class Tool2D -----//
//-------------------------//

class Tool2D {
 protected:


 public:
	enum { UNKNOWN = -99 , INDEPENDENT = 0 , INCLUDED = 1, EQUAL = 2 , INCLUDE = 3, CROSS = 4 };

		//----- get parameter "t" for line in 2 dimension.
		//.....  The paremeter t represents line PQ as:
		//.....   |R = |P + t * ( |Q - |P );
		//.....    t = 0: P,  t = 1: Q		
		//.....  Note:
		//.....   It is not checked that if point R is really on line PQ.
	static  double  getLineParameter(	const Vector2& R, 
						const Vector2& P, 
						const Vector2& Q, 
						double epsilon = 1.0e-5 );

		//----- Oritentation of points P, Q, R
		//.....  Arguments: Position vectors of points P, Q and R.
		//.....             Parameter epsilon gives allowed error.
		//.....  Return :
		//.....      +1 --> counter-clock-wise
		//.....      -1 --> clock-wise
		//.....       0 --> colinear
	static  int  orientation (	Vector2& P, Vector2& Q, Vector2& R, 
					double epsilon = 1.0e-6 );

		//----- Point vs  polygon
		//..... See if given point P is inside or on a edge of 
		//..... a convex polygon,
		//.....  vertex[0], ..., vertex[num_point -1] .
		//.....  Parameter epsilon is allowed error.
		//.....  Return :
		//.....  +1  if P is inside or on edge of the polygon
		//.....   0  if P is outside of of polygon
	static  int  isPointInsideOrOnEdgeOfPolygon( Vector2& P,  Vector2* vertex, int num_point, double epsilon = 1.0e-6 );

		//----- Point vs  polygon
		//..... See if given point P is inside a convex polygon,
		//.....  vertex[0], ..., vertex[num_point -1] .
		//.....  Parameter epsilon is allowed error.
		//.....  Return :
		//.....  +1  if P is inside  the polygon
		//.....   0  if P is outside or on a edge of the polygon
	static  int  isPointInsidePolygon( Vector2& P,  Vector2* vertex, int num_point, double epsilon = 1.0e-6 );	

		//----- investigate positioon of polygon {P[i]} relative to directed line AB
		//.....  Return:	 1	if positive (left)  side
		//.....			 0	if on line ( polygon is flat )	
		//.....			-1	if negative (right) side 	
		//.....			 2	if intersected
		//.....  This function can be applied to both convex and concave polygons
	static  int  lineVsPolygon(	Vector2& A , Vector2& B, 
				Vector2* P, int num_point, 
				double epsilon = 1.0e-6 ) ;


		//----- Relative position between polygon {P[i]} and polygon {Q[i]}
		//.....  Return:	  0	P is independent of Q
		//.....			  1	P is totally included by Q
		//.....			  2	P = Q 
		//.....			  3	P totally includes Q
		//.....                   4     P and Q intersect
		//.....  This function can be applied only to convex polygons
	static  int  polygonVsPolygon( Vector2* P,  int numP, Vector2* Q,  int numQ, double epsilon = 1.0e-6 );

		//----- Rough independency test
		//.....  Return:	 1	P is independent of Q
		//.....			 2	P totally includes Q or P = Q
		//.....			 0	P is intersected with Q, 
		//.....                         or P is totally inluded by Q
		//.....  		 	or P is independent of Q
		//.....  This function can be applied only to convex polygons
		//.....  Note that independency may exist for return value 0.
	static  int  polygonVsPolygon_sub( Vector2* P,  int numP, Vector2* Q,  int numQ, double epsilon = 1.0e-6 );


		//----- Get an intersection position between polygon {P[i]} and polygon {Q[i]}
		//.....  Arg:		X_array[]: returned intersection position
		//.....  		edgeP_array[], edgeQ_array[]: returned edge labels of intersection
		//.....  		tP_array[],    tQ_array[]: returned line parameters of intersection
		//.....			max_number of intersection needed ( default is 1 )
		//.....  Return:	  number of intersections
	static  int	getPolygonIntersection( Vector2* P,  		int numP, 
						Vector2* Q,  		int numQ, 
						Vector2* X_array , 
						int*	edgeP_array,	int* edgeQ_array, 
						double* tP_array,	double* tQ_array,  
						int	max_intersection = 1 , 
						double epsilon = 1.0e-6                );


		//-----  Solve simultaneous linear equations of rank 2: 
		//.....     a x + b y = e 
		//.....     c x + d y = f 
		//.....
		//.....  Return: 1  if  solution exists 
		//.....          0  if no solution exist
		//.....  Arguments: a, b, e, c, d, f --> coefficients
		//.....             *solution        --> solution ( NULL if  0 is returned)
		//.....             epsilon          --> error
		//....
	static  int  linearEq2(	double a, double b, double e, \
			double c, double d, double f, \
			Vector2*	solution, \
			double		epsilon = 1.0e-6);


		//----- Tool2D::linesegVslineseg ()
		//.....  Get intersection of two linesegments,
		//.....  P1--P2 and PP1--PP2.
		//.....  Intersection point is returned to *solution_p.
		//.....  Line parameters of intersection point is, 
		//.....  if any, returned to t_P1P2 ( for line P1P2 ) and 
		//.....  t_PP1PP2 ( for line PP1PP2), respectively.
		//.....
		//.....  Return:	 
		//.....		1   if intersection exists on linesegments,
		//.....             (0 <= t_P1P2 <=1, 0 <= t_PP1PP2 <=1 ), 
		//.....		0   if two lines are parallel or det = 0 for other reasons,
		//.....		    e.g length of a given linesegment is zero,
		//.....		-1  Lines P1P2 and PP1PP2 intersect,
		//.....              but the intersection point is 
		//.....             outside a linesegment.
	static  int  linesegVsLineseg(	Vector2& P1,  Vector2& P2,
					Vector2& PP1, Vector2& PP2,
					Vector2*  solution_p,
					double*   t_P1P2, double* t_PP1PP2,
					double epsilon = 1.0e-6 );

		//----- Tool2D::halfLineVsPolygon ()
		//..... Arguments:
		//.....  P: starting point of halfline PQ
		//.....  Q: another point on  halfline PQ
		//....   vertex    : vertex positions of polygon
		//....   num_vertex: number of vertices of polygon
		//....	 intersection_p: intersection if exists, 
		//.....  	     If plural intersection exist,
		//.....              the one for (non-negative) minimus *t is returned.
		//.....  t_hl:   line paramater of intersection on PQ   ( t = 0 for P and t = 1 for Q )
		//.....  t_edge: line paramater of intersection on edge
		//.....  edge_label: integer label of edge where intersection occurs,
		//.....              and -1 if no solution exists
		//.....  Return:	 
		//.....  1  if halfline PQ intersects with polygon
		//.....  0  if halfline PQ does not intersects with polygon
	static  int	halfLineVsPolygon(	Vector2& P,  Vector2& Q,
						Vector2*  vertex , int num_vertex ,
						Vector2*  intersection_p,
						double*   t_hl_p, 
						double*   t_edge_p, 
						int*      edge_label_p,
						double    epsilon );

	static  int	halfLineVsPolygon(	Vector2&  P,  Vector2& Q,
						Vector2** vertex , int num_vertex ,
						Vector2*  intersection_p,
						double*   t_hl_p, 
						double*   t_edge_p, 
						int*      edge_label_p,
						double    epsilon );


		//----- Tool2D::halfLineVsLineseg ()
		//..... Arguments:
		//.....  P: starting point of halfline PQ
		//.....  Q: another point on  halfline PQ
		//....   A: starting point of linesegment AB
		//....   B: ending   point of linesegment AB
		//....	 intersection_p: intersection if exists, 
		//.....  t_hl_p:      line paramater of intersection on halfline PQ   
		//.....  t_lineseg_p: line paramater of intersection on linesegment AB 
		//.....
		//.....  Return:	 
		//.....  1  if halfline PQ intersects with linesegment
		//.....  0  if halfline PQ does not intersects with linesegment
	static  int	halfLineVsLineseg(	Vector2& P,  Vector2& Q,
						Vector2& A,  Vector2& B,
						Vector2*  intersection_p,
						double*   t_hl_p, 
						double*   t_lineseg_p, 
						double    epsilon );


		//----- get number of convex vertices in a polygon
	static	int	getNumZeroLengthEdge(	Vector2* P, int numP,
						double epsilon = 1.0e-6 );

		//----- get number of convex vertices in a polygon
	static	int	getNumVertexOfGivenOrt (	Vector2* P, int numP , 
							int	ort_type     ,
							double epsilon = 1.0e-6 );

		//----- get number of convex vertices in a polygon
	static	int	getNumConvexVertex(	Vector2* P, int numP, 
						double epsilon = 1.0e-6 );


		//----- get number of convex vertices in a polygon
	static	int	getNumOrtZeroVertex(	Vector2* P, int numP, 
						double epsilon = 1.0e-6 );

		//----- get number of convex vertices in a polygon
	static	int	getNumConcaveVertex(	Vector2* P, int numP, 
						double epsilon = 1.0e-6 );

		//----- get a point inside polygon
		//.....  num_vertex = 3: 
		//.....		center = 0.333333 * ( vertex2d[0] + vertex2d[1] + vertex2d[2] ); 
		//...... num_vertex > 3: 
		//.....		center = 0.5 * ( vertex2d[0] + vertex2d[ num_vertex / 2 ] ) ;
	static   Vector2  getRoughPolygonCenter( Vector2* vertex2d, int num_vertex ) ;



		//----- Tool2D::directedLineVsConvexPolygon ()
		//..... Arguments:
		//.....  P: a       point on directed line PQ
		//.....  Q: another point on directed line PQ
		//....   vertex    : vertex positions of polygon
		//....   num_vertex: number of vertices of polygon
		//....	 intersection_array[0]: intersection (if exists) 
		//.....                   of the smaller  line parameter
		//....	 intersection_array[1]: intersection (if exists) 
		//.....                   of the larger  line parameter
		//.....  t_line_array[i]:   line paramater of intersection i ( = 0,1 ) 
		//.....  t_edge_array[i]: line paramater of intersection i ( = 0,1 ) on edge
		//.....  edge_label_array[i]: integer label of edge 
		//.....  where intersection i ( = 0,1 ) occurs,
		//..... Return:	 
		//.....  	number of intersection
	static  int	directedLineVsConvexPolygon(	Vector2&	P,  
							Vector2&	Q,
							Vector2*	vertex , 
							int		num_vertex ,
							Vector2*	intersection_array,
							double*		t_line_array, 
							double*		t_edge_array, 
							int*		edge_label_array,
							double		epsilon = 1.0e-6 );


		//----- max &  min of components
	static  void	getBoundingBox( Vector2*  P, int num_point, 
					double*		left ,
					double*		bottom ,
					double*		right ,
					double*		top );

		//----- Tool2D::getBoundingDiamond()
	static	void	getBoundingDiamond(	Vector2*  P, int num_point, 
						double*		xmin ,
						double*		xmax ,
						double*		ymin ,
						double*		ymax ,
						double*		umin ,
						double*		umax ,
						double*		vmin ,
						double*		vmax  );


		//----- normalVector to linesegment PQ
	static  Vector2	getLeftNormalVector( Vector2  P, Vector2  Q ) ; 


		//----- Tool2D::isLeftToRight()
		//..... Return: 
		//.....  1 (-1) , if directed linesegment AB intersects 
		//.....  with directed line PQ, 
		//.....  and direction of  AB is from left to right
		//.....   ( right to left ) of PQ.
		//.....  0,  if  linesegment AB does not intersect with
		//.....  line AB.
	static	int isLeftToRight(	Vector2&  P, Vector2&  Q , 
					Vector2&  A, Vector2&  B  ) ;

		//----- get the following position
		//..... R + distance * t_PQ 
		//..... where t_PQ is the unit vector in direction
		//..... of line PQ
	static	Vector2	
	getPosition(	const	Vector2&	R, 
			double			distance ,
			const	Vector2&	tangent_vector );

		//----- get position R = P + t ( Q - P )
	static Vector2	getPositionOnLine( const Vector2& P, const Vector2& Q, double t )
	{
		return ( P + t * ( Q - P ) );
	}


} ; // Tool2D 


//----------------------------------------------------//
//-----  inline member functions of class Tool2D -----//
//----------------------------------------------------//


#endif
//////////////// end of tool2d.h
