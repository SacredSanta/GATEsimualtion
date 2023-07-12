////////////////////
//////FRTrap.h /////
////////////////////

#if !defined FR_TRAP_H
#define FR_TRAP_H

class FRTrap
{
public:
	FRTrap();
	FRTrap(double dz, double theta, double phi,
		double h1, double bl1, double tl1, double alp1,
		double h2, double bl2, double tl2, double alp2);
	FRTrap(const FRTrap& trap);
	~FRTrap() { } 

	void setGeometry(double, double, double, double, double, double,
				double, double, double, double, double);

	double getDz()		const { return fDz; }
	double getTheta()	const { return fTheta; }
	double getPhi()		const { return fPhi; }
	double getH1()		const { return fH1; }
	double getBL1()		const { return fBL1; }
	double getTL1()		const { return fTL1; }
	double getAlpha1()	const { return fAlpha1; }
	double getH2()		const { return fH2; }
	double getBL2()		const { return fBL2; }
	double getTL2()		const { return fTL2; }
	double getAlpha2()	const { return fAlpha2; }

protected:
	double fDz, fTheta, fPhi;
	double fH1, fBL1, fTL1, fAlpha1;
	double fH2, fBL2, fTL2, fAlpha2; 
};

inline void FRTrap::setGeometry(double dz, double theta, double phi,
				double h1, double bl1, double tl1, double alp1,
				double h2, double bl2, double tl2, double alp2)
{
	fDz = dz; fTheta = theta; fPhi = phi;
	fH1 = h1; fBL1 = bl1; fTL1 = tl1; fAlpha1 = alp1;
	fH2 = h2; fBL2 = bl2; fTL2 = tl2; fAlpha2 = alp2;
}
inline FRTrap::FRTrap()
{
	setGeometry(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
inline FRTrap::FRTrap(double dz, double theta, double phi,
				double h1, double bl1, double tl1, double alp1,
				double h2, double bl2, double tl2, double alp2)
{
	setGeometry(dz, theta, phi, h1, bl1, tl1, alp1, h2, bl2, tl2, alp2); 
}
inline FRTrap::FRTrap(const FRTrap& trap)
{
	setGeometry(trap.fDz, trap.fTheta, trap.fPhi,
			trap.fH1, trap.fBL1, trap.fTL1, trap.fAlpha1,
			trap.fH2, trap.fBL2, trap.fTL2, trap.fAlpha2);
}

#endif
