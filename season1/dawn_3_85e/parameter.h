///////////////////////
///// parameter.h /////
///////////////////////

#if !defined  PARAMETER_H
#define PARAMETER_H

	//----- inclusion of header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#include <math.h>	
#include "dev_control.h"


	//----- constants
	//---------- enumeration
enum { WIREFRAME = 1 , WF_STORED = 2 , HID =3  , HID2 = 4 , HID3 = 5, DRAWING_MODE_END = 6 } ;
	// HID = lambert, HID2 = line, HID3 = PHONG
enum { LAMBERT = 0 , PHONG = 1 } ;
enum { PS = 1 , XWIN = 2, PS2 = 3 , XWIN2 = 4, OPEN_GL = 5, DEVICE_END = 6 } ;
enum { SOLID = 1, DASHED = 2 , INVISIBLE = 3 };

	//---------- DEFAULT values
const	int	DEFAULT_PHONG_POWER        = 3 ; // normally 0 -- 10
const	int	DEFAULT_SHADING_MODE       = LAMBERT ;
const	double	DEFAULT_BLANK_SCALE        = 0.5 ;
const	double	DEFAULT_THRESHOLD_ANGLE    = 70.0 ;
const	char	DEFAULT_HISTORY_FILE[]     = ".DAWN_1.history" ; 
const	char	DEFAULT_DEF_HIST_FILE[]    = ".DAWN_1.default" ; 

#if    defined USE_VC
 const	char	DEFAULT_PS_VIEWER[]        = "gsview32" ;
#elif  defined USE_CYGNUS
 const	char	DEFAULT_PS_VIEWER[]        = "gsview32" ;
#else 
// const	char	DEFAULT_PS_VIEWER[]        = "ghostview" ;
 const	char	DEFAULT_PS_VIEWER[]        = "gv" ;
#endif

const	char	DEFAULT_POLYGON_OUTPUT_FILE[] = "polygon.out" ;
const	char	DEFAULT_CAMERA_OUTPUT_FILE[]  = "camera.out" ;

	//---------- steps of RGB
const	int  	MAX_COLOR = 1   ;

	//----- rescaling ratio
const	double	TOLERANCE_RATIO		= 1.0e-4 ;
//const	double	CAMERA_DISTANCE_RATIO	= 2.0e+2 ;
const	double	CAMERA_DISTANCE_RATIO	= 1000.0 ;
const	double	COLOR_REDUCTION_RATIO	= 0.3    ;


	//----- viewport size 
	//.....  It must be 1.0 * 1.0 
const	double  VIEWPORT_WIDTH   = 1.0 ;
const	double  VIEWPORT_HEIGHT  = 1.0 ;

	//----- clipping area : A4 size
	//.....  in viewport unit
#include "a4size.h"
//const double  A4_WIDTH_MM  = 210.0 ;
//const double  A4_HEIGHT_MM = 297.0 ;
//const double  A4_WIDTH_PT  = (A4_WIDTH_MM / 25.4 * 72.0) ;
//const double  A4_HEIGHT_PT = (A4_HEIGHT_MM / 25.4 * 72.0) ;


	//---------------------------//
	//----- class Parameter -----//
	//---------------------------//

struct Parameter {

		//----- flag for general purpose

		//----- filenames
	char		history_file[256];
	char		ps_file[256];
	char		ps_previewer[256];

		//----- camera parameters
	int		flag_auto_target	;
	double		target_x ;
	double		target_y ;
	double		target_z ;
	double		camera_distance	;
	double		camera_v_angle	;
	double		camera_h_angle	;
	double		focal_distance ;
	double		scale   ; // User input to determine "focal_distance".
				  // This parameter may not hold a correct value
				  // after "focal_distance" is modified through
				  // e.g. auto zoom or reading polygon.out data.

		//----- drawing parameters
	int		drawing_mode    ;
	int		flag_haloing    ;
	int		shading_mode    ;
	int		transparency    ;
	int		flag_display_parameter ;
	int		device_label      ;
	double		blank_scale       ;
	double		viewport_center_x ;
	double		viewport_center_y ;

		//----- light parameters 
	double		Ip_red 	 ;
	double		Ip_green ;
	double		Ip_blue  ;
	double		Ia_red 	 ;
	double		Ia_green ;
	double		Ia_blue  ;


		//----- object parameters 
	double		ka_red, ka_green, ka_blue ;
	double		kd_red, kd_green, kd_blue ;
	double		ks_red, ks_green, ks_blue ;

	int		aux_edge_style  ;
	int		real_edge_style ;
	int		outline_style   ;
	int		line_style   ;

	double		real_edge_width 	;
	double		outline_width	;
	double		aux_edge_width	;
	double		line_width ;

	double		aux_threshold_angle ;

	double		light_v_angle ;
	double		light_h_angle ;

		//----- other parameter
	int		flag_sort_data  ;
	int		flag_focal_distance_given   ; 
	int		flag_output_polygon         ; 
	int		flag_read_polygon_out       ;
	int		flag_IP_mode                ;
	int		flag_skip_gui               ;
	double		epsilon2d	;
	double		epsilon3d	;
	int		winged_edge_mode ;
	int		max_polygon_per_session  ;
	double		xmin, ymin, zmin ;
	double		xmax, ymax, zmax ;
	int		num_object	;
	int		flag_swap_transparency ;
	int		flag_accumulate_data ;
	int		flag_back_edge_dashed ;
	int		flag_add_showpage ;

	int		flag_BFC_mode     ;
	int		flag_draw_edge    ;
	int		flag_use_stencil  ;

	int		flag_append_psfile ;

	int		flag_batch ;

	int             flag_hidline_removal ;

		//-----  constructor
		//.....  DEFAULT PARAMETERS  of system 
	Parameter() ;

} ; // Parameter 
#endif



