//////////////////////
///// FRConeSeg.h /////
//////////////////////

#if !defined FRCONESEG_H
#define FRCONESEG_H

#include <iostream.h>
#include "FRMath.h"

class FRConeSeg
{
public:
	FRConeSeg();
	FRConeSeg(double pRmin1, double pRmax1, double pRmin2, double pRmax2,
			double pDz, double pSPhi, double pDPhi);
	FRConeSeg(const FRConeSeg& cons);
	virtual ~FRConeSeg() { } 

	void setGeometry(double, double, double, double, double, double, double);
	
	double getRmin1() const { return fRmin1; }
	double getRmax1() const { return fRmax1; }
	double getRmin2() const { return fRmin2; }
	double getRmax2() const { return fRmax2; }
	double getDz()    const { return fDz   ; }
	double getSPhi()  const { return fSPhi ; }
	double getDPhi()  const { return fDPhi ; }	

protected:
	double fRmin1;		// radius of inside ( bottom )
	double fRmax1;		// radius of outside( bottom )
	double fRmin2;		// radius of inside ( top )
	double fRmax2;		// radius of outside( top )	
	double fDz; 		// half height
	double fSPhi;		// radian of start
	double fDPhi;		// radian of sweep	
};

inline void FRConeSeg::setGeometry(	double pRmin1,
					double pRmax1,
					double pRmin2,
					double pRmax2,
					double pDz   ,
					double pSPhi ,
					double pDPhi  )
{
		//----- local const
	const double FR_PI2       =  2.0 * FR_D_PI ;
	const double MINUS_FR_PI2 = -2.0 * FR_D_PI ;

		//----- set values
	fRmin1 = pRmin1; fRmax1 = pRmax1;
	fRmin2 = pRmin2; fRmax2 = pRmax2;
	fDz = pDz;
	fSPhi = pSPhi; fDPhi = pDPhi;

/////////////////////////////////////////////////////////
//		//----- error recovery for fRmin1
//	if ( fRmin1 < 0.0     ) { fRmin1 = 0.0 ;   }
//
//		//----- error recovery for fRmax1
//	if ( fRmax1 < fRmin1  ) { fRmax1 = fRmin1 ; }
//
//		//----- error recovery for fRmin2
//	if ( fRmin2 < 0.0     ) { fRmin2 = 0.0 ;   }
//
//		//----- error recovery for fRmax2
//	if ( fRmax2 < fRmin2  ) { fRmax2 = fRmin2 ; }
//
//		//----- error recovery for dz
//	if ( fDz < 0.0       ) { fDz = 0.0 ; }
//////////////////////////////////////////////////////////

//////////////////

		//----- error recovery for fRmin and fRmax
	if ( fRmin1 < 0.0   || fRmax1 <= fRmin1 || fRmin2 < 0.0   || fRmax2 <= fRmin2  ) {
		cerr << "WARNING: Irrelevant paramater for Cons 1 " << endl;
		cerr << "    rmin1 = " << fRmin1 << endl;
		cerr << "    rmax1 = " << fRmax1 << endl;
		cerr << "    rmin2 = " << fRmin2 << endl;
		cerr << "    rmax2 = " << fRmax2 << endl;

		fRmin1 = 0.0; 	fRmax1 = 1.0; 
		fRmin2 = 0.0; 	fRmax2 = 1.0; 

		cerr << "    Reset to: " << endl;
		cerr << "         rmax1 = " << fRmax1 << endl;
		cerr << "         rmin1 = " << fRmin1 << endl;
		cerr << "         rmax2 = " << fRmax2 << endl;
		cerr << "         rmin2 = " << fRmin2 << endl;
	}

		//----- error recovery for fRmin and fDz
	if ( fDz <= 0.0 ) {
		cerr << "WARNING: Irrelevant paramater for Cons 2 " << endl;
		cerr << "    dz = " << fDz << endl;

		fDz = 1.0;

		cerr << "    Reset to: " << endl;
		cerr << "         dz = " << fDz << endl;
	}

///////////////////

		//----- truncaton  for fSphi ( [-2 PI, +2 PI] )
	if ( fSPhi > FR_PI2       ) { fSPhi = FR_PI2       ; }
	if ( fSPhi < MINUS_FR_PI2 ) { fSPhi = MINUS_FR_PI2 ; }

		//----- truncation for fDphi ( [0, +2 PI] )
	if ( fDPhi > FR_PI2 ) { fDPhi = FR_PI2 ; }
	if ( fDPhi < 0.0    ) { fDPhi = 0.0    ; }

}

inline FRConeSeg::FRConeSeg()
{
	setGeometry(0, 0, 0, 0, 0, 0, 0);
}
inline FRConeSeg::FRConeSeg(double pRmin1, double pRmax1,
		double pRmin2, double pRmax2, double pDz, double pSPhi, double pDPhi)
{
	setGeometry(pRmin1, pRmax1, pRmin2, pRmax2,	pDz, pSPhi, pDPhi);
}
inline FRConeSeg::FRConeSeg(const FRConeSeg& cons)
{
	setGeometry(cons.fRmin1, cons.fRmax1, cons.fRmin2, cons.fRmax2,
			cons.fDz, cons.fSPhi, cons.fDPhi);
}

#endif
