///////////////////////
///// FRVector3.h /////
///////////////////////
#if !defined FR_VECTOR_3_H
#define FR_VECTOR_3_H

#include <math.h>
#include "FRBoolean.h"

const double FR_MIN_FRVECTOR3_LENGTH = 0.0 ;


class FRVector3{
private:
	double	mx, my, mz ;
public:
	FRVector3():mx(0.0),my(0.0),mz(0.0){ }
	FRVector3( double xx, double yy, double zz ):mx(xx),my(yy),mz(zz){ }
	FRVector3( const FRVector3& rhs ):mx(rhs.mx),my(rhs.my),mz(rhs.mz){ }

	void x( double xx ){ mx = xx ; }
	void y( double yy ){ my = yy ; }
	void z( double zz ){ mz = zz ; }
	
	double x() const { return mx ; }
	double y() const { return my ; }
	double z() const { return mz ; }
			
	double length() const { return  sqrt( mx*mx+my*my+mz*mz ) ; }
	
	FRVector3& normalize( double epsilon = FR_MIN_FRVECTOR3_LENGTH )
	{
		if( length() < epsilon ) *this = FRVector3();
		else *this /=length();

		return *this;
	}
	FRVector3 normal( double epsilon = FR_MIN_FRVECTOR3_LENGTH ) const
	{
		FRVector3 v(*this);

		return v.normalize(epsilon);
	}

	FRVector3  operator+() const
	{	return *this ;	}
	FRVector3  operator-() const
	{	return FRVector3( -mx, -my, -mz );	}
	FRVector3& operator()( double xx, double yy, double zz )
	{ mx = xx; my = yy; mz = zz; return *this ; }

	FRVector3& operator+=( const FRVector3& v )
	{ mx += v.x(); my += v.y(); mz +=v.z(); return *this ; }
	FRVector3& operator-=( const FRVector3& v )
	{ mx -= v.x(); my -= v.y(); mz -=v.z(); return *this ; }
	FRVector3& operator*=( double v )
	{ mx *= v; my *= v; mz *= v; return *this ; }
	FRVector3& operator/=( double v )
	{ mx /= v; my /= v; mz /= v; return *this ; }
	FRVector3& operator^=( const FRVector3& v )
	{ *this = FRVector3( my*v.z()-mz*v.y(), mz*v.x() - mx*v.z(), mx*v.y() - my*v.x() ); return *this ; }
};

inline FRVector3 operator+( const FRVector3& L, const FRVector3& R )
{ FRVector3 tmp( L ); return tmp += R; }
inline FRVector3 operator-( const FRVector3& L, const FRVector3& R )
{ FRVector3 tmp( L ); return tmp -= R; }
inline FRVector3 operator*( const FRVector3& L, double R )
{ FRVector3 tmp( L ); return tmp *= R; }
inline FRVector3 operator*( double L, const FRVector3& R )
{ FRVector3 tmp( R ); return tmp *= L; }
inline double operator*( const FRVector3& L, const FRVector3& R )
{	return	L.x()*R.x()+L.y()*R.y()+L.z()*R.z() ;	}
inline FRVector3 operator/( const FRVector3& L, double R )
{ FRVector3 tmp( L ); return tmp /= R; }
inline FRVector3 operator^( const FRVector3& L, const FRVector3& R )
{ FRVector3 tmp( L ); return tmp ^= R; }
inline FRBoolean operator==( const FRVector3& L, const FRVector3& R )
{	return	( L.x() == R.x() && L.y() == R.y() && L.z() == R.z() );	}
inline FRBoolean operator!=( const FRVector3& L, const FRVector3& R )
{	return	!( L == R );	}

#endif
