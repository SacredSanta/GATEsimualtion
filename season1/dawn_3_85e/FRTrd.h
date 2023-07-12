/////////////////////////
//////FRTrd.h /////
/////////////////////////

#if !defined FR_TRD_H
#define FR_TRD_H

class FRTrd
{
public:
	FRTrd();
	FRTrd(double pdx1, double pdx2, double pdy1, double pdy2, double pdz);
	FRTrd(const FRTrd& trd);
	~FRTrd() { } 

	void setGeometry(double, double, double, double, double);

	double getDx1() const { return fDx1; }		// half length x edge at bottom
	double getDx2() const { return fDx2; }		// half length x edge at top
	double getDy1() const { return fDy1; }		// half length y edge at bottom
	double getDy2() const { return fDy2; }		// half length y edge at top 	
	double getDz()  const { return fDz; }		// half height

protected:
	double fDx1, fDx2, fDy1, fDy2, fDz;		 	// half lengths of edges
};

inline void FRTrd::setGeometry(double x1, double x2, double y1, double y2, double z)
{
	fDx1 = x1; fDx2 = x2; fDy1 = y1; fDy2 = y2; fDz = z;
}
inline FRTrd::FRTrd()
{
	setGeometry(0, 0, 0, 0, 0);
}
inline FRTrd::FRTrd(double x1, double x2, double y1, double y2,	double z)
{
	setGeometry(x1, x2, y1, y2, z);
}
inline FRTrd::FRTrd(const FRTrd& trd)
{
	setGeometry(trd.fDx1, trd.fDx2, trd.fDy1, trd.fDy2, trd.fDz);
}

#endif
