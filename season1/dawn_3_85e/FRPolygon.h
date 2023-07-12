///////////////////////
///// FRPolygon.h /////
///////////////////////

#if !defined FR_POLYGON_H
#define FR_POLYGON_H

#include <iostream.h>

class FRPolygon
{
public:
	FRPolygon();
	FRPolygon(double pSPhi, double pDPhi, int pNdiv, int pNum,
			const double* pDzPtr = NULL,
			const double* pRminPtr = NULL,
			const double* pRmaxPtr = NULL);
	FRPolygon(const FRPolygon& pgon);
	~FRPolygon() { } 

	void setGeometry(double sphi, double dphi, int div, int num,
					const double* zPtr, const double* minPtr, const double* maxPtr);
	
	double getSPhi()   const { return fSPhi;  }
	double getDPhi()   const { return fDPhi;  }	
	int getNdiv()	   const { return fNdiv;  }
	int getNum() 	   const { return fNum;   }	
	double getDz(int pos) const;
	double getRmin(int pos) const;
	double getRmax(int pos) const;

protected:
	double fSPhi;				// radian of start
	double fDPhi;				// radian of sweep
	int	fNdiv;					// number of division 
	int	fNum;					// number of z plane ( at least two )
	const double* fDzPtr;		// z plane 				Array pointer
	const double* fRminPtr;		// radius of inside 	Array pointer
	const double* fRmaxPtr;		// radius of outside	Array pointer
};

inline void FRPolygon::setGeometry(double sphi, double dphi, int div, int num,
	 	const double* zPtr, const double* minPtr, const double* maxPtr)
{
	fSPhi = sphi; fDPhi = dphi; fNdiv = div; fNum = num; 
	fDzPtr = zPtr; fRminPtr = minPtr; fRmaxPtr = maxPtr;
}
inline FRPolygon::FRPolygon()
{
	setGeometry(0, 0, 0, 0, NULL, NULL, NULL);
}
inline FRPolygon::FRPolygon(double sphi, double dphi, int div, int num,
		const double* zPtr, const double* minPtr, const double* maxPtr)
{
	setGeometry(sphi, dphi, div, num, zPtr, minPtr, maxPtr);
}
inline FRPolygon::FRPolygon(const FRPolygon& pgon)
{
	setGeometry(pgon.fSPhi, pgon.fDPhi, pgon.fNdiv, pgon.fNum,
			pgon.fDzPtr, pgon.fRminPtr, fRmaxPtr = pgon.fRmaxPtr);
}
inline double FRPolygon::getDz(int pos) const
{
	if(pos < 1 || pos > fNum)
	{
		cerr << "Can not get Dz !!" << endl;
		return 0;
	}
	else
		return fDzPtr[pos - 1];
}
inline double FRPolygon::getRmin(int pos) const
{
	if(pos < 1 || pos > fNum)
	{
		cerr << "Can not get Rmin !!" << endl;
		return 0;
	}
	else
		return fRminPtr[pos - 1];
}
inline double FRPolygon::getRmax(int pos) const
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
