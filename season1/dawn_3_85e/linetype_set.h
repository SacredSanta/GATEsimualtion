///////////////////////////////////////////
///// linetype_set.h    1994/01/16   /////
//////////////////////////////////////////

#if !defined LTSET_H
#define LTSET_H

#include "linetype.h"

	//------------------------------------//
	//----- class LineTypeSet ------------//
	//------------------------------------//
class LineTypeSet {
		//----- commonly-used instances of class LineType 
	LineType lineCrystal ;
	LineType lineBlack ;
	LineType lineWhite ;
	LineType lineRed     ;
	LineType lineGreen   ;
	LineType lineBlue    ;
	LineType lineThick   ;
	LineType lineDash   ;

 public:
	LineTypeSet( void ) ;

	LineType	getLineCrystal( void )	const { return lineCrystal; }
	LineType	getLineBlack  ( void )	const { return lineBlack; }
	LineType	getLineWhite  ( void )	const { return lineWhite; }
	LineType	getLineRed    ( void )	const { return lineRed; }
	LineType	getLineGreen  ( void )	const { return lineGreen; }
	LineType	getLineBlue   ( void )	const { return lineBlue; }
	LineType	getLineThick  ( void )	const { return lineThick ; }
	LineType	getLineDash   ( void )	const { return lineDash ; }

} ; // LineTypeSet


inline
LineTypeSet::LineTypeSet(void)
{
		//----- local variables
	double	width ;
	ColorType color ;
	ColorType color_org ;

		//----- CRYSTAL line type
	color.invisible() ;
	lineCrystal.setColor ( color );

		//----- COLORs
//	color.ColorType();	// re-initialize color
	color = color_org ;

		//---------- BLACK line type 
	color.setRGB( 0.0, 0.0, 0.0 ) ;
	lineBlack.setColor( color );

		//---------- WHITE line type 
	color.setRGB( 1.0, 1.0, 1.0 ) ;
	lineWhite.setColor( color );

		//---------- RED line type 
	color.setRGB( 1.0, 0.0, 0.0 ) ;
	lineRed.setColor( color );

		//---------- GREEN line type 
	color.setRGB( 0.0, 1.0, 0.0 ) ;
	lineGreen.setColor( color );

		//---------- BLUE line type 
	color.setRGB( 0.0, 0.0, 1.0 ) ;
	lineBlue.setColor( color );

		//----- THICK line type 
	width = 2.0 * UNIT_LINE_WIDTH ;
	lineThick.setWidth ( width );

		//----- DASH
	lineDash.setStyle ( FR_LINE_ON_OFF_DASH ) ;

} // LineTypeSet::LineTypeSet()


//////////////////////////////////////
#endif
//////////////////// END OF linetype_set.h
