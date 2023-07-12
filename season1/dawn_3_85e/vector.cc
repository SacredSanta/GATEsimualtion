///////////////////////////////////
///// vector.cc   1995/05/20 //////
///////////////////////////////////


//---- proto-type declaration
#include "vector.h"
#include "FRMath.h"

//----------------------------------------//
//----- binary operators for Vector2  -----//
//----------------------------------------//
//---------- multyplying scalar from left 
Vector2	operator*( const double& c, const Vector2& v )
{
	Vector2		retVector ;
	retVector.x = c * v.x ;
	retVector.y = c * v.y ;
	return retVector ;
} //"c*v"


//---------- multyplying scalar from right
Vector2	operator*( const Vector2& v ,  const double& c )
{
	Vector2		retVector ;
	retVector.x = c * v.x ;
	retVector.y = c * v.y ;
	return retVector ;
} // "v*c"


//---------- operator + 
Vector2	operator+( const Vector2& u , const Vector2& v )
{ 
	Vector2		retVector ;
	retVector.x = u.x + v.x ;
	retVector.y = u.y + v.y ;
	return retVector ;
} // +

//---------- operator - 
Vector2	operator-( const Vector2& u , const Vector2& v )
{ 
	Vector2		retVector ;
	retVector.x = u.x - v.x ;
	retVector.y = u.y - v.y ;
	return retVector ;
}// -


//---------- operator * ( scalar product )
double	operator*( const Vector2& u , const Vector2& v )
{
	double retValue ;
	retValue = u.x * v.x  +  u.y * v.y  ;
	return retValue ;
} //"u*v"


//---------- operator == ( scalar product )
int	operator==( const Vector2& u , const Vector2& v )
{ 
	return ( (u.x == v.x) && (u.y == v.y) ) ; 
} // ==



//----------------------------------------//
//----- binary operators for Vector3  -----//
//----------------------------------------//
//---------- multyplying scalar from left 
Vector3	operator*( const double& c, const Vector3& v )
{
	Vector3		retVector ;
	retVector.x = c * v.x ;
	retVector.y = c * v.y ;
	retVector.z = c * v.z ;
	return retVector ;
} //"c*v"


//---------- multyplying scalar from right
Vector3	operator*( const Vector3& v ,  const double& c )
{
	Vector3		retVector ;
	retVector.x = c * v.x ;
	retVector.y = c * v.y ;
	retVector.z = c * v.z ;
	return retVector ;
} // "v*c"


//---------- operator * ( scalar product )
double	operator*( const Vector3& u , const Vector3& v )
{
	double retValue ;
	retValue = u.x * v.x  +  u.y * v.y  +  u.z * v.z ;
	return retValue ;
} //"u*v"


//---------- operator ^ ( vector product )
Vector3 operator^( const Vector3& u , const Vector3& v )
{
	Vector3 retVector ;

	retVector.x = u.y * v.z - u.z * v.y ;
	retVector.y = u.z * v.x - u.x * v.z ;
	retVector.z = u.x * v.y - u.y * v.x ;

	return retVector ;
}	


//---------- operator + 
Vector3 operator+( const Vector3& u , const Vector3& v )
{
	Vector3		retVector ;
	retVector.x = u.x + v.x ;
	retVector.y = u.y + v.y ;
	retVector.z = u.z + v.z ;
	return retVector ;
} //"+"

//---------- operator - 
Vector3	operator-( const Vector3& u , const Vector3& v )
{
	Vector3		retVector ;
	retVector.x = u.x - v.x ;
	retVector.y = u.y - v.y ;
	retVector.z = u.z - v.z ;
	return retVector ;
} // "-"

//---------- operator == 
int	operator==( const Vector3& u , const Vector3& v )
{
	return ( (u.x == v.x) && (u.y == v.y) && (u.z == v.z) ) ; 
} // ==


//----------------------------------------------//
//----- Member functions of Vector2 class ------//
//----------------------------------------------//

	//----- 3D --> 2D
void	Vector2::operator=( const Vector3& v ) {	x = v.x ;  y = v.y ; }

	//----- normalize()
Vector2
Vector2::normalize( double epsilon ) const 
{
		//----- local variables
	Vector2		retVector;
	double		length, factor ;

		//----- calc |*this|
	length = sqrt( x*x + y*y ) ;

		//----- calc normalized *this
	if( length > epsilon ) {
			//----- vector with non-zero length
		factor = 1.0 / length ;
		retVector.x = factor * x ;		
		retVector.y = factor * y ;		
	} else {

//		cerr << "WARNING: Tried to normalize zero vector (x,y)" << endl;

			//----- zero vector
		retVector( 0.0, 0.0 );
	}

		//----- return normalized vector
	return retVector ;

} // Vector2::normalize()


	//----- Vector2::selfNormalize()
void    Vector2::selfNormalize( double epsilon ) 
{
	*this = this->normalize( epsilon );
}

//----------------------------------------------//
//----- Member functions of Vector3 class ------//
//----------------------------------------------//



//---------- operator +=
void	Vector3::operator+=( const Vector3& v ) 
{
	this->x += v.x ;
	this->y += v.y ;
	this->z += v.z ;
} // +=


//---------- operator -=
void	Vector3::operator-=( const Vector3& v ) 
{
	this->x -= v.x ;
	this->y -= v.y ;
	this->z -= v.z ;
} // -=


//---------- operator =
void	Vector3::operator=( const Vector3& v ) 
{
	this->x = v.x ;
	this->y = v.y ;
	this->z = v.z ;
}


//---------- normalize()
Vector3
Vector3::normalize( double epsilon ) const
{
		//----- local variables
	Vector3		retVector;
	double		length, factor ;

		//----- calc |*this|
	length = sqrt( x*x + y*y + z*z ) ;

		//----- calc normalized *this
	if( length > epsilon ) {
			//----- vector with non-zero length
		factor = 1.0 / length ;
		retVector.x = factor * x ;		
		retVector.y = factor * y ;		
		retVector.z = factor * z ;		
	} else {
//		cerr << "WARNING: Tried to normalize zero vector (x,y, z)" << endl;

			//----- zero vector
		retVector(0.0, 0.0, 0.0 );
	}

		//----- return normalized vector
	return retVector ;

} // normalize()
		

	//----- Vector3::selfNormalize()
void    Vector3::selfNormalize( double epsilon ) 
{
	*this = this->normalize( epsilon );
}


//---------- toSpherical()
Vector3Sph 
Vector3::toSpherical()
{
	//----- 
	const double FR_PI	= FR_D_PI ;
	const double PI_INV_180	= 180.0 / FR_PI ;
	const double epsilon 	= 1.0e-7 ;

	//----- return vector
	Vector3Sph	retVector ;

	//----- calc spherical coordinate
	//----- r : distance
	retVector.r     = sqrt( x*x + y*y + z*z ) ;

	if( retVector.r < epsilon ) { 
		//----- phi: z-angle
		retVector.phi	= 0.0 ;

		//----- theta: x-angle
		retVector.theta	= 0.0 ;
	} else {
		//----- phi: z-angle
		retVector.phi   = PI_INV_180 * acos( z / retVector.r ) ;

		//----- theta: x-angle
		retVector.theta = PI_INV_180 * atan2( y , x ) ;
		if( retVector.theta < 0.0 ) { retVector.theta += 360.0 ;}
	}

	//----- return
	return retVector ;

} // toSpherical()


//---------- cylindrical()
Vector3Cyl 
Vector3::toCylindrical()
{
	//----- 
	const double FR_PI = FR_D_PI ;
	const double PI_INV_180      = 180.0 / FR_PI ;

	//----- return value 
	Vector3Cyl retVector ;

	//----- calc cylindrical coordinate
	retVector.r     = sqrt( x*x + y*y ) ;
	retVector.theta = PI_INV_180 * atan2( y , x ) ;
	if( retVector.theta < 0.0 ) { retVector.theta += 360.0 ;}
	retVector.z     = z ;

	//----- return
	return retVector ;

} // toCylindrical()


	//----- Vector3Sph::toCartecian()
Vector3 
Vector3Sph::toCartecian()
{
		//----- local consts
	const double FR_PI		= FR_D_PI ;
	const double PI_OVER_180	= FR_PI / 180.0;

		//----- local variables
	double	cos_phi,  sin_phi,  cos_theta, sin_theta ;
	Vector3	vector ;

		//----- cosine, sine
		//..... phi:z-angle, theta:x-angle			
	cos_phi   = cos( PI_OVER_180 * phi ) ;
	sin_phi   = sin( PI_OVER_180 * phi ) ;
	cos_theta = cos( PI_OVER_180 * theta ) ;
	sin_theta = sin( PI_OVER_180 * theta ) ;

		//----- calc cartecial coordinate
	vector.x = r * sin_phi * cos_theta ;
	vector.y = r * sin_phi * sin_theta ;
	vector.z = r * cos_phi ;

		//----- return vector incartecial coordinate 
	return vector ;

} // Vector3Sph::toCartecian()
