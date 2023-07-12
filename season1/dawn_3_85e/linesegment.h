//////////////////////////////////////
///// linesegment.h   1995/05/15 /////
//////////////////////////////////////

#if !defined LINESEGMENT_H
#define LINESEGMENT_H

	//----- include files
#include "linetype.h"
#include "vector.h"
#include "list.h"
#include "perspective.h"
#include "qsort.h"

	//------------------------------//
	//----- class Intersection2D ----//
	//------------------------------//
class	Intersection2D {
 public:
		//----- line paremeter where intersection occures
	double	lineParameter ;

		//----- width of intersecting line
	double	width ;

		//----- constructor and destructor
	Intersection2D () { lineParameter = width = 0.0 ; }
	~Intersection2D() {;}

		//----- operator = 
	void	operator = ( const Intersection2D& rhs ) 
		{
			lineParameter = rhs.lineParameter ;
			width          = rhs.width          ;
		}

} ; // Intersection2D


	//-----------------------------//
	//----- class LineSegment -----//
	//-----------------------------//
class LineSegment {
 public:
		//----- 3D position
	Vector3		begin ;
	Vector3		end   ;
	
		//----- linetype
	LineType	linetype ;

		//----- 2D position
	Vector2		begin2D ;
	Vector2		end2D   ;

		//----- boxel (3D screen coordinate)
	double		xmin, xmax ;
	double		ymin, ymax ;
	double		zmin, zmax ;
	double		umin, umax ;	
	double		vmin, vmax ;	

		//----- intersection in 2-dim
	int		numIntersection2D ;
	Intersection2D*	intersection2D    ;

 public:
	LineSegment()  ;
	LineSegment(	const Vector3& begin_given ,
			const Vector3& end_given   ,
			const LineType& linetype_given ) ;


	~LineSegment() { ; }

		//----- set begin and end
		//..... Return: 0 (err), 1(normal)
	void	setPosition(	const Vector3& begin_given ,
				const Vector3& end_given     ) ;

		//----- set and get linetype
	void		setLineType( const LineType& linetype_given  );
	LineType	getLineType() const { return linetype ; }

		//----- set begin2D and end2D
		//..... Return: 0 (err), 1(normal)
	int	setImage(	Perspective*	camera_p ,  
				double		epsilon3d = 1.0e-6 );

		//----- set intersecton2D and numIntersection2D from list
	void	setIntersection2D( List<Intersection2D>* input_list_p ) ;

		//----- sort Intersection2D in order of lineParameter
	void	sortIntersection2D();

		//----- equal
	void	operator = ( const LineSegment& rhs ) ;

		//----- LineSegment::sortIntersection2D()
	void	getBoundingDiamond(	double*		xmin_p ,
					double*		xmax_p ,
					double*		ymin_p ,
					double*		ymax_p ,
					double*		umin_p ,
					double*		umax_p ,
					double*		vmin_p ,
					double*		vmax_p  );

		//----- get begin and end points
	const	Vector3&	getBegin3D() { return begin ; }
	const	Vector3&	getEnd3D  () { return end   ; }

	const	Vector2&	getBegin2D() { return begin2D ; }
	const	Vector2&	getEnd2D  () { return end2D   ; }

		//----- get linetype
	const	LineType&	getLineType() { return linetype ; }

} ; // LineSegment


#endif
