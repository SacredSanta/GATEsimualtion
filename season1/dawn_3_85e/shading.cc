///////////////////////////////////
///// shading.cc   1995/08/11 /////
///////////////////////////////////

#include "shading.h" 

// #define  DEBUG_PHONG


////////////////////////////////////////////////
//////  member functions of class Shading  /////
////////////////////////////////////////////////

	//----- Shading::set()
void
Shading::set(	const  Vector3&    lightVector_given , 
		const  ColorType&  I_source_given, 
		const  ColorType&  I_a_given )
{
		//----- light vector
	setLightVector( lightVector_given ) ;

		//----- source light
	setI_source   ( I_source_given );

		//----- ambient light
	setI_a        ( I_a_given );

}// Shading::set()


	//----- Shading::set()
void	Shading::set(	double			v_angle , 
			double			h_angle , 
			const  ColorType&	I_source_given, 
			const  ColorType&	I_a_given )
{
	double		distance = 1.0 ;
	Vector3Sph	lingt_vector_sph( distance, v_angle, h_angle )  ;
	Vector3		light_vector ;

	light_vector = lingt_vector_sph.toCartecian() ,
	this->set(	light_vector    ,
			I_source_given  ,
			I_a_given        );

} // Shading::set()



	//----- Shading::Shading( all_data mambers ) 
	//.....  Type 1 constructor
Shading::Shading( const  Vector3& lightVector_given , 
		  const  ColorType& I_source_given  , 
		  const  ColorType& I_a_given )
{
	set( lightVector_given , I_source_given, I_a_given );
}


	//----- Shading::Shading( light_vector, light_color )
	//.....  Type 2 constructor
Shading::Shading( const  Vector3& lightVector_given ,
		  const  ColorType& light_color      )
{
	set( lightVector_given, light_color, light_color );
} 

	//----- Shading::Shading( light_vector )
	//.....  Type 3 constructor
	//.....  Default argumnet is Vector3(0, 0, 1).
Shading::Shading( const Vector3& lightVector_given )
{
		//----- local variables
	ColorType	white( 1.0, 1.0, 1.0 ) ;

		//----- call type 1 constructor
	set( lightVector_given, white, white );
}


	//----- Shading::setLightVector()
void
Shading::setLightVector( const Vector3& lightVector_given )
{
	lightVector = lightVector_given ;
	lightVector = lightVector.normalize() ;

} // Shading::setLightVector()


	//----- Shading::setI_source() 
void	
Shading::setI_source( const ColorType& I_source_given )
{

	I_source = I_source_given ;

} // Shading::setI_source()

	//----- Shading::setI_a()
void
Shading::setI_a( const ColorType& I_a_given )
{
	I_a = I_a_given ;
}


	//----- Shading::doLambert()
void	Shading::doLambert(	const Vector3&			normal_vector  , 
				ColorType*			color_output_p ,
				const ColorType&		kd             , 
				const ColorType&		ka              )          
{
		//----- local variables
	double	red, green, blue ;
	double	L_N ;	// lightVector * normal_vector
	
		//----- calc L_N
	L_N = lightVector * normal_vector ;
	if( L_N < 0.0 ) { L_N = 0.0 ; }


		//----- strength of output light 
	red 	=	kd.getRed() * I_source.getRed() * L_N 
		   +	ka.getRed() * I_a.getRed()  ;

	green 	=	kd.getGreen() * I_source.getGreen() * L_N 
		   +	ka.getGreen() * I_a.getGreen() ;

	blue 	=	kd.getBlue() * I_source.getBlue() * L_N 
		   +	ka.getBlue() * I_a.getBlue()  ;


		//----- normalized strength of output light ( 0 <= color <= 1.0 )
	if( red   > 1.0  ) { red   = 1.0 ; }  	if( red   < 0.0 )  { red   = 0.0 ; }
	if( green > 1.0  ) { green = 1.0 ; }  	if( green < 0.0 )  { green = 0.0 ; }
	if( blue  > 1.0  ) { blue  = 1.0 ; }  	if( blue  < 0.0 )  { blue  = 0.0 ; }

		//----- set output color
	color_output_p->setRGB( red, green, blue );
	
} // Shading::doLambert()



	//----- Shading::doPhong()
void	Shading::doPhong(	const Vector3&			normal_vector  , 
				ColorType*			color_output_p ,
				const ColorType&		kd             , 
				const ColorType&		ka             ,          
				const ColorType&		ks             ,          
				const Perspective& 		camera         ,
				int				n_power          )
{
		//----- local const
	const	Vector3		V = (-1.0) * ( camera.getEyeVector()) ;
				// reverse eye vector V

		//----- local variables
	int		i ;
	double		red, green, blue ;
	double		L_N ;	// lightVector * normal_vector
	Vector3		L2 ;
	double		L2_V ;
	double		L2_V_n ;
	
		//----- calc L_N
	L_N	= lightVector * normal_vector ;

		//----- cos(gamma) = L2 * V
	L2	=  2.0 * (L_N * normal_vector) - lightVector ;
	L2_V    =  L2 * V ;   if( L2_V < 0.0 ) { L2_V = 0.0 ; }
			  // cos(gamma)

		//----- cos^{n}(gamma) 
	if( n_power <= 0 ) {
		L2_V_n  =  0.0 ; // skip incorporating specular effects
				 // for an irrelavant paremater
	} else {
		L2_V_n  =  L2_V ; // initialize
		for( i = 2 ; i <= n_power ; i++ ) {
			L2_V_n  *=  L2_V ;		
		}
	}

		//----- negative inner products
	if( L_N < 0.0 ) { L_N = 0.0 ; }

		//----- strength of output light ( 0 <= color <= 2.0 )
	red 	=	kd.getRed() * I_source.getRed() * L_N 
		   +	ka.getRed() * I_a.getRed()  
		   +	ks.getRed() * I_source.getRed() * L2_V_n ;

	green 	=	kd.getGreen() * I_source.getGreen() * L_N 
		   +	ka.getGreen() * I_a.getGreen() 
		   +	ks.getGreen() * I_source.getGreen() * L2_V_n;

	blue 	=	kd.getBlue() * I_source.getBlue() * L_N 
		   +	ka.getBlue() * I_a.getBlue()  
		   +	ks.getBlue() * I_source.getBlue() * L2_V_n ;

		//----- normalized strength of output light ( 0 <= color <= 1.0 )
	if( red   > 1.0  ) { red   = 1.0 ; }  	if( red   < 0.0 )  { red   = 0.0 ; }
	if( green > 1.0  ) { green = 1.0 ; }  	if( green < 0.0 )  { green = 0.0 ; }
	if( blue  > 1.0  ) { blue  = 1.0 ; }  	if( blue  < 0.0 )  { blue  = 0.0 ; }

		//----- set output color
	color_output_p->setRGB( red, green, blue );
	
} // Shading::doPhong()




	//-----	Shading::doShading()
void	Shading::doShading(	int		  shading_mode   ,
				const Vector3&	  normal_vector  , 
				ColorType*	  color_output_p ,
				const ColorType&  kd             ,
				const ColorType&  ka             ,
				const ColorType&  ks             ,
				const Perspective& camera        ,	
				int		  n_power            )
{
	switch( shading_mode ) {
	 case LAMBERT:
		doLambert( 	normal_vector, 
				color_output_p ,
				kd, ka          ) ;
		break ;

	 case PHONG:
		doPhong( 	normal_vector, 
				color_output_p ,
				kd, ka , ks , camera, n_power );
		break ;
	 default:
		doLambert( 	normal_vector, 
				color_output_p ,
				kd, ka         ) ;
		break ;


	}

} // Shading::doShading()


void	Shading::operator = ( const Shading& rhs )
{
	lightVector	= rhs.get_lightVector();
	I_source	= rhs.get_I_source   ();
	I_a		= rhs.get_I_a        ();

} // Shading::operator = ( const Shading& rhs )
