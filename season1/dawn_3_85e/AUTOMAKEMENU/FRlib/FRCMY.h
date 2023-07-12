///////////////////
///// FRCMY.h /////
///////////////////

#if !defined FR_CMY_H
#define FR_CMY_H

#include "FRNDouble.h"

class FRCMY{
private:
	FRNDouble mC;
	FRNDouble mM;
	FRNDouble mY;
public:
	FRCMY():mC(0.0),mM(0.0),mY(0.0){ }
	FRCMY( FRNDouble c, FRNDouble m, FRNDouble y ):mC(c),mM(m),mY(y){ }
	FRCMY( const FRRGB& rgb ){ mC=1.0-rgb.R(); mM=1.0-rgb.G(); mY=1.0-rgb.B(); }

	void	C( FRNDouble c ){ mC = c ; }
	void	M( FRNDouble m ){ mM = m ; }
	void	Y( FRNDouble y ){ mY = y ; }

	FRNDouble	C() const { return mC ; }
	FRNDouble	M() const { return mM ; }
	FRNDouble	Y() const { return mY ; }

	FRCMY&	operator()( FRNDouble c, FRNDouble m, FRNDouble y )
	{ mC = c; mM = m; mY = y; return *this; }

	FRCMY&	operator=( const FRRGB& rgb )
	{ mC=1.0-rgb.R(); mM=1.0-rgb.G(); mY=1.0-rgb.B(); return *this; }

	operator FRRGB() const
	{ return FRRGB( 1.0-mC, 1.0-mM, 1.0-mY ); }
};

inline FRBoolean operator == ( const FRCMY& L, const FRCMY& R )
{ return ( L.C() == R.C() && L.M() == R.M() && L.Y() == R.Y() ); }

inline FRBoolean operator != ( const FRCMY& L, const FRCMY& R )
{ return !( L == R );	}

#endif
