//////////////////////////////
//////FRParallelepiped.h /////
//////////////////////////////

#if !defined FR_PARALLELEPIPED_H
#define FR_PARALLELEPIPED_H

class FRParallelepiped
{
public:
	 FRParallelepiped();
	 FRParallelepiped(double pDx, double pDy, double pDz,
				 	double pTalpha, double pTthetaCphi, double pTthetaSphi);
	 FRParallelepiped(const FRParallelepiped& para);
	~FRParallelepiped() { } 

	void setGeometry(double, double, double, double, double, double);

	double getDx() const { return fDx; }
	double getDy() const { return fDy; }
	double getDz() const { return fDz; }
	double getTalpha()	 { return fTalpha; }
	double getTthetaCphi()	 { return fTthetaCphi; }
	double getTthetaSphi()	 { return fTthetaSphi; }

protected:
	double fDx, fDy;		//	half lengths of edges
	double fDz;				//	half height
	double fTalpha,	fTthetaCphi, fTthetaSphi;
};

inline void FRParallelepiped::setGeometry(double pDx, double pDy, double pDz,
		double pTalpha, double pTthetaCphi, double pTthetaSphi)
{
	fDx = pDx; fDy = pDy; fDz = pDz;
	fTalpha = pTalpha; fTthetaCphi = pTthetaCphi; fTthetaSphi = pTthetaSphi;
}
inline FRParallelepiped::FRParallelepiped(double pDx, double pDy, double pDz,
		double pTalpha, double pTthetaCphi, double pTthetaSphi)
{
	setGeometry(pDx, pDy, pDz, pTalpha, pTthetaCphi, pTthetaSphi);
}
inline FRParallelepiped::FRParallelepiped()
{
	setGeometry(0, 0, 0, 0, 0, 0);
}
inline FRParallelepiped::FRParallelepiped(const FRParallelepiped& para)
{
	setGeometry(para.fDx, para.fDy, para.fDz,
			para.fTalpha, para.fTthetaCphi, para.fTthetaSphi);
}

#endif
