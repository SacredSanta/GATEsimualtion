//////////////////////////////
///// FRBrepGenerator.h  /////
//////////////////////////////

#if !defined FR_BREP_GENERATOR_H
#define FR_BREP_GENERATOR_H

#include <fstream.h>
#include "attribute.h"
#include "FRBrepSolid.h"
#include "FRVector3.h"
#include "FRTrigonometric.h"

class FRBrepGenerator
{
protected:
		//----- const
	enum    { HEADER_MAX       = 64 };
	const	double FR_PI  ;

 public:
		//----- constructor and destructor
		FRBrepGenerator( ostream& out_given = cout ) ;   
	virtual ~FRBrepGenerator(){ this->clear(); } 
	void	clear();

		//----- access functions (set)
	void	SetOrigin( const FRVector3& P ) { origin = P ; }

	void	SetBaseVector (	const FRVector3& P1, 
				const FRVector3& P2, 
				const FRVector3& P3 )
			{ e1 = P1 ; e2 = P2 ,  e3 = P3 ; }
	void	SetBaseVector (	const FRVector3& P1, 
				const FRVector3& P2 ) ;

	void	SetBoundingBox( const FRVector3& box_min , 
				const FRVector3& box_max  )
		{ boundingBoxMin = box_min; boundingBoxMax = box_max; }
	void	SetAttribute  ( Attribute* att ) { att_p = att ; }
	void	SetNdiv       ( unsigned N ) { trigon.SetDiv(N); }

		//----- access functions (get)
	const FRVector3&  GetOrigin() const { return origin ; }
	const FRVector3&  GetE1    () const { return e1     ; }
	const FRVector3&  GetE2    () const { return e2     ; }
	const FRVector3&  GetE3    () const { return e3     ; }
	void	GetBoundingBox( FRVector3* box_min_p , FRVector3* box_max_p ) 
		{ *box_min_p = boundingBoxMin; *box_max_p = boundingBoxMax ;}
	const Attribute*  GetAttribute() const { return att_p ; }
	ostream&	GetOut        () const { return out   ; }
	unsigned	GetNdiv       () const { return trigon.GetDiv() ; }
	double		GetPI         () const { return FR_PI    ; }
	const FRTrigonometricTable& GetTrigonometricTable() const { return trigon; }
	
		//----- generate brep data
	virtual void	GenerateBrepData( const FRBrepSolid& breps ) ;

		//----- generate a block of brep data
	virtual void	GenerateHeaderInformation () ;		//	1 + 2 + 3
	virtual void	GenerateHeaderComment     () ;		//	1
	virtual void	GenerateCoordBlock        () ;		//	2
	virtual void	GenerateBoundingBoxBlock  () ;		//	3
	virtual void	GenerateAttributeBlock    () ;

		//----- generate a line in brep data
	virtual void	GenerateVertexBlockHeader () {  out << "Vertex:"    << endl; }
	virtual void	GenerateDummyVertex()		 {  out << "0 0 0 0" << endl; }
	virtual void	GenerateVertexBlockTailer () {  out << "end_Vertex" << endl << endl; }
	virtual void	GenerateFacetBlockHeader  () { 	out << "Facet:"     << endl; }     
	virtual void	GenerateFacetBlockTailer  () { 	out << "end_Facet"  << endl; }   
	virtual void    GenerateBlankLine() { out << endl; }

	virtual void	GenerateVertex( int label, double x, double y, double z ) ;
	virtual void	GenerateVertex( int label, const FRVector3& P ) ;
	virtual void	GenerateFacet   ( int* vertex, int num_vertex, int att_label ) ;
	virtual void	GenerateSquare  ( int v0, int v1, int v2, int v3, int att_label ) ;
	virtual void	GenerateTriangle( int v0, int v1, int v2,         int att_label ) ;
	virtual void	GenerateLineseg ( int v0, int v1,                 int att_label ) ;

	virtual void	GenerateOptimizedData () { ; }

		//----- imported from fHexaghedron.h
	virtual void GenerateHexahedronVertices(const FRArray<FRVector3>&);
	virtual void GenerateHexahedronFacets  ();

		//----- imported from fCylinder.h
	virtual void GenerateCylindricalVertices(const FRSegmentTable&,
						double pRmin1,			// bottom radius (inside)
						double pRmax1,			// bottom radius (outside)
						double pRmin2,			// top radius (inside)
						double pRmax2,			// top radius (outside)
						double pDz			);	// half height
					
	virtual void GenerateColumnFacets        ( int N );
	virtual void GenerateColumnSegFacets     ( int N );
	virtual void GenerateCylindricalFacets   ( int N );
	virtual void GenerateCylindricalSegFacets( int N );

		//----- debug
	virtual void	debug() ;

protected:
	char		FR_brep_header[ HEADER_MAX ];
	FRVector3		origin     ; 
	FRVector3		e1, e2, e3 ; 
	FRVector3		boundingBoxMin ;
	FRVector3		boundingBoxMax ;
	Attribute*	att_p      ; 
	ostream&	out	   ;
	FRTrigonometricTable trigon;
	
}; // FRBrepGenerator

#endif
