///////////////////////////////
///// FRPolylineForBrep.h /////
///////////////////////////////

#if !defined FRPOLYLINE_FOR_BREP_H
#define FRPOLYLINE_FOR_BREP_H

#include "FRBrepSolid.h"
#include "FRPolyline.h"

// #define DEFINE_POLYLINE_FOR_BREP

class FRPolylineForBrep : public FRPolyline, public FRBrepSolid
{
public:
	 FRPolylineForBrep() : FRPolyline() { ; } 
	 FRPolylineForBrep( FRVector3* P , int numP )     { FRPolyline::Set( P, numP ) ; }
	 FRPolylineForBrep( const FRPolyline& polyline ): FRPolyline(polyline) { ; }
	 ~FRPolylineForBrep() { ; }

		//----- Generation of B-Rep data
	void	GenerateBrepData( FRBrepGenerator* ) const ;

}; // FRPolylineForBrep

#endif
