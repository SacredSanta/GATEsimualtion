//////////////////////
///// FRDegree.h /////
//////////////////////

#if !defined FR_DEGREE_ANGLE_H
#define FR_DEGREE_ANGLE_H

#include <math.h>
#include "FRBoolean.h"

class FRDegree{
private:
	double	degree;
	double	todegree( double r ) const { return r*180.0/M_PI ; }
	double	toradian( double d ) const { return M_PI*d/180.0 ; }
public:
	FRDegree( double d = 0 )
	{ degree = d ; }
	FRDegree( const FRDegree& x )
	{ degree = x.degree ; }
	FRDegree( const FRRadian& x )
	{ degree = x.deg() ; }

	operator double() const { return degree; }
	operator FRRadian() const { return FRRadian( this->rad() ); }
	
	void	deg( double d )
	{ degree = d ; }
	void	rad( double r )
	{ degree = todegree( r ) ; }

	FRDegree operator+() const { return *this; }
	FRDegree operator-() const { return FRDegree( -degree ); }

	FRDegree& operator()( double d ){ degree = d; return *this; }

	FRDegree& operator=( const FRDegree& x )
	{ degree = x.degree ; return *this ; }
	FRDegree& operator+=( const FRDegree& x )
	{ degree += x.degree ; return *this ; }
	FRDegree& operator-=( const FRDegree& x )
	{ degree -= x.degree ; return *this ; }
	FRDegree& operator*=( double x )
	{ degree *= x ; return *this ; }
	FRDegree& operator/=( double x )
	{ degree /= x ; return *this ; }

	FRDegree& normalize()
	{
		while( degree >= 360 ) degree -=360 ;
		while( degree < 0 ) degree +=360 ;
		
		return *this ;
	}

	FRDegree normal() const
	{
		FRDegree x(*this);

		return x.normalize();
	}
};

#endif
