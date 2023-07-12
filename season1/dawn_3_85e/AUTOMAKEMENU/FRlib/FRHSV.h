///////////////////
///// FRHSV.h /////
///////////////////

#if !defined FR_HSV_H
#define FR_HSV_H

#include "FRNDouble.h"
#include "FRRadian.h"

static const double HSV_CONSTANT = M_PI/3.0 ;

class FRHSV{
private:
	FRRadian mH;
	FRNDouble mS;
	FRNDouble mV;
public:
	FRHSV():mH(0.0),mS(0.0),mV(0.0){ }
	FRHSV( FRRadian h, FRNDouble s, FRNDouble v ):mH(h.normal()),mS(s),mV(v){ }
	FRHSV( const FRRGB& C ){ *this = C ; }

	void	H( FRRadian h ){ mH = h.normal() ; }
	void	S( FRNDouble s ){ mS = s ; }
	void	V( FRNDouble v ){ mV = v ; }

	FRRadian	H() const { return mH ; }
	FRNDouble	S() const { return mS ; }
	FRNDouble	V() const { return mV ; }

	FRHSV&	operator()( FRRadian h, FRNDouble s, FRNDouble v )
	{ mH = h.normal(); mS = s; mV = v; return *this; }

	FRHSV&	operator=( const FRRGB& C )
	{
		FRNDouble cd;

		mV=(C.R()>=C.G())?((C.R()<C.B())?C.B():C.R()):((C.G()>C.B())?C.G():C.B());
		cd=(C.R()<=C.G())?((C.R()>C.B())?mV-C.B():mV-C.R()):((C.G()<C.B())?mV-C.G():mV-C.B());
		mS=(mV==0.0)?0.0:cd/mV;
		if(mS==0.0) mH=0.0;
		else mH=(C.R()==mV)?(C.G()-C.B())/cd:((C.G()==mV)?2.0+(C.B()-C.R())/cd:4.0+(C.R()-C.G())/cd);
		mH*=HSV_CONSTANT;
		mH.normalize();

		return *this;
	}

	operator FRRGB() const 
	{
		double i,f,h=double(mH);
		h/=HSV_CONSTANT;
		f=modf(h,&i);
		switch((int)i){
			case 0: return FRRGB(mV,mV*(1.0-mS*(1.0-f)),mV*(1.0-mS));
			case 1: return FRRGB(mV*(1-mS*f),mV,mV*(1-mS));
			case 2: return FRRGB(mV*(1-mS),mV,mV*(1-mS*(1-f)));
			case 3: return FRRGB(mV*(1-mS),mV*(1-mS*f),mV);
			case 4: return FRRGB(mV*(1-mS*(1-f)),mV*(1-mS),mV);
			case 5: return FRRGB(mV,mV*(1-mS),mV*(1-mS*f));
		}
		return FRRGB(0,0,0);
	}
};

inline FRBoolean operator == ( const FRHSV& L, const FRHSV& R )
{return ( L.H() == R.H() && L.S() == R.S() && L.V() == R.V() ); }

inline FRBoolean operator != ( const FRHSV& L, const FRHSV& R )
{	return !( L == R ) ;	}

#endif
