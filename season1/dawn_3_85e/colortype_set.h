///////////////////////////////////////////
///// colortype_set.h    1995/01/16  /////
//////////////////////////////////////////

#if !defined COLORTYPE_SET_H
#define COLORTYPE_SET_H

#include "colortype.h" 


////////////////////////////////
/////  class ColorTypeSet  /////
////////////////////////////////

class ColorTypeSet {

		//----- commonly-used instances of class ColorType
	ColorType	black ; 	
	ColorType	white ;
	ColorType	red ;
	ColorType	green ;
	ColorType	blue ;
	ColorType	yellow ;
	ColorType	magenta ; 	// dark red
	ColorType	cyan ;		// bright blue

 public:
		//----- constructor ( set colors )
	ColorTypeSet( void ) ;

		//----- get colors
	ColorType	getBlack  ( void )	const	{ return black;   }
	ColorType	getWhite  ( void )	const	{ return white;   }
	ColorType	getRed    ( void )	const	{ return red;     }
	ColorType	getGreen  ( void )	const	{ return green;   }
	ColorType	getBlue   ( void )	const	{ return blue;    }
	ColorType	getYellow ( void )	const	{ return yellow;  }
	ColorType	getMagenta( void )	const	{ return magenta; }
	ColorType	getCyan   ( void )	const	{ return cyan ;   }

} ; // ColorTypeSet


inline
ColorTypeSet::ColorTypeSet(void)
{
		//---------- BLACK
	black.setRGB( 0.0, 0.0, 0.0 ) ;
		//---------- WHITE
	white.setRGB( 1.0, 1.0, 1.0 ) ;
		//---------- RED
	red.setRGB  ( 1.0, 0.0, 0.0 ) ;
		//---------- GREEN
	green.setRGB( 0.0, 1.0, 0.0 ) ;
		//---------- BLUE
	blue.setRGB ( 0.0, 0.0, 1.0 ) ;
		//---------- YELLOW
	yellow.setRGB ( 1.0, 1.0, 0.0 ) ;
		//---------- MAGEBTA
	magenta.setRGB ( 1.0, 0.0, 1.0 ) ;
		//---------- CYAN
	cyan.setRGB ( 0.0, 1.0, 1.0 ) ;

} // ColorTypeSet::ColorTypeSet()


//////////////////////////////////////
#endif
//////////////////// END OF colortype_set.h
