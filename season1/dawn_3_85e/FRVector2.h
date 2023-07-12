///////////////////////
///// FRVector2.h /////
///////////////////////
#if !defined FR_VECTOR_2_H
#define FR_VECTOR_2_H

#include "FRBoolean.h"

#include <math.h>

const double FR_MIN_FRVECTOR2_LENGTH = 0.0 ;

class FRVector2{
private:
	double	mx, my ;
public:
	FRVector2():mx(0.0),my(0.0){ }
	FRVector2( double xx, double yy ):mx(xx),my(yy){ }
	FRVector2( const FRVector2& rhs ):mx(rhs.mx),my(rhs.my){ }

	void x( double xx ){ mx = xx ; }
	void y( double yy ){ my = yy ; }
	
	double x() const { return mx ; }
	double y() const { return my ; }
			
	double length() const { return  sqrt( mx*mx+my*my ) ; }
	
	FRVector2& normalize( double epsilon = FR_MIN_FRVECTOR2_LENGTH )
	{
		if( length() < epsilon ) *this = FRVector2();
		else *this /=length();

		return *this;
	}
	FRVector2 normal( double epsilon = FR_MIN_FRVECTOR2_LENGTH ) const
	{
		FRVector2 v(*this);
		return v.normalize(epsilon);
	}

	FRVector2  operator+() const
	{	return *this ;	}
	FRVector2  operator-() const
	{	return FRVector2( -mx, -my );	}
	FRVector2& operator()( double xx, double yy )
	{ mx = xx; my = yy; return *this ; }

	FRVector2& operator+=( const FRVector2& v )
	{ mx += v.mx; my += v.my; return *this ; }
	FRVector2& operator-=( const FRVector2& v )
	{ mx -= v.mx; my -= v.my; return *this ; }
	FRVector2& operator*=( double v )
	{ mx *= v; my *= v; return *this ; }
	FRVector2& operator/=( double v )
	{ mx /= v; my /= v; return *this ; }
};

inline FRVector2 operator+( const FRVector2& L, const FRVector2& R )
{	FRVector2 tmp( L ); return tmp += R ;	}
inline FRVector2 operator-( const FRVector2& L, const FRVector2& R )
{	FRVector2 tmp( L ); return tmp -= R ;	}
inline FRVector2 operator*( const FRVector2& L, double R )
{	FRVector2 tmp( L ); return tmp *= R ;	}
inline FRVector2 operator*( double L, const FRVector2& R )
{	FRVector2 tmp( R ); return tmp *= L ;	}
inline double operator*( const FRVector2& L, const FRVector2& R )
{	return	L.x()*R.x()+L.y()*R.y();	}
inline FRVector2 operator/( const FRVector2& L, double R )
{	FRVector2 tmp( L ); return tmp /= R ;	}
inline FRBoolean operator==( const FRVector2& L, const FRVector2& R )
{	return	( L.x() == R.x() && L.y() == R.y() );	}
inline FRBoolean operator!=( const FRVector2& L, const FRVector2& R )
{	return	!( L == R );	}

#endif
