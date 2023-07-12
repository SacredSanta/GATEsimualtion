#include "FRMark.h"


FRMark::FRMark( void )
{ 
	type       = UNDEF; 
	position3D(0.0, 0.0, 0.0 );

	for( int i = 0  ; i < MAX_PARAM ; i++ ) {
		param[i] = 0.0 ;
	}

	color.setRGB(0.0, 0.0, 0.0) ;
}



FRMark::FRMark( int			type_given, 
		const Vector3&		P, 
		double			param0 ,
		const ColorType&	color_given )
{ 
	type = type_given ;

	if( type < UNDEF || type >= NUM_MARK ) 
	{ type = FRMark::UNDEF ; }

	setPosition3D (      P      ); 

	setColor      ( color_given ); 

	for( int i = 0  ; i < MAX_PARAM ; i++ ) {
		param[i] = 0.0 ;
	}
	param[0] = param0 ;
}


double	FRMark::getParam ( int i ) const 
{ 
	double	val = -999.0 ;

	if( i >= 0 && i < FRMark::MAX_PARAM ) {
		val = param[i] ; 
	} 

	return val ;
}


void	FRMark::setParam ( double val, int i ) 
{ 
	if( i >= 0 && i < FRMark::MAX_PARAM ) {
		param[i] = val ; 
	} 
}


void	FRMark::operator = ( const  FRMark& rhs ) 
{

	type       = rhs.type ;
	position3D = rhs.position3D ;

	for( int i = 0 ; i < FRMark::MAX_PARAM ; i++ ) {
		param[i] = rhs.param[i] ;
	}

	color      = rhs.color ;

	string     = rhs.string ;
}
