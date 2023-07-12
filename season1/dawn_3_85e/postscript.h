///////////////////////////////////////////////////
//////////  postscript.h    1997/11/28    /////////
///////////////////////////////////////////////////

#if !defined POSTSCRIPT_H
#define POSTSCRIPT_H


#include <fstream.h>
#include <stdlib.h>		
#include <string.h>
#include <stdio.h>	
#include "colortype.h"
#include "linetype.h"
#include "vector.h"
#include "box.h"

//////////////////////////////////////////////////////////////////////////
/////  <<Definitions for describing comments>>
/////
/////   Window: 
/////		Area of windowWidth * windowHeight.
/////		By default, it simply means "the whole A4 paper".
/////   Drawing area:  
/////		Drawable part of "window".
/////		Its width  is  windowWidth  - 2.0 * leftRightMargin
/////		Its height is  windowHeight - 2.0 * topBottomMargin
/////		Its center is assigned to center of "window".
/////	Viewport:
/////		A superposed 2D area where polygons passed from 
/////		3D renderers are drawn.
/////		By default, its center is assigned to center of 
/////           "window".  The center can be shifted by explicitly
/////           giving the second argument of open().
/////   Clipping Area:
/////		Square area defined by	clip_left, clip_bottom, 
/////		clip_width, and clip_height.
/////		By default, clipping area is identical to 
/////		"drawing area".
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/////  <<Origins and units>>
/////
/////	mm: 	Millimeter.
/////		For positions measured in mm, 
/////		its origin is chosen at left bottom of "drawing area"
/////	vp: 	Unit used in viewport, i.e. 3D renderer.
/////		For positions measured in vp,
/////		its origin is chosen at center of viewport.
/////		Note that the center coincides with centers of
/////		drawing area and window.
/////	pt: 	Point.
/////		(1 pt =  25.4 /72.27 mm in PostScript.)
/////		For positions measured in pt, 
/////		its origin is chosen at left bottom of "window"
/////
/////   Note: Private data members with dimension [L], such as "windowWidth"
/////         are described in units of "pt", which is the device-inherent 
/////         unit. On the other hand, for member functions, 
/////         the following convensions are adopted:
/////
/////          XXX()   :  This function should be used with unit "mm"
/////          XXX_vp():  This function should be used with unit "vp"
/////          XXX_pt():  This function should be used with unit "pt"
/////         
/////         Note that the standard unit "for users' of this class 
/////         is "mm".
//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/////  <<PS procedures generated by definePSfunctions()>>
/////
/////  (1) /PATH
/////       (a) Function: Make a path "newpath x0 y0 moveto ... xn yn lineto"
/////       (b) Usage:    [x0 y0 x1 y1 ... xn yn ] PATH
/////       (c) Definition:
/////           /PATH
/////           {  /POINT 0 def
/////              /X 0 def
/////              newpath
/////              {  X 1 eq { POINT 0 eq { moveto
/////                                       /POINT 1 def
/////                                     }{lineto }ifelse
/////                          /X 0 def
/////                        }{/X X 1 add def}ifelse
/////              } forall
/////           } def
/////      
/////       (d) Simulated behavior of "[x0 y0 x1 y1 x2 y2] /PATH":
/////       -------------------------------------------------------
/////       X  Point  execution                          result
/////       -------------------------------------------------------
/////       0  0      {x0 /X X 1 add def              }  x0
/////       1  0      {y0 moveto /POINT 1 def /X 0 def}  y0 moveto
/////       0  1      {x1 /X X 1 add def              }  x1
/////       1  1      {y1 lineto              /X 0 def}  y1 lineto
/////       0  1      {x2 /X X 1 add def              }  x2
/////       1  1      {y2 lineto              /X 0 def}  y2 lineto
/////       -------------------------------------------------------
/////
/////  (2) /FILLPOLYGON
/////       (a) Function: Fill a polygon 
/////       (b) Usage:    [x0 y0 x1 y1 ... xn yn ] FILLPOLYGON
/////       (c) Definition:
/////           /FILLPOLYGON{ PATH closepath fill }def
/////
/////  (3) /DRAWFILLPOLYGON
/////       (a) Function: Draw outline of a polygon 
/////           and then fill its inside.
/////       (b) Usage:    [x0 y0 x1 y1 ... xn yn ] DRAWFILLPOLYGON
/////       (c) Definition:
/////           /DRAWFILLPOLYGON{ 
/////                PATH closepath gsave stroke grestore fill 
/////           } def
/////
/////  (4) /DRAWPATH 
/////       (a) Function: Draw a path as a polyline
/////       (b) Usage:    [x0 y0 x1 y1 ... xn yn ] DRAWPATH 
/////       (c) Definition:
/////           /DRAWPATH { PATH stroke } def
///// 
/////  (4) /BOXPATH 
/////       (a) Function: Make a square path
/////       (b) Usage:    [llx lly urx ury] BOXPATH  
/////       (c) Definition:
/////           /BOXPATH
/////           {  /X 0 def
/////              {  X 0 eq{/LLX exch def}if
/////                 X 1 eq{/LLY exch def}if
/////                 X 2 eq{/URX exch def}if
/////                 X 3 eq{/URY exch def}if
/////                 /X X 1 add def
/////              }forall
/////              LLX LLY moveto
/////              URX LLY lineto
/////              URX URY lineto
/////              LLX URY lineto
/////              closepath
/////           }def
/////
/////////////////////////////////////////////////////////////////////////////



	//---------------------//
	//----- constants -----//
	//---------------------//

	//----- default filename
const char    DEFAULT_PSFILE_NAME[] = "data.ps";

	//----- default window size
#include "a4size.h"
//const double  A4_WIDTH_MM  = 210.0	;	// mm
//const double  A4_HEIGHT_MM = 297.0	;	// mm

	//----- default drawing area
const double  DEFAULT_LEFT_RIGHT_MARGINE = 20.0 ;	// mm
const double  DEFAULT_TOP_BOTTOM_MARGINE = 20.0 ;	// mm

	//----- default precision of floating point calculation
const int     DEFAULT_PRECISION          = 9 ;

	//----- default viewport size 
	//.....  This shoud be identical with 3D module.
const double  DEFAULT_VIEWPORT_WIDTH  = 1.0; // width  of viewport
const double  DEFAULT_VIEWPORT_HEIGHT = 1.0; // height of viewport

	//----- default font
const int	DEFAULT_FONT_SIZE         = 24; // in units of pt
const char	DEFAULT_FONT_NAME[] = "Times-Roman" ; 
const ColorType DEFAULT_STRING_COLOR(0.0, 0.0, 0.0);


	//--------------------------------//
	//-----  class PostScript  -------//
	//--------------------------------//

class PostScript {
 protected:
		//----- local consts
	enum{ ERROR = 0, NORMAL = 1 };
	enum{ NAME_LENGTH = 32 };

		//----- stream for output ps file
		//.....  "out" is initialized to fout.
		//.....  If open( "filename",...) ,
		//.....  out( fout ) ;
	ostream*	out_p  ;	
	ofstream	fout ;

		//----- status of output file
		//.....  1: output file is open 
		//.....  0: output file is already closed 
	int		isFileOpen ;

		//----- window size and margins (in units of pt)
	double		windowWidth ,      windowHeight ;
	double		halfWindowWidth ,  halfWindowHeight ;
	double		leftRightMargin,  topBottomMargin  ;
	Vector2		viewportCenter ;

	 	//----- size of full drawing area in window ( in units of pt )
	double		drawing_area_width ,	   drawing_area_height ;
	double		half_drawing_area_width ,  half_drawing_area_height ;

	 	//----- position and size of clipping area in window
		//.....  ( in units of pt )
	double		clip_left, clip_bottom , clip_width, clip_height ;

		//----- scale factors for coorditate-transformations
	double			ptOverMm ;  
	double			ptOverVp ;  
	double			vpOverMm ;  


		//----- current attributes
		//.....  Note: 
		//.....    These attributes are reset to new values
		//.....    when the new values are passed (or used, anyway) 
		//.....    to a drawing function.
		//.....    However, they are not reset in the following 
		//.....    exceptional cases:
		//.....
		//.....      Exception (1):  in case of error 
		//.....      Exception (2):  in case that invisible color or
		//.....                      invisible linetype are used
		//.....
		//.....    Also, in most of the drawing functions,  	
		//.....    Attributes are not reset if the new values are
		//.....    exactly the same as the old ones.

		//---------- (1) font name
	char	fontname[ NAME_LENGTH ];

		//---------- (2) font size
	int	fontsize;

		//---------- (3) color type
	ColorType  current_color_type ;

		//---------- (4) line type
	LineType   current_line_type ; // Width and offsets are given
                                       // in units of mm
                                       // Initial values: 
                                       //    width       = 1.0 mm
                                       //    offsets     =   0 mm
                                       //    dash_length =   0 mm
	
   public:
		//------------------------------------------//
		//----- protected member functions       -----//
		//----- using device dependent units   -----//
		//------------------------------------------//

		//------------------------//
		//-----  mm ---> pt  -----//
		//------------------------//
	double	lengthMmToPt	( const double& X_mm )                  ;
	void	positionMmToPt	( const Vector2& P_mm , Vector2* P_pt ) ;
	void	boxMmToPt	( const Box& box_mm , Box* box_pt )     ;

		//------------------------//
		//-----  pt ---> mm  -----//
		//------------------------//
	double	lengthPtToMm	( const double& X_pt )                  ;
	void	positionPtToMm	( const Vector2& P_pt , Vector2* P_mm ) ;
	void	boxPtToMm	( const Box& box_pt , Box* box_mm )     ;

		//-----------------------//
		//----- vp ---> pt  -----//
		//-----------------------//
	double	lengthVpToPt	( const double& X_vp )                  ;
	void	positionVpToPt	( const Vector2& P_vp , Vector2* P_pt ) ;
	void	boxVpToPt	( const Box& box_vp , Box* box_pt )     ;

		//-----------------------//
		//----- pt ---> vp  -----//
		//-----------------------//
	double	lengthPtToVp	( const double& X_pt )                  ;
	void	positionPtToVp	( const Vector2& P_vp , Vector2* P_pt ) ;
	void	boxPtToVp	( const Box& box_vp , Box* box_pt )     ;


		//-----------------------------------//
		//----- moveto , lineto etc     -----//
		//-----------------------------------//

		//----- moveto_pt
	void	moveto_pt( double X_pt , double Y_pt ) 
		{
			*out_p << X_pt << " " << Y_pt << " " << "moveto" << endl ;
		}

		//----- rmoveto_pt
	void	rmoveto_pt( double X_pt , double Y_pt ) 
		{
			*out_p  << X_pt << " " << Y_pt << " " << "rmoveto" << endl ;
		}

		//----- lineto_pt
	void	lineto_pt( double X_pt , double Y_pt ) 
		{
			*out_p  << X_pt << " " << Y_pt << " " << "lineto" << endl ;
		}

		//----- rlineto_pt
	void	rlineto_pt( double X_pt , double Y_pt ) 
		{
			*out_p  << X_pt << " " << Y_pt << " " ; 
			*out_p  << "rlineto" << endl ;
		}

		//----- clip a range in drawing area ( in units of point ) 
	void	clip_pt(	double left , double bottom , 
				double width , double height );

		//----- PostScript::setBoundingBox()
		//..... in units of point
	void	setBoundingBox_pt( double left, double bottom, double width, double height );

		//----- draw box
	void	drawBox_pt( const Box& box_pt , LineType* linetype_p );

		//----- get clipping area
	Box	getClippingArea_pt();

		//----- get drawing area
	Box	getDrawingArea_pt();

		//----- draw linesegment
	void	drawLineSegment_pt(	Vector2  P_pt, Vector2 Q_pt ,
					LineType* linetype_p    ) ;

		//----- output a line "X Y moveto\n" to file
		//.....  units: viewport coordinate unit
	void	moveto_vp( double X_v , double Y_v ) ; 

		//----- lineto 
		//.....  units: viewport coordinate unit
	void	lineto_vp( double X_v , double Y_v ) ; 

	void	BoxPath_pt( double llx, double lly, double urx, double ury ) ;

	void	Path_pt( Vector2* point_pt, int num_point ) ;

	void	Path_pt( const Vector2& P_pt, const Vector2& Q_pt ) ;

	void	Path_vp( Vector2* point_vp, int num_point ) ;

	void	Path_vp( const Vector2& P_vp, const Vector2& Q_vp ) ;

	void	definePSfunctions();

 public:
		//----- see status
	int	isOpen() const { return isFileOpen ; }


		//--------------------------------------------//
		//----- direct output to PostScript file -----//
		//----- ( only for PostScript )          -----//
		//--------------------------------------------//
		//----- output a given string to file
	void	output( const char* string )  { *out_p  << string ; }
	void    flush()                       { out_p->flush(); }

		//----- output a comment line to file
	void	comment( const char* string ) { *out_p  << "% " << string << endl ; }
	void	comment( const char* string , int number ) 
		{ *out_p  << "% " << string << number << endl ; }
	void	comment( const char* string , double number ) 
		{ *out_p  << "% " << string << number << endl ; }
	void	comment( const char* string , const char* string2 ) 
		{ *out_p  << "% " << string << string2 << endl ; }

		//------------------------------------------------//
		//----- constructor, destructor, open, close -----//
		//------------------------------------------------//

		//----- constructor
	PostScript();

		//----- open()
		//..... Unit of lengths is "mm" 
		//..... Return: NORMAL or ERROR
		//..... filename: If "fileneme" is NULL, 
		//.....           the output stream is connected to cout
	int
	open(	const char*  filename		            ,
		int    flag_append_psfile                   ,
		const  Vector2&	viewport_center	            ,
		double window_width	= A4_WIDTH_MM  , 
		double window_height	= A4_HEIGHT_MM , 
		double left_right_margin= DEFAULT_LEFT_RIGHT_MARGINE , 
		double top_bottom_margin= DEFAULT_TOP_BOTTOM_MARGINE , 
		int    precision        = DEFAULT_PRECISION , 	
		double viewport_width	= DEFAULT_VIEWPORT_WIDTH      , 
		double viewport_height	= DEFAULT_VIEWPORT_HEIGHT      );

		//----- output two lines "stroke\n" 
		//.....  and "showpage\n" and close file.
	void	close() ;

		//----- destructor
		//..... If file is not closed yet, 
		//..... call member function close()
	~PostScript() { if( isFileOpen ){ this->close(); }  }


		//------------------------------------------------//
		//----- set precision in outputting numbers  -----//
		//------------------------------------------------//
	void	setPrecision( int precision = DEFAULT_PRECISION ) 
		{ out_p->precision( precision ) ; }

		//---------------------------------//
		//----- information of units  -----//
		//---------------------------------//
	double	getPtOverMm() { return ptOverMm ; }
	double	getPtOverVp() { return ptOverVp ; }
	double	getVpOverMm() { return vpOverMm ; }

		//--------------------//
		//----- font     -----//
		//--------------------//

		//----- set font
		//..... fontname: Times-Roman, Helvetica, Courier, Symbol, etc
		//.....           Do not add "/" before the font names.
	void	setFont(const char *fontname_given = DEFAULT_FONT_NAME,  
			int fontsize_given   = DEFAULT_FONT_SIZE );
	void	setFontSize( int fontsize_given = DEFAULT_FONT_SIZE ) ;

		//----- get font name and font size
	char*	getFontName() ;
	int	getFontSize() ;

		//--------------------//
		//----- color    -----//
		//--------------------//

		//----- set gray scale (0.0 --- 1.0)
		//.....  Return: ERROR or NORMAL
	int setGray( double grey_scale );

		//----- set RGB scale (0.0 --- 1.0)
		//.....  Return: ERROR or NORMAL
	int setRGB( double red, double green, double blue ) ; 


		//-----------------------//
		//----- linetype    -----//
		//-----------------------//

		//----- set line width
		//.....  Return: ERROR or NORMAL
		//.....  Argument: line width in units of mm
	int	setLineWidth( double width );

		//----- set line type ( general )
	void	setLineType( LineType* linetype_p ) ;

		//----- set line type to solid
	void	setLineStyleToSolid() ;

		//----- set line type to dashed line
		//.....  Return: ERROR or NORMAL
	int	setLineStyleToDash(	double length_1 = DEFAULT_DASH_LENGTH , 
					double offset   = DEFAULT_DASH_OFFSET  ) ; 

		//------------------------//
		//-----  vp ---> mm  -----//
		//------------------------//
	double	lengthVpToMm  ( const double& X_vp );
	void	positionVpToMm(	const Vector2& P_vp , Vector2* P_mm );
	void	boxVpToMm( const Box& box_vp , Box* box_mm );

		//------------------------//
		//-----  mm ---> vp  -----//
		//------------------------//
	double	lengthMmToVp   ( const double& X_mm );
	void	positionMmToVp ( const Vector2& P_mm , Vector2* P_vp );
	void	boxMmToVp      ( const Box& box_mm , Box* box_vp );

		//---------------------------------------------------//
		//----- functions in units of mm  		-----//
		//-----  or automatically calculated units      -----//
		//---------------------------------------------------//
		//----- draw string 
		//.....  (X_mm , Y_mm) : starting point of string in drawing area
		//.....  unit : mm,  
		//.....  color: fixed to black
	void	drawString( const char* string , double X_mm , double Y_mm );
	void	drawStringAndDoubleValue( char* string , double value, double X_mm , double Y_mm );

	void	drawColoredString( const char* str, 
				   double X_mm, double Y_mm,  
				   const ColorType& colortype = DEFAULT_STRING_COLOR);    	

		//----- draw linesegment PQ
	void	drawLineSegment(	Vector2  P_mm, Vector2 Q_mm ,
					LineType* linetype_p    ) ;


		//----- clip a range in drawing area ( in units of mm ) 
	void	clip( double left , double bottom , double width , double height );

		//----- get clippint area
	Box	getClippingArea();

		//----- get drawing area
	Box	getDrawingArea();

		//----- draw boundary of clipping area 
	void	drawClippingBox( LineType* linetype_p );

		//----- draw box
	void drawBox( const Box& box , LineType* linetype_p );


		//--------------------------------------------------------------//
		//----- functions in units of viewport coord  in 3D module -----//
		//--------------------------------------------------------------//

		//----- draw a line segment in viewport
	void	drawLineSegment_vp(	Vector2  P, Vector2 Q ,
					LineType* linetype_p    ) ;

		//----- drawPolygon_vp
		//..... point_v: point in viewport coordinate
		//.....  Return: ERROR or NORMAL
	void drawPolygon_vp( Vector2* point_v , int num_point , LineType&  linetype ) ;
	void drawPolygon_vp( Vector2* point_v , int num_point , LineType*  linetypeArray ) ;
	void drawPolygon_vp( Vector2* point_v , int num_point , LineType** linetypeAddressArray ) ;

		//----- fill polygon
		//..... point_v: point in viewport coordinate
	void	fillPolygon_vp( Vector2* point_v, int num_point , ColorType*  color_type_p );
	void	drawfillPolygon_vp( Vector2* point_v, int num_point , ColorType*  color_type_p );

		//----- drawing in landscape form
	Vector2		toLandscape_vp( const Vector2& point_vp );
	void		drawLineSegment_landscape_vp(	const Vector2&  P, const Vector2& Q ,
							LineType* linetype_p    ) ;
	void		drawPolygon_landscape_vp( Vector2* point_v , int num_point , LineType&  linetype ) ;
	void		drawPolygon_landscape_vp( Vector2* point_v , int num_point , LineType*  linetypeArray ) ;
	void		drawPolygon_landscape_vp( Vector2* point_v , int num_point , LineType** linetypeAddressArray ) ;
	void		fillPolygon_landscape_vp( Vector2* point_v, int num_point , ColorType*  color_type_p );
	void		drawfillPolygon_landscape_vp( Vector2* point_v, int num_point , ColorType*  color_type_p );

	void		fillCircle_vp          ( const Vector2& center_vp, double radius_vp , ColorType* color_p );
	void		fillCircle_landscape_vp( const Vector2& center_vp, double radius_vp , ColorType* color_p );
	void		fillSquare_vp          ( const Vector2& center_vp, double dL_vp     , ColorType* color_p );
	void		fillSquare_landscape_vp( const Vector2& center_vp, double dL_vp     , ColorType* color_p );

	void		drawColoredString_vp (	const char* str        , 
						const ColorType& color ,
						double size_vp         ,
						double offset_x_vp     ,
						double offset_y_vp     ,
						double X_vp            , 
						double Y_vp              )  ;
	void		drawColoredString_landscape_vp (	const char* str, 
								const ColorType& color ,
								double size_vp         ,
								double offset_x_vp     ,
								double offset_y_vp     ,
								double X_vp  ,         
								double Y_vp  )  ;

} ; // PostScript



//////////////// end of postscript.h
#endif
