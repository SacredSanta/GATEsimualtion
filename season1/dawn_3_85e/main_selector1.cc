#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream.h>

#include "main_selector.h"

	//----- output parameters
// #define DEBUG_MAIN1

int
MainSelector::standAlone( int argc, char* argv[] , Command& command, Parameter& parameter )
{

	cerr << "***** B-rep mode (-b option)" << endl << endl;

		//---------------------//
		//----- variables -----//
		//---------------------//
	PostScript		ps ;
#if defined USE_XWIN
	X_Window		xwin ;
	PS2X			ps2x ;
#endif

#if defined USE_OPEN_GL
	FROpenGL		gl;
#endif
	List<Object>		objectServer ;
	List<LineSegment>	linesegServer ;
	List<NameAndValue>	objectNameServer ;
	FileReader		filereader ;
	Perspective		camera ;
	Hid			hid  ;
	HidLine			hidline ;
	Box3D			totalBoundingBox ;
	DataSize		totalDataSize    ;

		//------------------------------//
		//------ parameter setting -----//
		//------------------------------//
		//..... set user parameter
	command.setParameter( parameter );

		//-------------------------------------------//
		//----- set camera based on "parameter" -----//
		//-------------------------------------------//
	command.setCamera( parameter , camera );

		//-----------------------------//
		//----- scan data file -----//
		//-----------------------------//
	command.scanDataFile( argv, argc, filereader, camera, objectNameServer , totalBoundingBox , parameter );


		//--------------------------------------------------//
		//----- set total bounding box to  "parameter" -----//
		//--------------------------------------------------//
		//..... reset: parameter.*min,  parameter.*max
	command.setTotalBoundingBoxToParameter( totalBoundingBox, &parameter ) ;


		//--------------------------------------------------------------------//
		//----- reset parameter based on scanned data for AUTO attribute -----//
		//--------------------------------------------------------------------//
	if( parameter.flag_read_polygon_out ) {

			//----- polygon.out data is read!

			//----- reset: parameter.camera_distance
			//.....        parameter.camera_v_angle
			//.....        parameter.camera_h_angle
			//.....        parameter.target_xyz
			//.....        parameter.focal_distance
			//.....        camera.setRho(), setPhi(),  setTheta(), 
			//.....        setTarget(), setFocalDistance()
		command.setCamera( parameter , camera ); 

	}  else {

		command.setCameraAuto( totalBoundingBox, parameter , camera ); 

	} // if( parameter.flag_read_polygon_out )--else 


		//------------------------------------//
		//----- set tuned camera  to hid -----//
		//------------------------------------//
	hid.setCamera( &camera ) ;

		//---------------------------------------------------//
		//------ save current parameter to history file -----//
		//---------------------------------------------------//
	command.writeParameterToHistoryFile( parameter ) ; 

		//-----------------------//
		//----- open device -----//
		//-----------------------//
#if   defined USE_OPEN_GL
	command.openDevice( camera, parameter, ps, xwin, ps2x ,gl );
#elif defined USE_XWIN
	command.openDevice( camera, parameter, ps, xwin, ps2x     );
#else 
	command.openDevice( camera, parameter, ps  );
#endif


		//------------------------------------------------------------//
		//----- pass data to hidden line/surface removal routine -----//
		//------------------------------------------------------------//
	switch ( parameter.drawing_mode ) {
	 case WIREFRAME:
		if( parameter.device_label == OPEN_GL ){
#if defined USE_OPEN_GL
		command.readDataFile_DrawGL_Clear(	parameter          , 
							objectNameServer ,
							totalDataSize    ,
							camera             ,
							gl                  );
#endif
		}else{
		command.readDataFile_DrawWireFrame_Clear(	parameter          , 
								filereader         , 
								objectNameServer ,
								objectServer      , 
								linesegServer     ,
								totalDataSize    ,
								ps           ,
								camera        );
		}
		break ; // do nothing 

	 case WF_STORED:		
		if( parameter.device_label == OPEN_GL ){

#if defined USE_OPEN_GL
		command.readDataFile_DrawGL_Clear(	parameter          , 
							objectNameServer ,
							totalDataSize    ,
							camera             ,
							gl                  );
#endif
		}else{
		parameter.flag_haloing = 0 ; 
		command.readDataFile( parameter, filereader, objectNameServer, objectServer, linesegServer , totalDataSize );
		command.passEdgeToHidLineCMD(	objectServer  , 
						camera        ,
						hidline       ,
						parameter      );
		}
		break ;

	 default: // HID etc
		if( parameter.device_label == OPEN_GL ){
#if defined USE_OPEN_GL
		command.readDataFile_DrawGL_Clear(	parameter          , 
							objectNameServer ,
							totalDataSize    ,
							camera             ,
							gl                  );
#endif

		}else{
		command.readDataFile_doHiddenSurfaceElimination(	parameter        ,
									filereader       , 
									objectNameServer , 
									objectServer     , 
									linesegServer    , 
									hid              , 
									hidline          ,
									totalDataSize );
		}
		break ;

	} // switch ( parameter.drawing_mode ) 

		//----------------------------------------------------------------//
 		//----- Pass linesegment data to HidLine                     -----//
		//----- (in addition to edge data                            -----//
		//----- and reset linetype of linesegments in linesegServer )-----//
		//----------------------------------------------------------------//
	command.passLineSegmentToHidLine (	linesegServer , 
						camera        ,  
						hidline       , 
						parameter      ) ;

		//------------------------------------//
		//----- Hidden line elimination  -----//
		//------------------------------------//
		//----- Final set up before doing hidden line removal
		//..... (1) list ---> array
		//..... (2) pass address of hid
	hidline.install();
	hidline.setHid( &hid ); // reset hid

		//----- Do hidden line removal
	if (	parameter.drawing_mode >= WF_STORED && \
		parameter.drawing_mode <= HID3            )		
	{
		command.doHiddenLineElimination( parameter, hidline, parameter.epsilon3d, parameter.epsilon2d ) ;
	}


		//------------------------------------------//
		//----- drawing and painting to device -----//
		//------------------------------------------//
#if    defined USE_OPEN_GL
	command.drawAll(	parameter    ,
				ps           ,
				gl	     ,
				objectServer ,
				camera       ,
				hid          ,
				hidline      ,  
				""            );
#elif  defined USE_XWIN
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


		//----- Close Device and Invoke Previewer -----//
		//---------------------------------------------//
#if    defined USE_OPEN_GL
	command.closeDevice( parameter, ps, xwin, ps2x, gl );
#elif  defined USE_XWIN
	command.closeDevice( parameter, ps, xwin, ps2x     );
#else 
	command.closeDevice( parameter, ps );
#endif

		//----------------------//
		//----- Wait Event -----//
		//----------------------//
#if    defined USE_XWIN
	command.waitEvent( parameter , xwin );
#endif

		//--------------------------------------//
		//----- save visible-polygon data  -----//
		//--------------------------------------//
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

		//---------------------//
		//----- Debug info-----//
		//---------------------//
#if defined DISPLAY_LOG
	command.printDataSize( totalDataSize , cerr ) ;	cerr << "\n\n";
#endif
		//----- return 
	return 1 ;

} // MainSelector::standAlone()
