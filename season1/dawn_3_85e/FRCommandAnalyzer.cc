////////////////////////////////
///// FRCommandAnalyzer.cc /////
////////////////////////////////

#include "dev_control.h"
#if defined USE_UNIX
 #include <unistd.h>
#endif 

#include <stdlib.h>
#include "FRCommandAnalyzer.h"
#include "FRCommandList.h"
#include "FRVServer.h"

#include "FRMath.h"

//#define DEBUG_FR_CA 
//#define DEBUG_FR_CA2 

const char COMMENT_CHAR = '#' ;

const char    DEFAULT_EPS_FILE_NAME[]   = "g4.eps"   ; 
const char    DEFAULT_PRIM_FILE_NAME[]  = "g4.prim" ; 
const char    PRIM_EXT []               = ".prim"   ;
const char    EPS_EXT   []              = ".eps"     ;
const char    PRIM_VIEWER []            = "dawn -v" ;
const double  DEFAULT_STEP_COLOR_R = 0.0 ;
const double  DEFAULT_STEP_COLOR_G = 1.0 ;
const double  DEFAULT_STEP_COLOR_B = 0.0 ;
const int     DEFAULT_NDIV         = 24  ;


	//----- FRCommandAnalyzer::FRCommandAnalyzer()
FRCommandAnalyzer::FRCommandAnalyzer(	char*              prim_file_name  , 
					Command&           command_given   ,
					Parameter&         parameter_given , 
					const Attribute&   default_att     ):
			command    ( command_given   ) , 
			parameter  ( parameter_given ) ,
			brepFactory(                 ) ,
			defaultAttribute( default_att ), 
			num_primitive(0)
{
		//----- local

		//----- message
#if defined DEBUG_FR_CA
	cerr << "*** FRCommandAnalyzer is invoked" << endl;
#endif 

		//----- parameters to be fixed in this mode 
	parameter.flag_sort_data = 0 ;
	strcpy(parameter.ps_file, DEFAULT_EPS_FILE_NAME );

		//-----
	strcpy( primFile  , DEFAULT_PRIM_FILE_NAME );

		//----- 
	if       ( parameter.flag_IP_mode == 1 ) {
			// -v option: solid viewer mode
		strcpy ( primFile  , prim_file_name );
		strcpy(parameter.ps_file, primFile );
		ToolString::convertExtension( parameter.ps_file, PRIM_EXT , EPS_EXT );
	}


		//----- set default attributes
	curAttribute = defaultAttribute ;
	brepFactory.SetAttribute( &curAttribute ) ;
	setDefaultNdiv() ;

}

void	FRCommandAnalyzer::setDefaultNdiv()
{
		//----- local 
	int	default_ndiv = 3 ;

		//----- get Ndiv from environmental variable
	if( getenv( "DAWN_NDIV" ) != NULL ) {
		sscanf( getenv("DAWN_NDIV"), "%d", &default_ndiv ) ;
	} else {
		default_ndiv             = DEFAULT_NDIV ;
	}

		//----- error recovery 
	if ( default_ndiv < 3 ) { default_ndiv = 3 ; }

		//----- set Ndiv to brep factory
	brepFactory.SetNdiv( default_ndiv ) ;
}

	//----- FRCommandAnalyzer::clear()
void FRCommandAnalyzer::clear()
{
	solid_out.close();
	clearData();
}

	//----- FRCommandAnalyzer::clearData()
void FRCommandAnalyzer::clearData()
{
#if defined DEBUG_FR_CA
	cerr << "***** Data is cleared" << endl;
#endif 
		//-----
	brepFactory.clear() ;
	objectServer.clear() ;
	linesegServer.clear()              ;
	hid.clear()              ;
	hidline.clear()          ;
	totalBoundingBox.clear() ;
	totalDataSize.clear   () ;

	curAttribute = defaultAttribute ;
	brepFactory.SetAttribute( &curAttribute )     ; 
	setDefaultNdiv() ;

	polylineVertexList.clear() ;

	markServer.clear();
	vistextServer.clear();

#if   defined USE_OPEN_GL
	command.closeDevice( parameter, ps, xwin, ps2x, gl );
#elif defined USE_XWIN
	command.closeDevice( parameter, ps, xwin, ps2x     );
#else
	command.closeDevice( parameter, ps );
#endif

} // FRCommandAnalyzer::clearData()


#if defined USE_OPEN_GL
	//-----
void FRCommandAnalyzer::drawObject_GL_Clear( void )
{
	objectServer.toHead();
	command.drawObject_GL(	parameter                     , 
				camera                        ,
				objectServer.getItemAddress() ,
				gl                              );
	objectServer.clear();
}

	//----- 
void FRCommandAnalyzer::drawPolyline_GL_Clear( void )
{
	command.drawLine_GL( linesegServer , gl );
	linesegServer.clear();
}
#endif


void	FRCommandAnalyzer::incr_displayNumPrimitives( )
{

	num_primitive++ ;
	div_t	div_result = div( num_primitive, MESSAGE_INTERVAL ) ;

	if( num_primitive  && !div_result.rem ) {
		cerr << " ***** " << num_primitive << "  primitive are processed" << endl;
	}

}

	//----- FRCommandAnalyzer::makeObjectImage()
void FRCommandAnalyzer::makeObjectImage( void )
{

		//------------------------------//
		//----- Make Object Images -----//
		//------------------------------//
	if( parameter.device_label == OPEN_GL ){
#if defined USE_OPEN_GL
			//----- Draw head of ObjectServer and then clear it
			//.....  Note: Polylines are drawn in makePolylineImane()
		drawObject_GL_Clear(); 
#endif

	} else {
	switch ( parameter.drawing_mode ) {
	 case WIREFRAME:

			//----- draw object and clear
			//.....  Note: Polylines are drawn in makePolylineImane()
		objectServer.toHead();
		command.drawCurrentObject_WireFrame(	parameter    ,
							ps           ,
							objectServer ,
							camera        );
		command.clearCurrentObject          ( objectServer  ) ;


			//----- draw lines and clear
		command.drawLine(	parameter ,
					ps        ,
					camera    ,
					linesegServer  ) ;
		linesegServer.clear();

		break ; 

	 case WF_STORED:{

		parameter.flag_haloing = 0 ; 

		objectServer.toHead(); 
		command.passEdgeToHidLineCMD(	objectServer.getItemAddress()  , 
						camera        ,
						hidline       ,
						parameter      );
		break ;
	 }

	 default: // HID etc
		command.doIncrementalHiddenSurfaceElimination(	parameter          ,
								objectServer       , 
								hid                , 
								hidline             );
		break ;

	} // switch ( parameter.drawing_mode ) 
	}

} // FRCommandAnalyzer::makeObjectImage()


	//-----	FRCommandAnalyzer::fromBodyToWorld()
void	FRCommandAnalyzer::fromBodyToWorld(	const FRVector3& body, \
						FRVector3*       world_p ) 
{
	FRVector3 origin, e1, e2, e3 ; 
	origin = brepFactory.GetOrigin();
	e1     = brepFactory.GetE1    ();
	e2     = brepFactory.GetE2    ();
	e3     = brepFactory.GetE3    ();
	*world_p = origin + (body.x() * e1 ) + (body.y() * e2 ) + (body.z() * e3 ) ;
}


	//-----	FRCommandAnalyzer::fromBodyToWorld()
void	FRCommandAnalyzer::fromBodyToWorld(	const Vector3& body, \
						Vector3*       world_p ) 
{
		//----- local
	FRVector3  world_tmp ;

		//----- Transformation
	fromBodyToWorld(   FRVector3( body.x, body.y, body.z ),  &world_tmp );

		//----- convert FRVector3 to Vector3 
	(*world_p)( world_tmp.x(),  world_tmp.y(),  world_tmp.z() ) ; 
}

	//----- FRCommandAnalyzer::fromVerticesToLinesegments()
void	FRCommandAnalyzer::fromVerticesToLinesegments(	List<FRVector3>&	vertex_list, \
							List<LineSegment>*     lineseg_list_p ) 
{
		//----- local
	int			i ;
	int			num_vertex  = vertex_list.getNItem();
	int			num_lineseg = num_vertex - 1 ;
	FRVector3*		vertex_body  = new FRVector3 [ num_vertex ];
	FRVector3		vertex_world_tmp ;
	Vector3*		vertex_world = new Vector3 [ num_vertex ];
	const Attribute*	att_p = brepFactory.GetAttribute();
	LineType		linetype ;

		//----- set linetype
	linetype.setColor( att_p->getKd() );
	linetype.setWidth( parameter.line_width );

		//----- copy vertices from list to Array
	vertex_list.toArray( vertex_body, num_vertex );
	
		//----- from body coord to world coord
	for( i = 0 ; i < num_vertex ; i++ ) {
		fromBodyToWorld( vertex_body[i], &vertex_world_tmp );
		(vertex_world[i])( vertex_world_tmp.x(), vertex_world_tmp.y(), vertex_world_tmp.z() );
	}

		//----- set polyline 
	for( i = 0 ; i < num_lineseg ; i++ ) {
		lineseg_list_p->addItem( LineSegment( vertex_world[i], vertex_world[(i+1)], linetype ) );
	}

		//----- delete
	delete [] vertex_body   ;
	delete [] vertex_world  ;

} // FRCommandAnalyzer::fromVerticesToLinesegments()


	//----- FRCommandAnalyzer::makePolylineImage()
void FRCommandAnalyzer::makePolylineImage()
{
	fromVerticesToLinesegments(  polylineVertexList , &linesegServer  ) ;

		//-------------------------//
		//----- Make Polyline -----//
		//-------------------------//
	if( parameter.device_label == OPEN_GL ){
#if defined USE_OPEN_GL
				//----- Draw head of ObjectServer and then clear it
			drawPolyline_GL_Clear(); 
#endif

	} else {
	switch ( parameter.drawing_mode ) {
	 case WIREFRAME:
			//----- draw lines and clear
		command.drawLine(	parameter ,
					ps        ,
					camera    ,
					linesegServer  ) ;
		linesegServer.clear();
		break ; 

	 case WF_STORED:{
		parameter.flag_haloing = 0 ; 
		break ;
	 }

	 default: // HID etc
		break ;

	} // switch ( parameter.drawing_mode ) 
	}
} // FRCommandAnalyzer::makePolylineImage()



	//----- FRCommandAnalyzer::setObjectToFactory()
void FRCommandAnalyzer::setObjectToFactory( const char* obj_name )
{
	int	obj_label = objectServer.getNItem();
	Object*		object_p = NULL ;
	Object		empty_object ;
	objectServer.push( empty_object ) ;		
	object_p = objectServer.getItemAddress();

	object_p->setLabel( obj_label ) ; 
	object_p->setName ( obj_name  ) ; 

	brepFactory.setObject ( object_p );		
}


#if defined USE_XWIN 
	//----- void FRCommandAnalyzer::waitEvent()
void FRCommandAnalyzer::waitEvent( FRVServer* server_p , int flag_sendback )
{
		//-----  local
	char	viewer [256];	strcpy( viewer, PRIM_VIEWER); 
				strcat( viewer, " ");
				strcat( viewer,  primFile);

	if( parameter.device_label == PS || parameter.device_label == PS2 ) {
		if( flag_sendback ) { server_p->SendLine( FR_WAIT ); }
		return ;
	}

		//----- local
	XEvent	event ;

		//----- message
	cerr<<  "---------------------------------------------------\n";
	cerr << " Drawing completed. " << endl;
	cerr << " Click Drawing Area or Hit Any Key!" << endl;
	cerr << endl;
	cerr << " <Key>           : go on to next drawing" << endl; 
	cerr << " <Mouse Button1> : no effect"           << endl;
	cerr << " <Mouse Button2> : invoke GUI and redraw " << endl;
	cerr << " <Mouse Button3> : go on to next drawing ( = Key ) " << endl;
	cerr << endl;
	cerr<<  "---------------------------------------------------\n";
	cerr<< endl << endl;

		//-----
	while(1) {
		XNextEvent( xwin.getDisplay() , &event ) ;

		if( event.type == KeyPress    ) { 
			break ; 
		}
		else if( event.type == ButtonPress ) { 

			int flag_exit =  1 ;
			int pid       = -1 ;

			switch   ( event.xbutton.button ) {
			 case  Button1:
				flag_exit = 0 ;
				break ;
			 case  Button2:
#if defined DEBUG_FR_CA
				cerr << "***** Middle Button is clicked" << endl; 
#endif
				if( parameter.flag_accumulate_data ) 
				{ 
					cerr << "WARNING: Multiple windows are not available in " << endl;
					cerr << "         data accumulation mode." << endl;
					flag_exit = 0 ;
					break ;
				} else {

					if( ( pid = fork() ) == 0 ) { // child
						if ( execlp ( "dawn", "dawn", "-v" , primFile, (char *)0 ) < 0 ) 
						{
							perror("dawn -v g4.prim") ;
						}
					} else { // parent 
						flag_exit = 0 ;
						break ;
					}

				} // if( parameter.flag_accumulate_data )--else 
			 case  Button3:
#if defined DEBUG_FR_CA
				cerr << "***** Right Button is clicked" << endl; 
#endif

				flag_exit = 1 ;
				break ;
			 default: ;
#if defined DEBUG_FR_CA
				cerr << "***** Button Press Event is detected" << endl; 
#endif
				flag_exit = 1 ;
				break ;
			}


			if( flag_exit ) { break ; }
				// exit from while loop
		}
	}// while

	XFlush( xwin.getDisplay());

	if( flag_sendback ) { 	server_p->SendLine( FR_WAIT ); }

}// void FRCommandAnalyzer::waitEvent()

#endif // USE_XWIN


void  FRCommandAnalyzer::SetMarkAttributes(	const char* message, 
						int    type        , 
						int    size_dim    ,
						const ColorType& mark_color,
						FRMark* mark_p        )
{	
		//----- local
	char		dummy[WORD_MAX] ;	
	int		scanf_status ;
	Vector3		P ;               // given body-coord  position
	Vector3		P_world ;         // calculated world-coord position
	double		R_size    = 1.0 ; // given size 
					  // in either 3D or 2D units
	double		R_size_vp = 1.0 ; // calculated size in units of vp

		//----- scan value
	scanf_status = \
	sscanf( message, "%s %lg %lg %lg %lg" ,  \
		dummy, &(P.x), &(P.y), &(P.z), &R_size);

	if( scanf_status == EOF ) {
	  cerr << "ERROR: FRCommandAnalyzer::SetMarkAttrubutes()";
	  cerr << endl;
	}

		//----- world-coord position
	fromBodyToWorld( P, &P_world );

		//----- set size
	if( size_dim >= 3 ) {
			//----- size is given in world-coord unit
		R_size_vp = GetMark2DSize_vp ( R_size, P_world ); 
	} else {
			//----- size is given in pt (or pixel)
		R_size_vp = GetMark2DSSize_vp( R_size );

	}

///////////////////////////// Special Treatment for OpenGL 
	if( size_dim < 3  && parameter.device_label == OPEN_GL ){
		R_size_vp = 5.0 * ( 1.0 / A4_WIDTH_PT ) ;
	}
////////////////////////////


		//----- set parameters
	mark_p->setType      ( type ) ;
	mark_p->setPosition3D( P_world ) ;
	mark_p->setParam     ( R_size_vp ) ;
	mark_p->setColor     ( mark_color ) ;
}

void  FRCommandAnalyzer::SetMarkTextAttributes(	const char* message, 
						int    type        , 
						int    size_dim    ,
						const ColorType& mark_color,
						FRMark* mark_p        )
{	
		//----- local
	char		dummy [WORD_MAX] ;	
	char		dummy2[WORD_MAX] ;	
	int		scanf_status ;
	Vector3		P ;               // given body-coord  position
	Vector3		P_world ;         // calculated world-coord position
	double		R_size    = 1.0 ; // given size 
					  // in either 3D or 2D units
	double		R_size_vp = 1.0 ; // calculated size in units of vp
	double		offset_x    = 0.0   ;
	double		offset_x_vp = 0.0   ;
	double		offset_y    = 0.0   ;
	double		offset_y_vp = 0.0   ;

		//----- scan value
	scanf_status = \
	sscanf( message, "%s %lg %lg %lg %lg %lg %lg %s" ,  \
		dummy, &(P.x), &(P.y), &(P.z), &R_size, &offset_x, &offset_y, dummy2 );

	if( scanf_status == EOF ) {
	  cerr << "ERROR: FRCommandAnalyzer::SetMarkTextAttrubutes()";
	  cerr << endl;
	}

		//----- get string 
	FRString	mark_string( message ) ;
	int	i;
	const	int NUM_SKIP = 7 ;
	for ( i = 0 ; i < NUM_SKIP ; i++ ) {
		mark_string.RemoveFirstWord();	
	}
	mark_string.ReplaceTerminalNewlineWithNull();


		//----- world-coord position
	fromBodyToWorld( P, &P_world );

		//----- set size
	if( size_dim >= 3 ) {
			//----- size is given in world-coord unit
		R_size_vp   = GetMark2DSize_vp ( R_size, P_world ); 
		offset_x_vp = GetMark2DSize_vp ( offset_x, P_world ); 
		offset_y_vp = GetMark2DSize_vp ( offset_y, P_world ); 
	} else {
			//----- size is given in pt (or pixel)
		R_size_vp   = GetMark2DSSize_vp( R_size   );
		offset_x_vp = GetMark2DSSize_vp( offset_x );
		offset_y_vp = GetMark2DSSize_vp( offset_y );
	}

		//----- set parameters
	mark_p->setType      ( type ) ;
	mark_p->setString    ( mark_string.GetCharString() );
	mark_p->setPosition3D( P_world ) ;
	mark_p->setParam     ( R_size_vp , 0 ) ;
	mark_p->setParam     ( offset_x_vp , 1 ) ;
	mark_p->setParam     ( offset_y_vp , 2 ) ;
	mark_p->setColor     ( mark_color ) ;
}


double FRCommandAnalyzer::GetMark2DSize_vp (	double		size_world , 
						const Vector3&	P_world )
{
	double size_vp \
	= camera.mapSphereSizeToScreen (	size_world , 
						P_world , 
						parameter.epsilon3d );
	return size_vp ;

}

double FRCommandAnalyzer::GetMark2DSSize_vp( double size_pt ) 
{
	double size_vp = ps.lengthPtToVp( size_pt );
	return size_vp ;

/////////////////
//	PostScript ps_tmp; 
//	ps_tmp.open(); // make lengthPtToVp() function effective
//
//	double size_vp = ps_tmp.lengthPtToVp( size_pt );
//	ps_tmp.close();
//
//	return size_vp ;
/////////////////////
}

	//------ 
void
FRCommandAnalyzer::drawAllVisTexts() 
{
	FRString	string   ;
	Vector2		P_mm     ;
	ColorType	color    ;
	double		size_pt  ;

	FRVisText2D	vis_text_cur    ;

	vistextServer.toTail();
	while( vistextServer.getItem( vis_text_cur ) )
	{
		string		= vis_text_cur.GetString     ();
		P_mm		= vis_text_cur.GetPosition_mm();
		color		= vis_text_cur.GetColor      ();
		size_pt		= vis_text_cur.GetSize_pt    ();

		if( parameter.device_label == OPEN_GL ){
			// do nothing
		}else{
			command.drawColoredString_PS(	string.GetCharString(),
							color     ,
							size_pt   ,
							P_mm	  ,
							ps         );

		}

		vistextServer.upward();
	}

} // FRCommandAnalyzer::drawAllVisTexts() 


	//------ 
void
FRCommandAnalyzer::drawAllMark()
{
		int		type          ;
		Vector3		mark_position ;
		double		mark_param    ;
		ColorType	mark_color    ;

		FRMark		mark_cur      ;
		markServer.toTail();
		while( markServer.getItem( mark_cur ) )
		{
			type		= mark_cur.getType();
			mark_position	= mark_cur.getPosition3D();
			mark_param	= mark_cur.getParam(); // size in units of vp
			mark_color	= mark_cur.getColor();

			if( parameter.device_label == OPEN_GL ){
#if defined USE_OPEN_GL
				switch( type ) {
				 case FRMark::CIRCLE2D  :
					command.fillCircle_GL( parameter, gl, mark_position, camera, mark_param , &mark_color );
					break ;
				 case FRMark::SQUARE2D  :
					command.fillSquare_GL( parameter, gl, mark_position, camera, mark_param , &mark_color );
					break ;
				}
#endif

			}else{
				switch( type ) {
				 case FRMark::CIRCLE2D  :
					command.fillCircle_PS( parameter, ps, mark_position, camera, mark_param , &mark_color );
					break ;
				 case FRMark::SQUARE2D  :
					command.fillSquare_PS( parameter, ps, mark_position, camera, mark_param , &mark_color );
					break ;
				 case FRMark::TEXT2D  :
					FRString str ( mark_cur.getCharString() ) ; 
					double size_vp = mark_param ;
					double offset_x_vp = mark_cur.getParam(1);
					double offset_y_vp = mark_cur.getParam(2);
					command.drawColoredString_vp_PS(str.GetCharString(),
									mark_color ,
									size_vp      ,
									offset_x_vp  ,
									offset_y_vp  ,
									mark_position, 
									camera, 
									parameter, 
									ps       ); 
					break ;
				}
			}
			markServer.upward();
		}

#if defined USE_OPEN_GL
		if( parameter.device_label == OPEN_GL ) gl.Flush();
#endif

} // FRCommandAnalyzer::drawAllMark()


	//----- FRCommandAnalyzer::analyzeAndExecute ()
	//.....  RETURN: 
	//.....		1 : normal
	//.....		0 : QUIT
	//.....		-1: TERMINATE
int FRCommandAnalyzer::analyzeAndExecute ( const char* message , FRVServer* server_p ) 
{
#if defined DEBUG_FR_CA2
	cerr << ">>>>>>>>>> AnalyzedMessage = " << message << endl;
#endif 

///////// DUMMY codes to avoid warning 
#if   defined USE_OPEN_GL
 // DO NOTHING
#elif defined USE_XWIN
 // DO NOTHING
#else 
 FRVServer* dummy_server = server_p ; dummy_server = 0 ;
#endif


/////////////////////////////

	//========== Local Variables and constants ==========//
	
	// return value
	int	          status = 1 ; // return value

	// const
	enum { INITIAL_BLOCK, VERTEX_BLOCK, FACET_BLOCK }; 

	// local static variables
	static	int	flag_saving_prim_data	= 0 ; 
	static	int	cur_vertex_label	= 0 ;

	// buffers
	char doit  [WORD_MAX] ;	strcpy( doit  , "");
	char dummy [WORD_MAX] ;	strcpy( dummy , "");


	//============ pre-processing ===============//

		//----- skip comment line
	if( message[0] == COMMENT_CHAR ) {
		if( flag_saving_prim_data )
		{
			solid_out << message ;
		}
		return status ;
	}

		//----- skip null line  
	if( message[0] == '\n' ) {
		return status ;
	}

		//----- get first word fromm message
		//..... and store it in "doit"
	if( sscanf( message, "%s", doit ) == EOF ) { 
		return status ;  // skip NULL line
	}

	//============ save g4.prim-format data to file ===============//
	if( flag_saving_prim_data ){

			//----- output if neither FR_SAVE nor FR_END_SAVE
		if(	strcmp(doit, FR_SAVE            ) && 
			strcmp(doit, FR_END_SAVE        ) &&  
			strcmp(doit, FR_GUI             ) && 
			strcmp(doit, FR_DEVICE          ) && 
			strcmp(doit, FR_CAMERA_POSITION ) &&
			strcmp(doit, FR_TARGET_POINT    ) &&
			strcmp(doit, FR_ZOOM_FACTOR     ) &&
			strcmp(doit, FR_WIREFRAME       ) &&
			strcmp(doit, FR_SURFACE         ) &&
			strcmp(doit, FR_LINES           )    )
		{
			solid_out << message ;

		}
	} 

	//==============================================//
	//======== COMMAND ANALYSIS and ACTIONS ========//
	//==============================================//
	if ( !strcmp(doit, FR_BOX ) ) {
#if defined DEBUG_FR_CA
		cerr << "BOX" << endl;
#endif 
			//----- local
		int	scanf_status ;
		double dx = 1.0, dy = 1.0, dz = 1.0 ;

			//----- count and display
		incr_displayNumPrimitives( );

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg", dummy, &dx, &dy, &dz  );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), BOX " << endl;
		}

			//----- generate box data 
		FRBoxForBrep	box ( dx, dy, dz ); 
		setObjectToFactory("BOX");
		brepFactory.GenerateBrepData ( box );

			//----- precessing box data 
		makeObjectImage();
	}
	//==========================================//
	else if ( !strcmp(doit, FR_COLUMN ) ) {
#if defined DEBUG_FR_CA
		cerr << "COLUMN" << endl;
#endif 

			//----- local
		int	scanf_status ;
		double	R = 1.0, dh = 1.0 ;

			//----- count and display
		incr_displayNumPrimitives( );

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg", dummy, &R, &dh );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), COLUMN " << endl;
		}

			//----- generate column data 
		FRTubeSegForBrep	column ( 0.0, R, dh, 0.0, 2.0 * FR_D_PI );
		setObjectToFactory("COLUMN");
		brepFactory.GenerateBrepData ( column );

			//----- precessing column data 
		makeObjectImage();
	}

	//==========================================//
	else if ( !strcmp(doit, FR_TUBS ) ) {
#if defined DEBUG_FR_CA
		cerr << "TUBS" << endl;
#endif 

			//----- local
		int	scanf_status ;
		double	rmin = 0.0, rmax = 1.0, dz = 1.0; 
		double	sphi = 0.0, dphi = 1.0 ;

			//----- count and display
		incr_displayNumPrimitives( );

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg %lg %lg", dummy, &rmin, &rmax, &dz, &sphi, &dphi );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), TUBS " << endl;
		}

			//----- generate data 
		FRTubeSegForBrep	tube_segment ( rmin, rmax, dz, sphi, dphi );
		setObjectToFactory( "TUBS" );
		brepFactory.GenerateBrepData ( tube_segment );

			//----- precessing data 
		makeObjectImage();
	}
	//==========================================//
	else if ( !strcmp(doit, FR_CONS ) ) {
#if defined DEBUG_FR_CA
		cerr << "CONS" << endl;
#endif 
			//----- local
		int	scanf_status ;
		double	rmin1 = 0.0, rmax1 = 1.0, rmin2 = 0.0, rmax2= 1.0; 
		double	dz    = 1.0, sphi  = 0.0, dphi  = 1.0;

			//----- count and display
		incr_displayNumPrimitives( );

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg %lg %lg %lg %lg", dummy, &rmin1, &rmax1, &rmin2, &rmax2, &dz, &sphi, &dphi );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), CONS " << endl;
		}

			//----- generate data 
		FRConeSegForBrep	cone_segment ( rmin1, rmax1, rmin2, rmax2, dz, sphi, dphi );
		setObjectToFactory("CONS");
		brepFactory.GenerateBrepData ( cone_segment );

			//----- precessing data 
		makeObjectImage();
	}
	//==========================================//
	else if ( !strcmp(doit, FR_SPHERE ) ) {
#if defined DEBUG_FR_CA
		cerr << "SPHERE" << endl;
#endif 

			//----- local
		int	scanf_status ;
		double	R = 1.0 ;
				// Change Ndiv to 2 * Ndiv temporarily
		int 	ndiv_backup = brepFactory.GetNdiv() ;
		int 	ndiv_tmp    = ndiv_backup * 2 ;
		brepFactory.SetNdiv( ndiv_tmp ) ;

			//----- count and display
		incr_displayNumPrimitives( );

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg", dummy, &R );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), SPHERE " << endl;
		}

			//----- generate data 
		FRSphereForBrep	sphere ( R );
		setObjectToFactory("SPHERE");
		brepFactory.GenerateBrepData ( sphere );

			//----- precessing data 
		makeObjectImage();

			//----- restore original Ndiv
		brepFactory.SetNdiv( ndiv_backup ) ;
	}
	//==========================================//
	else if ( !strcmp(doit, FR_SPHERE_SEG ) ) {
#if defined DEBUG_FR_CA
		cerr << "SPHERE_SEG" << endl;
#endif 

			//----- local
		int	scanf_status ;
		double	rmin = 0.0, rmax = 1.0;
		double	s_theta = 0.0, d_theta = 1.0, s_phi = 0.0, d_phi = 1.0 ;
				// Change Ndiv to 2 * Ndiv temporarily
		int 	ndiv_backup = brepFactory.GetNdiv() ;
		int 	ndiv_tmp    = ndiv_backup * 2 ;
		brepFactory.SetNdiv( ndiv_tmp ) ;

			//----- count and display
		incr_displayNumPrimitives( );

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg %lg %lg %lg", dummy, &rmin, &rmax, &s_theta, &d_theta, &s_phi, &d_phi ) ;
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), SPHERE_SEG " << endl;
		}

			//----- generate data 
		FRSphereSegForBrep	sphere_seg ( rmax, rmin, s_phi , d_phi, s_theta, d_theta );
		setObjectToFactory("SPHERE_SEG");
		brepFactory.GenerateBrepData ( sphere_seg );

			//----- precessing data 
		makeObjectImage();

			//----- restore original Ndiv
		brepFactory.SetNdiv( ndiv_backup ) ;
	}

	//==========================================//
	else if ( !strcmp(doit, FR_TRD ) ) {
#if defined DEBUG_FR_CA
		cerr << "TRD" << endl;
#endif 
			//----- local
		int	scanf_status ;
		double	dx1 = 1.0, dx2 = 1.0, dy1 = 1.0, dy2 = 1.0, dz = 1.0;

			//----- count and display
		incr_displayNumPrimitives( );

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg %lg %lg", dummy, &dx1, &dx2, &dy1, &dy2, &dz );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), TRD " << endl;
		}

			//----- generate data 
		FRTrdForBrep 		trd( dx1, dx2, dy1, dy2, dz ); 
		setObjectToFactory("TRD");
		brepFactory.GenerateBrepData ( trd );

			//----- precessing data 
		makeObjectImage();
	}
	//==========================================//
	else if ( !strcmp(doit, FR_TRAP ) ) {
#if defined DEBUG_FR_CA
		cerr << "TRAP" << endl;
#endif 
			//----- local
		int	scanf_status ;
		double	dz, theta, phi, h1, bl1, tl1, alpha1, h2, bl2, tl2, alpha2 ;

			//----- count and display
		incr_displayNumPrimitives( );

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg", \
			dummy, &dz, &theta, &phi, &h1, &bl1, &tl1, &alpha1, &h2, &bl2, &tl2, &alpha2 ) ;
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), TRAP" << endl;
		}

			//----- generate data 
		FRTrapForBrep 		trap( dz, theta, phi, h1, bl1, tl1, alpha1, h2, bl2, tl2, alpha2 ) ;
		setObjectToFactory("TRAP");
		brepFactory.GenerateBrepData ( trap );

			//----- precessing data 
		makeObjectImage();
	}

	//==========================================//
	else if ( !strcmp(doit, FR_PARA ) ) {
#if defined DEBUG_FR_CA
		cerr << "PARA" << endl;
#endif 
			//----- local
		int	scanf_status ;
		double	dx, dy, dz, tanAlpha, tanTheta_cosPhi, tanTheta_sinPhi ;

			//----- count and display
		incr_displayNumPrimitives( );

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg %lg %lg %lg", dummy ,       \
				&dx       , &dy              , &dz   ,       \
				&tanAlpha , &tanTheta_cosPhi , &tanTheta_sinPhi );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PARA " << endl;
		}

			//----- generate data 
		FRParallelepipedForBrep		para(	dx, dy, dz, \
							tanAlpha , tanTheta_cosPhi , tanTheta_sinPhi );
		setObjectToFactory("PARA");
		brepFactory.GenerateBrepData ( para );

			//----- precessing data 
		makeObjectImage();
	}


	//==========================================//
	else if ( !strcmp(doit, FR_PCON ) ) {
#if defined DEBUG_FR_CA
		cerr << "PCON" << endl;
#endif 
			//----- local
		int	scanf_status ;
		double	sphi = 0, dphi = 2.0 * FR_D_PI ;	// initialization
		int	nz = 2         ;		// initialization
		double*	z_array    = 0 ;		// initialization
		double*	rmin_array = 0 ;		// initialization
		double*	rmax_array = 0 ;		// initialization
		const int num_skip   = 4 ;	// FR_PCON sphi  dphi  nz  z[nz]  rmin[nz]  rmax[nz]
						// Array start after skipping 4 word.

			//----- count and display
		incr_displayNumPrimitives( );

			//----- set sphi
		if( !ToolString::getDouble( message, &sphi, 1 ) ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PCON 1 (sphi)" << endl;
			sphi = 0 ;
		}

			//----- set dphi
		if( !ToolString::getDouble( message, &dphi, 2 ) ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PCON 2 (dphi)" << endl;
			dphi = 2.0 * FR_D_PI ;
		}


			//----- set nz
		if( !ToolString::getInt   ( message, &nz,   3 ) ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PCON 3 (nz)" << endl;
			nz = 2 ; 
		}


			//----- set z_array[], rmin_array[], rmax_array[]
		z_array = new double [ nz ];	rmin_array = new double [ nz ];	rmax_array  = new double [ nz ];
		scanf_status = \
		ToolString::getDoubleArray3 ( message , nz , z_array , rmin_array , rmax_array , num_skip );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PCON 4" << endl;
		}

			//----- generate data 
		FRPolyconeForBrep		pcon( sphi, dphi, nz, z_array, rmin_array, rmax_array );
		setObjectToFactory("PCON");
		brepFactory.GenerateBrepData ( pcon );

			//----- precessing data 
		makeObjectImage();

			//----- clean up
		delete [] z_array ;	delete [] rmin_array ;	delete [] rmax_array  ;

	}


	//==========================================//
	else if ( !strcmp(doit, FR_PGON ) ) {
#if defined DEBUG_FR_CA
		cerr << "PGON" << endl;
#endif 
			//----- local
		int	scanf_status ;
		double	sphi = 0, dphi = 2.0 * FR_D_PI ;	// initialization
		int	nz = 2         ;		// initialization
		int	n_div      = 4 ;		// initialization
		double*	z_array    = 0 ;		// initialization
		double*	rmin_array = 0 ;		// initialization
		double*	rmax_array = 0 ;		// initialization
		const int num_skip   = 5 ;		// FR_PGON  sphi  dphi  ndiv  nz  z[nz]  rmin[nz]  rmax[nz]
							// Array start after skipping 5 word.

			//----- count and display
		incr_displayNumPrimitives( );

			//----- set sphi
		if( !ToolString::getDouble( message, &sphi, 1 ) ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PGON 1 (sphi)" << endl;
			sphi = 0 ;
		}

			//----- set dphi
		if( !ToolString::getDouble( message, &dphi, 2 ) ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PGON 2 (dphi)" << endl;
			dphi = 2.0 * FR_D_PI ;
		}

			//----- set n_div
		if( !ToolString::getInt   ( message, &n_div, 3 ) ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PGON 3 (ndiv)" << endl;
			n_div = 4 ; 
		}


			//----- set nz
		if( !ToolString::getInt   ( message, &nz,   4 ) ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PGON 4 (nz)" << endl;
			nz = 2 ; 
		}

			//----- set z_array[], rmin_array[], rmax_array[]
		z_array = new double [ nz ];	rmin_array = new double [ nz ];	rmax_array  = new double [ nz ];
		scanf_status = \
		ToolString::getDoubleArray3 ( message , nz , z_array , rmin_array , rmax_array , num_skip );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PGON 5" << endl;
		}

			//----- generate   data 
		FRPolygonForBrep		pgon( sphi, dphi, n_div , nz, z_array, rmin_array, rmax_array );
		setObjectToFactory("PGON");
		brepFactory.GenerateBrepData ( pgon );

			//----- precessing data 
		makeObjectImage();

			//----- clean up
		delete [] z_array ;	delete [] rmin_array ;	delete [] rmax_array  ;

	}

	//==========================================//
	else if ( !strcmp(doit, FR_TORUS ) ) {
#if defined DEBUG_FR_CA
		cerr << "TORUS" << endl;
#endif 

			//----- local
		int	scanf_status ;
		double	rmin = 0.0, rmax = 1.0, rtor = 5.0; 
		double	sphi = 0.0, dphi = 5.0 ;

				// Change Ndiv to 2 * Ndiv temporarily
		int 	ndiv_backup = brepFactory.GetNdiv() ;
		int 	ndiv_tmp    = ndiv_backup * 2 ;
		brepFactory.SetNdiv( ndiv_tmp ) ;

			//----- count and display
		incr_displayNumPrimitives( );

			//----- scan value
		scanf_status = \
sscanf( message, "%s %lg %lg %lg %lg %lg", dummy, &rmin, &rmax, &rtor, &sphi, &dphi );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), TORUS " << endl;
		}

			//----- generate data 
		FRTorusForBrep	torus ( rmin, rmax, rtor, sphi, dphi );
		setObjectToFactory( "TORUS" );
		brepFactory.GenerateBrepData ( torus );

			//----- precessing data 
		makeObjectImage();

			//----- restore original Ndiv
		brepFactory.SetNdiv( ndiv_backup ) ;
	}

	//==========================================//
	else if ( !strcmp(doit, FR_POLYLINE ) ) {
#if defined DEBUG_FR_CA
		cerr << "POLYLINE" << endl;
#endif 
			//----- clear temporary storage of previous polyline
		polylineVertexList.clear();
	}
	//==========================================//
	else if (	!strcmp(doit, FR_PL_VERTEX     ) ||
			!strcmp(doit, FR_PL_VERTEX_OLD )    )
	{
#if defined DEBUG_FR_CA
		cerr << "PL_VERTEX" << endl;
#endif 

		int		scanf_status ;
		FRVector3	P ;
		double		x, y , z ;

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg"      , \
				 dummy, &x, &y, &z );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PL_VERTEX " << endl;
		}

			//----- vertex position
		P( x, y, z );

			//----- store read vertex to list
		polylineVertexList.push(P);		
	}
	//==========================================//
	else if ( !strcmp(doit, FR_END_POLYLINE ) ) {
#if defined DEBUG_FR_CA
		cerr << "END_POLYLINE" << endl;
#endif 
			//----- precessing POLYLINE data 
		makePolylineImage();
//		if( !flag_immediate_mode ) {	makePolylineImage()       ; }
//		else                       {	drawTransientPolyline() ; }

			//----- clear working space
		polylineVertexList.clear();
	}
	//==========================================//
	else if ( !strcmp(doit, FR_MARK_CIRCLE_2D ) ) {
#if defined DEBUG_FR_CA
		cerr << "CIRCLE_2D" << endl;
#endif 

		int size_dim = 3 ;
		FRMark	mark ; 
		ColorType color; color = brepFactory.GetAttribute()->getKd() ;
		SetMarkAttributes(	message, 
					FRMark::CIRCLE2D, 
					size_dim , 
					color    ,
					&mark ); 
		markServer.push( mark );
	}

	//==========================================//
	else if ( !strcmp(doit, FR_MARK_CIRCLE_2DS ) ) {
#if defined DEBUG_FR_CA
		cerr << "CIRCLE_2DS" << endl;
#endif 

		int size_dim = 2 ;
		FRMark	mark ; 
		ColorType color; color = brepFactory.GetAttribute()->getKd() ;
		SetMarkAttributes(	message, 
					FRMark::CIRCLE2D, 
					size_dim , 
					color    ,
					&mark ); 
		markServer.push( mark );
	}

	//==========================================//
	else if ( !strcmp(doit, FR_MARK_SQUARE_2D ) ) {
#if defined DEBUG_FR_CA
		cerr << "SQUARE_2D" << endl;
#endif 

		int size_dim = 3 ;
		FRMark	mark ; 
		ColorType color; color = brepFactory.GetAttribute()->getKd() ;
		SetMarkAttributes(	message, 
					FRMark::SQUARE2D, 
					size_dim , 
					color    ,
					&mark ); 
		markServer.push( mark );

	}

	//==========================================//
	else if ( !strcmp(doit, FR_MARK_SQUARE_2DS ) ) {
#if defined DEBUG_FR_CA
		cerr << "SQUARE_2DS" << endl;
#endif 

		int size_dim = 2 ;
		FRMark	mark ; 
		ColorType color; color = brepFactory.GetAttribute()->getKd() ;
		SetMarkAttributes(	message, 
					FRMark::SQUARE2D, 
					size_dim , 
					color    ,
					&mark ); 
		markServer.push( mark );

	}
	//==========================================//
	else if ( !strcmp(doit, FR_FONT_NAME ) ) {
#if defined DEBUG_FR_CA
		cerr << "FONT_NAME" << endl;
#endif 
		//----- local
		int scanf_status ;
		char fontname [WORD_MAX] ; strcpy( fontname, DEFAULT_FONT_NAME );

		//----- 
		scanf_status = \
		sscanf( message, "%s %s", dummy, fontname);

		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), FONT_NAME" << endl;
		}

		//----- 
		ps.setFont( fontname );

	}

	//==========================================//
	else if ( !strcmp(doit, FR_MARK_TEXT_2D ) ) {
#if defined DEBUG_FR_CA
		cerr << "TEXT_2D" << endl;
#endif 

		int size_dim = 3 ;
		FRMark	mark ; 
		ColorType color; color = brepFactory.GetAttribute()->getKd() ;
		SetMarkTextAttributes(	message, 
					FRMark::TEXT2D, 
					size_dim , 
					color    ,
					&mark ); 
		markServer.push( mark );

	}

	//==========================================//
	else if ( !strcmp(doit, FR_MARK_TEXT_2DS ) ) {
#if defined DEBUG_FR_CA
		cerr << "TEXT_2DS" << endl;
#endif 

		int size_dim = 2 ;
		FRMark	mark ; 
		ColorType color; color = brepFactory.GetAttribute()->getKd() ;
		SetMarkTextAttributes(	message, 
					FRMark::TEXT2D, 
					size_dim , 
					color    ,
					&mark ); 
		markServer.push( mark );

	}

	//==========================================//
	else if ( !strcmp(doit, FR_TEXT_2DS ) ) {
#if defined DEBUG_FR_CA
		cerr << "TEXT_2DS" << endl;
#endif 
		FRVisText2D	vis_text2d ; 

		char		dummy [WORD_MAX] ;	
		char		dummy2[WORD_MAX] ;	
		int		scanf_status ;
		Vector2		P_mm ;
		double		size_pt ; 
		ColorType	color; 


		//----- get current color 
		color = brepFactory.GetAttribute()->getKd() ;

		//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg %s" ,  \
		       dummy, &(P_mm.x), &(P_mm.y), &size_pt, dummy2 );

		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer (TEXT_2DS)";
			cerr << endl;
		}

			//----- get string 
		FRString	string( message ) ;
		int		i;
		const	int	NUM_SKIP = 4 ;

		for ( i = 0 ; i < NUM_SKIP ; i++ ) {
			string.RemoveFirstWord();	
		}

			//----- set attributes to text
		vis_text2d.SetString     ( string  );
		vis_text2d.SetPosition_mm( P_mm    );
		vis_text2d.SetColor      ( color   );
		vis_text2d.SetSize_pt    ( size_pt );


			//----- set text to server
		vistextServer.push( vis_text2d );

	}


	//==========================================//
	else if ( !strcmp(doit, FR_BASE_VECTOR ) ) {
		
#if defined DEBUG_FR_CA
		cerr << "BASE_VECTOR" << endl;
#endif 

		int		scanf_status ;
		double		x1 = 0.0, y1 = 0.0, z1 = 0.0 ;
		double		x2 = 0.0, y2 = 0.0, z2 = 0.0 ; 
		FRVector3	e1, e2 ;

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg %lg %lg %lg", \
				 dummy, &x1, &y1, &z1, &x2, &y2, &z2 ) ; 

		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), BASE_VECTOR" << endl;
		}

			//----- base vector
		e1( x1, y1, z1 );	e2( x2, y2, z2 ); 

			//----- set value to brep generator
		brepFactory.SetBaseVector( e1, e2 ) ;

	}
	//==========================================//
	else if ( !strcmp(doit, FR_ORIGIN ) ) {

#if defined DEBUG_FR_CA
		cerr << "ORIGIN" << endl;
#endif 
		
		int		scanf_status ;
		FRVector3	origin ;
		double		x = 0.0, y = 0.0, z = 0.0 ;

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg"      , \
				 dummy, &x, &y, &z );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), ORIGIN " << endl;
		}

			//----- origin
		origin( x, y, z );

			//----- set value to brep generator
		brepFactory.SetOrigin( origin ) ;

	}
	//==========================================//
	else if ( !strcmp(doit, FR_NDIV ) ) {
		
#if defined DEBUG_FR_CA
		cerr << "NDIV" << endl;
#endif 

		int	scanf_status ;
		int	ndiv = DEFAULT_NDIV ;

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %d" , dummy, &ndiv );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), NDIV " << endl;
		}

			//----- set value to brep generator
		brepFactory.SetNdiv( ndiv ) ;

	}
	//==========================================//
	else if ( !strcmp( doit, FR_DIFFUSE_RGB ) || \
		  !strcmp( doit, FR_COLOR_RGB   )      ) 
	{
		
#if defined DEBUG_FR_CA
		cerr << "COLOR_RGB" << endl;
#endif 

		int	scanf_status ;
		double red, green, blue ;


			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg",	
				 dummy, &red, &green, &blue   );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), DIFFUSE_RGB " << endl;
		}

			//----- set value to brep generator
		curAttribute.setKd( ColorType( red, green, blue ) ) ;
		brepFactory.SetAttribute( &curAttribute ) ;

	}
	//==========================================//
	else if ( !strcmp( doit, FR_SPECULAR_RGB ) ) {
		
#if defined DEBUG_FR_CA
		cerr << "SPECULAR_RGB" << endl;
#endif 

		int	scanf_status ;
		double red, green, blue ;

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg",	
				dummy, &red, &green, &blue   );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), SPECULAR_RGB " << endl;
		}

			//----- set value to brep generator
		curAttribute.setKs( ColorType( red, green, blue ) ) ;
		brepFactory.SetAttribute( &curAttribute ) ;

	}
	//==========================================//
	else if ( !strcmp( doit, FR_PHONG_POWER ) ) {
#if defined DEBUG_FR_CA
		cerr << "PHONG_POWER" << endl;
#endif 
		
		int	scanf_status ;
		int	phong_power ;

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %d" , dummy, &phong_power );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), PHONG_POWER " << endl;
		}

			//----- set value to brep generator
		curAttribute.setPhongPower( phong_power );
		brepFactory.SetAttribute( &curAttribute ) ;
	}
	//==========================================//
	else if (	( !strcmp( doit, FR_FORCE_WIREFRAME ) ) || \
			( !strcmp( doit, FR_TRANSPARENCY    ) )      ) 
	{
#if defined DEBUG_FR_CA
		cerr << "FORCE_WIREFRAME" << endl;
#endif 
		
		int	scanf_status ;
		int	bool_flag = 0 ; // opaque

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %d" , dummy, &bool_flag );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), TRANSPARENCY " << endl;
		}

			//----- 0 or 1 
		if( parameter.flag_swap_transparency ) {
		  bool_flag = ( bool_flag == 0 );
		} else {
		  bool_flag = ( bool_flag != 0 );
		}

			//----- set value to brep generator
		curAttribute.setTransparency( bool_flag );
		brepFactory.SetAttribute( &curAttribute ) ;
	}
	//==========================================//
	else if ( !strcmp( doit, FR_VISIBILITY ) ) {
#if defined DEBUG_FR_CA
		cerr << "VISIBILITY" << endl;
#endif 
		
		int	scanf_status ;
		int	bool_flag = 1 ; // visible

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %d" , dummy, &bool_flag );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), VISIBILITY " << endl;
		}

			//----- 0 or 1 
		bool_flag = ( bool_flag != 0 ); 

			//----- set value to brep generator
		curAttribute.setVisibility( bool_flag );
		brepFactory.SetAttribute( &curAttribute ) ;
	}
	//==========================================//
	else if( !strcmp( doit, FR_CLEAR_DATA ) )   {

#if defined DEBUG_FR_CA
		cerr << "CLEAR_DATA" << endl;
#endif 
		this->clearData() ;

	} 
	//==========================================//
	else if( !strcmp(doit, FR_WIREFRAME ) ) {
#if defined DEBUG_FR_CA
		cerr << "WIREFRAME" << endl;
#endif 
			//----- default action
		parameter.drawing_mode = WIREFRAME ;

			//----- if the envionmental variable is set ...
		if( parameter.flag_accumulate_data  ) 
		{
			parameter.drawing_mode = WF_STORED ; 
		} 

			//----- output revised parameters
		command.writeParameterToHistoryFile( parameter ) ; 

	} 
	//==========================================//
	else if( !strcmp(doit, FR_SURFACE ) ) {
#if defined DEBUG_FR_CA
		cerr << "SURFACE" << endl;
#endif 
			//----- Lambert shading mode
		parameter.drawing_mode = HID ;

			//----- output revised parameters
		command.writeParameterToHistoryFile( parameter ) ; 
	} 
	//==========================================//
	else if( !strcmp(doit, FR_LINES) ) {
#if defined DEBUG_FR_CA
		cerr << "LINES" << endl;
#endif 
			//----- Line drawing mode
		parameter.drawing_mode = HID2 ;

			//----- output revised parameters
		command.writeParameterToHistoryFile( parameter ) ; 
	} 
	//==========================================//
	else if( !strcmp(doit, FR_BOUNDING_BOX ) ) {

#if defined DEBUG_FR_CA
		cerr << "BOUNDING_BOX" << endl;
#endif 

		int	scanf_status ;
		double xmin = 0.0 , ymin = 0.0 , zmin = 0.0 ; 
		double xmax = 0.0 , ymax = 0.0 , zmax = 0.0 ; 

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg %lg %lg %lg",	
				dummy, \
				&xmin, &ymin, &zmin, \
				&xmax, &ymax, &zmax             );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), BOUNDING_BOX" << endl;
		}

		command.setTotalBoundingBox(	xmin, ymin, zmin , 
						xmax, ymax, zmax , 
						&totalBoundingBox ); 
		command.setTotalBoundingBoxToParameter(	totalBoundingBox ,
						        &parameter        ) ; 
		

	} 
	//==========================================//
	else if ( !strcmp(doit, FR_CAMERA_POSITION ) ) {
		
#if defined DEBUG_FR_CA
		cerr << "CAMERA_POSITION" << endl;
#endif 

		int	scanf_status ;
		double	rho = 0.0 , v_angle = 0.0 , h_angle = 0.0 ;

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg",	
				dummy, &rho, &v_angle, &h_angle        );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), CAMERA_POSITION" << endl;
		}

			//----- set camera position
		command.setCameraPositionToParameter( rho, v_angle, h_angle , parameter ) ;
		command.writeParameterToHistoryFile( parameter ) ; // output revised parameters
	}

	//==========================================//
	else if ( !strcmp(doit, FR_TARGET_POINT ) ) {
		
#if defined DEBUG_FR_CA
		cerr << "TARGET_POINT" << endl;
#endif 

		int	scanf_status ;
		double	x = 0.0 , y = 0.0 , z = 0.0 ;

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg",	
				dummy, &x, &y, &z   );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), TARGET_POINT" << endl;
		}

			//----- set camera position
		command.setTargetPointToParameter( x, y, z , parameter ) ;
		command.setAutoTargetFlagToParameter( 0 , parameter ) ;
			// parameter.flag_auto_target = 0
		command.writeParameterToHistoryFile( parameter ) ; 
	}
	//==========================================//
	else if ( !strcmp(doit, FR_ZOOM_FACTOR ) ) {
		
#if defined DEBUG_FR_CA
		cerr << "ZOOM_FACTOR" << endl;
#endif 

		int	scanf_status ;
		double	zoom_factor;
		double  st_x, st_y, st_z ; // standard target

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg %lg %lg %lg",	
				dummy, &zoom_factor, &st_x, &st_y, &st_z );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), ZOOM_FACTOR" << endl;
		}

			//----- calc and set focal distance
		Parameter    param_tmp ; param_tmp = parameter ;
					param_tmp.scale = zoom_factor;
		Perspective  camera_tmp; 

		command.setScaleToParameter ( zoom_factor, parameter) ;
			// Set a dummy value to parameter.scale.
			// The set value is unused,
			// since flag_focal_distance_given
			// is set to 1 below.
		camera_tmp.setTarget( Vector3(st_x, st_y, st_z ) );
			// Set target point to camera_tmp.
			// Focal distance is calc below,
			// assuming that camera is targetting 
			// the "standard target point".
		camera_tmp.setCameraPosition( parameter.camera_distance , 
					      parameter.camera_v_angle  , 
					      parameter.camera_h_angle  );
			// Set camera position to camera_tmp.
			// We assume that it is already set to "paramater".
		command.autoZoom( totalBoundingBox, param_tmp, camera_tmp ) ;
			// Focal distance is calculated and 
			// returned to param_tmp (and also to camera_tmp).
			// We assume that totalBoundingBox is 
			// is already calculated.
		command.setFocalDistanceToParameter(param_tmp.focal_distance,\
						    parameter               ) ;
			// Set focal distance to "parameter" database.
		camera.setFocalDistance( param_tmp.focal_distance, param_tmp.epsilon3d );
			// Set calculated focal distance real camera.
			// Auto zoom will be is skipped, since
			// parameter.flag_focal_distance_given is set to 1
			// below.
		parameter.flag_focal_distance_given = 1 ;
			// Remember that focal distance is directly given
		command.writeParameterToHistoryFile( parameter ) ; 
			// parameter.scale is output as fdXXX
	}
	//==========================================//
	else if ( !strcmp(doit, FR_SCALE ) ) {
#if defined DEBUG_FR_CA
		cerr << "SCALE" << endl;
#endif 
		int	scanf_status ;
		double	scale;

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg",	
				dummy, &scale );
		if( scanf_status == EOF ) {
			cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), SCALE" << endl;
		}
		command.setScaleToParameter ( scale, parameter ) ;
		parameter.flag_focal_distance_given = 0 ;
		command.writeParameterToHistoryFile( parameter ) ; 
			// parameter.scale is output as fdXXX
	}
	//==========================================//
	else if ( !strcmp(doit, FR_FOCAL_DISTANCE ) ) {
		
#if defined DEBUG_FR_CA
		cerr << "FR_FOCAL_DISTANCE" << endl;
#endif 

		int	scanf_status ;
		double	focal_distance;

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %lg", dummy, &focal_distance );
		if( scanf_status == EOF ) {
		  cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute (), ";
		  cerr << "FR_FOCAL_DISTANCE" << endl;
		}

			//----- calc and set focal distance
		command.setScaleToParameter ( 0.9, parameter ) ;
			// Set a dummy value to parameter.scale.
			// The set value is unused,
			// since flag_focal_distance_given
			// is set to 1 below.
		command.setFocalDistanceToParameter(focal_distance, parameter);
			// Set focal distance to "parameter" database.
		camera.setFocalDistance( focal_distance , parameter.epsilon3d );
			// Set received focal distance real camera.
			// Auto zoom will be is skipped, since
			// parameter.flag_focal_distance_given is set to 1
			// below.
		parameter.flag_focal_distance_given = 1 ;
			// Remember that focal distance is directly given
		command.writeParameterToHistoryFile( parameter ) ; 
			// parameter.scale is output as fdXXX
	}

	//==========================================//
	else if ( !strcmp(doit, FR_GUI ) ) {
#if defined DEBUG_FR_CA
		cerr << "GUI" << endl;
#endif 

		command.setParameter( parameter );  // revise parameters with GUI

#if defined USE_UNIX
#if defined USE_SOCKET 

		if( parameter.flag_IP_mode != 1 ) { server_p->SendLine( FR_GUI );} 
				// if non-viewer mode, sendback the same message.
#endif
#endif
	}

	//==========================================//
	else if ( !strcmp(doit, FR_DEVICE ) ) {
#if defined DEBUG_FR_CA
		cerr << "DEVICE" << endl;
#endif 
		int	scanf_status ;
		int	dev        ;

			//----- scan value
		scanf_status = \
		sscanf( message, "%s %d", dummy, &dev  );
		if( scanf_status == EOF ) {
		  cerr << "ERROR: FRCommandAnalyzer::analyzeAndExecute(), ";
		  cerr << "FR_DEVICE" << endl;
		}

		command.setDeviceToParameter( dev, parameter );
		command.writeParameterToHistoryFile( parameter ) ;
	}

	//==========================================//
	else if ( !strcmp(doit, FR_SET_CAMERA ) ) {
#if defined DEBUG_FR_CA
		cerr << "SET_CAMERA" << endl;
#endif 
			//----- set parameters to detabase
		parameter.flag_skip_gui = 1 ;
		command.setParameter( parameter ); 
		parameter.flag_skip_gui = 0 ;

			//----- set camera parameters
		command.setCameraAuto( totalBoundingBox, parameter , camera );

			//----- output the result
////////////////////////////////////////////////////////////////
//		parameter.flag_focal_distance_given = 1 ;    //
///////////////////////////////////////////////////////////////
		command.writeParameterToHistoryFile( parameter ) ;

			//----- set canera to hid
		hid.setCamera( &camera ) ;		
	}
	//==========================================//
	else if ( !strcmp(doit, FR_OPEN_DEVICE ) ) {
#if defined DEBUG_FR_CA
		cerr << "OPEN_DEVICE" << endl;
#endif 
#if    defined USE_OPEN_GL
		command.openDevice( camera, parameter, ps, xwin, ps2x, gl );
#elif  defined USE_XWIN 
		command.openDevice( camera, parameter, ps, xwin, ps2x     );
#else
		command.openDevice( camera, parameter, ps  );
#endif
	}
	//==========================================//
	else if ( !strcmp(doit, FR_CLOSE_DEVICE   ) ) {
#if defined DEBUG_FR_CA
		cerr << "CLOSE_DEVICE" << endl;
#endif 
			//----- close device and get X event
#if   defined USE_OPEN_GL
		command.closeDevice( parameter, ps, xwin, ps2x, gl );
#elif defined USE_XWIN 
		command.closeDevice( parameter, ps, xwin, ps2x     );
#else
		command.closeDevice( parameter, ps );
#endif

		if( parameter.flag_accumulate_data ) { 
			// do not clear data
		} else {
			this->clearData() ;	
		}
	}
	//==========================================//
	else if ( !strcmp(doit, FR_BEGIN_MODELING ) ) {
#if defined DEBUG_FR_CA
		cerr << "BEGIN_MODELING (do nothing)" << endl;
#endif 
			//----- primitive counter initialization
		num_primitive = 0 ;

			//----- mode for constructing winged edge structure
		command.setWingedEdgeModeToParameter( parameter ) ; 
		brepFactory.setWingedEdgeMode( parameter.winged_edge_mode );		

	}
	//==========================================//
	else if ( !strcmp(doit, FR_END_MODELING ) ) {
#if defined DEBUG_FR_CA
		cerr << "END_MODELING (flush, hidline)" << endl;
#endif 

			//-----------------//
			//----- FLUSH -----//
			//-----------------//
		switch ( parameter.drawing_mode ) {
			case WIREFRAME:
			case WF_STORED:		
				break ; // do nothing 
			default: // HID etc

			command.doIncrementalHiddenSurfaceElimination(	parameter          ,
									objectServer       , 
									hid                , 
									hidline            , 1  );
			break ;
		} // switch 


			//------------------------------------//
			//----- Hidden line elimination  -----//
			//------------------------------------//
			//----- Final set up before doing hidden line removal
			//..... (1) list ---> array
			//..... (2) pass address of hid

		command.doHiddenLineElimination(	parameter      , 
							linesegServer  ,
							hidline        ,
							hid            ,
							camera           ) ;

			//----- set value to brep generator
		ColorType step_color ( DEFAULT_STEP_COLOR_R, DEFAULT_STEP_COLOR_G, DEFAULT_STEP_COLOR_B ) ;
		curAttribute.setKd( step_color );
		brepFactory.SetAttribute( &curAttribute ) ;
	}
	//==========================================//
	else if ( !strcmp(doit, FR_DRAW_ALL ) ) {
#if defined DEBUG_FR_CA
		cerr << "DRAW_ALL" << endl;
#endif 

#if defined    USE_OPEN_GL
		command.drawAll(	parameter    ,
					ps           ,
					gl	     ,
					objectServer ,
					camera       ,
					hid          ,
					hidline      ,  
					""            );
#elif defined  USE_XWIN
		command.drawAll(	parameter    ,
					ps           ,
					objectServer ,
					camera       ,
					hid          ,
					hidline      ,  
					""            );

#else
		command.drawAll(	parameter    ,
					ps           ,
					objectServer ,
					camera       ,
					hid          ,
					hidline      ,  
					""            );

#endif


		drawAllMark();

		drawAllVisTexts(); 

		if( parameter.flag_output_polygon ) 
		{
			if(	parameter.drawing_mode == HID  || \
				parameter.drawing_mode == HID2 || \
				parameter.drawing_mode == HID3       )
			{
				command.savePolygon( parameter, hid ); 
				command.saveCamera ( parameter      ); 			
			}
		}


	}
	//==========================================//
	else if ( !strcmp(doit, FR_QUIT ) ) {

#if defined DEBUG_FR_CA
		cerr << "QUIT" << endl;
#endif 
		flag_saving_prim_data = 0 ;
		solid_out.close();
		status = 0 ;
	}
	//==========================================//
	else if ( !strcmp(doit, FR_TERMINATE_DAWND ) ) {

#if defined DEBUG_FR_CA
		cerr << "TERMINATE_DAWND" << endl;
#endif 
		flag_saving_prim_data = 0 ;
		solid_out.close();
		status = -1 ;
	}
	//==========================================//
	else if ( !strcmp(doit, FR_POLYHEDRON ) ) {
#if defined DEBUG_FR_CA
		cerr << "POLYHEDRON" << endl;
#endif 
		cur_vertex_label = 0 ;

		setObjectToFactory("POLYHEDRON");

		brepFactory.GenerateCoordBlock();
		brepFactory.GenerateAttributeBlock();
		brepFactory.GenerateDummyVertex();
	}
	//==========================================//
	else if ( !strcmp(doit, FR_VERTEX ) ) {
#if defined DEBUG_FR_CA
		cerr << "VERTEX" << endl;
#endif 
		if( brepFactory.GetObject() ) {
			cur_vertex_label++ ;
			Polyhedron::GenerateOneVertex(cur_vertex_label,
						      FRString( message ), 
						      &brepFactory );

		}
	}
	//==========================================//
	else if ( !strcmp(doit, FR_FACET ) ) {
#if defined DEBUG_FR_CA
		cerr << "FR_FACET" << endl;
#endif 
		if( brepFactory.GetObject() ) {
			FRString  str( message );  // /Facet a b c d 
			str.RemoveFirstWord();     // a b c d

			Polyhedron::GenerateOneFacet( FRString( message ), 
						      &brepFactory );
		}

	}
	//==========================================//
	else if ( !strcmp(doit, FR_END_POLYHEDRON ) ) {
#if defined DEBUG_FR_CA
		cerr << "END_POLYHEDRON" << endl;
#endif 
			//----- count and display
		incr_displayNumPrimitives( );

			//----- complete generation of polyhedron data
		brepFactory.GenerateOptimizedData();

			//----- precessing polyhedron data 
		makeObjectImage();

			//----- get out of brep-reading mode
		brepFactory.setObject(NULL) ;
	}
	//==========================================//
	else if ( !strcmp(doit, FR_CLIPPING_PLANE ) ) {
#if defined DEBUG_FR_CA
		cerr << "CLIPPING_PLANE" << endl;
#endif 
		// DO NOTHING (data used by DAWNCUT utility)
	}
	//==========================================//
	else if ( !strcmp(doit, FR_SAVE ) ) {
#if defined DEBUG_FR_CA
		cerr << "SAVE" << endl;
#endif 
		if( !flag_saving_prim_data ) {
			if( !(parameter.flag_accumulate_data) )  
			{
				solid_out.open( primFile );
				flag_saving_prim_data = 1 ;
			}
		}
	}
	//==========================================//
	else if ( !strcmp(doit, FR_END_SAVE ) ) {
#if defined DEBUG_FR_CA
		cerr << "END_SAVE" << endl;
#endif 
		if( flag_saving_prim_data ) {
			if( !(parameter.flag_accumulate_data) )  
			{
				flag_saving_prim_data = 0 ;
				solid_out.close();
			}
		}
	}
	//==========================================//
	else if ( !strcmp(doit, FR_WAIT ) ) {
#if defined DEBUG_FR_CA
		cerr << "WAIT" << endl;
#endif 
#if defined USE_XWIN
		waitEvent( server_p );
#endif
	}
	//==========================================//
	else if ( !strcmp(doit, FR_PAUSE ) ) {
#if defined DEBUG_FR_CA
		cerr << "PAUSE" << endl;
#endif 
#if defined USE_XWIN
		int flag_sendback = 0 ;
		waitEvent( server_p , flag_sendback );
#endif
	}
	//==========================================//
	else {
		cerr << " ***** UNDEFINED command " << doit << endl;
	}


		//----- return status
	return status ;

} // FRCommandAnalyzer::analyzeAndExecute ()
