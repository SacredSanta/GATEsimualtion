////////////////////////////////
/////  command2.cc         /////
/////  (I/O of parameters) /////
////////////////////////////////

#include "dev_control.h"

#if defined USE_VC
	#include <strstrea.h>
#else
	#include <strstream.h>
#endif

#include "command.h"

// #define  DEBUG_COMMAND2

	//----- Command::setParameter()
void 
Command::setParameter( Parameter& parameter ) 
{
		//----- default parameter
	const Parameter	default_parameter ;

		//------ read history file 
	if( !readHistoryFile( parameter ) ) 
	{
		// error recovery
		writeParameterToHistoryFile( parameter ) ; 
		readHistoryFile( parameter ) ; 
	}

		//----- use GUI to imput parameters
		//.....  Note: If flag_batch is 1, 
		//.....        flag_skip_gui is automatically made 1,
		//.....        in Parameter::Parameter()
	if( !parameter.flag_skip_gui ) {
		this->inputParameter_Tk( parameter );
		writeParameterToHistoryFile( parameter ) ; 
			// Note: history file generated by GUI
			//       may be incomplete.
	}

		//----- Batch mode 
		//..... rewrite parameters read from the history file
	if( parameter.flag_batch )
	{
		// Device 
		parameter.device_label  = PS ; 	

		if( getenv( "DAWN_DEVICE" ) != NULL ) {
		  if ( !strcmp( getenv( "DAWN_DEVICE" ),"3" ) ) {
		    parameter.device_label  = PS2 ; 	
		  }
		}

		// Append mode
		if( !strcmp( getenv( "DAWN_BATCH" ),"a" ) || !strcmp( getenv( "DAWN_BATCH" ),"A" ) )
		{
			parameter.flag_append_psfile = 1 ;
			parameter.flag_add_showpage  = 1 ;
		}

		// Message
		cerr << "***** Note: DAWN is running in the BATCH mode." << endl;
		cerr << "            Rendering parameters in the parameter file are used," << endl;
		cerr << "            unless they are overwritten with, e.g., Geant4 (G)UI." << endl;
		cerr << "            In order to remove the parameter file, " << endl;
		cerr << "            invoke DAWN with -c option: \"dawn - c\". " << endl;
		cerr << endl;

	}

		//----- Non-supprted drawing modes
	if( parameter.drawing_mode >= DRAWING_MODE_END ) {
		cerr << "WARNING: This drawing mode is not supported yet." << endl;
		cerr << "         Default drawing mode is adopted instead." << endl;
		parameter.drawing_mode = default_parameter.drawing_mode ;
	}

		//----- Non-supprted drawing device
	if ( parameter.device_label >= DEVICE_END ) {
		cerr << "WARNING: This device is not supported yet." << endl;
		cerr << "         Default device is adopted instead." << endl;
		parameter.device_label = default_parameter.device_label ;
	}

} // Command::setParameter()


	//----- Command::readHistoryFile()
int
Command::readHistoryFile(  Parameter& parameter , char* history_file_given )
{

		//----- local
	int		status ;
	ifstream	hin ;
	char		history_file[256] ;
	char		FOCAL_LENGTH_HEADER[]   = "fd";

		//----- set history file name
	if( history_file_given != NULL ) {
		strcpy( history_file, history_file_given ) ;
	} else {
		strcpy( history_file, parameter.history_file ) ;		
	}

		//----- open history file
	hin.open( history_file ) ;
		
		//----- read parameter into the 2nd arg
	if( !hin ) {
		status = ERROR ;
		// do nothing
	} else {

			//----- local
		char	buf[256] ;
		double	val      ;
		const Parameter param_default ;

			//-----
		status = NORMAL ;

			//----------------------------------//
			//----- Page 1 of GUI (Camera) -----//
			//----------------------------------//
		hin >> parameter.camera_distance ;
		hin >> parameter.camera_v_angle  ;
		hin >> parameter.camera_h_angle  ;
		hin >> parameter.flag_auto_target ;

		hin >> parameter.target_x ;
		hin >> parameter.target_y ;
		hin >> parameter.target_z ;

			//----- scale and focal_distance
		hin >> buf ;
		val = -1.0 ; // dummy
		if( ToolString::removeHeader( buf, FOCAL_LENGTH_HEADER , &val ) ) {
				//----- focal_distance is directly given
				//..... "scale" will not be used.
				//..... If val <= 0, focal_distance will be
				//..... recalculated based on "scale"
			parameter.flag_focal_distance_given = 1 ;
			parameter.focal_distance            = val ;
			parameter.scale                     = param_default.scale ; 
								// not used if val > 0

		} else {
				//----- scale is given
				//..... focal_distance will be 
				//..... calculated automatically
				//..... based on the value of "scale"
		  	parameter.flag_focal_distance_given = 0 ;
				// Auto calc of parameter.focal_distance
			parameter.focal_distance            = -1.0 ; 
				// This means auto, too. (To make it sure!)
				// See Command.autoZoom()

			int status_tmp = sscanf( buf, "%lg", &val ) ;
			if( status_tmp != EOF && val > 0.0 ) {
				parameter.scale           = val  ; 
			} else {
				Parameter param_tmp ;
				parameter.scale = param_tmp.scale ;
					// Use default value (e.g. 0.9)
			}

		}

		hin >> parameter.drawing_mode ;
		hin >> parameter.epsilon3d ;
		hin >> parameter.flag_display_parameter ;

			//---------------------------------//
			//----- Page 2 of GUI (Light) -----//
			//---------------------------------//

		hin >> parameter.Ip_red >> parameter.Ip_green >> parameter.Ip_blue ;
		hin >> parameter.Ia_red >> parameter.Ia_green >> parameter.Ia_blue ;
		hin >> parameter.light_v_angle >> parameter.light_h_angle ; // v_angle and h_angle

			//---------------------------------//
			//----- Page 3 of GUI (Edge) -----//
			//---------------------------------//

		hin >> parameter.real_edge_width      ;
		hin >> parameter.outline_width        ;
		hin >> parameter.aux_edge_width       ;

		hin >> parameter.aux_edge_style       ;
		hin >> parameter.aux_threshold_angle  ;
		hin >> parameter.line_width           ;
		hin >> parameter.flag_haloing         ;
		hin >> parameter.flag_back_edge_dashed ;

			//----------------------------------//
			//----- Page 4 of GUI (Device) -----//
			//----------------------------------//
		hin >> parameter.device_label ;
		hin >> parameter.ps_previewer ;
		hin >> parameter.flag_add_showpage ;
		hin >> parameter.flag_append_psfile ;

			//---------- END of pages ----------//
		hin.close();
	}

		//----- return status 
	return	status ;

} // Command::readHistoryFile()


	//----- Command::writeParameterToHistoryFile()
int
Command::writeParameterToHistoryFile(  const Parameter& parameter , char* history_file_given )
{

		//----- local
	int		status ;
	ofstream	hout ;
	char		history_file[256] ;


		//----- set history file name
	if( history_file_given != NULL ) {
		strcpy( history_file, history_file_given ) ;
	} else {
		strcpy( history_file, parameter.history_file ) ;		
	}

		//----- open history file
	hout.open( history_file ) ;
		
		//----- read parameter into the 2nd arg
	if( !hout ) {
		status = ERROR ;
		// do nothing
	} else {
		status = NORMAL ;

			//----------------------------------//
			//----- Page 1 of GUI (Camera) -----//
			//----------------------------------//

		hout << parameter.camera_distance << endl ;
		hout << parameter.camera_v_angle  << endl ; 
		hout << parameter.camera_h_angle  << endl ;
		hout << parameter.flag_auto_target  << endl ;

		hout << parameter.target_x        << endl ;
		hout << parameter.target_y        << endl ;
		hout << parameter.target_z        << endl ;

			// scale
		if( parameter.flag_focal_distance_given ) {
			hout << "fd" << parameter.focal_distance << endl ;
		} else {
			hout << parameter.scale << endl ;
		}

		hout << parameter.drawing_mode      << endl ;
		hout << parameter.epsilon3d       << endl ;
		hout << parameter.flag_display_parameter << endl ;

			//---------------------------------//
			//----- Page 2 of GUI (Light) -----//
			//---------------------------------//

		hout << parameter.Ip_red             << endl ;
		hout << parameter.Ip_green           << endl ;
		hout << parameter.Ip_blue            << endl ;

		hout << parameter.Ia_red             << endl ;
		hout << parameter.Ia_green           << endl ;
		hout << parameter.Ia_blue            << endl ;

		hout << parameter.light_v_angle   << endl ; // v_angle
		hout << parameter.light_h_angle   << endl ; // h_angle

			//---------------------------------//
			//----- Page 3 of GUI (Edge) -----//
			//---------------------------------//

		hout << parameter.real_edge_width << endl ;
		hout << parameter.outline_width   << endl ;
		hout << parameter.aux_edge_width  << endl ;

		hout << parameter.aux_edge_style  << endl ;

		hout << parameter.aux_threshold_angle << endl ;
		hout << parameter.line_width      << endl ;
		hout << parameter.flag_haloing    << endl ;
		hout << parameter.flag_back_edge_dashed << endl;

			//----------------------------------//
			//----- Page 4 of GUI (Device) -----//
			//----------------------------------//
		hout << parameter.device_label       << endl ;
		hout << parameter.ps_previewer       << endl ;		
		hout << parameter.flag_add_showpage  << endl ;
		hout << parameter.flag_append_psfile << endl ;

			//---------- END of pages ----------//
		hout.close();
	}

		//----- return status 
	return	status ;

} // Command::writeParameterToHistoryFile()


	//----- Command::printParameter()
void
Command::printParameter (  const Parameter& parameter , ostream& out)
{
			//----- Page 1 (Camera)
		out << "camera_distance          = " << parameter.camera_distance << endl ;
		out << "camera_v_angle           = " << parameter.camera_v_angle  << endl ; 
		out << "camera_h_angle           = " << parameter.camera_h_angle  << endl ;
		out << "flag_auto_target         = " << parameter.flag_auto_target  << endl ;
		out << "target                   = " << parameter.target_x << " " << parameter.target_y << " " << parameter.target_z << endl;
		out << "scale (fd)               = " << parameter.scale << " (" << parameter.focal_distance << ")" << endl ;
		out << "drawing_mode             = " << parameter.drawing_mode      << endl ;
		out << "##(1:WF, 2:WF-Stored, 3:Surface, 4:Line, 5:Surface2)" << endl;
		out << "epsilon3d                = " << parameter.epsilon3d       << endl ;
		out << "flag_display_parameter   = " << parameter.flag_display_parameter << endl ;
		out << endl;

			//----- Page 2 (Light)
		out << "Ip(RGB)                  = " << parameter.Ip_red << " " << parameter.Ip_green << " " << parameter.Ip_blue << endl ;
		out << "Ia(RGB)                  = " << parameter.Ia_red << " " << parameter.Ia_green << " " << parameter.Ia_blue << endl ;
		out << "light_v_angle            = " << parameter.light_v_angle   << endl ; // v_angle
		out << "light_h_angle            = " << parameter.light_h_angle   << endl ; // h_angle
		out << endl;

			//----- Page 3 (Edge)
		out << "real_edge_width          = " << parameter.real_edge_width << endl ;
		out << "outline_width            = " << parameter.outline_width   << endl ;
		out << "aux_edge_width           = " << parameter.aux_edge_width  << endl ;
		out << "aux_edge_style           = " << parameter.aux_edge_style  << endl ;
		out << "##(1:Solid, 2:Dashed, 3:Invisible)" << endl;
		out << "aux_threshold_angle      = " << parameter.aux_threshold_angle << endl ;
		out << "line_width               = " << parameter.line_width      << endl ;
		out << "Haloing                  = " << parameter.flag_haloing    << endl ;
		out << "Dashed Back-Face edges   = " << parameter.flag_back_edge_dashed  << endl ;
		out << endl;

			//----- Page 4 (Device)
		out << "device_label             = " << parameter.device_label      << endl ;
		out << "##(1:EPS, 2:X, 3:EPS-LS, 4:X-LS, 5:OpenGL)" << endl;
		out << "PS previewer             = " << parameter.ps_previewer      << endl ;
		out << "Add showpage             = " << parameter.flag_add_showpage << endl ;
		out << "Append mode              = " << parameter.flag_append_psfile << endl ;
		out << endl;

			//----- others
		out << "epsilon2d                = " << parameter.epsilon2d       << endl ;
		out << "flag_output_polygon      = " << parameter.flag_output_polygon  << endl; ; 
		out << "flag_IP_mode             = " << parameter.flag_output_polygon  << endl; ; 
		out << "xmin                     = " << parameter.xmin       << endl;  
		out << "ymin                     = " << parameter.ymin       << endl;  
		out << "zmin                     = " << parameter.zmin       << endl;  
		out << "xmax                     = " << parameter.xmax       << endl;  
		out << "ymax                     = " << parameter.ymax       << endl;  
		out << "zmax                     = " << parameter.zmax       << endl;  
		out << endl;

} // Command::printParameter ()
	

	//----- Command::inputObjectRenderingParameter()
void	Command::readObjectRenderingParameter(	ifstream& fin , Parameter& parameter   ) 
{ 
		//----- local
	char	dummy[256] ;
	char	buf[256]; 
	const	double	RMAX_COLOR = (double)(MAX_COLOR) ;

		//-------------------------------//
		//----- 1. Reflection Coeff -----//
		//-------------------------------//
		//---------- input 
	if( !( fin >> dummy >> parameter.kd_red >> parameter.kd_green >> parameter.kd_blue ) ) { return ; }    

		//---------- rescale red
	parameter.kd_red /= RMAX_COLOR ;
	if      ( parameter.kd_red > 1.0 ) { parameter.kd_red = 1.0 ; }
	else if ( parameter.kd_red < 0.0 ) { parameter.kd_red = 0.0 ; }

		//---------- rescale green
	parameter.kd_green /= RMAX_COLOR ;
	if      ( parameter.kd_green > 1.0 ) { parameter.kd_green = 1.0 ; }
	else if ( parameter.kd_green < 0.0 ) { parameter.kd_green = 0.0 ; }

		//---------- rescale blue
	parameter.kd_blue /= RMAX_COLOR ;
	if      ( parameter.kd_blue > 1.0 ) { parameter.kd_blue = 1.0 ; }
	else if ( parameter.kd_blue < 0.0 ) { parameter.kd_blue = 0.0 ; }

		//----------  diffusion reflection coeff for ambient color 
		//.......... (= those for source light)
	parameter.ka_red   = parameter.kd_red   ;
	parameter.ka_green = parameter.kd_green ;
	parameter.ka_blue  = parameter.kd_blue  ;

		//----------  specular reflection coeff (full reflection)
	parameter.ks_red   = parameter.ks_green = parameter.ks_blue = 1.0 ;

		//---------------------------//
		//----- 2. transparency -----//
		//---------------------------//
	if( !( fin >> dummy >> buf ) ) { return ; }    
	if( (!strcmp( buf, "on" )) || (!strcmp( buf, "ON" )) )
	{
		parameter.transparency = 1 ;
	} else {
		parameter.transparency = 0 ;
	}

		//------------------------------//
		//----- 3. LIGHT DIRECTION -----//
		//------------------------------//
	if( !( fin >> dummy >> parameter.light_v_angle >> parameter.light_h_angle ) ) { return ; }    


		//--------------------------------------//
		//------ 4 real edge width-----//
		//--------------------------------------//
	if( !( fin >> dummy >> parameter.real_edge_width ) ) { return ; }    
	if( parameter.real_edge_width < 0.0 ) { 
		parameter.real_edge_style = INVISIBLE ; 
	}

		//------------------------------//
		//------ 5 outline width   -----//
		//------------------------------//
	if( !( fin >> dummy >> parameter.outline_width ) ) { return ; }    
	if( parameter.outline_width < 0.0 ) { 
		parameter.outline_style = INVISIBLE ; 
	}

		//----------------------------//
		//------ 6 auxedge width -----//
		//----------------------------//
	if( !( fin >> dummy >> parameter.aux_edge_width ) ) { return ; }    
	if( parameter.aux_edge_width < 0.0 ) { 
		parameter.aux_edge_style = INVISIBLE ; 
	}

		//------------------------------------//
		//------ 7 line style of auxedge -----//
		//------------------------------------//
	if( !( fin >> dummy >> parameter.aux_edge_style ) ) { return ; }    
	if( parameter.aux_edge_style < SOLID || parameter.aux_edge_style > INVISIBLE ) { parameter.aux_edge_style = SOLID ; }
	if( parameter.aux_edge_width < 0.0 ) { 
		parameter.aux_edge_style = INVISIBLE ; 
	}

		//-----------------------------------------//
		//----- 8. threshold angle of auxedge -----//
		//-----------------------------------------//
	if( !( fin >> dummy >> parameter.aux_threshold_angle ) ) { return ; }    
	if( parameter.aux_threshold_angle < 0.0   ) { parameter.aux_threshold_angle = 0.0   ; }
	if( parameter.aux_threshold_angle > 180.0 ) { parameter.aux_threshold_angle = 180.0 ; }

} // Command::readObjectRenderingParameter()



	//----- Command::printDataSize  ()
void	Command::printDataSize  (  const DataSize&  data_size , ostream& out ) 
{
	out << "  total number of vertices = " << data_size.num_vertex << endl;
	out << "  total number of facets   = " << data_size.num_facet  << endl;
	out << "  total number of lines    = " << data_size.num_line   << endl;
}


	//----- Command::saveCamera()
void	Command::saveCamera(	const Parameter& 	parameter,
				const char*		output_file   )
{
		//----- local
	ofstream	output     ( output_file ) ;

		//----- err checking
	if( !output ) {
		cerr << "WARNING (Command::saveCamera() ) \n";
		return ;
	}

		//----- header
	output << "%dawn data format version-PO\n" << endl;

		//----- hint data
	output << "ObjectOrigin:\t0\t0\t0" << endl;
	output << "CameraTarget:\t" << parameter.target_x << "\t" << parameter.target_y << "\t" << parameter.target_z << endl;
	output << "CameraPosition:\t" << parameter.camera_distance << "\t" << parameter.camera_v_angle << "\t" << parameter.camera_h_angle << endl;
	output << "FocalDistance:\t" << parameter.focal_distance <<  endl;
	output << "MinPosition:\t"    << parameter.xmin << "\t" << parameter.ymin << "\t" << parameter.zmin << endl;
	output << "MaxPosition:\t"    << parameter.xmax << "\t" << parameter.ymax << "\t" << parameter.zmax << endl;
	output << endl;

		//----- other data
	output << "Tolerance3D:     " << parameter.epsilon3d << endl;
	output << "Tolerance2D:     " << parameter.epsilon2d << endl;
	output << endl;

		//----- beginning of vertex block
	output << BEGIN_VERTEX  << endl;

		//----- beginning of facet block
	output << BEGIN_FACET   << endl;

		//-----
	output.close();

} // Command::saveCamera()


	//----- Command::setWingedEdgeModeToParameter()
int	Command::setWingedEdgeModeToParameter( Parameter& parameter )
{
			//----- Auto setting
	if(	parameter.drawing_mode == WIREFRAME )
	{
		parameter.winged_edge_mode = 1 ;
	} else {
		parameter.winged_edge_mode = 1 ;
	}

	return parameter.winged_edge_mode ;
}

//-----------------------------------------------------------------------
// See also the following functions etc when menu is updated
//
// void 
// Command::inputParameter_Tk(	Parameter& parameter  , 
//					char* data_file_given   ) ;
// 
// Command::analyzeOption()
//
//
// DEFAULT_HISTORY_FILE, DEFAULT_DEF_HIST_FILE in parameter.h
//-----------------------------------------------------------------------