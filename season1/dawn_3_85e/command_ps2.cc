//////////////////////////
/////  command_ps2.cc  /////
//////////////////////////


#include "command.h" 
#include "message.h"


	//----- Command::drawLine_landscape_PS()
void
Command::drawLine_landscape_PS(	PostScript&		ps            ,
				HidLine&		hidline       ,
				double			blank_scale    )
{
		//----- local 
	int	i , j ;
	int	num_visible_lineseg  = hidline.getNumVisibleLineSegment() ;
	int	numIntersection2D ;
	double		t ;
	double		half_width_vp ;
	double		blank_length_vp, half_blank_length_vp ;
	Intersection2D	intersection2D ;
	Vector2		begin2D , end2D ;
	Vector2		P, Q , Q_tmp ;
	Vector2		tangent, tangent_rev ; 
	LineType*	linetype_p ;
	double t_P, t_Q ;
	Vector2 P_tranc, Q_tranc ;
	Vector2 P_bak ;

		//----- no lineseg to be drawn?
	if ( num_visible_lineseg  == 0 ) {
		return ;
	} 

		//----- draw linesegments
	for( i = 0 ; i < num_visible_lineseg ; i++ ) 	{
			//----- begin & end of linesegment
		begin2D 	= hidline.getBegin2DVisible(i) ;
		end2D 		= hidline.getEnd2DVisible(i) ;

			//----- direction of linesegment
		tangent		= end2D - begin2D ;
		tangent		= tangent.normalize();
		tangent_rev	= (-1.0) * tangent ;

			//----- linetype of i-th linesegment
		linetype_p = &(hidline.getLineTypeVisible(i) );

			//----- number of intersected lineseg on screen
		numIntersection2D = hidline.getNumIntersection2DVisible(i) ;

			//----- draw linesegment
		if( numIntersection2D == 0 ) {
			//----- draw whole part of linesegment
			ps.drawLineSegment_landscape_vp(	begin2D, end2D ,
								linetype_p       );
		} else {
				//----- draw each visible part of linesegment
			P = begin2D ; // starting point of first sub lineseg
			for( j = 0 ; j < numIntersection2D ;j++ ) {
				
					//----- backup P
				P_bak = P ;

					//----- get line parameter for intersection point
				hidline.getIntersection2DVisible(i, j , &intersection2D) ;
				t 	= intersection2D.lineParameter ;

					//----- calc blank_length in viewport coordinate
				half_width_vp = 0.5 * (intersection2D.width) * ( ps.getVpOverMm() );
				half_blank_length_vp =  blank_scale * ( ps.getVpOverMm() ) \
							+ half_width_vp ;
				blank_length_vp      = 2.0 * half_blank_length_vp     ;
							

					//----- get end point of current sub linesegment
				Q_tmp = Tool2D::getPositionOnLine( begin2D, end2D, t ) ;
				Q     = Tool2D::getPosition( Q_tmp, half_blank_length_vp , tangent_rev ) ;

					//----- trancated P and Q
				t_P = Tool2D::getLineParameter( P, begin2D, end2D ) ;
				if       ( t_P < 0.0 ) { P_tranc = begin2D ; }
				else if  ( t_P > 1.0 ) { P_tranc = end2D ; }
				else                   { P_tranc = P ; }

				t_Q = Tool2D::getLineParameter( Q, begin2D, end2D ) ;
				if       ( t_Q < 0.0 ) { Q_tranc = begin2D ; }
				else if  ( t_Q > 1.0 ) { Q_tranc = end2D ; }
				else                   { Q_tranc = Q ; }

				if( t_P < t_Q ) { 
						//----- draw current sub linesegment
					ps.drawLineSegment_landscape_vp( P_tranc , Q_tranc, linetype_p ) ;

						//----- get starting point of next sub linesegment
					P = Tool2D::getPosition( Q, blank_length_vp  , tangent );
				} else {

						//----- get starting point of next sub linesegment
					P = Tool2D::getPosition( Q, blank_length_vp  , tangent );
				}

			} // for(j)


				//----- trancated P 
			t_P = Tool2D::getLineParameter( P, begin2D, end2D ) ;
			if       ( t_P < 0.0 ) { P_tranc = begin2D ; }
			else if  ( t_P > 1.0 ) { P_tranc = end2D ; }
			else                   { P_tranc = P ; }

			ps.drawLineSegment_landscape_vp(	P_tranc , end2D, linetype_p ) ;

		} // if--else

	} // for(i)
				
} // Command::drawLine_landscape_PS()


	//----- Command::draw_Hid_landscape_PS()
void
Command::draw_Hid_landscape_PS(	const Parameter&	parameter ,
				PostScript&		ps        , 
				Hid& 			hid       ,
				List<Object>*		object_server_p )
{
#if defined DISPLAY_LOG
		//----- message
	cerr << "-----------------------\n";
	cerr << " Draw  objects \n";
	cerr << "   * landscape mode    \n";
	cerr << "-----------------------\n";
	cerr<< endl << endl;
#endif

		//----- local
	int		i ;
	int		npoly2d = hid.getNumPolygon2D();
	Polygon2D	poly2d ;

		//----- STEP 1: fill polygons in polygon2d[]
	if( parameter.drawing_mode != HID2 ) // not drawing with line
	{
			//----- fill polygons
		for ( i = (npoly2d-1) ; i >= 0 ; i-- ) 
		{
			poly2d = hid[i] ;
			ps.drawfillPolygon_landscape_vp( poly2d.vertex2D ,  poly2d.numVertex, poly2d.color );
		}
	}
	
		//----- STEP 2: Draw edges of polygons
	if( parameter.drawing_mode != HID2 ) 
	{
		if( parameter.flag_draw_edge ) {
			for ( i = (npoly2d-1) ; i >= 0 ; i-- ) 
			{
				poly2d = hid[i] ;
				ps.drawPolygon_landscape_vp( poly2d.vertex2D ,  poly2d.numVertex, poly2d.edgeType );
			}
		} else {
			// do nothing 
		}

	} else {
		for ( i = (npoly2d-1) ; i >= 0 ; i-- ) 
		{
				//----- current 2D polygon
			poly2d = hid[i] ;

				//----- local
			int		num_vertex = poly2d.getNumVertex() ;
			LineType*	linetype_array_tmp  = new LineType [ num_vertex ];
			ColorType	color = ((*object_server_p)[ poly2d.getObjectLabel() ]).getKd();

				//----- if white, black
			color.whiteToBlack();	

				//----- reset edge color
			for( int j = 0 ; j < num_vertex ; j++ ) {
				linetype_array_tmp[j] = *(poly2d.edgeType[j]);
				if        ( linetype_array_tmp[j].getVisibility() ) {
					linetype_array_tmp[j].setColor( color );
				} 
			}

				//----- 
			ps.drawPolygon_landscape_vp( poly2d.vertex2D ,  num_vertex, linetype_array_tmp );

				//----- delete
			delete [] linetype_array_tmp ;

		}// for

	} // if( parameter.drawing_mode != HID2 ) --- else 

} // Command::draw_Hid_landscape_PS()


	//------ drawVisiblePartOfLineSegment_landscape_PS ()
void 
Command::drawVisiblePartOfLineSegment_landscape_PS ( const	Vector3&	A ,
						   const	Vector3&	B ,
						   const	LineType&	linetype , 
						   Hid&			hid      ,
						   PostScript&		ps ,
						   const Parameter&	parameter )
{
		//----- local
	LineSegment	lineseg( A, B, linetype ) ;
	HidLine		hidline ;
	Perspective*	camera_p = hid.getCamera();

		//----- set imame data of linesegment
	lineseg.setImage( camera_p , parameter.epsilon3d );

		//----- make hidline ready to use
	hidline.addItem_lineSegmentList( lineseg );
	hidline.install();
	hidline.setHid( &hid ); // reset hid
	hidline.doHiddenPartElimination( parameter.epsilon3d , 
					 parameter.epsilon2d  );
		//----- draw
	this->drawLine_landscape_PS(	ps            ,
					hidline      ,
					parameter.blank_scale    );

} // Command::drawVisiblePartOfLineSegment_landscape_PS ()



	//----- Command::displayAxes_landscape_PS()
void	Command::displayAxes_landscape_PS(	PostScript&      ps      ,
					Perspective&     camera  ,
					const Parameter& parameter )
{
	LineType	redline  ( ColorType( 1.0, 0.0, 0.0 ), 0.1 );
	LineType	greenline( ColorType( 0.0, 1.0, 0.0 ), 0.1 );
	LineType	blueline ( ColorType( 0.0, 0.0, 1.0 ), 0.1 );

	double x = 2.0 * parameter.xmax ;
	double y = 2.0 * parameter.ymax ;
	double z = 2.0 * parameter.zmax ;

	Vector3	 O3d( 0.0 , 0.0, 0.0 );
	Vector3  X3d( x   , 0.0, 0.0 );
	Vector3  Y3d( 0.0 , y  , 0.0 );
	Vector3  Z3d( 0.0 , 0.0, z   );

	Vector2  O2d, X2d, Y2d, Z2d ;

	camera.worldToScreen( O3d, O2d, parameter.epsilon3d );
	camera.worldToScreen( X3d, X2d, parameter.epsilon3d );
	camera.worldToScreen( Y3d, Y2d, parameter.epsilon3d );
	camera.worldToScreen( Z3d, Z2d, parameter.epsilon3d );

	ps.drawLineSegment_landscape_vp( O2d, X2d , &redline   );
	ps.drawLineSegment_landscape_vp( O2d, Y2d , &greenline );
	ps.drawLineSegment_landscape_vp( O2d, Z2d , &blueline  );

} // Command::displayAxes_landscape_PS()
