///////////////////
///// FRRGB.h /////
///////////////////

#if !defined FR_RGB_H
#define FR_RGB_H

#include "FRBoolean.h"
#include "FRNDouble.h"

class FRRGB{
private:
	FRNDouble red;
	FRNDouble green;
	FRNDouble blue;
public:
	FRRGB():red(0.0),green(0.0),blue(0.0){ }
	FRRGB( FRNDouble r, FRNDouble g, FRNDouble b):red(r),green(g),blue(b){ }

	void	R( FRNDouble r ){ red = r ; }
	void	G( FRNDouble g ){ green = g ; }
	void	B( FRNDouble b ){ blue = b ; }

	FRNDouble	R() const { return red ; }
	FRNDouble	G() const { return green ; }
	FRNDouble	B() const { return blue ; }

	FRRGB&	operator()( FRNDouble r, FRNDouble g, FRNDouble b )
	{ red = r; green = g; blue = b; return *this; }
	FRRGB&	operator=( const FRRGB& rgb )
	{ red = rgb.red; green = rgb.green; blue = rgb.blue; return *this;}
	FRRGB&	operator+=( const FRRGB& rgb )
	{ red += rgb.red; green += rgb.green; blue += rgb.blue; return *this;}
	FRRGB&	operator-=( const FRRGB& rgb )
	{ red -= rgb.red; green -= rgb.green; blue -= rgb.blue; return *this;}
	FRRGB&	operator*=( double x )
	{ red *= x; green *= x; blue *= x; return *this;}
	FRRGB&	operator/=( double x )
	{ red /= x; green /= x; blue /= x; return *this;}
};

inline FRBoolean	operator ==( const FRRGB& L, const FRRGB& R )
{return ( L.R() == R.R() && L.G() == R.G() && L.B() == R.B() ) ; }
inline FRBoolean	operator !=( const FRRGB& L, const FRRGB& R )
{	return !( L == R ) ;	}
inline FRRGB	operator+( const FRRGB& L, const FRRGB& R )
{	return FRRGB( L ) += R ;	}
inline FRRGB	operator-( const FRRGB& L, const FRRGB& R )
{	return FRRGB( L ) -= R ;	}
inline FRRGB	operator*( const FRRGB& L, double R )
{	return FRRGB( L ) *= R ;	}
inline FRRGB	operator*( double L, const FRRGB& R )
{	return FRRGB( R ) *= L ;	}
inline FRRGB	operator/( const FRRGB& L, double R )
{	return FRRGB( L ) /= R ;	}

#endif
