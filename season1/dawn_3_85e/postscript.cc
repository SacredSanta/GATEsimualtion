///////////////////////////////////////////////////
//////////  postscript.cc    1997/11/28   /////////
///////////////////////////////////////////////////



#include "postscript.h"

	//----- PostScript::lengthMmToPt()
double	PostScript::lengthMmToPt( const double& X_mm )
{ 
	return ( X_mm * ptOverMm ) ;
}

	//----- PostScript::positionMmToPt()
void	PostScript::positionMmToPt( const Vector2& P_mm , Vector2* P_pt )
{
		//----- local variables
	double	X_mm, Y_mm ;
	double	X_pt, Y_pt ;	

		//----- set X_mm and Y_mm
	X_mm = P_mm.x ;		Y_mm = P_mm.y ;

		//----- rescale: mm --> point
	X_pt = X_mm * ptOverMm ;
	Y_pt = Y_mm * ptOverMm ;

		//----- shift origin 
	X_pt += leftRightMargin ;	Y_pt += topBottomMargin ;

		//----- set result
	P_pt->x = X_pt ;
	P_pt->y = Y_pt ;

} // positionMmToPt()



	//----- PostScript::boxMmToPt()
void	PostScript::boxMmToPt( const Box& box_mm , Box* box_pt )
{
		//----- local variables
	Vector2  corner0_mm, corner2_mm ;
	Vector2  corner0_pt, corner2_pt ;

		//-----  in viewort
	corner0_mm = box_mm.getCorner0() ;
	corner2_mm = box_mm.getCorner2() ;

		//-----  in drawing area (in units of pt )
	positionMmToPt( corner0_mm,  &corner0_pt );
	positionMmToPt( corner2_mm,  &corner2_pt );

		//-----
	box_pt->set(	corner0_pt , corner2_pt ) ;

} // PostScript::boxVpToPt()


	//----- PostScript::lengthPtToMm()
double	PostScript::lengthPtToMm( const double& X_pt )
{ 
	return ( X_pt / ptOverMm ) ;
}

	//----- PostScript::positionPtToMm()
void	PostScript::positionPtToMm( const Vector2& P_pt , Vector2* P_mm )
{
		//----- local variables
	double X_mm, Y_mm ;

		//----- X_mm
	X_mm = P_pt.x - leftRightMargin ; // shift origin
	X_mm /= ptOverMm ;		 

		//----- Y_mm
	Y_mm = P_pt.y - topBottomMargin ; // shift origin
	Y_mm /= ptOverMm ;		  // rescaling

		//----- P_mm( X_mm, Y_mm )
	P_mm->x = X_mm ;	P_mm->y = Y_mm ;	
	
} // PostScript::positionPtToMm()


	//----- PostScript::boxPtToMm()
void	PostScript::boxPtToMm( const Box& box_pt , Box* box_mm )
{
		//----- local variables
	Vector2  corner0_pt, corner2_pt ;
	Vector2  corner0_mm, corner2_mm ;

		//-----  in viewort
	corner0_pt = box_pt.getCorner0() ;
	corner2_pt = box_pt.getCorner2() ;

		//-----  in drawing area (in units of pt )
	positionPtToMm( corner0_pt,  &corner0_mm );
	positionPtToMm( corner2_pt,  &corner2_mm );

		//----- set returning box
	box_mm->set(	corner0_mm , corner2_mm ) ;

} // PostScript::boxPtToMm()


	//----- PostScript::lengthVpToPt()
double	PostScript::lengthVpToPt( const double& X_vp ) 
{ 
	return ( X_vp * ptOverVp ) ;
}


	//----- PostScript::positionVpToPt()
void PostScript::positionVpToPt(	const Vector2&	P_vp , 
					Vector2*	P_pt )
{
		//----- rescale: vp --> point
	double  X_pt = P_vp.x * ptOverVp ;
	double  Y_pt = P_vp.y * ptOverVp ;

		//----- shift origin 
	X_pt += ( halfWindowWidth   + viewportCenter.x ) ;
	Y_pt += ( halfWindowHeight  + viewportCenter.y ) ;

		//----- set result
	P_pt->x = X_pt ;	P_pt->y = Y_pt ;

} // PostScript::positionVpToPt( Vector2 )


	//----- PostScript::boxVpToPt()
void	PostScript::boxVpToPt( const Box& box_vp , Box* box_pt )
{
		//----- local variables
	Vector2  corner0_vp, corner2_vp ;
	Vector2  corner0_pt, corner2_pt ;

		//-----  in viewort
	corner0_vp = box_vp.getCorner0() ;
	corner2_vp = box_vp.getCorner2() ;

		//-----  in drawing area (in units of pt )
	positionVpToPt( corner0_vp,  &corner0_pt );
	positionVpToPt( corner2_vp,  &corner2_pt );

		//----- set result 
	box_pt->set(	corner0_pt , corner2_pt ) ;

} // PostScript::boxVpToPt()



	//----- PostScript::lengthPtToVp()
double	PostScript::lengthPtToVp( const double& X_pt ) 
{ 
	return ( X_pt / ptOverVp ) ;
}


	//----- PostScript::positionPtToVp()
void PostScript::positionPtToVp(	const Vector2&	P_pt , 
					Vector2*	P_vp )
{
		//----- rescale: pt --> vp
	double  X_vp = P_pt.x / ptOverVp ;
	double  Y_vp = P_pt.y / ptOverVp ;

		//----- shift origin 
	X_vp -= ( ( halfWindowWidth  + viewportCenter.x ) / ptOverVp ) ;
	Y_vp -= ( ( halfWindowHeight + viewportCenter.y ) / ptOverVp ) ; 

		//----- set result
	P_vp->x = X_vp ;	P_vp->y = Y_vp ;

} // PostScript::positionPtToVp( Vector2 )


	//----- PostScript::boxPtToVp()
void	PostScript::boxPtToVp( const Box& box_pt , Box* box_vp )
{
		//----- local variables
	Vector2  corner0_pt, corner2_pt ;
	Vector2  corner0_vp, corner2_vp ;

		//-----  in viewort
	corner0_pt = box_pt.getCorner0() ;
	corner2_pt = box_pt.getCorner2() ;

		//-----  in drawing area (in units of pt )
	positionPtToVp( corner0_pt,  &corner0_vp );
	positionPtToVp( corner2_pt,  &corner2_vp );

		//----- set result 
	box_vp->set(	corner0_vp , corner2_vp ) ;

} // PostScript::boxPtToVp()

	//----- PostScript::clip_pt()
	//..... in units of point
void	PostScript::clip_pt( double left, double bottom, double width, double height )
{
		//----- set data members
	clip_left    = left   ;
	clip_bottom  = bottom ;
	clip_width   = width  ;
	clip_height  = height ;

	double clip_right = clip_left + width;
	double clip_top = clip_bottom + height;

		//----- set clipping area to device
	BoxPath_pt( clip_left, clip_bottom, clip_right, clip_top);

	*out_p  << "clip" << endl;

} // PostScript::clip_pt()


	//----- PostScript::setBoundingBox()
	//..... in units of point
void	PostScript::setBoundingBox_pt( double left, double bottom, double width, double height )
{
		//----- local
	Vector2	ll, ur ;

		//----- left-bottom
	ll.x =  left   + leftRightMargin;
	ll.y =  bottom + topBottomMargin; 

		//----- right-top
	ur.x = ll.x + width ; 
	ur.y = ll.y + height ;

		//----- set clipping area to device
	*out_p  << "%%BoundingBox: " << ll.x << " " << ll.y << " " << ur.x << " " << ur.y << endl;

} // PostScript::setBoundingBox()

	//----- PostScript::drawBox_pt()
void PostScript::drawBox_pt( const Box& box_pt , LineType* linetype_p )
{
		//----- skip drawing invisible line
	if( (linetype_p->getVisibility()) == 0 ) { return ; }

	setLineType( linetype_p ) ;

	BoxPath_pt( box_pt.getLeft(), box_pt.getBottom(), box_pt.getRight(), box_pt.getTop());

	*out_p  << "stroke" << endl;

} // PostScript::drawBox_pt()

	//----- PostScript::getClippingArea_pt()
Box	PostScript::getClippingArea_pt()
{
	double clip_right = clip_left   + clip_width ;
	double clip_top   = clip_bottom + clip_height ;
	return Box( clip_left, clip_bottom, clip_right, clip_top );

} // PostScript::getClippingArea()


	//----- PostScript::getDrawingArea_pt()
Box	PostScript::getDrawingArea_pt()
{
	double  da_right = leftRightMargin + drawing_area_width  ;
	double  da_top   = topBottomMargin + drawing_area_height ;
	return Box( leftRightMargin, topBottomMargin , da_right, da_top );

} // PostScript::getDrawingArea_pt()


	//-----PostScript::drawLineSegment_pt()
void	PostScript::drawLineSegment_pt(	Vector2  P_pt, Vector2 Q_pt ,
					LineType* linetype_p    ) 
{
		//----- skip drawing invisible line
	if( (linetype_p->getVisibility()) == 0 ) { return ; }

		//----- set line type
	setLineType( linetype_p ) ;

		//----- draw give line segment
	Path_pt( P_pt, Q_pt ) ;
	*out_p  << "DRAWPATH" << endl;

} // PostScript::drawLineSegment_pt( x0,y0, x1,y1)


	//----- PostScript::moveto_vp()
void PostScript::moveto_vp( double X_vp , double Y_vp )
{
	Vector2		P_vp( X_vp, Y_vp ), P_pt ;

	positionVpToPt( P_vp, &P_pt );
	moveto_pt( P_pt.x, P_pt.y );

} // PostScript::moveto_vp()


	//----- PostScript::lineto_vp()
void PostScript::lineto_vp( double X_vp , double Y_vp )
{
	Vector2	P_vp( X_vp, Y_vp ), P_pt ;

	positionVpToPt( P_vp, &P_pt );
	lineto_pt( P_pt.x, P_pt.y );

} // PostScript::lineto_vp()


	//----- 
void PostScript::BoxPath_pt( double llx, double lly, double urx, double ury )
{
	*out_p << "[" << llx << " " << lly << " " ;
	*out_p << urx << " " << ury << "] BOXPATH " ;
}

	//----- 
void PostScript::Path_pt( Vector2* point_pt, int num_point)
{
	int i;
	
	*out_p << "[ " ;
	for( i = 0 ; i < num_point ; i++ )
		*out_p << point_pt[i].x << " " << point_pt[i].y << " " ;
	*out_p << "] " ;
}

	//----- 
void PostScript::Path_pt( const Vector2& P_pt, const Vector2& Q_pt)
{
	*out_p << "[ " << P_pt.x << " " << P_pt.y ;
	*out_p << " " << Q_pt.x << " " << Q_pt.y << "] " ;
}

	//----- 
void PostScript::Path_vp( Vector2* point_vp, int num_point)
{
	int i;
	Vector2 point_pt ;
	
	*out_p << "[ " ;
	for( i = 0 ; i < num_point ; i++ ){
		positionVpToPt( point_vp[i], &point_pt );
		*out_p << point_pt.x << " " << point_pt.y << " " ;
	}
	*out_p << "] " ;
}

	//----- 
void PostScript::Path_vp( const Vector2& P_vp, const Vector2& Q_vp)
{
	Vector2 P_pt, Q_pt;

	positionVpToPt( P_vp, &P_pt );
	positionVpToPt( Q_vp, &Q_pt );
	
	*out_p << "[ " << P_pt.x << " " << P_pt.y ;
	*out_p << " " << Q_pt.x << " " << Q_pt.y << "] " ;
}

	//----- 
void PostScript::definePSfunctions()
{
	*out_p	<< "/PATH\n";
	*out_p  << "{/POINT 0 def\n";
	*out_p	<< " /X 0 def\n";
	*out_p	<< " newpath\n";
	*out_p	<< " {X 1 eq{POINT 0 eq{moveto\n";
	*out_p	<< "                    /POINT 1 def\n";
	*out_p	<< "                  }{lineto }ifelse\n";
	*out_p	<< "         /X 0 def\n";
	*out_p	<< "       }{/X X 1 add def}ifelse\n";
	*out_p	<< " }forall\n";
	*out_p	<< "} def\n";
	*out_p	<< "/FILLPOLYGON{ PATH closepath fill }def\n";
	*out_p	<< "/DRAWFILLPOLYGON{ PATH closepath gsave stroke grestore fill }def\n";
	*out_p	<< "/DRAWPOLYGON{ PATH closepath stroke }def\n";
	*out_p	<< "/DRAWPATH { PATH stroke } def\n";
	*out_p	<< "/BOXPATH\n";
	*out_p	<< "{/X 0 def\n";
	*out_p	<< " {X 0 eq{/LLX exch def}if\n";
	*out_p	<< "  X 1 eq{/LLY exch def}if\n";
	*out_p	<< "  X 2 eq{/URX exch def}if\n";
	*out_p	<< "  X 3 eq{/URY exch def}if\n";
	*out_p	<< "  /X X 1 add def\n";
	*out_p  << " }forall\n";
	*out_p  << " LLX LLY moveto\n";
	*out_p  << " URX LLY lineto\n";
	*out_p  << " URX URY lineto\n";
	*out_p  << " LLX URY lineto\n";
	*out_p  << " closepath\n";
	*out_p  << "}def\n";

} // PostScript::definePSfunctions()	


	//----- PostScript::PostScript()
PostScript::PostScript():current_color_type(),current_line_type() 
{ 
		//----- status of output file
	isFileOpen = 0 ; // initialize flag to "file not open",
	out_p = &cout  ; // in which case out_p is set to &cout

		//----- window size, margin, drawing area
	windowWidth         = 0.0 ;	windowHeight        = 0.0 ;
	halfWindowWidth     = 0.0 ;	halfWindowHeight    = 0.0 ;
	leftRightMargin     = 0.0 ;	topBottomMargin     = 0.0 ;
	drawing_area_width  = 0.0 ;	drawing_area_height = 0.0;
	viewportCenter.x    = 0.0 ;	viewportCenter.y    = 0.0 ;
			
		//----- clipping area
		//..... initiallize to dummy values
	clip_width   =  -1.0;		clip_height  =  -1.0;
	clip_left    = -1.0e+5;		clip_bottom  = -1.0e+5;

		//----- set scale factors ( drawing_area / viewport )
	ptOverMm	= 72.0 / 25.4 ;	// decisive value
	ptOverVp	= 1.0 ;		// dummy value ( reset in open() )
	vpOverMm	= 1.0 ;		// dummy value ( reset in open() )

		//----- font
	strcpy(fontname,"") ; 
	fontsize = 0 ;

} // PostScript::PostScript()


	//----- PostScript::open()
int
PostScript::open(	const char*  filename	, 
			int          flag_append_psfile , 
			const  Vector2&	viewport_center ,
			double window_width	, 
			double window_height	, 
			double left_right_margin, 
			double top_bottom_margin, 
			int    precision        , 	
			double viewport_width	, 
			double viewport_height	) 
{
		//----- local constants and variables 
	double	scaleX, scaleY ;

		//----- output stream 
		//..... In constructor out_p is 
		//..... initialized to &cout,
		//..... and isFileOpen is initialized to 0 ;
	if( filename != NULL ) {

		if ( flag_append_psfile == 0 ) { 
			fout.open( filename );			
		} else {
			fout.open( filename , ios::app );	
		}

		if( !fout ) {
			isFileOpen = 0 ;  // cannot open output file
			return ERROR ;	
		} else {
			isFileOpen = 1 ;
			out_p = &fout ;
		}
	}

		//----- set paramaters for defining window and drawing area
		//.....  ( set data members )
	windowWidth      = ptOverMm * window_width  ;
	windowHeight     = ptOverMm * window_height ;

	halfWindowWidth  = 0.5 * windowWidth     ;
	halfWindowHeight = 0.5 * windowHeight ;

	leftRightMargin = ptOverMm * left_right_margin ;
	topBottomMargin = ptOverMm * top_bottom_margin ;

		//----- viewport center in drawing area
	viewportCenter.x	 = ptOverMm * viewport_center.x ;
	viewportCenter.y	 = ptOverMm * viewport_center.y ;

	 	//----- calc size of drawing area in window ( in units of pt )
		//.....  ( set data members )
	drawing_area_width  = windowWidth  - 2.0 * leftRightMargin ;
	drawing_area_height = windowHeight - 2.0 * topBottomMargin ;
	half_drawing_area_width  = 0.5 * drawing_area_width  ;
	half_drawing_area_height = 0.5 * drawing_area_height ;

		//----- set scale factors ( drawing_area / viewport )
	scaleX   =  drawing_area_width  / viewport_width  ;
	scaleY   =  drawing_area_height / viewport_height ;
	if( scaleX <= scaleY ) {
		ptOverVp = scaleX ;
		vpOverMm = viewport_width / (drawing_area_width / ptOverMm);
	} else {
		ptOverVp = scaleY ;
		vpOverMm = viewport_height / (drawing_area_height / ptOverMm);
	}
	
		//----- set precision of output
	setPrecision( precision ) ;

		//----- output 1st and 2nd line ( comment line ) to file 
	*out_p  << "%!PS-Adobe-3.0 EPSF-3.0" << endl;
	this->setBoundingBox_pt( 0.0, 0.0, drawing_area_width , drawing_area_height );
	*out_p  << "% FILE: " << filename << endl << endl;

		//----- set default font 
	*out_p  << "% DEFAULT FONT:\n" ;
	this->setFont();
	*out_p  << endl;

		//----- initialize default color
	*out_p << "% DEFAULT COLOR:\n" ;
	current_color_type = ColorType(0.0,0.0,0.0);  // black
	*out_p << current_color_type.getRed  () << " " ;
	*out_p << current_color_type.getGreen() << " " ;
	*out_p << current_color_type.getBlue () << " " ;
	*out_p << "setrgbcolor" << endl << endl;

		//----- initialize current linetype 
	*out_p << "% DEFAULT LINE:\n" ;

	current_line_type \
	= LineType( current_color_type ); 
		// use default arguments of class LineType 

		//----- output initial line type

		//---------- (0) color 
		//  Already output above

		//---------- (1) line width
	*out_p << lengthMmToPt( current_line_type.getWidth()  ) << " ";
	*out_p << "setlinewidth" << endl;

		//---------- (2) dash length
	if( current_line_type.getDashLength() == 0.0 ) {
		*out_p << "[]" ;
	}else{
		*out_p << "[" << lengthMmToPt( current_line_type.getDashLength() ) << "]" ;
	}

		//---------- (3) dash offset
	*out_p << " " << lengthMmToPt( current_line_type.getDashOffset() ) << " ";
	*out_p << "setdash" << endl << endl;

		//----- define PostScript functions
	*out_p << "% DEFINE PostScript Functions:\n" ;
	this->definePSfunctions();
	*out_p  << endl;

		//----- clipping: clip areas already defined as BoundingBox
	*out_p  << "% CLIPPING:\n" ;
	this->clip_pt( leftRightMargin, topBottomMargin , drawing_area_width , drawing_area_height );
	*out_p  << endl ; 

		//----- normal return 
	return NORMAL ;

} // PostScript::open()


	//----- PostScript::close()
void PostScript::close()
{ 
		//----- If this function has already been called, 
		//...... do nothing.
	if( isFileOpen ) { 
		fout.close();
		out_p = &cout ; // re-initialize out_p to &cout
	}

		//----- Reset flag to "file not open".
	isFileOpen = 0 ; 

} // PostScript::close()


	//----- PostScript::setFont()
void	PostScript::setFont( const char *fontname_given , int fontsize_given )
{

		//----- error: do nothing 
	if( fontname_given == NULL ) { return ; }
	if( fontsize_given <= 0    ) { return ; }

		//----- set font name
	strcpy( fontname, fontname_given );

		//----- set font size
	fontsize = fontsize_given ;

		//----- output 
	*out_p  << "/";
	*out_p  << fontname    << " " ;
	*out_p  << "findfont"  << " " ;
	*out_p  << fontsize    << " " ;
	*out_p  << "scalefont" << " " ;
	*out_p  << "setfont"   << " " ;
	*out_p  << endl;

} // PostScript::setFont()


	//----- PostScript::setFontSize()
void	PostScript::setFontSize( int fontsize_given )
{
	setFont( this->getFontName(), fontsize_given );
}

	//----- PostScript::getFontName()
char*	PostScript::getFontName(){ return  fontname ;}

	//----- PostScript::getFontSize()
int	PostScript::getFontSize(){ return  fontsize ;}


		//----- PostScript::setGray()
int PostScript::setGray( double gray_scale )
{
		//----- local variables
	int	status = ERROR ; // initialization

		//----- no need to change color?
	if(	gray_scale == current_color_type.getRed  () && \
		gray_scale == current_color_type.getGreen() && \
		gray_scale == current_color_type.getBlue ()      ) 
	{
		return NORMAL ; 
	}

		//----- set gray_scale
	if( gray_scale < 0.0 || gray_scale > 1.0 ) {
			//----- error: do nothing
		status = ERROR ;
		cerr << "ERROR (PostScript::setGray())\n";
	} else {
		status = NORMAL ;

			//----- output 
		*out_p  << gray_scale <<  " setgray" << endl ;

			//----- reset current color 
		current_color_type.setRGB  ( gray_scale, gray_scale, gray_scale );
		current_line_type.setColor ( current_color_type                 );
	}
		//----- return status
	return status ;

} // PostScript::setGray()



		//----- PostScript::setRGB()
int PostScript::setRGB( double red, double green, double blue ) 
{
		//----- local variables
	int	status = ERROR ;

		//----- no need to change color?
	if(	red   == current_color_type.getRed  ()	&& \
		green == current_color_type.getGreen()	&& \
		blue  == current_color_type.getBlue ()      ) 
	{
		return NORMAL ; 
	}
		
		//----- set color 
	if(	red   < 0.0 || red   > 1.0 || \
		green < 0.0 || green > 1.0 || \
		blue  < 0.0 || blue  > 1.0      ) 
	{
			//----- error: do nothing
		status = ERROR ;
		cerr << "ERROR (PostScript::setRGB())\n";
		cerr << "(RGB) = (" << red << " " << green << " " << blue << endl;

	} else {
		status = NORMAL ;

			//----- output		
		*out_p  << red <<  " " << green << " " << blue << " " ;
		*out_p  << "setrgbcolor" << endl;

			//----- set current color
		current_color_type.setRGB  ( red, green, blue   );
		current_line_type.setColor ( current_color_type );
	}
		//----- return status
	return status ;

} // PostScript::setRGB()



	//----- PostScript::setLineWidth()
int PostScript::setLineWidth( double width_mm )
{
		//----- local variables
	int	status   = ERROR  ; // initialization
	double	width_pt = lengthMmToPt ( width_mm );

		//----- same line width as before?
	if( width_mm == current_line_type.getWidth() ) {
		return NORMAL ; 
	}

		//----- set line width
	if( width_pt < 0.0 ) {
			//----- error : do nothing
		status = ERROR ;
		cerr << "ERROR (PostScript::setLineWidth())\n";
	} else {
		status = NORMAL ;

			//----- output (units is pt)
		*out_p  << width_pt <<  " setlinewidth" << endl ;

			//-----  reset current line width (unit is mm)
		current_line_type.setWidth( width_mm );
	}

		//----- return status
	return status ;

} // PostScript::setLineWidth()



	//----- PostScript::setLineStyleToSolid()
void 
PostScript::setLineStyleToSolid()
{ 
		//----- same line style as before ?
	if(	current_line_type.getStyle      () == FR_LINE_SOLID && \
		current_line_type.getDashLength () == 0.0           && \
		current_line_type.getDashOffset () == 0.0               )
	{
		return ;	
	}

		//----- output dash length and offsets
	*out_p  << "[] 0  setdash" << endl; 

		//---- reset current line style to solid
	current_line_type.setStyle      ( FR_LINE_SOLID ) ;
	current_line_type.setDashLength ( 0.0 ) ;
	current_line_type.setDashOffset ( 0.0 ) ;
}



	//----- PostScript::setLineStyleToDash()
int PostScript::setLineStyleToDash(	double length_mm, 
					double offset_mm     ) 
{
		//----- local variables
	int	status    =  ERROR ; // initialization
	double	length_pt =  lengthMmToPt ( length_mm );
	double	offset_pt =  lengthMmToPt ( offset_mm );

		//----- same line style as before ?
	if(	current_line_type.getStyle      () == FR_LINE_ON_OFF_DASH && \
		current_line_type.getDashLength () == length_mm           && \
		current_line_type.getDashOffset () == offset_mm               )
	{
		return NORMAL ;	
	}

		//----- set new values 
	if( length_pt < 0.0 || offset_pt < 0.0 ) 
	{ 
			//----- error: do nothing
		status = ERROR ;
		cerr << "ERROR (PostScript::setLineStyleToDash())\n";		

	} else {
			//----- output new values in units of pt
		if( length_pt == 0.0 ) {
			*out_p  << "[] " ;
		} else {
			*out_p  << "[" << length_pt << "] " ;
		}
		*out_p  << offset_pt << " setdash" << endl;

			//----- reset current line type to dash (unit is mm)
		current_line_type.setStyle      ( FR_LINE_ON_OFF_DASH );
		current_line_type.setDashLength ( length_mm );
		current_line_type.setDashOffset ( offset_mm );
	}

		//----- return status
	return status ;

} // PostScript::setLineStyleToDash()



	//----- PostScript::setLineType()
void  PostScript::setLineType( LineType* linetype_p )
{
		//----- invisible line type?
	if( ( linetype_p->getVisibility() ) == 0 ) {
		return ;	// do nothing and return
	}

		//----- color 
	setRGB(	linetype_p->getRed  () , 
		linetype_p->getGreen() , 
		linetype_p->getBlue ()    );

		//----- width (mm)
	setLineWidth( ( linetype_p->getWidth() ) ) ;

		//----- style
	if(	( linetype_p->getStyle() ) == FR_LINE_ON_OFF_DASH || \
		( linetype_p->getStyle() ) == FR_LINE_DOUBLE_DASH     )
	{
			//----- dash
		double  length_mm = linetype_p->getDashLength() ;
		double  offset_mm = linetype_p->getDashOffset() ;

		setLineStyleToDash( length_mm, offset_mm );

	} else {
			//----- solid
		setLineStyleToSolid();
	}

} // PostScript::setLineType()


	//----- PostScript::lengthVpToMm()
double	PostScript::lengthVpToMm( const double& X_vp )
{ 
	return ( X_vp / vpOverMm );
}

	//----- PostScript::positionVpToMm()
void	PostScript::positionVpToMm( const Vector2& P_vp , Vector2* P_mm )
{
		//----- rescaling
	double  X_mm = P_vp.x / vpOverMm ;
	double  Y_mm = P_vp.y / vpOverMm ;

		//----- shift origin
	X_mm += ( ( half_drawing_area_width  + viewportCenter.x ) / ptOverMm );
	Y_mm += ( ( half_drawing_area_height + viewportCenter.y ) / ptOverMm );

		//----- set result
	P_mm->x = X_mm ;	P_mm->y = Y_mm ;
	
} // PostScript::positionVpToMm()


	//----- PostScript::boxVpToMm()
void	PostScript::boxVpToMm( const Box& box_vp , Box* box_mm )
{
		//----- local variables
	Vector2  corner0_vp, corner2_vp ;
	Vector2  corner0_mm, corner2_mm ;

		//-----  left-bottom and right-top ( in units of vp )
	corner0_vp = box_vp.getCorner0() ;
	corner2_vp = box_vp.getCorner2() ;

		//-----  in drawing area (in units of mm )
	positionVpToMm( corner0_vp,  &corner0_mm );
	positionVpToMm( corner2_vp,  &corner2_mm );

		//-----
	box_mm->set(	corner0_mm , corner2_mm ) ;

} // PostScript::boxVpToMm()


	//----- PostScript::lengthMmToVp()
double	PostScript::lengthMmToVp( const double& X_mm )
{ 
	return ( X_mm * vpOverMm );
}

	//----- PostScript::positionMmToVp()
void	PostScript::positionMmToVp(	const Vector2& P_mm , 
					Vector2* P_vp )
{
		//----- rescaling
	double  X_vp = P_mm.x * vpOverMm ;
	double  Y_vp = P_mm.y * vpOverMm ;

		//----- shift origin
	X_vp -= ( ( half_drawing_area_width  + viewportCenter.x ) / ptOverVp );
	Y_vp -= ( ( half_drawing_area_height + viewportCenter.y ) / ptOverVp );

		//----- set result
	P_vp->x = X_vp ;	P_vp->y = Y_vp ;
	
} // PostScript::positionMmToVp()


	//----- PostScript::boxMmToVp()
void	PostScript::boxMmToVp( const Box& box_mm , Box* box_vp )
{
		//----- local variables
	Vector2  corner0_mm, corner2_mm ;
	Vector2  corner0_vp, corner2_vp ;

		//-----  left-bottom and right-top ( in units of vp )
	corner0_mm = box_mm.getCorner0() ;
	corner2_mm = box_mm.getCorner2() ;

		//-----  in drawing area (in units of mm )
	positionMmToVp ( corner0_mm,  &corner0_vp );
	positionMmToVp ( corner2_mm,  &corner2_vp );

		//-----
	box_vp->set(	corner0_vp , corner2_vp ) ;

} // PostScript::boxMmToVp()


	//----- PostScript::drawString()
void	PostScript::drawString(	const char* string , 
				double X_mm        , 
				double Y_mm          )
{

		//----- local
	Vector2	 P_mm( X_mm, Y_mm ) , P_pt ;

		//----- mm --> point ( in drawing area )
	positionMmToPt( P_mm , &P_pt );

		//----- move to starting positon of string 
	moveto_pt( P_pt.x, P_pt.y );

		//----- draw string and reset current color to black
	setRGB( 0.0, 0.0, 0.0 ) ;
	*out_p  << "(" << string << ")" << "show" << endl;

} // PostScript::drawString()


	//----- FRPostScript::drawColoredString()
void PostScript::drawColoredString(const char* str, 
				   double X_mm, double Y_mm,  
				   const ColorType& colortype)
{
		//----- invisible color ?
	if( ( colortype.getVisibility()) == 0 ) {
		return ;
	}

		//----- local
	Vector2 P_mm( X_mm, Y_mm ), P_pt;

		//----- mm --> point ( in drawing area )
	positionMmToPt( P_mm, &P_pt );

		//----- moveto starting position of string
	moveto_pt( P_pt.x, P_pt.y );

		//----- draw string and reset current color
	setRGB(colortype.getRed(), colortype.getGreen(), colortype.getBlue());
	*out_p  << "("<< str <<")" << "show" << endl;

} // FRPostScript::drawColoredString()


	//----- PostScript::drawStringAndDoubleValue()
void	PostScript::drawStringAndDoubleValue(	char* string , double value, 
						double X_mm , double Y_mm )
{
		//----- local
	Vector2	 P_mm( X_mm, Y_mm ) , P_pt ;

		//----- mm --> point
	positionMmToPt( P_mm , &P_pt );

		//----- move to starting positon of drawing string
	moveto_pt( P_pt.x, P_pt.y );

		//----- draw string and reset current color to black
	setRGB( 0.0, 0.0, 0.0 ) ;
	*out_p  << "(" << string << " " << value << ")" << "show" << endl;

} // PostScript::drawStringAndDoubleValue()


	//----- PostScript::drawLineSegment()
void	PostScript::drawLineSegment(	Vector2  P_mm, Vector2 Q_mm ,
					LineType* linetype_p    ) 
{
		//----- skip drawing invisible line
	if( (linetype_p->getVisibility()) == 0 ) { return ; }

		//----- local
	Vector2	P_pt, Q_pt ;

		//----- P, Q : mm --> pt
	positionMmToPt( P_mm, & P_pt );
	positionMmToPt( Q_mm, & Q_pt );

		//----- draw PQ
	drawLineSegment_pt( P_pt, Q_pt , linetype_p ) ;

} // PostScript::drawLineSegment( P_vp, Q_vp )


	//----- PostScript::clip()
	//..... in units of mm
void	PostScript::clip(	double left_mm,  double bottom_mm, 
				double width_mm, double height_mm )
{
		//----- local variables
	Box	clip_box_mm(	left_mm, 
				bottom_mm,  
				left_mm  + width_mm ,
				bottom_mm + height_mm );	
	Box	clip_box_pt ;

		//----- clipping box: mm --> pt
	boxMmToPt( clip_box_mm, &clip_box_pt );

		//----- set data members
	clip_left   = clip_box_pt.getLeft();
	clip_bottom = clip_box_pt.getBottom();
	clip_width  = clip_box_pt.getWidth();
	clip_height = clip_box_pt.getHeight();

	double clip_right  = clip_left + clip_width ;
	double clip_top = clip_bottom + clip_height ;

		//----- define clipping area
	BoxPath_pt( clip_left, clip_bottom, clip_right, clip_top);

	*out_p  << "clip" << endl;

} // PostScript::clip()


	//----- get clippint box
Box	PostScript::getClippingArea()
{
		//----- local variables
	Box 	box_mm ;

		//----- clipping box in units of pt
	boxPtToMm( getClippingArea_pt(), &box_mm );

		//----- return obtained box in units of mm
	return box_mm ;

} // PostScript::getClippingArea()


	//----- PostScript::getDrawingArea()
Box	PostScript::getDrawingArea()
{
		//----- local variables
	Box 	box_mm ;

		//----- clipping box in units of pt
	boxPtToMm( getDrawingArea_pt(), &box_mm );

		//----- return obtained box in units of mm
	return box_mm ;

} // PostScript::getDrawingArea()


		//----- draw boundary of clipping area 
		//.....  with given linetype
void	PostScript::drawClippingBox( LineType* linetype_p )
{
	drawBox_pt( getClippingArea_pt(),  linetype_p );

} // PostScript::drawClippingBox()


	//----- PostScript::drawBox()
void PostScript::drawBox( const Box& box_mm , LineType* linetype_p )
{
		//----- draw
	Box box_pt ;
	boxMmToPt( box_mm, &box_pt ) ;
	drawBox_pt( box_pt, linetype_p );

} // PostScript::drawBox()
	

		//-----PostScript::drawLineSegment_vp()
void	PostScript::drawLineSegment_vp(	Vector2 P_vp, 
					Vector2 Q_vp, 
					LineType* linetype_p ) 
{
		//----- skip drawing invisible line
	if( (linetype_p->getVisibility()) == 0 ) { return ; }

		//----- set line type
	setLineType( linetype_p ) ;

		//----- create a  new path
	Path_vp( P_vp, Q_vp ) ;

		//----- draw
	*out_p  << "DRAWPATH" << endl;

} // PostScript::drawLineSegment_vp( x0,y0, x1,y1)


	//----- void PostScript::drawPolygon_vp()
void PostScript::drawPolygon_vp(	Vector2* point , 
					int num_point  , 
					LineType&  linetype )
{
		//----- invisible linetype?
	if( (linetype.getVisibility()) == 0 ) {
		return ;
	}

		//----- set line type
	setLineType( &linetype ) ;

		//----- begin new path
	Path_vp( point, num_point );

		//----- draw
	*out_p  << "DRAWPOLYGON" << endl ;

} //PostScript::drawPolygon_vp()


	//----- PostScript::drawPolygon_vp( LineTypeAddressArray )
void PostScript::drawPolygon_vp(	Vector2* point , 
					int num_point , 
					LineType** linetypeAddressArray ) 
{
		//----- local variables
	int	i, i_plus ;

		//----- make path
	for( i = 0 ; i < num_point ; i++ ) {
		i_plus = i + 1 ;	
		if( i_plus == num_point ) { i_plus = 0 ; }
		
		drawLineSegment_vp(	point[i], point[i_plus], 
					linetypeAddressArray[i] ) ;
	}

} // PostScript::drawPolygon_vp(LineType**)


	//----- PostScript::drawPolygon_vp( LineTypeArray )
void PostScript::drawPolygon_vp(	Vector2* point , 
					int num_point , 
					LineType* linetypeArray ) 
{
		//----- local variables
	int	i, i_plus ;

		//----- make path
	for( i = 0 ; i < num_point ; i++ ) {
		i_plus = i + 1 ;	
		if( i_plus == num_point ) { i_plus = 0 ; }
		
		drawLineSegment_vp(	point[i], point[i_plus] ,
					&(linetypeArray[i])       ) ;
	}


} // PostScript::drawPolygon_vp(LineType*)


		//----- PostScript::fillPolygon_vp()
void	PostScript::fillPolygon_vp(	Vector2* point, 
					int num_point , 
					ColorType*  color_p )
{
		//----- invisible linetype?
	if( (color_p->getVisibility()) == 0 ) {
		return ;
	}

		//----- set line type
	setRGB(	color_p->getRed(), 
		color_p->getGreen(), 
		color_p->getBlue() ); 

		//----- begin new path
	Path_vp( point, num_point );

		//----- draw
	*out_p  << "FILLPOLYGON" << endl ;		

} // PostScript::fillPolygon_vp()


		//----- PostScript::drawfillPolygon_vp()
void	PostScript::drawfillPolygon_vp(	Vector2*    point     , 
					int         num_point ,	
					ColorType*  color_p )
{
		//----- invisible linetype?
	if( (color_p->getVisibility()) == 0 ) {
		return ;
	}

		//----- set line type
		//.....  Edges are drawn with minimal line width 
		//.....  and with current color. 
	setRGB(	color_p->getRed(), 
		color_p->getGreen(), 
		color_p->getBlue() ); 
	setLineWidth      ( 0.0 ) ; 
	setLineStyleToSolid();

		//----- begin new path
	Path_vp( point, num_point );

		//----- draw
	*out_p  << "DRAWFILLPOLYGON" << endl ;		

} // PostScript::drawfillPolygon_vp()


		//----- PostScript::fillCircle_vp( )
void	PostScript::fillCircle_vp(	const Vector2& center_vp , 
					double radius_vp         , 
					ColorType* color_p        )
{
		//----- invisible linetype?
	if( (color_p->getVisibility()) == 0 ) {
		return ;
	}

		//----- local variables
	Vector2	center_pt ;
	double	radius_pt ;

		//----- units
	this->positionVpToPt     ( center_vp , &center_pt ) ;
	radius_pt = lengthVpToPt ( radius_vp );

		//----- set color 
	setRGB(	color_p->getRed  (), 
		color_p->getGreen(), 
		color_p->getBlue () ); 

		//----- output 
	*out_p  << "newpath " ;
	*out_p  << (center_pt.x) << " " << (center_pt.y) << " " ;
	*out_p  << radius_pt     << " " ;
	*out_p  << "0 360  arc fill" << endl ;		

} // PostScript::fillCircle_vp()


	//-----  PostScript::fillSquare_vp()
void	PostScript::fillSquare_vp(	const Vector2&	center_vp  ,
					double		dL_vp      ,
					ColorType*	color_p     )
{
		//----- invisible linetype?
	if( (color_p->getVisibility()) == 0 ) {
		return ;
	}

		//----- local variables
	Vector2	center_pt ;
	double	dL_pt ;

		//----- rescaling
	this->positionVpToPt ( center_vp , &center_pt ) ;
	dL_pt = lengthVpToPt ( dL_vp );

		//----- set color
	setRGB(	color_p->getRed(), 
		color_p->getGreen(), 
		color_p->getBlue() ); 

		//-----  output
	double xmin = center_pt.x - dL_pt ; 
	double xmax = center_pt.x + dL_pt ; 
	double ymin = center_pt.y - dL_pt ; 
	double ymax = center_pt.y + dL_pt ; 

	*out_p  << "newpath " << endl;
	*out_p  << xmin << " " << ymin << "  moveto" << endl;
	*out_p  << xmax << " " << ymin << "  lineto" << endl;
	*out_p  << xmax << " " << ymax << "  lineto" << endl;
	*out_p  << xmin << " " << ymax << "  lineto" << endl;
	*out_p  << "fill" << endl ;		

} //  PostScript::fillSquare_vp()


	//----- FRPostScript::drawColoredString_vp ()
void PostScript::drawColoredString_vp (	const char* str        , 
					const ColorType& color ,
					double size_vp         ,
					double offset_x_vp     ,
					double offset_y_vp     ,
					double X_vp            , 
					double Y_vp              )  
{
		//----- invisible color ?
	if( ( color.getVisibility()) == 0 ) {
		return ;
	}

		//----- local
	Vector2 P_vp( X_vp, Y_vp ), P_pt;
	int	size_pt , offset_x_pt, offset_y_pt ;

		//----- units
	this->positionVpToPt     ( P_vp , &P_pt ) ;
	size_pt     = (int) (lengthVpToPt   ( size_vp      ) ) ;
	offset_x_pt = (int) (lengthVpToPt   ( offset_x_vp    ) ) ;
	offset_y_pt = (int) (lengthVpToPt   ( offset_y_vp    ) ) ;

		//----- moveto starting position of string
	moveto_pt( (P_pt.x + offset_x_pt) , (P_pt.y + offset_y_pt) ) ; 

		//-----draw string and reset current font and color
	setFontSize ( size_pt );
	setRGB(color.getRed(), color.getGreen(), color.getBlue());
	*out_p  << "("<< str <<")" << "show" << endl;

} // FRPostScript::drawColoredString_vp ()
