////////////////////////////////
///// FRCommandAnalyzer.h  /////
////////////////////////////////

#if !defined FR_COMMAND_ANALIZER_H
#define FR_COMMAND_ANALIZER_H


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream.h>

#include "FRDevice.h"
#include "main_selector.h"

#include "command.h"
#include "filereader.h"
#include "hid.h" 
#include "hidline.h" 
#include "linesegment.h"
#include "name_and_value.h"
#include "object.h"
#include "parameter.h" 
#include "perspective.h"
#include "postscript.h" 
#include "list.h"
#include "toolstring.h"

#include "FRVector3.h"
#include "FRString.h"

#include "FRBrepSolid.h"
#include "FRObjectGenerator.h"

#include "FRMark.h"
#include "FRPolylineForBrep.h"
#include "FRBoxForBrep.h"
#include "FRTrdForBrep.h"
#include "FRTrapForBrep.h"
#include "FRConeSegForBrep.h"
#include "FRTubeSegForBrep.h"
#include "FRSphereForBrep.h"
#include "FRSphereSegForBrep.h"
#include "FRParallelepipedForBrep.h"
#include "FRPolyconeForBrep.h"
#include "FRPolygonForBrep.h"
#include "FRTorusForBrep.h"
#include "FRVisText2D.h"
#include "polyhedron.h"


class FRVServer ;

	//-----------------------------------//
	//----- class FRCommandAnalyzer -----//
	//-----------------------------------//

class FRCommandAnalyzer {

 protected:
	enum { WORD_MAX =   32        } ;
	enum { MESSAGE_INTERVAL = 250 } ;


	Command&	command ;
	Parameter&	parameter ;

	char			primFile   [256] ;
	List<FRString>		polyhedron_data ;
	FRObjectGenerator	brepFactory ;
	PostScript		ps ;
#if defined USE_XWIN
	X_Window		xwin ;
	PS2X			ps2x ;
#endif
#if defined USE_OPEN_GL
	FROpenGL		gl ;
#endif	
	List<Object>		objectServer ;
	List<NameAndValue>	objectNameServer ;

	List<LineSegment>	linesegServer ;
	FileReader		filereader ;
	Perspective		camera ;
	Hid			hid  ;
	HidLine			hidline ;
	Box3D			totalBoundingBox ;
	DataSize		totalDataSize    ;
	Attribute		defaultAttribute ;
	Attribute		curAttribute ;
	List<FRVector3>	polylineVertexList ;
	List<FRMark>		markServer       ;
	List<FRVisText2D>	vistextServer       ;
	ofstream  		solid_out        ;
	int			num_primitive	 ;		

 protected:
#if defined USE_OPEN_GL
	void	drawObject_GL_Clear  ( void );
	void    drawPolyline_GL_Clear( void );
#endif
	void	makeObjectImage( void );
	void	fromVerticesToLinesegments(	List<FRVector3>& vertex_list, \
						List<LineSegment>*     lineseg_list_p ) ;
	void	fromBodyToWorld(	const FRVector3& body, \
					FRVector3*       world_p ) ;
	void	fromBodyToWorld(	const Vector3& body, \
					Vector3*       world_p ) ;
	void	makePolylineImage( void );
	void	setObjectToFactory   ( const char* obj_name = "NO_NAME" ) ;

 public:
	FRCommandAnalyzer(	char*			prim_file_name  , 
				Command&		command_given   ,
				Parameter&		parameter_given ,
				const Attribute&	default_att      );

	~FRCommandAnalyzer() { this->clear() ;	}
	void	clear     () ;
	void	clearData() ;

	int	analyzeAndExecute ( const char* message , FRVServer* ) ;

#if defined USE_XWIN 
	void	waitEvent( FRVServer* server_p , int flag_sendback = 1 ) ;
#endif

	void	setCurrentAttribute( const Attribute& att ) { curAttribute = att ; }	
	void	SetMarkAttributes(	const char* message, 
					int    type        , 
					int    size_dim    ,
					const ColorType& mark_color,
					FRMark* mark_p        );
	void	SetMarkTextAttributes(	const char* message, 
					int    type        , 
					int    size_dim    ,
					const ColorType& mark_color,
					FRMark* mark_p        );

	double	GetMark2DSize_vp (	double		size_world , 
					const Vector3&	position_world );
	double	GetMark2DSSize_vp( double size_pt ) ;

	void	drawAllMark    ();
	void	drawAllVisTexts();
	void	incr_displayNumPrimitives( );
	void	setDefaultNdiv()            ;

		//----- 


} ; // FRCommandAnalyzer 

#endif
