/////////////////////////////////
///// FRSphereSegForBrep.cc /////
/////////////////////////////////

#include "FRSphereSegForBrep.h"
#include "FRBrepGenerator.h"
#include "FRTrigonometric.h"

void FRSphereSegForBrep::GenerateBrepData(FRBrepGenerator* factory_ptr) const
{
	factory_ptr->GenerateHeaderInformation();
	factory_ptr->GenerateVertexBlockHeader();
	factory_ptr->GenerateDummyVertex();

	const double  R    	 =  fR ;			// radius of sphere outside
	const double  r   	 =  fr ;			// radius of sphere inside
	const double  phi  	 =  fPhi;			// azimuthal start
	const double  dphi 	 =  fdPhi;    		// azimuthal sweep
	const double  theta	 =  fTheta;			// polar start
	const double  dtheta =  fdTheta;   		// polar sweep
	const int     Ndiv 	 = factory_ptr->GetNdiv();
			
	double		XX, YY ,ZZ;			// X-Y-Z coord 
	double 		dR, dr; 			// radius  

	const FRTrigonometricTable ard(Ndiv);
	const FRVector2*	azimuthal_temp;
	const FRVector2*	polar_temp;
	FRSegmentTable azimuthal(ard, phi, dphi);
	FRSegmentTable polar(ard, theta, dtheta);

	int	i, j, k;
	const int Na = azimuthal.GetSize();
	const int Np = polar.GetSize();

	azimuthal_temp = azimuthal.GetTable();
	polar_temp = polar.GetTable();

	for(k = 0, dr = R; k < 2; k++, dr = r)
	{
		for(j = 0; j < Np; j++)
		{
			dR 	  = dr * polar_temp[j].y();
			ZZ    = dr * polar_temp[j].x();
				
			for(i = 0; i < Na; i++)
			{	
				XX    = dR * azimuthal_temp[i].x();
				YY    = dR * azimuthal_temp[i].y();

				int vertex_number = i + 1 + Na * j + k * Na * Np;
				factory_ptr->GenerateVertex(vertex_number, FRVector3(XX, YY, ZZ));
			}
		}
	}
	factory_ptr->GenerateVertexBlockTailer();
	factory_ptr->GenerateAttributeBlock();
	factory_ptr->GenerateFacetBlockHeader();

	int a, b, c, d;

	for(j = 0; j < Np - 1; j++)
	{
	    for(i = 0; i < Na-1; i++)
		{
			a = Na * j       + i + 2;
		    b = Na * j       + i + 1;
		    c = Na * (j + 1) + i + 1;
		    d = Na * (j + 1) + i + 2;
			factory_ptr->GenerateSquare(a, b, c, d, 0);
		}
	}
	for(j = 0; j < Np - 1; j++)
	{
	    for(i = 0; i < Na - 1; i++)
		{
			a = Na * j       + i + 2 + Np * Na;
		    b = Na * j       + i + 1 + Np * Na;
		    c = Na * (j + 1) + i + 1 + Np * Na;
		    d = Na * (j + 1) + i + 2 + Np * Na;
			factory_ptr->GenerateSquare(d, c, b, a, 0);
		}
	}
	for(i = 0; i < Np - 1; i++)
	{
		a = Na * i	 + 1;
		b = Na * (i + 1) + 1;
		c = Na * Np + Na * (i + 1) + 1;
		d = Na * Np + Na * i + 1;
		factory_ptr->GenerateSquare(-d, c, -b, a, 0);		
	}
	for(i = 0; i < Np - 1; i++)
	{
		a = Na * (i + 1);
		b = Na * (i + 2);
		c = Na * Np + Na * (i + 2);
		d = Na * Np + Na * (i + 1);
		factory_ptr->GenerateSquare(-a, b, -c, d, 0);		
	}
	for(i = 0; i < Na - 1; i++)
	{
		a = i + 2;
		b = i + 1;
		c = Na * Np + i + 1;
		d = Na * Np + i + 2;
		factory_ptr->GenerateSquare(d, c, b, a, 0);		
	}
	for(i = 0; i < Na - 1; i++)
	{
		a = (Np - 1) * Na + i + 2;
		b = (Np - 1) * Na + i + 1;
		c = Na * Np + (Np - 1) * Na + i + 1;
		d = Na * Np + (Np - 1) * Na + i + 2;
		factory_ptr->GenerateSquare(a, b, c, d, 0);		
	}
	factory_ptr->GenerateFacetBlockTailer();	

	factory_ptr->GenerateOptimizedData();

} // FRSphereSegForBrep::GenerateBrepData()
