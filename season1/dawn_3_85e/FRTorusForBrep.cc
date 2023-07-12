/////////////////////////////
///// FRTorusForBrep.cc /////
/////////////////////////////

#include <math.h>	
#include "FRArray2.h"
#include "FRTorusForBrep.h"
#include "FRMath.h"

#define  FOREACH_ij   for( i = 0; i <= Ns; i++ ) { for( j = 0; j < N ;j++ ) { 
#define  FOREACH2_ij  for( i = 0; i <  Ns; i++ ) { for( j = 0; j < N ;j++ ) { 
#define  END_LOOP_ij  }}

const double epsilon_radius = 0.0;
const double epsilon_radian = 0.0001;

void FRTorusForBrep::GenerateBrepData(FRBrepGenerator* factory_ptr) const
{
	//----- local const
	const  double  PI_x_2     = 2.0 * FR_D_PI ;

	//----- local variable 
	int	vlabel = 0 ;

	double  r          = fRmin ;
	double  R          = fRmax ;
	double  t          = fRtor ;

	int flag_filled_torus = 0 ; 
	if( fRmin <= epsilon_radius ) {
		flag_filled_torus = 1 ; 
	} else {
		flag_filled_torus = 0 ; 
	}

	int flag_closed_torus = 0 ; 
	if( fDphi >  (PI_x_2 - epsilon_radian ) ) { flag_closed_torus = 1 ; }

			// Angle egment around line extending along 
			// the center of the curved tube	
	int     N          = factory_ptr->GetNdiv();
	double  theta_step = PI_x_2 / ((double) N  ) ;

			// Azimutal-angle segment around body-coordinate origin 
	int     Ns         = (int) ( ( (double) N ) * fDphi / PI_x_2 );
	if( flag_closed_torus ) { Ns = N ; }

	double  phi_0      = fSphi ;
	double  phi_step   = fDphi  / ((double) Ns ) ;
	if( flag_closed_torus ) { phi_step = theta_step ; }

	FRArray2 <int>  Q( (Ns + 1) , N );  // N+1: UEKI-ZAN (Japanese)
	FRArray2 <int>  q( (Ns + 1) , N );

	double* cos_phi   = new double [ Ns + 1 ];
	double* sin_phi   = new double [ Ns + 1 ];
	double* cos_theta = new double [ N  ];
	double* sin_theta = new double [ N  ];

	//----- local working variables 
	int	i, j , ip1, jp1 ;
	double	x, y, z ;
	int     v0, v1, v2, v3 ;
	double	phi_i   ;
	double	theta_j ;
	int*      label_on_circle = new int [ N ] ;

	//----- calc sin_cos_table
	//      FOREACH_ij:  0 < i <= Ns,  0 < j < N  
	FOREACH_ij
		phi_i   = phi_0 + ((double)i) * phi_step ;
		theta_j =         ((double)j) * theta_step ;

		cos_phi  [ i ]   = cos( phi_i   ) ;
		sin_phi  [ i ]   = sin( phi_i   ) ;
		cos_theta[ j ]   = cos( theta_j ) ;
		sin_theta[ j ]   = sin( theta_j ) ;

	END_LOOP_ij


	//----- Header
	factory_ptr->GenerateHeaderInformation();

	//----- Vertex block

	//---------- Beginning part of vertex block
	factory_ptr->GenerateVertexBlockHeader();
	factory_ptr->GenerateDummyVertex();

	//---------- Generate outer vertices Q
	//      FOREACH_ij:  0 < i <= Ns,  0 < j < N  
	FOREACH_ij

		x = t * cos_phi[i] + (R * cos_theta[j]) * cos_phi[i] ;
		y = t * sin_phi[i] + (R * cos_theta[j]) * sin_phi[i] ; 
		z = R * sin_theta[j];

		factory_ptr->GenerateVertex( ++vlabel, x, y, z );
		Q (i,j) = vlabel ;

	END_LOOP_ij

	//---------- Generate inner vertices q
	//      FOREACH_ij:  0 < i <= Ns,  0 < j < N  
	if( flag_filled_torus ) {
		// Do not generate inner vertices for filled torus
	} else {
		FOREACH_ij

			x = t * cos_phi[i] + (r * cos_theta[j]) * cos_phi[i] ;
			y = t * sin_phi[i] + (r * cos_theta[j]) * sin_phi[i] ; 
			z = r * sin_theta[j];

			factory_ptr->GenerateVertex( ++vlabel, x, y, z );
			q (i,j) = vlabel ;

		END_LOOP_ij
	}

	//---------- Ending part of vertex block
	factory_ptr->GenerateVertexBlockTailer();

	//----- Attribute block
	factory_ptr->GenerateAttributeBlock();

	//----- Facet block

	//---------- beginning part of facet block
	factory_ptr->GenerateFacetBlockHeader();

	//---------- outer facets  made of Q's
	//           FOREACH2_ij:  0 < i < Ns,  0 < j < N  
	FOREACH2_ij
		ip1 = i + 1 ; if( flag_closed_torus && ip1 == N ) { ip1 = 0 ; } 
		jp1 = j + 1 ; if( jp1 == N  ) { jp1 = 0 ; } 

		v0 = Q ( i   , j   );
		v1 = Q ( ip1 , j   );
		v2 = Q ( ip1 , jp1 );
		v3 = Q ( i   , jp1 );

		factory_ptr->GenerateSquare( v0, v1, v2, v3 , 0);		

	END_LOOP_ij

	//---------- inner facets  made of q's 
	//           FOREACH2_ij:  0 < i < Ns,  0 < j < N  
	if( flag_filled_torus ) {
		// Do not generate inner facets for filled torus
	} else {
		FOREACH2_ij
			ip1 = i + 1 ; if( flag_closed_torus && ip1 == N ) { ip1 = 0 ; } 
			jp1 = j + 1 ; if( jp1 == N  ) { jp1 = 0 ; } 

			v0 = q ( i   , j   );
			v1 = q ( i   , jp1 );
			v2 = q ( ip1 , jp1 );
			v3 = q ( ip1 , j   );

			factory_ptr->GenerateSquare( v0, v1, v2, v3 , 0 );		

		END_LOOP_ij
	}

	//---------- first facet (cross section of torus) 
	//            Not necessary in case of a closed torus
	if( !flag_closed_torus ) {
		if( flag_filled_torus ) {
			// Filled Circle	
			for( j = 0 ; j <  N ; j++ ) { 
				label_on_circle[j] = Q ( 0, j );
			}
			factory_ptr->GenerateFacet( label_on_circle, N , 0 );
		} else {
			// Circle with hole: decompose into squares
			for( j = 0 ; j <  N ; j++ ) { 
				jp1 = j + 1 ; if( jp1 == N  ) { jp1 = 0 ; } 

				v0 = Q ( 0   , j   );
				v1 = Q ( 0   , jp1 );
				v2 = q ( 0   , jp1 );
				v3 = q ( 0   , j   );

				factory_ptr->GenerateSquare( v0, v1, v2, v3 , 0 );
			}
		}
	}
	
	//---------- last facet (cross section of torus) 
	//            Not necessary in case of a closed torus
	if( !flag_closed_torus ) {
		if( flag_filled_torus ) {
			// Filled Circle	
			for( j = (N - 1) ; j >=  0 ; j-- ) { 
				int j_reverse = N - j - 1 ;
				label_on_circle[j] = Q ( Ns , j_reverse );
			}
			factory_ptr->GenerateFacet( label_on_circle, N , 0 );
		} else {
			// Circle with hole: decompose into squares
			for( j = 0 ; j <  N ; j++ ) { 
				jp1 = j + 1 ; if( jp1 == N  ) { jp1 = 0 ; } 

				v0 = Q ( Ns , j   ) ;
				v1 = q ( Ns , j   ) ;
				v2 = q ( Ns , jp1 ) ;
				v3 = Q ( Ns , jp1 ) ;

				factory_ptr->GenerateSquare( v0, v1, v2, v3 , 0 );
			}
		}
	}

	//---------- ening part of facet block
	factory_ptr->GenerateFacetBlockTailer();

	//----- Optimization
	factory_ptr->GenerateOptimizedData();

	//----- clean up 
	delete [] cos_phi ;
	delete [] sin_phi ;
	delete [] cos_theta ;
	delete [] sin_theta ;
	delete [] label_on_circle ;

}
