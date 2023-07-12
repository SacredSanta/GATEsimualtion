////////////////////////
/////// FRTorus.h //////
////////////////////////
#if !defined FR_TORUS_H
#define FR_TORUS_H

class FRTorus
{
public:
	FRTorus();
	FRTorus(	double rmin, 
			double rmax, 
			double rtor, 
			double sphi, 
			double dphi  ) ;
	FRTorus(const FRTorus& torus);
	virtual ~FRTorus() { } 

	void setGeometry( double, double, double, double, double );

	double getRmin () const { return fRmin ; }
	double getRmax () const { return fRmax ; }
	double getRtor () const { return fRtor ; }
	double getSphi () const { return fSphi ; }
	double getDphi () const { return fDphi ; }

protected:

	double fRmin ; // Minimal radius of  the curve tube
	double fRmax ; // Minimal radius of  the curve tube
	double fRtor ; // Distance between the origin and 
                       // the circle extending in 
                       // the middle of the curved tube
	double fSphi ; // Starging azimuthal angle around z axis
	double fDphi ; // Extension of azimuthal angle

}; // FRTorus

inline void FRTorus::setGeometry(	double rmin, 
					double rmax, 
					double rtor, 
					double sphi, 
					double dphi  ) 
{
	fRmin = rmin ;
	fRmax = rmax ; 
	fRtor = rtor ; 
	fSphi = sphi ; 
	fDphi = dphi ;
}

inline FRTorus::FRTorus()
{
	setGeometry(0, 0, 0, 0, 0 );
}

inline FRTorus::FRTorus(	double rmin, 
				double rmax, 
				double rtor, 
				double sphi, 
				double dphi  ) 
{
	setGeometry( rmin, rmax, rtor, sphi, dphi ) ;

}
inline FRTorus::FRTorus(const FRTorus& torus)
{
	setGeometry(	torus.fRmin, 
			torus.fRmax, 
			torus.fRtor, 
			torus.fSphi, 
			torus.fDphi  );
}

#endif
