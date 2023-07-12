////////////////////////////////
///// FRPolylineForBrep.cc /////
////////////////////////////////

#include "FRPolylineForBrep.h"
#include "FRBrepGenerator.h"



	//----- FRPolylineForBrep::GenerateBrepData()
void FRPolylineForBrep::GenerateBrepData( FRBrepGenerator* factory_ptr ) const
{
          //----- local working variables
	int		i ;  
	FRVector3	position_tmp ;

          //------------------------------//
          //----- Header Comment     -----//  
          //----- ObjectOrigin Block -----//  
          //----- BoundingBox BLOCK ------//  
          //------------------------------//
	factory_ptr->GenerateHeaderInformation();

          //------------------------//
          //----- VERTEX BLOCK -----//  
          //------------------------//
	factory_ptr->GenerateVertexBlockHeader ();
	factory_ptr->GenerateDummyVertex       ();
	for ( i = 0; i < fNumVertex; i++ ) {
        		//----- vertex block
		int	vertex_label = i + 1 ;	// fVertex[0]  is the 1st vertex
						// fVertex[fNumVertex-1] is the fNumVertex-th vertex
		position_tmp = fVertex[i] ;
		factory_ptr->GenerateVertex ( vertex_label, position_tmp );		
	}
	factory_ptr->GenerateVertexBlockTailer ();
	factory_ptr->GenerateBlankLine ();

          //---------------------------//
          //----- ATTRIBUTE BLOCK -----//
          //---------------------------//
	factory_ptr->GenerateAttributeBlock() ;

          //------------------------//
          //----- FACET BLOCK ------//
          //------------------------//
	factory_ptr->GenerateFacetBlockHeader ();
	for ( i = 1; i < fNumVertex  ; i++ ) {
		int begin_label = i     ; 
		int end_label   = i + 1 ; 
		factory_ptr->GenerateLineseg( begin_label , end_label, 0 ) ;
	}
	factory_ptr->GenerateFacetBlockTailer ();
	
	factory_ptr->GenerateOptimizedData();

} // FRBoxForBrep::GenerateBrepData()
