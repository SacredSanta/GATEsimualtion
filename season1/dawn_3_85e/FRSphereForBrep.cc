//////////////////////////////
///// FRSphereForBrep.cc /////
//////////////////////////////

#include "FRSphereForBrep.h"
#include "FRBrepGenerator.h"
#include "FRTrigonometric.h"

void FRSphereForBrep::GenerateBrepData(FRBrepGenerator* factory_ptr) const
{
	const double FR_PI = factory_ptr->GetPI();	  
	const int Ndiv  = factory_ptr->GetNdiv();	  
	const FRTrigonometricTable trigon(Ndiv);
	const FRSegmentTable segment_azimuthal(trigon);
	const FRSegmentTable segment_polar(trigon, 0.0, FR_PI);
	const FRVector2* azimuthal_table = segment_azimuthal.GetTable();	
	const FRVector2* polar_table 	 = segment_polar.GetTable();	
	const int aN = segment_azimuthal.GetSize();	
	const int pN = segment_polar.GetSize();	

	int	i, j;
	double XX, YY, ZZ;				// X-Y-Z coord 
	double dR; 						// radius on X-Y plane 
	FRVector3 temp;

	factory_ptr->GenerateHeaderInformation();
	factory_ptr->GenerateVertexBlockHeader();
	factory_ptr->GenerateDummyVertex();

		//----- first vertex is only one
	dR = fR * polar_table[0].y();
	XX = dR * azimuthal_table[0].x();
	YY = dR * azimuthal_table[0].y();
	ZZ = fR * polar_table[0].x();

		//----- vertex on plane
	temp = FRVector3(XX, YY, ZZ);
	factory_ptr->GenerateVertex(1, temp);

	for(j = 1; j < pN - 1; j++)
	{
			//----- theta, dR, Z
		dR = fR * polar_table[j].y();
		ZZ = fR * polar_table[j].x();
			
		for(i = 0; i < aN; i++)
		{
				//----- phi, X, Y
			XX = dR * azimuthal_table[i].x();
			YY = dR * azimuthal_table[i].y();

				//----- vertex on plane
			temp = FRVector3(XX, YY, ZZ);
			factory_ptr->GenerateVertex(i + 2 + aN * (j - 1), temp);
		}
	}
		//----- last vertex 
	dR = fR * polar_table[pN - 1].y();
	XX = dR * azimuthal_table[0].x();
	YY = dR * azimuthal_table[0].y();		
	ZZ = fR * polar_table[pN - 1].x();

		//----- vertex on plane
	temp = FRVector3(XX, YY, ZZ);
	factory_ptr->GenerateVertex( aN * (pN - 2) + 2, temp );

	factory_ptr->GenerateVertexBlockTailer();
	factory_ptr->GenerateAttributeBlock();
	factory_ptr->GenerateFacetBlockHeader();

	int a, b, c, d;

	for(i = 0; i < aN - 1; i++)
		factory_ptr->GenerateTriangle(1, i + 2, i + 3, 0);

	factory_ptr->GenerateTriangle(1, aN + 1, 2, 0);			

	for(j = 0; j < pN - 3; j++)
	{
		for(i = 0; i < aN - 1; i++)
		{
			a = aN * j       + i + 3;
		    b = aN * j       + i + 2;
		    c = aN * (j + 1) + i + 2;
		    d = aN * (j + 1) + i + 3;
			factory_ptr->GenerateSquare(a, b, c, d, 0);
		}
		a = aN * j       + 2;
		b = aN * j       + aN + 1;
		c = aN * (j + 1) + aN + 1;
		d = aN * (j + 1) + 2;
		factory_ptr->GenerateSquare(a, b, c, d, 0);			
	}
	for(i = 0; i < aN - 1; i++)
	{
		a = (pN - 2) * aN + 2;
		b = (pN - 3) * aN + i + 3;
		c = (pN - 3) * aN + i + 2;
		factory_ptr->GenerateTriangle(a, b, c, 0);
	}
	a = (pN - 2) * aN + 2;
	b = (pN - 3) * aN + 2;
	c = (pN - 2) * aN + 1;
	factory_ptr->GenerateTriangle(a, b, c, 0);

	factory_ptr->GenerateFacetBlockTailer();

	factory_ptr->GenerateOptimizedData();

}
