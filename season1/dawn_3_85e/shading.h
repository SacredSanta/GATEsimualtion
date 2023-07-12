//////////////////////////////////
///// shading.h   1995/08/11 /////
//////////////////////////////////

#if !defined SHADING_H
#define SHADING_H

#include <math.h>	
#include <stdlib.h>
#include "vector.h" 
#include "colortype.h" 
#include "perspective.h" 
#include "parameter.h" 

////////////////////////////
//////  class Shading  /////
////////////////////////////

class Shading {

		//-----  Normalized vector indicating direction "towards"
		//.....  source of light.
	Vector3		lightVector ;

		//----- set strength of source light
	ColorType	I_source ;

		//----- set strength of ambient light
	ColorType	I_a ;

 public:
		//----- const 
//	enum{ LAMBERT = 0 , PHONG = 1 };


		//----- constructors
		//---------- type 1: set all data members explicitly
	Shading( const  Vector3& lightVector_given ,
		 const  ColorType& I_source_given  ,
		 const  ColorType& I_a_given        );

		//---------- type 2: give lightVector and color of light
		//..........         Given color is copied to 
		//.........          I_source and I_a_given.
	Shading(  const  Vector3& lightVector_given ,
		  const  ColorType& light_color      );

		//---------- type 3: give light vector 
		//..........         ( default is light from top)
	Shading( const Vector3& lightVector_given = Vector3( 0.0, 0.0, 1.0 ) );

		//----- destructor
	~Shading() { ; }

		//----- set all data members
	void	set(	const Vector3&    lightVector_given , 
			const ColorType&  I_source_given, 
			const ColorType&  I_a_given );

	void	set(	double	v_angle , 
			double	h_angle , 
			const   ColorType&  I_source_given, 
			const   ColorType&  I_a_given );


		//----- set light vector
		//.....   Note: Normalization is performed within this function.
		//.....         So lightVector_given need not be normalized.
	void	setLightVector( const  Vector3& lightVector_given );

		//----- set strength of light source
	void	setI_source ( const  ColorType& I_source_given ) ;

		//----- set strength of ambient light
	void	setI_a      ( const  ColorType& I_a_given );

		//----- get
	const Vector3&		get_lightVector() const { return lightVector ; }
	const ColorType&	get_I_source   () const { return I_source    ; }
	const ColorType&	get_I_a        () const { return I_source    ; }

		//----- simple shading based on Lambert cosine law
		//.....  Arguments:
		//.....   normalVector: normal vector of plane in world coord
		//.....   color_output_p: output of calculated color
		//.....                   (minRGB <= r,g,b <= 1 )
		//.....   kd:           reflection coefficient of plane 
		//.....                 for direct light
		//.....                 ( Default is "full reflection".)
		//.....   ka:           reflection coefficients of plane 
		//.....                 for ambient light
		//.....                 ( Default is "full reflection".)
	void	doLambert(	const Vector3&   normal_vector, 
				ColorType*       color_output_p ,
				const ColorType& kd , 
				const ColorType& ka                ) ;

		//----- Shading::doPhong()
	void	doPhong(	const Vector3&			normal_vector  , 
				ColorType*			color_output_p ,
				const ColorType&		kd             , 
				const ColorType&		ka             ,          
				const ColorType&		ks             ,          
				const Perspective& 		camera         ,
				int				n_power         );

		//----- do selected shading
	void	doShading(	int		  shading_mode                  ,
				const Vector3&	  normal_vector                 , 
				ColorType*	  color_output_p                ,
				const ColorType&  kd                            , 
				const ColorType&  ka                            ,
				const ColorType&  ks                            ,
				const Perspective& camera                       ,	
				int		  n_power  = DEFAULT_PHONG_POWER    );

	void	operator = ( const Shading& rhs );

} ; // Shading

#endif
