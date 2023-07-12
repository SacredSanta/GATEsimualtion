//////////////////////////////////////
/////  colortype.h  (1995.05.15) /////
//////////////////////////////////////

#if !defined COLORTYPE_H
#define COLORTYPE_H

#include <iostream.h>

/////////////////////////////
/////  class ColorType  /////
/////////////////////////////

//---------------------------//
//----- class ColorType -----//
//---------------------------//
class ColorType {
 protected:
	double	red, green, blue ;	// RGB (0.0 --- 1.0)
	double	alpha ;			// alpha factor (0.0 --- 1.0)
	int	visibility ;		// 1: visible, 0: invisible
					// used to define "crystal color"
 public: 
		//----- constructors
		ColorType( void ) ;
		ColorType( double r, double g, double b , double a = 1.0 );
		ColorType( const ColorType& color_type ){ *this = color_type ;}

		//----- set R, G, B, and A intensities
		//..... min = 0.0, max = 1.0 
	void	setRGB( double r, double g, double b );
	void	setAlpha( double a );

		//------ set visibility
	void	visible()   { visibility = 1 ; }
	void	invisible() { visibility = 0 ; }

		//----- get
	double	getRed()   const { return red   ; }
	double	getGreen() const { return green ; }
	double	getBlue()  const { return blue  ; }
	double	getAlpha() const { return alpha ; }
	int	getVisibility() const { return visibility ; }

	double	R()  const { return red   ; }
	double	G()  const { return green ; }
	double	B()  const { return blue  ; }
	double	A()  const { return alpha ; }

	double	Red  ()  const { return red   ; }
	double	Green()  const { return green ; }
	double	Blue ()  const { return blue  ; }
	double	Alpha()  const { return alpha ; }


		//----- operator =
	void	operator = ( const ColorType& rhs ) 
	{
		red 		= rhs.getRed() ;
		green		= rhs.getGreen() ;
		blue		= rhs.getBlue() ;
		alpha		= rhs.getAlpha() ;
		visibility	= rhs.getVisibility();
	}


		//----- print
	void	printRGB () const
	{ cerr << "( " << red << " " << green << " " << blue << " )"; }	
	void	printoutRGB () const
	{ cout << "( " << red << " " << green << " " << blue << " )"; }	
	void	printerrRGB () const
	{ cerr << "( " << red << " " << green << " " << blue << " )"; }	

		//----- if white, transform to black
	void	whiteToBlack() ;


} ; // ColorType


	//-------------------------------------------------//
	//----- inline functions of class ColorType -------//
	//-------------------------------------------------//


	//----- setRGB(r, g, b )
inline void
ColorType::setRGB( double r, double g, double b )
{
		//----- set red 
	if      ( r >= 0.0 && r <= 1.0 ) { red   = r   ; }
	else if ( r >  1.0             ) { red   = 1.0 ; }
	else                             { red   = 0.0 ; }

		//----- set green
	if      ( g >= 0.0 && g <= 1.0 ) { green = g   ; }
	else if ( g >  1.0             ) { green = 1.0 ; }
	else                             { green = 0.0 ; }

		//----- set blue
	if      ( b >= 0.0 && b <= 1.0 ) { blue  = b   ; }
	else if ( b >  1.0             ) { blue  = 1.0 ; }
	else                             { blue  = 0.0 ; }

} // ColorType::setRGB(r, g, b)



	//----- setAlpha()
inline void
ColorType::setAlpha( double a )
{
		//----- set alpha
	if( a >= 0.0 && a <= 1.0 ) { alpha   = a ; }
	else { alpha = 1.0 ; }

} // ColorType::setAlpha()




	//----- setAlpha()
inline void
ColorType::whiteToBlack()
{
	if(	getRed()   > 0.99 && \
		getGreen() > 0.99 && \
		getBlue()  > 0.99 ) 
	{	
		setRGB(0.0, 0.0, 0.0 ) ; 
		// if white facet, make its edges black.
	} 

} // ColorType::setAlpha()


	//----- ColorType( void ) constructor 
inline 
ColorType::ColorType( void )
{
		//----- black
	setRGB( 0.0, 0.0, 0.0 );	
		//----- opaque
	setAlpha( 1.0 ) ;
		//----- visible
	visible();

} // ColorType::ColorType( void )


	//----- ColorType(r, g, b, a) constructor 
inline 
ColorType::ColorType( double r, double g, double b, double a )
{
	setRGB( r, g, b );	setAlpha( a ) ;	
	if( a == 0.0 ) { invisible(); } else { visible() ; }
} 


//---------------------------------------//
//-----  Default ColorType instance -----//
//---------------------------------------//
// const ColorType THE_DEFAULT_COLORTYPE ;


////////////// 
#endif
////////////// end of colortype.h
