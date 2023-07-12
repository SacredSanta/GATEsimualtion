//////////////////////
///// FRTubeSeg.h /////
//////////////////////

#if !defined FR_TUBE_SEG_H
#define FR_TUBE_SEG_H

#include <iostream.h>
#include "FRMath.h"

class FRTubeSeg
{
public:
	FRTubeSeg();
	FRTubeSeg(double pRmin, double pRmax, double pDz, double pSPhi, double pDPhi);
	FRTubeSeg(const FRTubeSeg& tubs);
	~FRTubeSeg() { } 

	void setGeometry(double, double, double, double, double);

	double getRmin() const { return fRmin; }
	double getRmax() const { return fRmax; }
	double getDz()   const { return fDz;   }
	double getSPhi() const { return fSPhi; }
	double getDPhi() const { return fDPhi; }	

protected:
	double fRmin;		  // radius of inside
	double fRmax;		  // radius of outside
	double fDz;			  // half height
	double fSPhi;		  // radian of start
	double fDPhi;		  // radian of sweep	
};

inline void FRTubeSeg::setGeometry(	double min ,
					double max ,
					double dz  ,
					double sphi,
					double dphi )
{
		//----- local const
	const double FR_PI2       =  2.0 * FR_D_PI ;
	const double MINUS_FR_PI2 = -2.0 * FR_D_PI ;

		//----- set values
	fRmin = min;
	fRmax = max;
	fDz   = dz;
	fSPhi = sphi;
	fDPhi = dphi;

		//----- error recovery for fRmin and fRmax
	if ( fRmin < 0.0   || fRmax <= fRmin  ) {
		cerr << "WARNING: Irrelevant paramater for Tubs 1 " << endl;
		cerr << "    rmin = " << fRmin << endl;
		cerr << "    rmax = " << fRmax << endl;

		fRmin = 0.0; 	fRmax = 1.0; 

		cerr << "    Reset to: " << endl;
		cerr << "         rmax = " << fRmax << endl;
		cerr << "         rmin = " << fRmin << endl;
	}

		//----- error recovery for fRmin and fDz
	if ( fDz <= 0.0 ) {
		cerr << "WARNING: Irrelevant paramater for Tubs 2 " << endl;
		cerr << "    dz = " << fDz << endl;

		fDz = 1.0;

		cerr << "    Reset to: " << endl;
		cerr << "         dz = " << fDz << endl;
	}

		//----- truncation for fSphi ( [-2 PI, +2 PI] )
	if ( fSPhi > FR_PI2       ) { fSPhi = FR_PI2    ; }
	if ( fSPhi < MINUS_FR_PI2 ) { fSPhi = MINUS_FR_PI2 ; }

		//----- truncation for fDphi ( [0, +2 PI] )
	if ( fDPhi > FR_PI2 ) { fDPhi = FR_PI2 ; }
	if ( fDPhi < 0.0    ) { fDPhi = 0.0    ; }

}

inline FRTubeSeg::FRTubeSeg()
{
	setGeometry(0, 0, 0, 0, 0);
}
inline FRTubeSeg::FRTubeSeg(double pRmin, double pRmax, double pDz,
	double pSPhi, double pDPhi)
{
	setGeometry(pRmin, pRmax, pDz, pSPhi, pDPhi);
}
inline FRTubeSeg::FRTubeSeg(const FRTubeSeg& tubs)
{
	setGeometry(tubs.fRmin, tubs.fRmax, tubs.fDz, tubs.fSPhi, tubs.fDPhi);
}

#endif
