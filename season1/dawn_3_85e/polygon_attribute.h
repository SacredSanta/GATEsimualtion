/////////////////////////////////////////////
///// polygon_attribute.h   1995.05.15 //////
/////////////////////////////////////////////


#ifndef POLYGON_ATTRIBUTE_H
#define POLYGON_ATTRIBUTE_H

#include "object.h"

	//--------------------------------------//
	//-----  class PolygonAttribute  -------//
	//--------------------------------------//

struct PolygonAttribute {
	int	label ;
	double	red , green, blue ; // 0--1
	int		color_mode ; 
		//	enum { Facet::SOFT_COLOR , Facet::HARD_COLOR } ;
 public:

	PolygonAttribute(void) {
		label = -1 ;
		red = green =  blue = 1.0 ;
		color_mode = Facet::SOFT_COLOR ;
	}

	void operator() ( int label_given , double red_given, double green_given, double blue_given ) 
	{
		set( label_given , red_given, green_given, blue_given );
	}

	void set ( int label_given , double red_given, double green_given, double blue_given ) 
	{
		label = label_given ;
		red   = red_given ;
		green = green_given ;
		blue  = blue_given ;

		if( label < 0 ) { label = 0 ; }
		if( red   > 1.0 )   { red   = 1.0 ; }  	if( red   < 0.0 )   { red   = 0.0 ; } 
		if( green > 1.0 )   { green = 1.0 ; }  	if( green < 0.0 )   { green = 0.0 ; } 
		if( blue  > 1.0 )   { blue  = 1.0 ; }	if( blue  < 0.0 )   { blue  = 0.0 ; } 
	}

	void	setColorMode( int mode ) 
		{ 
			color_mode = mode ; 
			if( color_mode <= Facet::OBJECT_COLOR || color_mode > Facet::HARD_COLOR ) 
			{
				color_mode = Facet::SOFT_COLOR ;
			}
		}
					

	int	getLabel() const { return label ; }

	double getRed  ()  const { return red   ; }
	double getGreen()  const { return green ; }
	double getBlue ()  const { return blue  ; }

	int	getColorMode() const { return color_mode ; }

};// PolygonAttribure

#endif
