//////////////////////
/////// FRBox.h //////
//////////////////////
#if !defined FR_BOX_H
#define FR_BOX_H

class FRBox
{
public:
	FRBox();
	FRBox(double pdx, double pdy, double pdz);
	FRBox(const FRBox& box);
	~FRBox() { } 

	void setGeometry(double, double, double);

	double getDx() const { return fDx; }
	double getDy() const { return fDy; }
	double getDz() const { return fDz; }

protected:
	double fDx, fDy, fDz;
};

inline void FRBox::setGeometry(double pdx, double pdy, double pdz)
{
	fDx = pdx; fDy = pdy; fDz = pdz;
}
inline FRBox::FRBox()
{
	setGeometry(0, 0, 0);
}
inline FRBox::FRBox(double pdx, double pdy, double pdz)
{
	setGeometry(pdx, pdy, pdz);
}
inline FRBox::FRBox(const FRBox& box)
{
	setGeometry(box.fDx, box.fDy, box.fDz);
}

#endif
