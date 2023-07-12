//////////////////////
///// FRSphere.h /////
//////////////////////

#if !defined FR_SPHERE_H
#define FR_SPHERE_H

class FRSphere
{
public:
	FRSphere();
	FRSphere(double R);
	FRSphere(const FRSphere& sphere);
	~FRSphere() { } 

	void setGeometry(double pR);

	double getR() const	{ return fR; }

protected:
	double fR; 			 // radius of sphere
};

inline void FRSphere::setGeometry(double pR)
{
	fR = pR;
}
inline FRSphere::FRSphere()
{
	setGeometry(0);
}
inline FRSphere::FRSphere(double pR)
{
	setGeometry(pR);
}
inline FRSphere::FRSphere(const FRSphere& sphere)
{
	setGeometry(sphere.fR);
}

#endif
