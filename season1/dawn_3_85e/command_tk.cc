///////////////////////////
/////  command_tk.cc  /////
///////////////////////////


#include "command.h" 
#include "toolstring.h"

#include "dev_control.h"

#if defined USE_VC
  #include <process.h>
  #include<stdio.h>
  #include <error.h>
#endif

#if   defined USE_CYGNUS
const	char	FILE_DAWN_MENU_TK[] = "DAWN_GUI.bat" ;
#elif defined USE_VC
const	char	FILE_DAWN_MENU_TK[] = "DAWN_GUI.exe" ;
#else 
const	char	FILE_DAWN_MENU_TK[] = "DAWN_GUI" ;
#endif


	//----- Command::inputParameter_Tk()
void Command::inputParameter_Tk( Parameter& parameter  ) 
{
		//----- local
	Parameter	default_param ;
	int		status ;

		//----- invoke GUI
#if defined USE_VC
	int termstat ;
	int pid = spawnlp(_P_NOWAIT, FILE_DAWN_MENU_TK, FILE_DAWN_MENU_TK, NULL);
	_cwait(  &termstat, pid, _WAIT_CHILD);
	status = 0 ;		
#else 

	char gui_command[256];
	strcpy( gui_command, FILE_DAWN_MENU_TK ) ;
	status = system ( gui_command );

#endif

		//----- EXIT button is pressed or GUI menu cannot be invoked
	if( status ) 
	{ 
			//----- IP mode: Do not exit
		if( parameter.flag_IP_mode >= 2 ) { 
			cerr << "***** Parameter resetting is canceled." << endl;
			return ; 
		} else {
			//----- Standalone mode: Do exit
			exit(1);
		}
	}

		//----- read parameter from updated history file
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
	this->readHistoryFile( parameter ) ; 
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//


		//----- Auto setting
	this->setWingedEdgeModeToParameter( parameter ) ; 

		//----- error recovery 

		//---------- camera distance
		//..........  recovered by autoPositioning()
//	if( parameter.camera_distance < 0.0 ) { parameter.camera_distance  = default_param.camera_distance ; }		

		//---------- tolerance (epsilon3d)
	if( parameter.epsilon3d < 0.0 ) { parameter.epsilon3d = default_param.epsilon3d ; }	

		//---------- scale
	if( parameter.scale < 0.0 ) { parameter.scale = default_param.scale ; }		

		//---------- edge and line width 
	if( parameter.real_edge_width < 0.0 ) { 
		parameter.real_edge_style  = INVISIBLE ;
		parameter.flag_draw_edge = 0 ;
	}		
	if( parameter.outline_width   < 0.0 ) { 
		parameter.outline_style    = INVISIBLE ; 
		parameter.flag_draw_edge = 0 ;
	}		
	if( parameter.aux_edge_width  < 0.0 ) { 
		parameter.aux_edge_style   = INVISIBLE ; 
		parameter.flag_draw_edge = 0 ;
	}
	if( parameter.line_width      < 0.0 ) { 
		parameter.line_width       = default_param.line_width      ; 
	}		

		//---------- edge and line style
	if  ( parameter.flag_BFC_mode ) { 
		parameter.flag_draw_edge  = 0 ;
	}

		//---------- threshold angle
	if( parameter.aux_threshold_angle > 180.0 ) { parameter.aux_threshold_angle =180.0; } 
	if( parameter.aux_threshold_angle <   0.0 ) { parameter.aux_threshold_angle =  0.0; } 

		//---------- color of light
	if( parameter.Ip_red    > 1.0 ) { parameter.Ip_red   = 1.0 ; }
	if( parameter.Ip_red    < 0.0 ) { parameter.Ip_red   = 0.0 ; }
	if( parameter.Ip_green  > 1.0 ) { parameter.Ip_green = 1.0 ; }
	if( parameter.Ip_green  < 0.0 ) { parameter.Ip_green = 0.0 ; }
	if( parameter.Ip_blue   > 1.0 ) { parameter.Ip_blue  = 1.0 ; }
	if( parameter.Ip_blue   < 0.0 ) { parameter.Ip_blue  = 0.0 ; }

	if( parameter.Ia_red    > 1.0 ) { parameter.Ia_red   = 1.0 ; }
	if( parameter.Ia_red    < 0.0 ) { parameter.Ia_red   = 0.0 ; }
	if( parameter.Ia_green  > 1.0 ) { parameter.Ia_green = 1.0 ; }
	if( parameter.Ia_green  < 0.0 ) { parameter.Ia_green = 0.0 ; }
	if( parameter.Ia_blue   > 1.0 ) { parameter.Ia_blue  = 1.0 ; }
	if( parameter.Ia_blue   < 0.0 ) { parameter.Ia_blue  = 0.0 ; }

} // Command::inputParameter_Tk( Parameter& parameter , char* data_file_given ) 
