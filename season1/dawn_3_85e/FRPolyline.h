////////////////////////
///// FRPolyline.h /////
////////////////////////

#if !defined FRPOLYLINE_H
#define FRPOLYLINE_H

#include <iostream.h>
#include "FRVector3.h"

// #define DEFINE_POLYLINE

	//----------------------------//
	//----- class FRPolyline -----//
	//----------------------------//
class FRPolyline {

 public:

		//----- access functions (set)
	int	Set( const FRVector3* P , int numP ) 
		{ 
			delete [] fVertex ; // clear old data
			fNumVertex = numP ;  fVertex = new FRVector3 [ fNumVertex ];
			for( int i = 0 ; i < fNumVertex ; i++ ) { fVertex [i] = P[i] ;}
			return 1 ; // dummy for HP
		}
	int	clear() 
		{ 
			delete [] fVertex ;  fVertex = NULL; fNumVertex = 0 ; 
			return 1 ; // dummy for HP
		}

		//----- constructors and destructor
	FRPolyline () : fNumVertex(0), fVertex(NULL){;}
	FRPolyline ( const FRVector3* P , int numP ) { this->Set( P, numP ) ; }
	FRPolyline ( const FRPolyline& rhs )         { this->Set( rhs.fVertex, rhs.fNumVertex ) ; }
	virtual ~FRPolyline() { this->clear() ; } 

		//----- access functions (get)
	int		GetNumVetex ( void      ) const { return fNumVertex ;}
	FRVector3 	GetVetex    ( int index ) const 
	{ 
		if( index < 0 || index >= fNumVertex )
		{
			cerr << "ERROR(FRPolyline::GetVertex()): irreagal index " ;
			cerr << index << endl;
		}
		return fVertex[index]; 
	}

 protected:
	int		fNumVertex     ;
	FRVector3*	fVertex        ;

} ; // FRPolyline


#endif
