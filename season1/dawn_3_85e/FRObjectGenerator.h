//////////////////////////////////
/////  FRObjectGenerator.h   /////
//////////////////////////////////

#if !defined FR_OBJECT_GENERATOR_H
#define FR_OBJECT_GENERATOR_H

#include "FRBrepGenerator.h"
class Object ;

	//-----------------------------------//
	//----- class FRObjectGenerator -----//
	//-----------------------------------//
class FRObjectGenerator : public FRBrepGenerator {
 public:
	FRObjectGenerator( ostream& out_given )\
	: FRBrepGenerator( out_given ), object_p(NULL), winged_edge_mode(1) { ; }

	FRObjectGenerator( void )\
	: FRBrepGenerator(), object_p(NULL), winged_edge_mode(1) { ; }

	virtual ~FRObjectGenerator(){;} 
	void	setObject        ( Object* obj_p )  { object_p = obj_p         ; }
	void	setWingedEdgeMode( int mode = 1  )  { winged_edge_mode = mode  ; }
	
		//----- generate a block of brep data
	virtual void	GenerateHeaderInformation ()  ;         // 1 + 2 + 3
	virtual void	GenerateHeaderComment     (){ ; }       // 1
	virtual void	GenerateCoordBlock        ()  ;         // 2
	virtual void	GenerateBoundingBoxBlock  (){ ; }       // 3
	virtual void	GenerateAttributeBlock    () ;

		//----- generate a line in brep data
	virtual void	GenerateVertexBlockHeader () { ; }
	virtual void	GenerateDummyVertex()	       ;
	virtual void	GenerateVertexBlockTailer () { ; }
	virtual void	GenerateFacetBlockHeader  () { ; }
	virtual void	GenerateFacetBlockTailer  () { ; }
	virtual void    GenerateBlankLine()          { ; }

	virtual void	GenerateVertex( int label, double x, double y, double z ) ;
	virtual void	GenerateVertex( int label, const FRVector3& P ) ;
	virtual void	GenerateFacet    ( int* vertex, int num_vertex, int dummy ) ;
	virtual void	GenerateSquare   ( int v0, int v1, int v2, int v3, int dummy ) ;
	virtual void	GenerateTriangle ( int v0, int v1, int v2,         int dummy ) ;
	virtual void	GenerateLineseg  ( int v0, int v1,                 int att_label ) ; 
			 // call base-class function

	virtual void	GenerateOptimizedData () ; // objectP->install()

	Object*		GetObject() { return object_p ; }

 private:
	Object*	object_p          ;
	int	winged_edge_mode  ; 

}; // FRObjectGenerator

#endif
