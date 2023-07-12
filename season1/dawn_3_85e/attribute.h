////////////////////////////////////
///// attributte.h  1995/12/04 /////
////////////////////////////////////

#if !defined ATTRIBUTE_H
#define ATTRIBUTE_H

#include "colortype.h"

const int DEFAULT_ATT_PHONG_POWER = 3 ;

class Attribute {

		//----- label
	int	label ; // initially -1

		//----- reflection coefficients
	ColorType	kd ; // diffuse
	ColorType	ks ; // specular

		//----- power used in phong shading
		//.....  positive integer 
	int		phong_power ; 

		//----- transparency ( a flag )
		//..... on: 1 , off: 0
	int		transparency ; 

		//----- visibility ( a flag )
		//..... on: 1 , off: 0
	int		visibility   ; 

 public:
		//----- set data
	void  setLabel      ( int label_given ) { label = label_given; }
	void  setKd         ( const ColorType& kd_given ){ kd = kd_given; }
	void  setKs         ( const ColorType& ks_given ){ ks = ks_given; }
	void  setPhongPower ( int power )    
	  { phong_power  = power; if( phong_power < 0 ) { phong_power = 0 ;} }
	void  setTransparency( int on_off ) { transparency = !(on_off == 0 );}
	void  setVisibility ( int on_off ) { visibility   = !(on_off == 0 );}

		//----- get data
	int        getLabel       ( ) const { return label        ;}
	ColorType  getKd          ( ) const { return kd           ;}
	ColorType  getKs          ( ) const { return ks           ;}
	int        getPhongPower  ( ) const { return phong_power  ;}
	int        getTransparency( ) const { return transparency ;}
	int        getVisibility  ( ) const { return visibility   ;}

		//----- constructor and destructor
	Attribute () ;
	~Attribute(){;}

		//----- operator =
	void	operator = ( const Attribute& rhs ) ;
	void	set        ( const Attribute& rhs ) ;

}; // Attribute


	//----- Attribute::Attribute() 
inline 
Attribute::Attribute() 
{ 
	setLabel(-1) ; // undef
	kd.setRGB(1.0,1.0,1.0) ; // diffuse
	ks.setRGB(1.0,1.0,1.0) ; // specular
	setPhongPower(DEFAULT_ATT_PHONG_POWER) ; 
	setTransparency(0) ; // off (non-transparent)
	setVisibility  (1) ; // on  (visible)
}
		
	//----- Attribute::operator = ()
inline void
Attribute::operator = ( const Attribute& rhs ) 
{
	this->setLabel( rhs.getLabel() );
	this->setKd( rhs.getKd() );
	this->setKs( rhs.getKs() );
	this->setPhongPower( rhs.getPhongPower() );
	this->setTransparency ( rhs.getTransparency( ) ) ;
	this->setVisibility  ( rhs.getVisibility() ) ;
}

	//----- Attribute::set()
inline void
Attribute::set ( const Attribute& rhs ) 
{ 
	*this = rhs ; 
}


#endif
