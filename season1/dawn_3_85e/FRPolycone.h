////////////////////////
///// FRPolycone.h /////
////////////////////////

#if !defined FR_POLYCONE_H
#define FR_POLYCONE_H

#include <iostream.h>

class FRPolycone
{
public:
	FRPolycone();
	FRPolycone(double pSPhi, double pDPhi, int pNum,
			const double* pDzPtr = NULL,
			const double* pRminPtr = NULL,
			const double* pRmaxPtr = NULL);
	FRPolycone(const FRPolycone& pcone);
	~FRPolycone() { } 

	void setGeometry(double sphi, double dphi, int num,
					const double* zPtr, const double* minPtr, const double* maxPtr);
	
	double getSPhi()   const { return fSPhi;  }
	double getDPhi()   const { return fDPhi;  }	
	double getNum()    const { return fNum;   }	
	double getDz(int pos) const;
	double getRmin(int pos) const;
	double getRmax(int pos) const;

protected:
	double fSPhi;				// radian of start
	double fDPhi;				// radian of sweep
	int fNum;	 				// number of z plane ( at least two )
	const double* fDzPtr;		// z plane 				Array pointer
	const double* fRminPtr;		// radius of inside 	Array pointer
	const double* fRmaxPtr;		// radius of outside	Array pointer
};

inline void FRPolycone::setGeometry(double sphi, double dphi, int num,
		const double* zPtr, const double* minPtr, const double* maxPtr)
{
	fSPhi = sphi; fDPhi = dphi; fNum = num; 
	fDzPtr = zPtr; fRminPtr = minPtr; fRmaxPtr = maxPtr;
}
inline FRPolycone::FRPolycone()
{
	setGeometry(0, 0, 0, NULL, NULL, NULL);
}
inline FRPolycone::FRPolycone(double sphi, double dphi, int num,
		const double* zPtr, const double* minPtr, const double* maxPtr)
{
	setGeometry(sphi, dphi, num, zPtr, minPtr, maxPtr);
}
inline FRPolycone::FRPolycone(const FRPolycone& pcon)
{
	setGeometry(pcon.fSPhi, pcon.fDPhi, pcon.fNum,
			pcon.fDzPtr, pcon.fRminPtr, fRmaxPtr = pcon.fRmaxPtr);
}
inline double FRPolycone::getDz(int pos) const
{
	if(pos < 1 || pos > fNum)
	{
		cerr << "Can not get Dz !!" << endl;
		return 0;
	}
	else
		return fDzPtr[pos - 1];
}
inline double FRPolycone::getRmin(int pos) const
{
	if(pos < 1 || pos > fNum)
	{
		cerr << "Can not get Rmin !!" << endl;
		return 0;
	}
	else
		return fRminPtr[pos - 1];
}
inline double FRPolycone::getRmax(int pos) const
{
	if(pos < 1 || pos > fNum)
	{
		cerr << "Can not get Rmax !!" << endl;
		return 0;
	}
	else
		return fRmaxPtr[pos - 1];
}

#endif
