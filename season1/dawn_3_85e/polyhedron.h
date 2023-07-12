//////////////////////////////
/////// Polyhedron.h ///////
//////////////////////////////
#if !defined FR_POLYHEDRON_H
#define      FR_POLYHEDRON_H

#include <iostream.h>
#include "FRString.h"
#include "FRObjectGenerator.h"
#include "FRBrepSolid.h"
#include "list.h"



class Polyhedron : public FRBrepSolid {

 public:

	static void GenerateOneVertex(	int               label       ,
					const FRString&   string_data ,
					FRBrepGenerator*  factory_prt   ) ;

	static void GenerateOneVertex(	const FRString&   string_data ,
					FRBrepGenerator*  factory_prt   ) ;

	static void GenerateOneFacet(	const FRString&   string_data ,
					FRBrepGenerator* factory_prt    ) ;

}; // Polyhedron 



#endif
