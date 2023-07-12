////////////////////////
///// parameter.cc /////
////////////////////////

#include "parameter.h" 

const	int	DEFAULT_MAX_POLYGON_PER_SESSION = 5000 ;

Parameter::Parameter()
{
		//----- initialize general-purpose flag

	//------------------------------------------//
	//------ DEFAULT PARAMETERS  of system -----//
	//------------------------------------------//
		
		//----- history file
	strcpy( history_file, DEFAULT_HISTORY_FILE ) ;

		//----- ps file
	strcpy( ps_file,   "" ) ;

		//----- ps previewer
	if( getenv( "DAWN_PS_PREVIEWER" ) != NULL ) {
		strcpy( ps_previewer, getenv( "DAWN_PS_PREVIEWER" ) ) ;	
	} else {
		strcpy( ps_previewer, DEFAULT_PS_VIEWER ) ;
	}

		//----- max number of polygons proccesed in one session
	if( getenv( "DAWN_MAX_POLYGON_PER_SESSION" ) != NULL ) {
		sscanf( getenv("DAWN_MAX_POLYGON_PER_SESSION"), "%d", &max_polygon_per_session ) ;
	} else {
		max_polygon_per_session = DEFAULT_MAX_POLYGON_PER_SESSION ;
	}


		//----- sorting of objects
	if( getenv( "DAWN_SORT_DATA" ) != NULL ) {
		if( !strcmp( getenv( "DAWN_SORT_DATA" ),"0" ) ) 
		{ 
			flag_sort_data		=	0 ;
		} else {
			flag_sort_data		=	1 ;
		}
	} else { // No environmental variable: do sort
		flag_sort_data		=	1 ;
	}

//////////////////////////////////////////////
//		//----- haloing
//	flag_haloing	=	0 ;
//////////////////////////////////////////////

		//----- haloing
	if( getenv( "DAWN_HALOING" ) != NULL ) {
		if( !strcmp( getenv( "DAWN_HALOING" ),"1" ) ) 
		{ 
			flag_haloing			=	1 ;
		} else {
			flag_haloing			=	0 ;
		}
	} else { // No environmental variable: do sort
		flag_haloing				=	0 ;
	}

		//----- epsilon2d
	if( getenv( "DAWN_TOLERANCE_2D" ) != NULL ) {
		sscanf( getenv("DAWN_TOLERANCE_2D"), "%lg", &epsilon2d ) ;
	} else {
		epsilon2d	=	1.0e-7 ;
	}

		//----- device
	device_label = PS ; 	
/***************************
#if defined USE_XWIN 

	device_label = XWIN ; 
	if( getenv( "DAWN_DEVICE" ) != NULL ) {
		sscanf( getenv("DAWN_DEVICE"), "%d", &device_label ) ;
		if( device_label < PS || device_label >= DEVICE_END ) {
		  device_label = XWIN ; 
		}
	} 
#else 
	device_label = PS ; 	
#endif
******************************/

		//----- data accumulation
	flag_accumulate_data        =   0 ;     // do not accumulate data
	if( getenv( "DAWN_DATA_ACCUMULATION" ) != NULL ) {
		if( !strcmp( getenv( "DAWN_DATA_ACCUMULATION" ),"0" ) ) 
		{ 
			flag_accumulate_data  = 0 ;
		} else {
			flag_accumulate_data  = 1 ;
		}
	} 

		//----- camera parameters
	flag_auto_target	=	1     ;
	scale   		=	0.9   ;
	focal_distance		=	0.0   ; // auto
	camera_distance		=	0.0   ; // auto
	camera_v_angle		=	13.0  ;
	camera_h_angle		=	79.0  ;
	target_x		=	0.0   ;
	target_y		=	0.0   ;
	target_z		=	0.0   ;

		//----- drawing parameters
	drawing_mode 		=	WIREFRAME   ;

	shading_mode		=	DEFAULT_SHADING_MODE ; 
	transparency		=	0    ; // non-transparent
	flag_display_parameter =	0    ;
	blank_scale       	=	DEFAULT_BLANK_SCALE ;
	viewport_center_x 	=	0.0  ;
	viewport_center_y 	=	0.0  ;

		//----- color
	Ip_red			=	1.0  ;
	Ip_green		=	1.0  ;
	Ip_blue 		=	1.0  ;
	Ia_red			=	0.5  ;
	Ia_green		=	0.5  ;
	Ia_blue 		=	0.5  ;

		//----- object parameters 
	ka_red = ka_green = ka_blue = 1.0 ;
	kd_red = kd_green = kd_blue = 1.0 ;
	ks_red = ks_green = ks_blue = 1.0 ;

	aux_edge_style  	=	INVISIBLE ;
	real_edge_style 	=	SOLID ;
	outline_style  		=	SOLID ;  
	line_style     		=	SOLID ;  

	real_edge_width 	=	0.1  ;
	outline_width		=	0.1  ;
	aux_edge_width		=	0.1  ;
	line_width		=	0.1  ;	

	aux_threshold_angle 	=	DEFAULT_THRESHOLD_ANGLE ;
	light_v_angle 		=	13.0  ;
	light_h_angle 		=	79.0 ;

		//----- others
	epsilon3d		=	1.0e-3 ;
	winged_edge_mode	=	1 ;
	xmin = ymin = zmin	=	0.0 ;
	xmax = ymax = zmax	=	0.0 ;
	num_object		=	0 ;
	flag_focal_distance_given   =   0 ;	// focal_distance is
						// indirectly given by "scale"
	flag_output_polygon         =   0 ; 	// initialized to "do not output polygon.out"
	flag_read_polygon_out       =   0 ;	// initialized to "no polygon.out file"
	flag_IP_mode                =   1 ;	// prim viewer mode
	flag_skip_gui               =   0 ;	// do not skip gui
	flag_swap_transparency      =   0 ;	// do not swap transparency

	flag_back_edge_dashed       = 	1 ;	// edges of back faces become dashed 
						//  in WF_Stored mode
	flag_add_showpage           =   0 ;


	flag_draw_edge  = 1 ;

	flag_BFC_mode   = 0 ; 
	if( getenv( "DAWN_BFC_MODE" ) != NULL ) {
		if( !strcmp( getenv( "DAWN_BFC_MODE" ),"0" ) ) 
		{ 
			flag_BFC_mode  = 0 ;
		} else {
			flag_BFC_mode	= 1 ;
			flag_draw_edge  = 0 ;
		}
	} 

	flag_use_stencil = 0 ;
	if( getenv( "DAWN_USE_STENCIL" ) != NULL ) {
		if( !strcmp( getenv( "DAWN_USE_STENCIL" ),"0" ) ) 
		{ 
			flag_use_stencil  = 0 ;
		} else {
			flag_use_stencil  = 1 ;
		}
	} 


	// Append mode
	flag_append_psfile = 0 ;

	// Batch mode
	flag_batch = 0 ;
	if( getenv( "DAWN_BATCH" ) != NULL ) {
		if( strcmp( getenv( "DAWN_BATCH" ),"0" ) ) {
			// flag
			flag_batch = 1 ;

			// GUI 		
			flag_skip_gui = 1  ;

			// Device 
			device_label  = PS ; 	
			if( getenv( "DAWN_DEVICE" ) != NULL ) {
			  if ( !strcmp( getenv( "DAWN_DEVICE" ),"3" ) ) {
			    device_label  = PS2 ; 	
			  }
			}

		}

		// Append mode
		if( !strcmp( getenv( "DAWN_BATCH" ),"a" ) || !strcmp( getenv( "DAWN_BATCH" ),"A" ) )
		{
			flag_append_psfile = 1 ;
			flag_add_showpage  = 1 ;
		}

	}


	// Hidden-line removal 
	flag_hidline_removal = 1 ;
	if( getenv( "DAWN_HIDLINE_REMOVAL" ) != NULL ) {
		if( !strcmp( getenv( "DAWN_HIDLINE_REMOVAL" ),"0" ) ) {
			flag_hidline_removal = 0 ;
		}
	}


} // Paremater::Parameter
