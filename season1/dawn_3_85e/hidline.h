//////////////////////////////////
//////////  hidline.h   //////////
//////////////////////////////////////////////////////////////

#if !defined HIDLINE_H
#define HIDLINE_H

#include <iostream.h>	
#include <stdlib.h>	

#include "linesegment.h" 
#include "hid.h" 
#include "list.h"
#include "vector.h"
#include "linetype.h"


	//-------------------------------//
	//----- class Hid    -----//
	//-------------------------------//
class HidLine {
	
private:
		//---- const
	enum { NOT_HIDDEN2 = -1 , NOT_HIDDEN = 0 , HIDDEN = 1 };
	enum { INDEPENDENT = 0 } ;
	enum {	PLUS_PLUS  , PLUS_ZERO  , PLUS_MINUS , \
		ZERO_PLUS  , ZERO_ZERO  , ZERO_MINUS , \
		MINUS_PLUS , MINUS_ZERO , MINUS_MINUS    }  ;


		//----- all input linesegment (temporary space)
	List<LineSegment>	lineSegmentList ;


		//----- all input linesegment
 	int			numLineSegment ;
	LineSegment*		lineSegment ;

		//----- visible part of linesegments
 	int			numVisibleLineSegment ;
	LineSegment*		visibleLineSegment ;

		//----- instance of Hid class to be used with
	Hid*			hid_p ;

 public:

		//----- constructor
	HidLine( Hid* hid_p_given = NULL ) ;

		//----- destructor 
	~HidLine();

		//----- clear all data
	void	clear();


		//----- reset hid_p
	void	setHid( Hid* hid_p_given ) { hid_p = hid_p_given ; }

		//----- push linesegment to temporart list
	int	addItem_lineSegmentList  ( LineSegment& lineseg ) ;

		//----- set linesegment to vislbleLineSegment
		//.....  Note: old data are removed
	void	setVisibleLineSegment( List<LineSegment>& lineseg_list ) ;

		//----- move items in lineSegmentList to visibleLineSegmenArray[]
		//..... Note : lineSegmentList is cleared
	void	moveLineSegmentListToVisibleLineSegmentArray();


		//----- set numLineSegment and lineSegment, 
		//..... and make lineSegmentList clear.
		//.....  Note:     Old lineSegment[] are cleared
	void	setArray();

		//----- zsort of lineSegment (far_from_eye ---> near_from_eye)
	void	zsortLineSegment();

		//----- set numLineSegment and lineSegment, 
		//..... and make lineSegmentList clear.
		//..... setArray(); lineSegmentList.clear();
	void	install();


		//----- see if linesgement is hidden by polygon
		//.....  RETURN:
		//.....      NOT_HIDDEN    = 0	if  P is not hidden by Q
		//.....      NOT_HIDDEN2   = -1	if  P is not hidden by Q,
		//.....	                          and P.zmax < Q.zmin
		//.....      HIDDEN        =  1	if  P may be hidden by Q
		//.....
		//.....  relation_2d:
		//.....      * One of the followings is returned.
		//.....		  Tool2D::UNKNOWN  , Tool2D::INDEPENDENT,
		//.....           Tool2D::INCLUDED , Tool2D::CROSS       
		//.....      * Tool2D::UNKNOWN is returned only if 
		//.....      * return value is "NOT_HIDDEN" or "NOT_HIDDEN2".
		//.....      * If *relation2d is Tool2D::CROSS, 
		//.....        informations on intersection points
		//.....        are returned two-dim array, 
		//.....        X_array[], t_line_array, t_edge_array, 
		//.....        and edge_lagel_array.
		//.....  X3D_array[2]      : intersection positions in 3-dim.
		//.....  t_line_X3D_array[2] : line parameter of intersection
		//.....  lineseg_side_p: one of { PLUS_PLUS, PLUS_MINUS, MINUS_PLUS, MINUS_MINUS }
		//.....                  The former is side of begin3D relative to plane of polygon,
		//.....                  and the latter is of end3D.
		//.....                  Note that point just on plane is  regarded as PLUS
		//.....
	int	isHidden(	LineSegment*	lineseg_p    ,    
				Polygon3D*	polygon_p    ,    
				int*		relation_2d  ,    
				Vector3*	X3D_array    ,    
				double*		t_line_X3D_array, 
				int*		X3D_side_p     , 
			  	double		epsilon3d = 1.0e-6 , 
				double		epsilon2d = 1.0e-6    );

		//----- devide out visible parts of linesegment
		//.....  Return: number of output linesegment
	int	hideLineSegmentByPolygon(	LineSegment*		lineseg_p    ,    
						Polygon3D*		polygon_p    ,    
						Vector3*		X3D_array    ,    
						double*			t_line_X3D_array , 
						int			X3D_side     ,
						List<LineSegment>*	output_lineseg_list_p ,
			  			double			epsilon3d = 1.0e-6 , 
						double			epsilon2d = 1.0e-6    ) ;

		//----- max min test for 8 direction
		//.....  Return
		//.....		0:  if a is independent of b
		//.....		    on screen.
		//.....		1:  else
	int	maxMinTest(	LineSegment* lineseg_p , 
				Polygon3D*  polygon_p  , 
				double epsilon2d = 1.0e-6 ) ;
	int	maxMinTest(	LineSegment* linesegA_p , 
				LineSegment* linesegB_p , 
				double epsilon2d = 1.0e-6 ) ;
	int	maxMinTest(	LineSegment* lineseg_p , 
				double xmin, double xmax,
				double ymin, double ymax,
				double umin, double umax,
				double vmin, double vmax,
				double epsilon2d = 1.0e-6 )  ;

		//----- Polygon3D::depthOverlapTest()
		//.....  Depth overlapping test
		//.....  Return
		//.....		 1: if (*a)->zmax < (*b)->zmin.
		//.....		-1: if (*b)->zmax < (*a)->zmin.
		//.....		 0: else (ovrlaps in z direction)
		//..... (zmax, zmin are eye coordinate.)
	int	depthOverlapTest(	LineSegment* lineseg_p , 
					Polygon3D*  polygon_p  , 
					double epsilon3d = 1.0e-6 ) ;
	int	depthOverlapTest(	LineSegment* linesegA_p , 
					LineSegment* linesegB_p , 
					double epsilon3d = 1.0e-6 ) ;

		//----- acces functions
	List<LineSegment>&  getLineSegmentList () { return lineSegmentList ; }
	int	getNumLineSegment	(){ return numLineSegment ;}
	int	getNumVisibleLineSegment(){ return numVisibleLineSegment ;}

		//----- get 2D position and linetype of 
		//.....  the index-th linesegment
	Vector2&	getBegin2D  ( int  index );
	Vector2&	getEnd2D    ( int  index );
	LineType&	getLineType ( int  index );

		//----- get 2D position and linetype of 
		//.....  the index-th visible linesegment
	Vector2&	getBegin2DVisible(int	index );
	Vector2&	getEnd2DVisible  (int	index );
	LineType&	getLineTypeVisible( int index );

		//----- get number of Intersection2D for
		//.....  the index-th visible linesegment
	int	getNumIntersection2DVisible (int index );

		//----- get index_intersection2D-th  Intersection2D for
		//.....  the index_visible_lineseg-th visible linesegment
		//.....  to the third argument
		//.....  Return: 1: returned
		//.....          0: not returned for these indices
	int	getIntersection2DVisible (	int index_visible_lineseg , 
						int index_intersection2D  ,
						Intersection2D* intersection2D_p );


		//----- set visibleLineSegment numVisibleLineSegment
		//.....  Return:   numVisibleLineSegment
		//.....  Note:     Old visibleLineSegment[] are deleted
		//.....  Note:     lineSegment[] data are cleared
	int	extractVisiblePart( double epsilon3d = 1.0e-6 , double epsilon2d = 1.0e-6 );
	int	makeAllLinesVisible( double epsilon3d = 1.0e-6 , double epsilon2d = 1.0e-6 );

		//----- perform hidden part elimination
		//.....  Return:   numVisibleLineSegment
	int	doHiddenPartElimination( double epsilon3d = 1.0e-6 , double epsilon2d = 1.0e-6 );
	int	doNotHiddenPartElimination( double epsilon3d = 1.0e-6 , double epsilon2d = 1.0e-6 );
	int	doNotHiddenPartElimination2( double epsilon3d = 1.0e-6 , double epsilon2d = 1.0e-6 );

		//----- calc intersection between linesegment A and B
		//..... if A is hidden by B
		//..... Return:
		//.....		NOT_HIDDEN: if A is hidden by B
		//.....		HIDDEN:     if A is hidden by B
		//.....
		//..... If Return is HIDDEN, information on 2D intersection
		//..... is returned to the 3rd argument
	int	
	getHiddenIntersection(	LineSegment*	linesegA_p   ,    
				LineSegment*	linesegB_p   ,    
				Intersection2D*	intersection_p ,
				double		epsilon3d = 1.0e-6 ,
				double		epsilon2d = 1.0e-6  );

		//----- set Intersection2D instances to visibleLinesegment[]
	void	setIntersection2DToVisibleLineSegment(	double epsilon3d = 1.0e-6 ,
							double epsilon2d = 1.0e-6  );

		//----- getBoundingDiamond()
	int	getBoundingDiamond(	double*		xmin ,
					double*		xmax ,
					double*		ymin ,
					double*		ymax ,
					double*		umin ,
					double*		umax ,
					double*		vmin ,
					double*		vmax );


} ; // Hid



#endif
////////////////////////////// end of hidline.h

