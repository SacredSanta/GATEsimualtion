//////////////////////////
///// clip_tester.h //////
//////////////////////////

#if !defined  CLIP_TESTER_H
#define       CLIP_TESTER_H


#include "box.h"
#include "perspective.h"
#include "tool2d.h"
#include "vector.h"
#include "parameter.h"


const	double  CLIPPING_VIEWPORT_WIDTH  = VIEWPORT_WIDTH  ;
const	double  CLIPPING_VIEWPORT_HEIGHT = VIEWPORT_HEIGHT * (297.0 -20.0 ) /( 210.0 - 20.0 ) ;
enum { NOT_CLIPPED_OUT = 0, CLIPPED_OUT = 1} ;
enum { NO_CLIPPING = 0 , CLIPPING = 1 , LANDSCAPE_CLIPPING = 2 };

	//----------------------------//
	//----- class ClipTester -----//
	//----------------------------//
class ClipTester {

 public:

			// clipping_mode =  
			// enum {	NO_CLIPPING        = 0 ,
			//		CLIPPING           = 1 ,
			//		LANDSCAPE_CLIPPING = 2 }
	int isBox2DClippedOut (	const Box&  box2d ,
				int clipping_mode     = CLIPPING ) ;

	int isBox3DClippedOut (	const	Box3D&	box3d, 
				Perspective&	camera,
				double		epsilon3d,        
				int		clipping_mode = CLIPPING ) ;
				
} ; // ClipTester




	//----- ClipTester::isBox2DClippedOut()
inline int
ClipTester::isBox2DClippedOut(	const Box& box2d ,
				int clipping_mode )
{ 
		//----- local vairables
	const	double LARGE_VALUE = 1.0e+99 ;
	int	status = CLIPPED_OUT ;

	double		clip_xmax ;
	double		clip_xmin ;
	double		clip_ymax ;
	double		clip_ymin ;

		//----- clipping area
	switch ( clipping_mode ) {
	 case NO_CLIPPING: // no clipping mode
		clip_xmax =   LARGE_VALUE ;
		clip_xmin =  -LARGE_VALUE ;
		clip_ymax =   LARGE_VALUE ;   
		clip_ymin =  -LARGE_VALUE ;
		break ;
	 case CLIPPING: // clipping mode 
		clip_xmax =   0.505 * CLIPPING_VIEWPORT_WIDTH ;
		clip_xmin =  -0.505 * CLIPPING_VIEWPORT_WIDTH ;
		clip_ymax =   0.505 * CLIPPING_VIEWPORT_HEIGHT ;
		clip_ymin =  -0.505 * CLIPPING_VIEWPORT_HEIGHT ;
		break ;
	 case LANDSCAPE_CLIPPING: // landscape clipping mode
		clip_ymax =   0.505 * CLIPPING_VIEWPORT_WIDTH ;
		clip_ymin =  -0.505 * CLIPPING_VIEWPORT_WIDTH ;
		clip_xmax =   0.505 * CLIPPING_VIEWPORT_HEIGHT ;
		clip_xmin =  -0.505 * CLIPPING_VIEWPORT_HEIGHT ;
		break ;
	 default: // = CLIPPING
		clip_xmax =   0.505 * CLIPPING_VIEWPORT_WIDTH ;
		clip_xmin =  -0.505 * CLIPPING_VIEWPORT_WIDTH ;
		clip_ymax =   0.505 * CLIPPING_VIEWPORT_HEIGHT ;
		clip_ymin =  -0.505 * CLIPPING_VIEWPORT_HEIGHT ;
		break ;
	}


		//----- get box2d info
	double left   = box2d.getLeft   () ;
	double bottom = box2d.getBottom () ;
	double right  = box2d.getRight  () ;
	double top    = box2d.getTop    () ;

		//----- do clipping
	if(	left   > clip_xmax ||
		right  < clip_xmin ||
		bottom > clip_ymax || \
		top    < clip_ymin    ) 
	{
		status =  CLIPPED_OUT ; 
	} else {
		status =  NOT_CLIPPED_OUT ; 
	}

		//----- return
		//.....  1: clipped out
		//.....  0: not clipped out
	return status ;

} // ClipTester::isBox2DClippedOut( const Vector2& P_2d, int clipping_mode  )



	//----- ClipTester::isBox3DClippedOut()
inline int
ClipTester::isBox3DClippedOut(	const	Box3D&		box3d     , 
				Perspective&		camera    ,
				double			epsilon3d ,
				int			clipping_mode )

{
		//----- local
	int		status ;
	Vector3 	P_3d ;
	Vector2		P_2d[8] ;
	int		i ;


		//----- calc 2d box on screen
	for( i = 0 ; i < 8 ; i++ ) {

			//----- get a corner of 3d box
		ToolBox3D::GetCorner( i, box3d, P_3d ) ;
//		P_3d = box3d.getCorner(i);

			//----- to screen coordinate
		camera.worldToViewport(	P_3d  ,
					P_2d[i]  , 
					epsilon3d ) ;

	} // for

		//----- get bounding box on screen 
	double		left ;
	double		bottom ;
	double		right ;
	double		top ;

	Tool2D::getBoundingBox(	P_2d, 8 ,
				&left ,
				&bottom ,
				&right ,
				&top );
	
	status = isBox2DClippedOut( Box2D( left, bottom, right, top ) , clipping_mode );


		//----- return
		//.....  1: clipped out
		//.....  0: not clipped out
	return status ;

} // ClipTester::isBox3DClippedOut()


#endif
