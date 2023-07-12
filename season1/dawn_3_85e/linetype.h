///////////////////////////////////////////
///// linetype.h    1995/07/28        /////
///////////////////////////////////////////

#if !defined LINETYPE_H
#define LINETYPE_H

	//----- line style
enum { FR_LINE_SOLID, FR_LINE_ON_OFF_DASH,  FR_LINE_DOUBLE_DASH };

#include "colortype.h"	// class ColorType


//-----------------------//
//-----  constants  -----//
//-----------------------//

#if !defined LINE_PARAMETER_DEFINED
	#define	LINE_PARAMETER_DEFINED
	const double  DEFAULT_LINE_WIDTH	= 0.1 ;
	const double  DEFAULT_DASH_LENGTH	= 0.5 ; 
	const double  DEFAULT_DASH_OFFSET	= 0.0 ; 
#endif

const	double	UNIT_LINE_WIDTH         = 1.0 ;

//---------------------------//
//-----  CLASS LineType -----//
//---------------------------//


class LineType {
 protected:

	ColorType	color ;
	double		width;	
	int		style;	
	double		dash_offset ;	
	double		dashes;		
	
 public:
		//----- constructors
	LineType( void ) ;
	LineType(	ColorType	color_, 
			double		width_ = DEFAULT_LINE_WIDTH , 
			int		style_ = FR_LINE_SOLID      , 
			double		dash_offset_ = DEFAULT_DASH_OFFSET, 
			double		dashes_      = DEFAULT_DASH_LENGTH );
	LineType( const LineType& linetype ) { *this = linetype ; }
					// copy constructor

		//----- set
	void setColor( ColorType color_ )    { color = color_ ; }
	void setWidth( double width_     = DEFAULT_LINE_WIDTH ) 
		{ width = width_ ; if( width < 0.0 ) { width = DEFAULT_LINE_WIDTH ; } }
	void setStyle( int style_        = FR_LINE_SOLID ) 
		{ style       = style_ ; }
	void setDashOffset( double d_off = DEFAULT_DASH_OFFSET )  
		{ dash_offset = d_off  ; }
	void setDashes        ( double dashes_ = DEFAULT_DASH_LENGTH )
		{ dashes      = dashes_; }
	void setDashLength    ( double dashes_ = DEFAULT_DASH_LENGTH )
		{ dashes      = dashes_; }

		//----- get color attributes
		//.....  For efficiency, function to 
		//.....  directly access color.*** is prepared.
	ColorType  getColor() const { return color ; }
	double     getRed  () const { return color.getRed() ; }
	double     getGreen() const { return color.getGreen(); }
	double     getBlue () const { return color.getBlue() ; }
	double     getAlpha() const { return color.getAlpha() ; }
	int        getVisibility() const { return color.getVisibility() ; }

		//----- get width, style, and dash attributes
	double	getWidth () const { return width ; }
	int	getIntegerWidth () const { return (int)(width) ; }

	int	getStyle () const { return style ; } 
	double	getDashOffset () const { return  dash_offset; }	
	double	getDashes     () const { return  dashes     ; }
	double	getDashLength () const { return  dashes     ; }
	 
		//----- operator =
	void	operator = ( const LineType& rhs ) 
	{
		color 		= rhs.getColor() ;
		width		= rhs.getWidth() ;
		style		= rhs.getStyle() ;
		dash_offset	= rhs.getDashOffset() ;
		dashes		= rhs.getDashes();
	}

}; // LineType


//------------------------------------------------------//
//-----  Define LineType* type                     -----//
//------------------------------------------------------//
typedef	LineType* LineType_p;


//------------------------------------------------------//
//-----  inline member functions of class LineType -----//
//------------------------------------------------------//

	//----- LineType( void ) constructor
inline
LineType::LineType ( void ) 
{
		//----- set default values
	setColor( ColorType() ) ;	
	setWidth() ;			
	setStyle() ; 			
	setDashOffset() ; 		
	setDashes() ;			
} // LineType::LineType ( void ) 


	//----- Linetype( color, width, style, dash_offset, dashes ) 
	//.....  constructor
inline
LineType::LineType (	ColorType	color_, 
			double		 width_, 
			int		style_ , 
			double		dash_offset_ , 
			double		dashes_         )
{
	setColor( color_ ) ; 
	setWidth( width_ ) ; 
	setStyle( style_ ) ; 
	setDashOffset( dash_offset_ ) ; 
	setDashes( dashes_ ) ;

} // LineType::LineType ( color, width, style, dash_offset, dashes ) 



//--------------------------------------//
//-----  Default LineType instance -----//
//--------------------------------------//
// const LineType THE_DEFAULT_LINETYPE ;


//////////////////////////////////////
#endif
//////////////////// END OF linetype.h
