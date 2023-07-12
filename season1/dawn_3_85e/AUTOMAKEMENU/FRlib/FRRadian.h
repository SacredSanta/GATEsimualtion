//////////////////////
///// FRRadian.h /////
//////////////////////

#if !defined FR_RAGIAN_ANGLE_H
#define FR_RAGIAN_ANGLE_H

#include <math.h>
#include "FRBoolean.h"

static const double	TWO_PI = 2.0*M_PI;

class FRRadian{
private:
	double	radian;
	double	todegree( double r ) const { return r*360.0/TWO_PI ; }
	double	toradian( double d ) const { return TWO_PI*d/360.0 ; }
public:
	FRRadian( double r = 0 )
	{ radian = r ; }
	FRRadian( const FRRadian& x )
	{ radian = x.radian ; }

	operator double() const { return radian; }
	
	void	rad( double r )
	{ radian = r ; }
	void	deg( double d )
	{ radian = toradian( d ) ; }

	double	rad() const { return radian ; }
	double	deg() const { return todegree( radian ) ; }

	FRRadian operator+() const { return *this ; }
	FRRadian operator-() const { return FRRadian( -radian ); }

	FRRadian& operator()( double r ){ radian = r; return *this; }

	FRRadian& operator=( const FRRadian& x )
	{ radian = x.radian ; return *this ; }
	FRRadian& operator+=( const FRRadian& x )
	{ radian += x.radian ; return *this ; }
	FRRadian& operator-=( const FRRadian& x )
	{ radian -= x.radian ; return *this ; }
	FRRadian& operator*=( double x )
	{ radian *= x ; return *this ; }
	FRRadian& operator/=( double x )
	{ radian /= x ; return *this ; }

	FRRadian& normalize()
	{
		while( radian >= TWO_PI ) radian -= TWO_PI ;
		while( radian < 0 ) radian += TWO_PI ;
		
		return *this ;
	}

	FRRadian normal() const
	{
		FRRadian x(*this);

		return x.normalize();
	}
};

#endif
