////////////////////////
///// command.h    /////
////////////////////////

#if !defined COMMAND_H
#define COMMAND_H

	//----- inclusion of header files
#include <stdio.h>
#include <ctype.h>
#include <fstream.h>
#include <stdlib.h>
#include <string.h>	

#include "FRDevice.h"

#include "box.h" 
#include "colortype.h" 
#include "filereader.h" 
#include "hid.h"
#include "hidline.h"
#include "linetype.h"
#include "name_and_value.h"
#include "object.h"
#include "parameter.h"
#include "perspective.h"

#include "list.h"
#include "shading.h"
#include "version.h"



	//-------------------------//
	//----- class Command -----//
	//-------------------------//
class Command {
	enum { ERROR, NORMAL };

 public:
	Command () {;} 
	~Command() {;}

		//-------------------//
		//----- general -----//
		//-------------------//

		//----- error message
	void	errorMessage(	char* string1 = NULL, 
				char* string2 = NULL, 
				char terminal = '\n' ) ;

		//----- display opening title to stdout
	void	openDawn( int& argc, char* argv[] , Parameter& parameter ) ;

		//----- set camera
	void	setCamera( const Parameter& parameter , Perspective& camera ) ; 

	void	setCamera(	double		distance , 
				double		v_angle  , 
				double		h_angle  , 
				Vector3		target   , 
				double		focal_distance ,
				double		epsilon3d       ,
				Perspective&	camera           ) ;
	void	setCameraAuto(	const Box3D&	totalBoundingBox, 
				Parameter&	parameter, 
			        Perspective&	camera           ) ;

	void	setCameraPositionToParameter(	double camera_distance , 
						double camera_v_angle ,
						double camera_h_angle ,
						Parameter& parameter ) ;


	void	setTargetPointToParameter(	double x, 
						double y,
						double z,
						Parameter& parameter ) ;
	void	setScaleToParameter(	double scale_given, 
					Parameter& parameter );
	void	setFocalDistanceToParameter(	double focal_distance, 
						Parameter& parameter );
	void	setAutoTargetFlagToParameter(	int        is_auto, 
						Parameter& parameter );
	void	setDeviceToParameter(	int        device ,
					Parameter& parameter );

	void	setTarget( const Vector3& target , Parameter& parameter, Perspective& camera );


		//----- auto positioning
		//.....  set parameter.camera_distance and camera.rho by auto.
	void	autoPositioning( double total_rough_size , Parameter& parameter, Perspective& camera );

		//----- auto target
	void	autoZoom( const Box3D& total_bounding_box, Parameter& parameter, Perspective& camera );


		//----- auto tolerance setting
	void	autoToleranceSetting( double total_size, double ratio, Parameter& parameter ) 
		{	
			if( ratio >= 0.0 ) { parameter.epsilon3d = total_size * ratio ;}
		}

		//----- set total bounding box to parameter
	void	setTotalBoundingBox(	double xmin, double ymin, double zmin, 
					double xmax, double ymax, double zmax, 
					Box3D* total_bounding_box_p              ) ;
	void	setTotalBoundingBoxToParameter(	const Box3D& total_bounding_box ,
						Parameter* parameter             ) ;

		//----- analyse option 
		//..... Return: 1  if argv_i is a option (with "-")
		//.....         0  else
	int	analyzeOption( char* argv_i, Parameter& parameter ) ;

		//----- scan data file
		//..... (1) object_name_server cleared and then 
		//..... (2) make database of data file names 
		//.....     and set it to object_name_server
		//..... (3) analyze option
		//..... (4) sort data if necessary
		//..... (5) see if polygon.out data is included 
		//.....     (If it is included parameter.read_polyon_out becomes 1 )
		//..... (6) calc total bounding box
		//..... (7) decide ps file name and set it to parameter.ps_file
	void	scanDataFile( 	char*			argv[],
				int			argc , 
				FileReader&		filereader ,
				Perspective&		camera     ,
				List<NameAndValue>&	object_name_server , 
				Box3D&			total_bounding_box ,
				Parameter&		parameter          );

		//----- read data file
	void	readDataFile(	const Parameter&	parameter, 
				FileReader&		filereader , 
				List<NameAndValue>&	object_name_server ,
				List<Object>&		object_server , 
				List<LineSegment>&	lineseg_server , 
				DataSize&		total_data_size     );	

	void	readDataFile_DrawWireFrame_Clear(	const Parameter&	parameter          , 
							FileReader&		filereader         , 
							List<NameAndValue>&	object_name_server ,
							List<Object>&		object_server      , 
							List<LineSegment>&	lineseg_server     ,
							DataSize&		total_data_size    ,
							PostScript&		ps           ,
							Perspective&		camera        );

		//----- set objects to hidden surface removal routine
	void	passFacetToHidCMD(	Object*			object_p      , 
					Perspective&		camera        ,
					Hid&			hid           ,
					HidLine&		hidline       ,
					const Parameter&	parameter     ,
					int		backface_culling_mode  = 1 ) ; 

	void	passFacetToHidCMD(	List<Object>&	object_server , 
					Perspective&	camera        ,
					Hid&		hid           ,
					HidLine&	hidline       ,
					Parameter&	parameter     );
						 // backface_culling_mode  = 1 ;


	void	passtFaceFromCurrentObjectToHid(	List<Object>&		object_server , 
							Perspective&		camera        ,
							Hid&			hid           ,
							HidLine&		hidline       ,
							Parameter&		parameter       );


		//----- Command::passEdgeToHidLine()
	void	passEdgeToHidLineCMD(	List<Object>&		object_server , 
					Perspective&		camera        ,
					HidLine&		hidline       ,
					const Parameter&	parameter      ) ;

	void	passEdgeToHidLineCMD(	Object*			object_p      , 
					Perspective&		camera        ,
					HidLine&		hidline       ,
					const Parameter&	parameter      ) ;


		//----- pass linesegment data to HidLine 
		//..... and set linetype to linesegments 
		//..... in linesegServer 
	void	passLineSegmentToHidLine (	List<LineSegment>&	linesegServer , 
						Perspective&		camera        ,  
						HidLine& 		hidline       , 
						Parameter&		parameter      ) ;

		//----- Command::readDataFile_doHiddenSurfaceElimination()
	void	readDataFile_doHiddenSurfaceElimination(	const Parameter&	parameter          ,
								FileReader&		filereader         , 
								List<NameAndValue>&	object_name_server , 
								List<Object>&		object_server      , 
								List<LineSegment>&	lineseg_server     , 
								Hid&			hid                , 
								HidLine&		hidline            ,
								DataSize&		total_data_size      );	

	void	doIncrementalHiddenSurfaceElimination(	const Parameter&	parameter          ,
						        List<Object>&		object_server      , 
						        Hid&			hid                , 
						        HidLine&		hidline            , 
							int			flush_mode = 0       );


		//----- do hidden line elimination
	void	doHiddenLineElimination   (	const Parameter&	parameter          ,
						HidLine&		hidline            , 
						double			epsilon3d = 1.0e-6 ,
						double			epsilon2d = 1.0e-6   ); 
	void	doHiddenLineElimination(	Parameter&		parameter , 
						List<LineSegment>&	lineseg_server       ,
						HidLine&		hidline_remover      ,
						Hid&			hid_surface_remover  ,
						Perspective&		camera                );


		//----- output result of hidden line/surface removal to file
	void	savePolygon( const Parameter& parameter , Hid& hid ) ;

		//----------------------------//
		//----- parameter setting-----//
		//----------------------------//
	void	setParameter( Parameter& parameter );

		//----- read history file
		//..... Return: ERROR or NORMAL
	int	readHistoryFile(	Parameter& parameter , 
					char* history_file_given = NULL ) ;

		//----- write parameters to history file
		//..... Return: ERROR or NORMAL
	int	writeParameterToHistoryFile(	const Parameter& parameter , 
						char* history_file_given = NULL ) ;
	void	inputParameter_Tk( Parameter& parameter );
	void	printParameter (  const Parameter& parameter , ostream& out ) ;
	void	printDataSize  (  const DataSize&  data_size , ostream& out ) ;
	void	saveCamera(	const Parameter& 	parameter	,
				const char*		output_file = DEFAULT_CAMERA_OUTPUT_FILE );

		//----- set parameter.winded_edge_mode
	int	setWingedEdgeModeToParameter( Parameter& parameter ) ;

		//-----
	void	readObjectRenderingParameter(	ifstream& fin , Parameter& parameter ) ;
	void	setLineTypeToObject ( const Parameter& parameter , Object* object_p ) ;
	void	setShading          ( Parameter& parameter , Shading* shading_p ) ;


		//----- clearing 
	void	clearCurrentObject( List<Object>& object_server ) ;

		//----- set clipping mode 
				// clipping_mode =  0 ( no cllipping)
				//               =  1 ( clipping) 
				//               =  2 ( landscape clipping) 
	int	getClippingMode ( const Parameter& parameter );


		//-----------------------------//
		//----- Drawing functions -----//
		//-----------------------------//

		//----- open device
		//..... Return: 1 (normal) or 0 (error)
	int	open_PS( PostScript& ps , Parameter& paremeter ) ;

#if defined USE_XWIN
	int	open_X ( PostScript& ps , X_Window& xwin, PS2X& ps2x , Parameter& parameter , int flag_immediate = 0 )  ;
#endif

		//----- DRAW
#if defined USE_OPEN_GL
	void	openDevice (	Perspective&		camera	     ,
				Parameter&              parameter    ,
				PostScript&		ps           ,
				X_Window&		xwin         , 
				PS2X&			ps2x         ,
				FROpenGL&		gl	     ,
				int			flag_immediate = 0 )  ;
	int	closeDevice (	Parameter&              parameter    ,
				PostScript&		ps           ,
				X_Window&		xwin         , 
				PS2X&			ps2x         ,
				FROpenGL&		gl	      );

	void	drawAll(	const Parameter&        parameter    ,
				PostScript&		ps           ,
				FROpenGL&		gl	     ,
				List<Object>&		objectServer ,
				Perspective&		camera       ,
				Hid&			hid          ,
				HidLine&		hidline      ,
				char*			title          );

#elif defined USE_XWIN

	void	openDevice (	Perspective&		camera	     ,
				Parameter&              parameter    ,
				PostScript&		ps           ,
				X_Window&		xwin         , 
				PS2X&			ps2x         ,
				int			flag_immediate = 0 )  ;
	int	closeDevice (	Parameter&              parameter    ,
				PostScript&		ps           ,
				X_Window&		xwin         , 
				PS2X&			ps2x         );

	void	drawAll(	const Parameter&        parameter    ,
				PostScript&		ps           ,
				List<Object>&		objectServer ,
				Perspective&		camera       ,
				Hid&			hid          ,
				HidLine&		hidline      ,
				char*			title          );
#else 
	void	openDevice (	Perspective&		camera	     ,
				Parameter&              parameter    ,
				PostScript&		ps           ) ;

	int	closeDevice (	Parameter&              parameter    ,
				PostScript&		ps           ) ;

	void	drawAll(	const Parameter&        parameter    ,
				PostScript&		ps           ,
				List<Object>&		objectServer ,
				Perspective&		camera       ,
				Hid&			hid          ,
				HidLine&		hidline      ,
				char*			title          );
#endif

	void	drawLine(	const Parameter&        parameter    ,
				PostScript&		ps           ,
				HidLine&		hidline       ) ;

	void	drawLine(	const Parameter&        parameter    ,
				PostScript&		ps           ,
				Perspective&		camera       ,
				List<LineSegment>&	lineseg_server  ) ;

	void	setImage(	const Parameter&        parameter    ,
				List<LineSegment>&	lineseg_server  , 
				Perspective&		camera        );

	void	drawCurrentObject_WireFrame(	const Parameter&        parameter    ,
						PostScript&		ps           ,
						List<Object>&		object_server ,
						Perspective&		camera        );

		//----- draw line data (PS)
	void
	drawLine_PS(	PostScript&		ps            ,
			HidLine&		hidline       ,
			double			blank_scale  = 0.0 ); 

		//----- draw (hidden line/surface removal, PS )
	void	draw_Hid_PS(	const Parameter&	parameter ,
				PostScript&		ps        , 
				Hid& 			hid       ,
				List<Object>*		object_server_p = NULL );

		//----- Command::drawVisiblePartOfLineSegment_PS ()
	void
	drawVisiblePartOfLineSegment_PS (	const	Vector3&	A ,
						const	Vector3&	B ,
						const	LineType&	linetype , 
						Hid&			hid      ,
						PostScript&		ps ,
						const Parameter&	parameter ) ;

		//----- draw frame (PS)
	void	drawFrame_PS( PostScript& ps , LineType linetype ) ;

		//----- close PostScript routine
	void	close_PS( Parameter& parameter , PostScript& ps ) ;

#if defined USE_XWIN
	int	close_X ( PostScript& ps , X_Window& xwin, PS2X& ps2x ) ;
			// 0: mouse button press
			// 1: key press
	int	showpage_X( PostScript& ps , X_Window& xwin );
	void	waitEvent( const Parameter& parameter, X_Window& xwin) ;
#endif

		//----- preview (PS)
		//..... Return: return of previewer
//	int	preview_PS( char* previewer, char* psfile ) ;
	int	preview_PS( const Parameter& parameter    ) ;

		//----- display string
	void	drawString_PS( PostScript& ps, char* string , double x , double y );

		//----- display parematers
	void	displayParameter_PS(	PostScript&	ps,
					Perspective&	camera    ,
					double		epsilon2d  ,
					double		epsilon3d  ) ;

		//----- display parematers
	void	displayAxes_PS(	PostScript&	ps,
					Perspective&	camera    ,
					const Parameter& parameter );
	void	displayAxes_landscape_PS(	PostScript&	ps,
						Perspective&	camera    ,
						const Parameter& parameter );


		//----- Command::drawLine_landscape_PS()
	void	drawLine_landscape_PS(	PostScript&		ps            ,
					HidLine&		hidline       ,
					double			blank_scale    );


		//----- Command::draw_Hid_landscape_PS()
	void	draw_Hid_landscape_PS(	const Parameter&	parameter ,
					PostScript&		ps        , 
					Hid& 			hid       ,
					List<Object>*		object_server_p = NULL );

		//------ Command::drawVisiblePartOfLineSegment_landscape_PS ()
	void 	drawVisiblePartOfLineSegment_landscape_PS (	const	Vector3&	A ,
								const	Vector3&	B ,
								const	LineType&	linetype , 
								Hid&			hid      ,
								PostScript&		ps ,
								const Parameter&	parameter );

		//----- draw marks
	void	fillCircle_PS(	const Parameter& parameter      , 
				PostScript&		ps      ,
				const Vector3&		P	,
				Perspective&		camera	, 
				double			R_mm	, 
				ColorType*		color_p );

	void	fillSquare_PS(	const  Parameter& parameter     , 
				PostScript&		ps      ,
				const Vector3&		P	,
				Perspective&		camera	, 
				double			dL_mm	, 
				ColorType*		color_p  );

		//----- draw string
	void	drawColoredString_PS(	const char*		string    , 
					const ColorType&	color     ,
					double			size_pt   ,
					const Vector2&		P_mm	  ,
					PostScript&		ps         );
	void	drawColoredString_vp_PS(const char*             string    , 
					const ColorType&	color     ,
					double			size_vp   ,
					double			offset_x_vp ,
					double			offset_y_vp ,
					const Vector3&		P	  ,
					Perspective&		camera	  , 
					const  Parameter&	parameter , 
					PostScript&		ps         );

#if defined USE_OPEN_GL

	void	fillCircle_GL(	const Parameter& parameter      , 
				FROpenGL&		gl	,
				const Vector3&		P	,
				Perspective&		camera	, 
				double			dR	, 
				ColorType*		color_p ,
				size_t			Ndiv = 24 );

	void	fillSquare_GL(	const  Parameter& parameter     , 
				FROpenGL&		gl	,
				const Vector3&		P	,
				Perspective&		camera	, 
				double			dL	, 
				ColorType*		color_p  );

		//----- OpenGL
	void	open_GL(	Perspective& camera , 
				X_Window&          xwin   ,
				FROpenGL&	   gl	  ,
				const Parameter&   parameter );

	void	close_GL( void );
	void	readDataFile_DrawGL_Clear(	const Parameter&	parameter          , 
						List<NameAndValue>&	object_name_server ,
						DataSize&		total_data_size    ,
						Perspective&		camera             ,    
						FROpenGL&		gl                   ) ;


	void	drawObject_GL(	const Parameter&	parameter          , 
				Perspective&		camera             ,
				Object*			object_p           ,
				FROpenGL&		gl                  ) ;
	void	drawPolygon_GL ( Hid& polygon_server , List<Object>& object_server, FROpenGL& gl );
	void	fillPolygon_GL ( Hid& polygon_server , Object* object_p, FROpenGL& gl , const Parameter& parameter );
	void	drawLine_GL    ( List<LineSegment>& lineseg_server , FROpenGL& gl );

	void	displayAxes_GL(	FROpenGL&        gl      ,
				const Parameter& parameter );

#endif

}; // Command

////////////////////////////// end of command.h
#endif
