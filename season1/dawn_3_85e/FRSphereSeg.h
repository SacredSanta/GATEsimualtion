/////////////////////////
///// FRSphereSeg.h /////
/////////////////////////

#if !defined FR_SPHERESEG_H
#define FR_SPHERESEG_H

#include<math.h>
#include "FRMath.h"

class FRSphereSeg
{
public:
	FRSphereSeg();
	FRSphereSeg(double R, double r, double phi, double dphi,
	 			double theta, double dtheta);
	FRSphereSeg(const FRSphereSeg& sphes);
	~FRSphereSeg() { } 
	 
	void setGeometry(double R, double r, double phi, double dphi,
					double theta, double dtheta);

	void SetR(double R) 			{ fR = R; }
	void Setr(double r) 			{ fr = r; }
	void SetPhi(double phi)			{ fPhi  = phi; }
	void SetdPhi(double dphi)		{ fdPhi = dphi; }		
	void SetTheta(double theta)		{ fTheta  = theta; }
	void SetdTheta(double dtheta)	{ fdTheta = dtheta; }		

	double getR() const 	   { return fR; }
	double getr() const 	   { return fr; }
	double getPhi() const 	   { return fPhi; }
	double getdPhi() const 	   { return fdPhi; }		
	double getTheta() const    { return fTheta; }
	double getdTheta() const   { return fdTheta; }		

protected:
	double fR;			// outside radius of sphereseg
	double fr; 			// inside radius of sphereseg
	double fPhi;		// azimuthal start 
	double fdPhi;		// azimuthal sweep 
	double fTheta;		// polar start 
	double fdTheta;		// polar sweep 
};

inline void FRSphereSeg::setGeometry(	double R     ,
					double r     ,
					double phi   ,
					double dphi  ,
					double theta ,
					double dtheta )
{

		//----- local const
	const double FR_PI        =  FR_D_PI ;
	const double FR_PI2       =  2.0 * FR_D_PI ;
	const double MINUS_FR_PI2 = -2.0 * FR_D_PI ;

		//----- set values
	fR     = R     ; fr = r;           // radiuses
	fPhi   = phi   ; fdPhi = dphi;     // azimuthal angle
	fTheta = theta ; fdTheta = dtheta; // polar angle


		//----- error recovery for fSphi ( [-2 PI, +2 PI] )
	if ( fPhi > FR_PI2       ) { fPhi = FR_PI2       ; }
	if ( fPhi < MINUS_FR_PI2 ) { fPhi = MINUS_FR_PI2 ; }

		//----- error recovery for fDphi ( [0, +2 PI] )
	if ( fdPhi > FR_PI2 ) { fdPhi = FR_PI2 ; }
	if ( fdPhi < 0.0    ) { fdPhi = 0.0    ; }


		//----- error recovery for fTheta ( [0 , PI] )
	if ( fTheta > FR_PI ) { fTheta = FR_PI ; }
	if ( fTheta < 0.0   ) { fTheta = 0.0   ; }

		//----- error recovery for fdTheta ( [0, PI] )
	if ( fdTheta > FR_PI ) { fdTheta = FR_PI ; }
	if ( fdTheta < 0.0   ) { fdTheta = 0.0    ; }

}

inline FRSphereSeg::FRSphereSeg()
{
	setGeometry(0, 0, 0, 0, 0, 0);
}
inline FRSphereSeg::FRSphereSeg(double R, double r, double phi, double dphi,
		double theta, double dtheta)
{
	setGeometry(R, r, phi, dphi, theta, dtheta);
}
inline FRSphereSeg::FRSphereSeg(const FRSphereSeg& sphes) 
{
	setGeometry(sphes.fR, sphes.fr, sphes.fPhi, sphes.fdPhi,
			sphes.fTheta, sphes.fdTheta);
}

#endif
