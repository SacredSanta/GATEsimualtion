////////////////////////////
///// FRBrepSolid.h ///////
////////////////////////////

#if !defined FR_BREP_SOLID_H
#define FR_BREP_SOLID_H

class FRBrepGenerator;

class FRBrepSolid
{
public:
    FRBrepSolid() { }
	virtual	~FRBrepSolid() { }

		//----- generate Brep data
	virtual	void GenerateBrepData(FRBrepGenerator*) const = 0;

}; // FRBrepSolid

#endif
