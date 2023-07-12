///////////////////////////
/////  command_gl.cc  /////
///////////////////////////


// #define DEBUG_COMMAND_GL


#include "command.h" 


#if defined USE_OPEN_GL


#include "GLviewport.h"

// Make the line below active to draw edges
// #define USE_STENCIL

	//----- Command::open_GL()
void Command::open_GL(	Perspective& camera , 
			X_Window&          xwin   ,
			FROpenGL&	   gl	  ,
			const Parameter&   parameter ) 
{

		//----- local
	char	window_title[100] ;
	
		//----- window title
	if( parameter.flag_IP_mode == 1 ) {
		strcpy( window_title, "FR SUB Window (Click Button or Hit Key to Proceed!)");
	} else {
		strcpy( window_title, "FR MAIN Window (Click Button or Hit Key to Proceed!)");
	}

///////////////////// BEGIN GL code
		//----- open OpenGL
	if( !gl.isOpen() ){
		//----- close unnecessary devices
		if( xwin.isOpen() ) xwin.close();
		gl.open( &xwin,window_title );
	}

	gl.Clear( ColorType(1.0,1.0,1.0));
	gl.DepthClear();
	gl.SetCamera( camera, Box3D( parameter.xmin, parameter.ymin, parameter.zmin,
					parameter.xmax, parameter.ymax, parameter.zmax ));
//	gl.SetViewport((int)((A4_WIDTH_PT-(A4_WIDTH_PT*1.0*170.0/210.0))/2.0),(int)((A4_HEIGHT_PT-(A4_WIDTH_PT*1.0*170.0/210.0))/2.0),(int)(A4_WIDTH_PT*1.0*170.0/210.0),(int)(A4_WIDTH_PT*1.0*170.0/210.0));
	gl.SetViewport(GL_VIEWPORT_LEFT, GL_VIEWPORT_BOTTOM, GL_VIEWPORT_WIDTH, GL_VIEWPORT_HEIGHT );

	glEnable( GL_STENCIL_TEST );
	glClear( GL_STENCIL_BUFFER_BIT );
	glStencilFunc( GL_ALWAYS, 0, 1 );
	glStencilOp( GL_INVERT, GL_INVERT, GL_INVERT );

	glFinish();
///////////////////// END GL code

} // Command::open_GL()



	//----- Command::close_GL()
void	Command::close_GL( void )
{
//	gl.close();
} // Command::close_GL()


	//----- Command::readDataFile_DrawGL_Clear()
void	
Command::readDataFile_DrawGL_Clear(	const Parameter&	parameter          , 
					List<NameAndValue>&	object_name_server ,
					DataSize&		total_data_size    ,
					Perspective&		camera             ,
					FROpenGL&		gl                  )
{
		//----- message

		//----- local
	int		num_read_object = 0 ;
	Object*		object_p ;
	const	int	HEADER_SIZE = 100 ;
	char		header[HEADER_SIZE];
	NameAndValue	object_name    ;
	FileReader	filereader     ;
	List<Object>	object_server  ;
	List<LineSegment>	lineseg_server ;
	Hid		polygon_server ;	polygon_server.setCamera( &camera );
	HidLine		transparent_polygon_server ;


		//----- make winged_edge structure ?
//	int		mode_winged_edge = parameter.winged_edge_mode ;

		//-----  get vector sum of points
	object_name_server.toTail();
	num_read_object = 0 ;
	while( object_name_server.getItem( object_name ) )
	{
			//----- local
		Object		empty_object ;
		char*	cur_name  = object_name.getName();
		int	cur_label = object_server.getNItem();

			//----- set current name and label
		object_server.push( empty_object ) ;		
		object_p = object_server.getItemAddress();
		object_p->setName ( cur_name  ) ; 
		object_p->setLabel( cur_label ) ; 

#if defined DISPLAY_LOG
		cerr<< endl << endl;
		cerr<< "-----------------------------------------\n";
		cerr << " Reading data file \"" << (object_p->getName())<< "\"  ..." << endl;
		cerr<< "-----------------------------------------\n";
#endif
			//----- open current data file
		filereader.open( ( object_p->getName() ) );

			//----- get header of data file
		filereader.getLine( header, HEADER_SIZE, '\n' );

		if(	strncmp(header, FORMAT_VERSION_TITLE, FORMAT_VERSION_TITLE_LENGTH ) ) 
		{
				//----- bad data format
			cerr << "ERROR (Command::readDataFile_DrawWireFrame_Clear(), 1)" << endl;
			cerr << "   Data file \"" << (object_name.getName()) << "\" is not of the format version " << FORMAT_VERSION << endl;
		} 


			//----- 
		 if (	'0' == header[ FORMAT_VERSION_TITLE_LENGTH     ]   &&\
			'0' == header[ FORMAT_VERSION_TITLE_LENGTH +1  ]       )
		{
				//----- old data format
			filereader.close();
			filereader.open( ( object_p->getName() ) );
			filereader.readOldPlanData( *object_p, &lineseg_server , 
						    parameter.winged_edge_mode  , total_data_size );
			filereader.close();
		} else 	if( 	FORMAT_VERSION[0] == header[ FORMAT_VERSION_TITLE_LENGTH     ]  ||\
				FORMAT_VERSION[1] == header[ FORMAT_VERSION_TITLE_LENGTH + 1 ]       )
		{
				//----- valid data format
			filereader.close();
			filereader.open( ( object_p->getName() ) );
			filereader.readPlanData( *object_p, &lineseg_server , parameter , total_data_size );
			filereader.close();

		} else 	if( 	'P' == header[ FORMAT_VERSION_TITLE_LENGTH     ]  ||\
				'O' == header[ FORMAT_VERSION_TITLE_LENGTH + 1 ]       )
		{
				//----- polygon.out file
			filereader.close();
			filereader.open( ( object_p->getName() ) );
			filereader.readPolygonOut( *object_p, &lineseg_server , total_data_size );
			filereader.close();

		} else {
				//----- bad data format
			cerr << "ERROR (Command::readDataFile_DrawWireFrame_Clear(), 2)" << endl;
			cerr << "   Data file \"" <<  (object_name.getName()) << "\" is not of the format version " << FORMAT_VERSION << endl;
		}


			//--------------- END READING ---------------//


		
			//----- set polygons
		int back_face_culling = 0;
		passFacetToHidCMD(	object_p         ,  
					camera           ,	
					polygon_server   ,
					transparent_polygon_server   ,
					parameter       ,
					back_face_culling );
		polygon_server.movePolygon3DListToArray( parameter.epsilon3d , parameter.epsilon2d );

			//----- draw object
		if        ( parameter.drawing_mode <= WF_STORED ){ 
			drawPolygon_GL( polygon_server , object_server, gl );
			drawLine_GL   ( transparent_polygon_server.getLineSegmentList(), gl ); 
		} else if ( parameter.drawing_mode >= HID ) {
			fillPolygon_GL( polygon_server , object_p, gl , parameter );
			drawLine_GL   ( transparent_polygon_server.getLineSegmentList(), gl ); 
		} else {
			cerr << "WARNING: Unknown drawing mode using OpenGL" << endl;
			fillPolygon_GL( polygon_server , object_p, gl , parameter );
			drawLine_GL   ( transparent_polygon_server.getLineSegmentList(), gl ); 
		}
		clearCurrentObject( object_server ) ;
		polygon_server.clearPolygon();


			//----- draw lines and clear
		setImage( parameter, lineseg_server, camera );
		drawLine_GL( lineseg_server, gl  ) ;
		lineseg_server.clear();

			//----- updation 
		object_name_server.upward(); num_read_object++ ;

	} // while

//	gl.Flush();
} // Command::readDataFile_DrawWireFrame_Clear()

////////////////////////////

	//----- Command::drawObject_GL()
void	
Command::drawObject_GL(	const Parameter&	parameter          , 
			Perspective&		camera             ,
			Object*			object_p           ,
			FROpenGL&		gl			)
{
		//----- message

		//----- local
	List<Object>		object_server  ; 
	List<LineSegment>	lineseg_server ;
	Hid			polygon_server ;	polygon_server.setCamera( &camera );
	HidLine			transparent_polygon_server ;


		//----- error cheking
	if( object_p == NULL ) { 
		cerr << "***** WARNING: Pointer to Object is NULL" << endl;
		return ; 
	}

		//----- 
	object_server.push( *object_p );


		//----- back-face culling
	int back_face_culling = 1 ;
	if( parameter.drawing_mode == WIREFRAME || \
	    parameter.drawing_mode == WF_STORED     )
	{
	  back_face_culling = 0 ;
	}

		//----- set polygons
	passFacetToHidCMD(	object_p         ,  
				camera           ,	
				polygon_server   ,
				transparent_polygon_server   ,
				parameter        , 
			        back_face_culling );

	polygon_server.movePolygon3DListToArray( parameter.epsilon3d , parameter.epsilon2d );

		//----- draw object
	if        ( parameter.drawing_mode <= WF_STORED ){ 
		drawPolygon_GL( polygon_server , object_server, gl );
		drawLine_GL   ( transparent_polygon_server.getLineSegmentList(), gl ); 
	} else if ( parameter.drawing_mode >= HID ) {
		fillPolygon_GL( polygon_server , object_p, gl , parameter );
		drawLine_GL   ( transparent_polygon_server.getLineSegmentList(), gl ); 
	} else {
		cerr << "WARNING: Unknown drawing mode using OpenGL" << endl;
		fillPolygon_GL( polygon_server , object_p, gl , parameter );
		drawLine_GL   ( transparent_polygon_server.getLineSegmentList(), gl ); 
	}

	clearCurrentObject( object_server ) ;
	polygon_server.clearPolygon();


		//----- draw lines and clear
	setImage( parameter, lineseg_server, camera );
	drawLine_GL( lineseg_server, gl  ) ;
	lineseg_server.clear();

} // Command::drawObject_GL()

////////////////////////////

	//----- Command::drawPolygon_GL()
void
Command::drawPolygon_GL( Hid& polygon_server , List<Object>& object_server, FROpenGL& gl )
{
	int	i, j ;
	int	num_polygon3d = polygon_server.getNumPolygon3D();

	for( i = (num_polygon3d - 1)  ; i >= 0 ; i-- ) {

			//----- local
		Polygon3D&	polygon3d   = polygon_server.getPolygon3D(i);
		int		num_vertex  = polygon3d.getNumVertex(); 
		ColorType	line_color  ;
		Vector3*	vertex3D    = new Vector3  [ num_vertex ];
		LineType*	linetype    = new LineType [ num_vertex ];
		
			//----- line color ( = object color )
		line_color = object_server[ polygon3d.getObjectLabel() ].getKd();
		line_color.whiteToBlack();  // if white, make it black

			//----- set 3D polygon data
		for( j = 0 ; j < num_vertex ; j++ ) {

				//----- (1) 3D vertex position
			vertex3D[j]   = polygon3d.getVertex(j) ;

				//----- (2) edge type
			linetype[j] = polygon3d.getEdgeType(j) ;
			if ( linetype[j].getVisibility() ) {
				linetype[j].setColor( line_color );
			} 
		}

/////////////////////  BEGIN GL code

			//----- draw polygon
		gl.DrawPolygon( vertex3D, num_vertex , linetype );

///////////////////// END GL code

		delete [] vertex3D;  
		delete [] linetype;  linetype = NULL ; 

	} // for (i)

} // Command::drawPolygon_GL()


	//----- Command::fillPolygon_GL()
void
Command::fillPolygon_GL( Hid& polygon_server , 
			 Object* object_p    , 
			 FROpenGL& gl        ,
			 const Parameter& parameter)
{
	int	   i, j ;
	int	   num_polygon3d = polygon_server.getNumPolygon3D();
	ColorType  object_color ; 
	object_color = object_p->getKd(); object_color.whiteToBlack();	


	for( i = (num_polygon3d - 1)  ; i >= 0 ; i-- ) {

			//----- local
		Polygon3D&	polygon3d   = polygon_server.getPolygon3D(i);
		int		num_vertex  = polygon3d.getNumVertex(); 
		ColorType	color       = polygon3d.getColor    (); 	
		Vector3*	vertex3D    = new Vector3  [ num_vertex ];
		LineType*	linetype    = new LineType [ num_vertex ];
		
			//----- 3D vertex position
		for( j = 0 ; j < num_vertex ; j++ ) {
			vertex3D[j]   = polygon3d.getVertex(j) ;
		}

///////////////////// BEGIN GL code
	  // if line drawing ...
	if ( parameter.drawing_mode == HID2 ){
		 color = ColorType( 1, 1, 1 );
			//----- draw polygon

        glPolygonMode( GL_FRONT, GL_LINE ) ;
	//	gl.DrawPolygon( vertex3D, num_vertex , ColorType( 0, 0, 0 ) );
		gl.DrawPolygon( vertex3D, num_vertex , object_color );
		glStencilFunc( GL_EQUAL, 0, 1 );
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
        glPolygonMode( GL_FRONT, GL_FILL ) ;
		gl.DrawPolygon( vertex3D, num_vertex , color );
		glStencilFunc( GL_ALWAYS, 0, 1 );
		glStencilOp( GL_INVERT, GL_INVERT, GL_INVERT );
        glPolygonMode( GL_FRONT, GL_LINE ) ;
//		gl.DrawPolygon( vertex3D, num_vertex , ColorType( 0, 0, 0 ) );
		gl.DrawPolygon( vertex3D, num_vertex , object_color );
	}else{
	  // if not line drawing ...
//#ifdef USE_STENCIL
	if( parameter.flag_use_stencil )
	{

        	glPolygonMode( GL_FRONT, GL_LINE ) ;
		gl.DrawPolygon( vertex3D, num_vertex , ColorType( 0, 0, 0 ) );
		glStencilFunc( GL_EQUAL, 0, 1 );
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
	        glPolygonMode( GL_FRONT, GL_FILL ) ;
	}
//#endif
		gl.DrawPolygon( vertex3D, num_vertex , color );
//#ifdef USE_STENCIL
	if( parameter.flag_use_stencil )
	{
		glStencilFunc( GL_ALWAYS, 0, 1 );
		glStencilOp( GL_INVERT, GL_INVERT, GL_INVERT );
	        glPolygonMode( GL_FRONT, GL_LINE ) ;
		gl.DrawPolygon( vertex3D, num_vertex , ColorType( 0, 0, 0 ) );
	}
//#endif
	}
///////////////////// END GL code

		delete [] vertex3D; 
		delete [] linetype;  linetype = NULL ; 

	} // for (i)

} // Command::fillPolygon_GL()


	//----- Command::drawLine_GL()
void
Command::drawLine_GL( List<LineSegment>& lineseg_server , FROpenGL& gl )
{

		//----- local
	LineSegment	lineseg ;
	Vector3		P, Q ; // linesegment PQ
	LineType	linetype ;

	lineseg_server.toTail(); 
	while( lineseg_server.getItem( lineseg ) ) 
	{
		P = lineseg.getBegin3D();
		Q = lineseg.getEnd3D  ();
		linetype = lineseg.getLineType();

///////////////////// BEGIN GL code

		gl.DrawLine( P, Q, linetype ) ;

///////////////////// END GL code
		
		lineseg_server.upward();
	}

} // Command::drawLine_GL()

	//-----
void
Command::fillCircle_GL( const Parameter&	parameter,
			FROpenGL&		gl,
			const Vector3&		P,
			Perspective&		camera,
			double			dR,
			ColorType*		color_p,
			size_t			Ndiv     )
{
	Vector2 P_vp ;
//	double view = 0.5 ,two_pi = atan(1) * 8.0 ;
	double view = 0.5 * VIEWPORT_WIDTH ;
	double two_pi = atan(1.0) * 8.0 ;

//	Vector3 polygon[Ndiv] ;
	Vector3* polygon = new Vector3 [Ndiv];

	dR *= parameter.scale ; 
	camera.worldToViewport( P, P_vp, parameter.epsilon3d ) ;

	for( size_t i = 0; i < Ndiv; i++ )
		polygon[i] = Vector3( P_vp.x + cos( two_pi * i / Ndiv ) * dR, P_vp.y + sin( two_pi * i / Ndiv ) * dR, 0 );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

        glPolygonMode( GL_FRONT, GL_FILL ) ;
	glOrtho( -view, view, -view/GL_VIEWPORT_ASPECT, view/GL_VIEWPORT_ASPECT, -1, 1 );
	glDisable( GL_DEPTH_TEST );

	gl.DrawPolygon( polygon, Ndiv, *color_p );

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	delete [] polygon ; // CERN
}

	//-----
void
Command::fillSquare_GL( const Parameter&	parameter,
			FROpenGL&		gl,
			const Vector3&		P,
			Perspective&		camera,
			double			dL,
			ColorType*		color_p )
{
	Vector2 P_vp;
	Vector3 polygon[4];
	double view = 0.5 ;

	dL *= parameter.scale ;
	camera.worldToViewport( P, P_vp, parameter.epsilon3d ) ;

	polygon[0] = Vector3( P_vp.x-dL, P_vp.y-dL, 0 );
	polygon[1] = Vector3( P_vp.x+dL, P_vp.y-dL, 0 );
	polygon[2] = Vector3( P_vp.x+dL, P_vp.y+dL, 0 );
	polygon[3] = Vector3( P_vp.x-dL, P_vp.y+dL, 0 );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

        glPolygonMode( GL_FRONT, GL_FILL ) ;
	glOrtho( -view, view, -view/GL_VIEWPORT_ASPECT, view/GL_VIEWPORT_ASPECT, -1, 1 );
	glDisable( GL_DEPTH_TEST );

	gl.DrawPolygon( polygon, 4, *color_p );

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
}



	//----- Command::displayAxes_GL()
void	Command::displayAxes_GL(	FROpenGL&        gl      ,
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

	gl.DrawLine( O3d, X3d , redline   );
	gl.DrawLine( O3d, Y3d , greenline );
	gl.DrawLine( O3d, Z3d , blueline  );

} // Command::displayAxes_GL()

#endif //USE_OPEN_GL
