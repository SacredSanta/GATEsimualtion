///////////////////////////////
///// FRPolygonForBrep.cc /////
///////////////////////////////

#include <math.h>
#include "FRPolygonForBrep.h"
#include "FRBrepGenerator.h"
#include "FRTrigonometric.h"
#include "FRMath.h"


const double epsilon_radius = 0.0;
const double epsilon_radian = 0.0001;

void FRPolygonForBrep::GenerateBrepData(FRBrepGenerator* factory_ptr) const
{
	factory_ptr->GenerateHeaderInformation();
	factory_ptr->GenerateVertexBlockHeader();
	factory_ptr->GenerateDummyVertex();
	
	const double dphi = fDPhi / fNdiv;	//  radian of (sweep/number of division) 

	int	i, j, N = fNdiv + 1;		//---- number of division (sweep)

	if(fDPhi == 2 * FR_D_PI)
		N = fNdiv;

	for(j = 0; j < fNum; j++)
	{
		for(i = 0; i < N; i++)
		{
			double XX, YY;
			FRVector3 temp;	

				// rotate around z-axes
			XX = cos(fSPhi + (double)i * dphi);		//	normal vector.x()
			YY = sin(fSPhi + (double)i * dphi);		//	normal vector.y()
	
				//----- vertex on z plane (inside)
			temp = FRVector3(fRminPtr[j] * XX, fRminPtr[j] * YY, fDzPtr[j]);
			factory_ptr->GenerateVertex(2 * N * j + i + 1, temp);

				//----- vertex on z plane (outside)
			temp = FRVector3(fRmaxPtr[j] * XX, fRmaxPtr[j] * YY, fDzPtr[j]);
			factory_ptr->GenerateVertex(2 * N * j + N + i + 1, temp);
		}
	}
	factory_ptr->GenerateVertexBlockTailer();
	factory_ptr->GenerateBlankLine();
	factory_ptr->GenerateAttributeBlock() ;
	factory_ptr->GenerateFacetBlockHeader ();	

	const int planeN = fNum - 1;		//	number of plane ( z_1 -> z_n )
	const int aN = 2 * N * (fNum - 2);	// all vertices except for z(n) plane
	const int pN = 2 * N;			// number of vertices on one z plane 

	if(fabs(fDPhi - 2 * FR_D_PI ) <= epsilon_radian)	// Exception (Sweep == 2 * FR_D_PI)
	{
		for(i = 0; i < N - 1; i++)
		{
			for(j = 0; j < planeN; j++)
			{
				factory_ptr->GenerateSquare(i+2+pN*j, i+1+pN*j, 2*N+i+1+pN*j, 2*N+i+2+pN*j, 0);	
				factory_ptr->GenerateSquare(N+i+1+pN*j, N+i+2+pN*j, 3*N+i+2+pN*j, 3*N+i+1+pN*j, 0);
			}
			factory_ptr->GenerateSquare(-(i+1), i+2, -(N+i+2), N+i+1, 0);	
			factory_ptr->GenerateSquare(-(3*N+aN+i+1), 3*N+aN+i+2, -(2*N+aN+i+2), 2*N+aN+i+1, 0);
		}
		for(j = 0; j < planeN; j++)
		{
			factory_ptr->GenerateSquare(1+pN*j, N+pN*j, 3*N+pN*j, 2*N+1+pN*j, 0);	
			factory_ptr->GenerateSquare(2*N+pN*j, N+1+pN*j, 3*N+1+pN*j, 4*N+pN*j, 0);
		}
		factory_ptr->GenerateSquare(-(N + 1), 2 * N, -N, 1, 0);	
		factory_ptr->GenerateSquare(-(4*N+aN), 3*N+1+aN, -(2*N+1+aN), 3*N+aN, 0);
	}
	else
	{
		for(i = 0; i < planeN; i++)
		{
			int a = pN * i + 1;
			int b = pN * i + N + 1;
			int c = pN * i + 3 * N + 1;
			int d = pN * i + 2 * N + 1;
			int e = pN * i + N;
			int f = pN * i + 3 * N;			
			int g = pN * i + 4 * N;
			int h = pN * i + 2 * N;

			if(planeN == 1)
			{
				factory_ptr->GenerateSquare(a, b, c, d, 0);
				factory_ptr->GenerateSquare(e, f, g, h, 0);
			}
			else
			{
				if(i == 0)
				{
					factory_ptr->GenerateSquare(a, b, c, -d, 0);
					factory_ptr->GenerateSquare(e, f, -g, h, 0);
				}
				else if(i == planeN - 1)
				{
					factory_ptr->GenerateSquare(a, -b, c, d, 0);
					factory_ptr->GenerateSquare(-e, f, g, h, 0);
				}
				else
				{
					factory_ptr->GenerateSquare(a, -b, c, -d, 0);
					factory_ptr->GenerateSquare(-e, f, -g, h, 0);
				}
			}
		}
		for(i = 0; i < N - 1; i++)
		{
			for(j = 0; j < planeN; j++)
			{
				factory_ptr->GenerateSquare(i+2+pN*j, i+1+pN*j, 2*N+i+1+pN*j, 2*N+i+2+pN*j, 0);	
				factory_ptr->GenerateSquare(N+i+1+pN*j, N+i+2+pN*j, 3*N+i+2+pN*j, 3*N+i+1+pN*j, 0);
			}
		}
		for(i = 0; i < N - 1; i++)
		{
			int a = i + 1;
			int b = i + 2;
			int c = N + i + 2;
			int d = N + i + 1;
			int e = 3 * N + aN + i + 1;
			int f = 3 * N + aN + i + 2;
			int g = 2 * N + aN + i + 2;
			int h = 2 * N + aN + i + 1;

			if(N == 2)				//  fNdiv == 1
			{
				factory_ptr->GenerateSquare(a, b, c, d, 0);	
				factory_ptr->GenerateSquare(e, f, g, h, 0);
			}
			else if(i == 0 && N != 2)
			{
				factory_ptr->GenerateSquare(a, b, -c, d, 0);	
				factory_ptr->GenerateSquare(e, f, -g, h, 0);	
			}
			else if(i == N - 2 && N != 2)
			{
				factory_ptr->GenerateSquare(-a, b, c, d, 0);	
				factory_ptr->GenerateSquare(-e, f, g, h, 0);
			}
			else
			{
				factory_ptr->GenerateSquare(-a, b, -c, d, 0);	
				factory_ptr->GenerateSquare(-e, f, -g, h, 0);
			}
		}
	}
	factory_ptr->GenerateFacetBlockTailer();

	factory_ptr->GenerateOptimizedData();

} // FRPolygonForBrep::GenerateBrepData()
