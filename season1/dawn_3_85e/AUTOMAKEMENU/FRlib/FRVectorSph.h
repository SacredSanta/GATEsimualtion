///////////////////////
///// FRVectorSph.h /////
///////////////////////
#if !defined FR_VECTOR_Sph_H
#define FR_VECTOR_Sph_H

#include "FRRadian.h"
#include "FRVector3.h"
#include <math.h>

class FRVectorSph{
private:
	static const double epsilon = 1.0e-8;
	double mrho;
	FRRadian mphi, mtheta;
public:
	FRVectorSph():mrho(0.0),mphi(0.0),mtheta(0.0){ }
	FRVectorSph( double r, FRRadian p, FRRadian t ):
	mrho(r),mphi(p.normal()),mtheta(t.normal()){ }
	FRVectorSph( const FRVectorSph& v ):mrho(v.rho()),mphi(v.phi()),mtheta(v.theta()){ }
	FRVectorSph( const FRVector3& v );

	void rho( double r ){ mrho = r ; }
	void phi( FRRadian p ){ mphi = p.normal() ; }
	void theta( FRRadian t ){ mtheta = t.normal() ; }

	double rho() const { return mrho ; }
	FRRadian phi() const { return mphi ; }
	FRRadian theta() const { return mtheta ; }

	operator FRVector3() const ;

	FRVectorSph& operator()( double r, FRRadian p, FRRadian t )
	{ mrho = r ; mphi = p.normal() ; mtheta = t.normal() ; return *this ; }

	FRVectorSph& operator=( const FRVectorSph& v )
	{ mrho = v.rho() ; mphi = v.phi() ; mtheta = v.theta() ; return *this ; }
};

inline FRVectorSph::FRVectorSph( const FRVector3& v )
{ mrho = v.length(); mphi = atan2( v.y(), v.x() ) ; mtheta = acos( v.z()/mrho ) ; }

//FRVectorSph::operator FRVector3() const
//{ return FRVector3(mrho*sin(mtheta)*cos(mphi),mrho*sin(mtheta)*sin(mphi),mrho*cos(mtheta));}

inline FRVectorSph::operator FRVector3() const 
{
	double x, y, z ;
	x = mrho*sin(mtheta)*cos(mphi);
	y = mrho*sin(mtheta)*sin(mphi);
	z = mrho*cos(mtheta);
	x = ( fabs(x) > epsilon ) ? x : 0 ;
	y = ( fabs(y) > epsilon ) ? y : 0 ;
	z = ( fabs(z) > epsilon ) ? z : 0 ;
	return FRVector3( x, y, z );
}

#endif
