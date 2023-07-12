///////////////////////////////
///// FRBrepGenerator.cc  /////
///////////////////////////////

#include <string.h>
#include "FRBrepGenerator.h"
#include "FRMath.h"

const	char	FR_BREP_HEADER[] = "%dawn data format version-02" ;
const	int	PRECISION = 9 ;

	//----- FRBrepGenerator::debug() 
void	FRBrepGenerator::debug() 
{
	cerr.precision(PRECISION) ;
	cerr << "PI  =     " << (GetPI())  << endl;
	cerr << "HEADER = " << FR_brep_header << endl;
	cerr << "Ndiv   = " << GetNdiv() << endl;
}

	//----- FRBrepGenerator::FRBrepGenerator()
FRBrepGenerator::FRBrepGenerator( ostream& out_given ) :
	FR_PI(FR_D_PI), 
	origin(), 
	e1(1.0, 0.0, 0.0), 
	e2(0.0, 1.0, 0.0), 
	e3(0.0, 0.0, 1.0), 
	boundingBoxMin() ,
	boundingBoxMax() ,
	att_p(NULL), out(out_given)
{ 
	strcpy( FR_brep_header,  FR_BREP_HEADER ) ;
	out.precision( PRECISION ) ;
}

	//-----
void	FRBrepGenerator::clear()
{
	origin( 0.0, 0.0, 0.0 )       ; 
	e1(1.0, 0.0, 0.0)             ; 
	e2(0.0, 1.0, 0.0)             ;
	e3(0.0, 0.0, 1.0)             ;
	boundingBoxMin(0.0, 0.0, 0.0) ;
	boundingBoxMax(0.0, 0.0, 0.0) ; 
	att_p =    NULL               ;
}

	//----- FRBrepGenerator::SetBaseVector ()
void	FRBrepGenerator::SetBaseVector (	const FRVector3& P1, 
						const FRVector3& P2 ) 
{ 
	e1 = P1.normal() ;
	e2 = P2.normal() ;
	e3 = e1^e2 ; e3.normalize() ;
}
	//----- FRBrepGenerator::GenerateHeaderInformation()
void	FRBrepGenerator::GenerateHeaderInformation()
{
	this->GenerateHeaderComment();
	this->GenerateCoordBlock();
	this->GenerateBoundingBoxBlock();
}
	//----- FRBrepGenerator::GenerateHeaderComment()
void	FRBrepGenerator::GenerateHeaderComment()
{
	out << FR_brep_header << endl;
	out << endl;
}

	//----- FRBrepGenerator::GenerateCoordBlock()
void	FRBrepGenerator::GenerateCoordBlock()
{
	out << "ObjectOrigin:"    << endl;
	out << "0 0 0"            << endl;      
	out << "end_ObjectOrigin" << endl;
	out << endl;
}

	//----- FRBrepGenerator::GenerateBoundingBoxBlock()
void	FRBrepGenerator::GenerateBoundingBoxBlock()
{
	out << "BoundingBox:" << endl;
	out << boundingBoxMin.x() << " " << boundingBoxMin.y() << " " << boundingBoxMin.z() << endl;
	out << boundingBoxMax.x() << " " << boundingBoxMax.y() << " " << boundingBoxMax.z() << endl;
	out << "end_BoundingBox" << endl;
	out << endl;

} // FRBrepGenerator::GenerateBoundingBoxBlock()


	//----- FRBrepGenerator::GenerateAttributeBlock() 
void FRBrepGenerator::GenerateAttributeBlock() 
{
		//----- local
	Attribute	att_default ;
	Attribute*	att_p_local;
	if( !att_p ) {
		att_p_local = &att_default ;
	} else {
		att_p_local = att_p ;
	}

	ColorType	kd;	kd = att_p_local->getKd();
	ColorType	ks;	ks = att_p_local->getKs();
	int		phong_power  = att_p_local->getPhongPower();
	int		transparency = att_p_local->getTransparency();
	int		visibility   = att_p_local->getVisibility  ();

		//----- beginning of the block
	out << "Attribute:"                       << endl;

		//----- Attribute 0
	out << "define 0"                         << endl;
	out << "     diffuse   rgb  " ;
	out << 	kd.getRed() << " " << kd.getGreen() << " " << kd.getBlue() << endl;
	out << "     specular  rgb  " ;
	out << 	ks.getRed() << " " << ks.getGreen() << " " << ks.getBlue() << endl;
	out << "     phong_power    "      << phong_power  << endl;  
	if( transparency ) {
		out << "     transparency   on "      << endl;
	} else {
		out << "     transparency   off "     << endl;
	}
	if( visibility ) {
		out << "     visibility     on "      << endl;
	} else {
		out << "     visibility     off "     << endl;
	}
	out << "enddef"                           << endl;

		//----- ending of the block
	out << "end_Attribute"                    << endl;
	out << endl;       

} // FRBrepGenerator::GenerateAttributeBlock() 

	//----- FRBrepGenerator::GenerateVertex( label , x, y, z )// transformation
void	FRBrepGenerator::GenerateVertex( int label, double x, double y, double z ) 
{
	FRVector3  World = origin + (x * e1) + (y * e2) + (z * e3);

	out << label << " " << World.x() << " " << World.y() << " " << World.z() << endl;
}

	//----- FRBrepGenerator::GenerateVertex( label , Body ) //transformation
void	FRBrepGenerator::GenerateVertex( int label, const FRVector3& Body ) 
{
	FRVector3 World = origin + (Body.x() * e1) + (Body.y() * e2) + (Body.z() * e3);

	out << label << " " << World.x() << " " << World.y() << " " << World.z() << endl;
}

	//----- FRBrepGenerator::GenerateFacet ()
void	FRBrepGenerator::GenerateFacet ( int* vertex, int num_vertex, int att_label ) 
{
	int i;
	
	for( i = 0 ; i <  num_vertex ; i++ ){
		out << vertex[i] << " ";
	}
	out << "# " << att_label << " ;" << endl;
}

	//----- FRBrepGenerator::GenerateLineseg ()
void	FRBrepGenerator::GenerateLineseg ( int v0, int v1, int att_label ) 
{
	out << v0 << " ";
	out << v1 << " ";
	out << "# " << att_label << " ;" << endl;
}

	//----- FRBrepGenerator::GenerateTriangle ()
void	FRBrepGenerator::GenerateTriangle ( int v0, int v1, int v2,  int att_label ) 
{
	out << v0 << " ";
	out << v1 << " ";
	out << v2 << " ";
	out << "# " << att_label << " ;" << endl;
}

	//----- FRBrepGenerator::GenerateSquare ()
void	FRBrepGenerator::GenerateSquare   ( int v0, int v1, int v2, int v3, int att_label ) 
{
	out << v0 << " ";
	out << v1 << " ";
	out << v2 << " ";
	out << v3 << " ";	
	out << "# " << att_label << " ;" << endl;
}

	//----- FRBrepGenerator::GenerateBrepData()
void FRBrepGenerator::GenerateBrepData( const FRBrepSolid& breps ) 
{ 
	breps.GenerateBrepData( this ) ;
}

