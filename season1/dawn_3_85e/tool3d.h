/////////////////////////////////
///// tool3d.h  1995/11/14  /////
/////////////////////////////////

#if !defined _TOOL3D_H
#define _TOOL3D_H

#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"


//----- CLASS Tool3D
class Tool3D {

 public:

		//----- constant
	enum { UNKNOWN = -99 , NEGATIVE_SIDE = -1 , ON_PLANE = 0 , POSITIVE_SIDE = 1, CROSS_WITH_PLANE = 2 };	


		//----- constructor
	Tool3D () { ; }

		//----- getNormalVector()
		//.....  Get normal vector of polygon with given vertex points.
		//.....  Arguments: point_vector --> vertex points 
		//.....             num_point    --> numver of given vertex points
		//.....  Return: obained normal vector
	static Vector3 getNormalVector( Vector3*  point_vector, int num_point );
	static Vector3 getNormalVector( Vector3** point_vector, int num_point );

		//----- planeVsPoint()
		//.....  Arguments: 
		//.....  		normal: normal vector of plane S
		//.....         P_on  : a point on plane S 
		//.....         P     : a point in 3D space
		//.....         F     : signed distance between S and P
		//.....         epsilon: small positive value ;
		//.....
		//.....  Return:  POSITIVE_SIDE =  1  if point P is in the positive side of plane S
		//.....        :  ON_PLANE      =  0  if point P is just on plane S
		//.....        :  NEGATIVE_SIDE = -1  if point P is in the minus side of plane S
		//.....
		//.....  "On plane S" means that distance(P, S) < epsilon .
		//.....  returnValue * distance( P, S ) is returned to *F_p .
	static	int  planeVsPoint( Vector3 normal, Vector3 P_on, Vector3 P, double* F_p, double epsilon = 1.0e-6 ) ;

		//----- planeVsLinesegment()

		//.....  Arguments: 
		//.....  		normal : normal vector of plane S
		//.....         P_on   : a point on plane S 
		//.....         P      : array of points 
		//.....         npoint : number of given points
		//.....         epsilon: small positive value 
		//.....		*intersect: position of intersection
		//.....	Return:
		//.....		\sum_{i} planeVsPoint( P[i] )
	static	int	planeVsLinesegment( Vector3 normal, Vector3 P_on,  Vector3 P, Vector3 Q,  Vector3* intersect, double epsilon ) ;


		//----- planeVsPolygon()
		//.....  Arguments: 
		//.....		<input>
		//..... 	normal : normal vector of plane S
		//.....         P_on   : a point on plane S 
		//.....         P      : positions of vertices (array) 
		//.....         nvertex: number of vertices of polygon
		//.....         epsilon: small positive value 
		//.....		<output>
		//.....		nvertex_on_plane:
		//.....			number of vertices just on the plane
		//.....			When polygon intersects with plane, 
		//.....			nvertex_on_plane is set to zero.
		//.....
		//.....  Return:  1: if polygon is totally in positive side of S 
		//.....          -1: if polygon is totally in negative side of S 
		//.....           0: otherwide 
		//.....			a) polygon is just on plane      ( *nvertex_on_plane = nvertex )
		//.....			b) polygon intersects with plane ( *nvertex_on_plane = 0 != nvertex )
		//.....      
		//.....  Note: 1) If all vertices of the polygon is on plane S
		//.....           return value is 0.
		//.....        2) If some vertices of polygon is on plane S and 
		//.....           all other vertices are totally in positive 
		//.....           (negative) side of plane S, 
		//.....           return value becomes 1 (-1).
		//.....        
	static int 
	planeVsPolygon( Vector3 normal, Vector3 P_on,  Vector3* P, int nvertex, int* nvertex_on_plane, double epsilon = 1.0e-6 ) ;

	static int 
	planeVsPolygon( Vector3 normal, Vector3 P_on,  Vector3** P, int nvertex, int* nvertex_on_plane, double epsilon = 1.0e-6 ) ;


		//----- get position R = P + t ( Q - P )
	static Vector3	getPositionOnLine( const Vector3& P, const Vector3& Q, double t )
	{
		return ( P + t * ( Q - P ) );
	}

		//----- get intersection X between plane and line AB
		//.....  Return: 1 if normal, 0 if error
	static int	getPlaneLineIntersection( const Vector3& normal , 
						  const Vector3& P_on   ,  
						  const Vector3& A      , 
						  const Vector3& B      ,
						  Vector3* X_p          , 
						  double*  t_X_p        , 
						  double epsilon3d = 1.0e-6    ,
						  double epsilon_nodim = 1.0e-6  );

	static int	getPlaneLineIntersection( const Vector3& normal , 
						  const Vector3& P_on   ,  
						  const Vector3& A     , 
						  const Vector3& B     ,
						  Vector3* X_p         , 
						  double epsilon3d     = 1.0e-6  ,
						  double epsilon_nodim = 1.0e-6   );

	static double	getZmin( Vector3* P, int numP ) ;

	static Vector3	getCenter( Vector3* P, int numP );

		//----- Tool3D::getBoundingBox()
	static void	getBoundingBox(	const Vector3& A, const Vector3& B , 
					Vector3* min_position_p            , 
					Vector3* max_position_p              );

	static double	getBoundingBox(	Vector3* P, int numP , 
					Vector3* min_position_p ,
					Vector3* max_position_p );


		//----- see if point P is inside 3D polygon
		//.....  Return: TRUE (1) , or FALLSE (0)
		//.....  If normal_vector_p is NULL, 
		//.....  normal vector is automatically calculated
		//.....  inside the function.
	static int	
	isPointInsideOrOnEdgeOfPolygon(	const Vector3&	P           ,
					Vector3_p*  vertex_position ,
					int         num_vertex      ,
					Vector3*    normal_vector_p = NULL ,
					double      epsilon       = 1.0e-6 ,
					double      epsilon_nodim = 1.0e-6  );

		//----- Tool3D::isLineIntesectWithPolygon()
		//.....  Argument:
		//.....   A, B: two points on line
		//.....   vertex_position, num_vertex, normal_vector_p: plane
		//.....   X_p : intersection if any
		//.....   t_X_p: line parameter of intersection if any ( t = 0 for A and t = 1 for B )
		//.....   epsilon: 3D tolerance
		//.....   epsilon_nodim: dimensionless small number
		//.....  Note:
		//.....   If given normal_vector_p is NULL, 
		//.....   normal vector is automatically calculated
		//.....   inside the function.
	static	int	
	isLineIntersectedWithPolygon(	const Vector3&  A               , 
				  	const Vector3&  B               , 
					Vector3_p*	vertex_position ,
					int		num_vertex      ,
					Vector3*	normal_vector_p ,
					Vector3*	X_p             , 
				  	double*		t_X_p           , 
					double		epsilon       = 1.0e-6   ,
					double		epsilon_nodim = 1.0e-6    ) ;


		//----- see if line AB intersects with box
		//..... Return: number of intersection
		//.....  X_array[0]:  intersection with front facet
		//.....  X_array[1]:  intersection with back  facet
		//.....  t_X_array[]:  line parameter of X_array []  on  line AB 
	static  int
	isLineIntersectedWithBox(	const Vector3&  A               , 
				  	const Vector3&  B               , 
				  	const Vector3&  box_min         , 
				  	const Vector3&  box_max         , 
					Vector3*	X_array         , 
				  	double*		t_X_array       , 
					double		epsilon         ,
					double		epsilon_nodim      );


		//----- get angle between vector A and B
		//.....  
	static  double
	getAngleRad ( const Vector3& A,  const Vector3& B) ;  

	static  double
	getAngleDeg ( const Vector3& A,  const Vector3& B) ;  


		//----- rotate point P 
		//.....  axis : (u, v, w ) 
		//.....  angle: theta
	static	Vector3
	rotate(	const Vector3& P , 
		double u, double v, double w ,
		double theta                  );

		//----- LinesegmentVsPoint()
		//.....  Arguments:
		//.....			A, B   : edges   of linesegment 
		//.....			P      : a point to be examined
		//.....
		// Return 
		//  1:         A ----- P ----- B         : P is on line seg AB
		//  1:       P=A ----- P ----- B         : P coincides with A
		//  1:         A ----- P ----- B=P       : P coincides with B
		//  0:         A ------------- B ----- P : P is out of 
		//  0: P ----- A ------------- B         :  line segment AB
		//  0:         A ------------- B         : P is out of line AB 
		//                    P *               
	static	int
	LinesegmentVsPoint( const Vector3& A      ,
                            const Vector3& B      ,
                            const Vector3& P      ,
			    double epsilon_3dim   ,
                            double epsilon_nodim );

	static int
	LinesegmentVsLinesegment( const Vector3& A1   ,
                                  const Vector3& B1   ,
                                  const Vector3& A2   ,
                                  const Vector3& B2   ,
                                  double epsilon_3dim ,
                                  double epsilon_nodim );
}; // Tool3D

////////////// end of tool3d.h
#endif
