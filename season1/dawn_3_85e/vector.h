//////////////////////
///// vector.h   /////
//////////////////////

#if !defined _VECTOR_H
#define _VECTOR_H

#include <iostream.h>
#include <fstream.h>
#include "dev_control.h"
#if defined USE_VC
	#include <strstrea.h>
#else
	#include <strstream.h>
#endif

#include <math.h>

//----- const 
const double FR_MIN_VECTOR2_LENGTH = 0.0 ;
const double FR_MIN_VECTOR3_LENGTH = 0.0 ;

//----- prototype
class Vector2 ;
class IntVector2 ;
class Vector3 ;
class Vector3Cyl ;
class Vector3Sph ;

//----- typedef  types of pointers
typedef Vector2*	Vector2_p ;
typedef IntVector2* 	IntVector2_p ;
typedef Vector3*	Vector3_p ;
typedef Vector3Cyl*	Vector3Cyl_p ;
typedef Vector3Sph* 	Vector3Sph_p ;


	//-------------------------//
	//----- CLASS Vector2 -----//
	//-------------------------//
class Vector2 {
public:
	//----- components of 2-dimensional vector
	double	x, y;

	//----- constructors
		Vector2 () { x = 0.0; y = 0.0; }
		Vector2 ( double xx, double yy ) { x = xx; y = yy; }
		Vector2	( const Vector2& rhs ) { x = rhs.x ; y = rhs.y; }

	//----- substitution
	void	operator () ( double xx, double yy ) { x = xx; y = yy; }

	//----- print out components
	void	print () const { cerr << "( " << x << " " << y << " )"; }
	void	print (ofstream& out) const { out << "( " << x << " " << y << " )"; }
	void	printerr () const { cerr << "( " << x << " " << y << " )"; }
	void	printout () const { cout << "( " << x << " " << y << " )"; }

	//----- get length of vector 
	double	getLength() const { return sqrt( x * x + y * y )  ; }

	//----- multiplying scalar from left
	friend
	Vector2	operator*( const double& c, const Vector2& v );

	//----- multyplying scalar from right
	friend
	Vector2 operator*( const Vector2& v ,  const double& c ) ;

	//----- operator + 
	friend 
	Vector2	operator+( const Vector2& u , const Vector2& v );

	//----- operator - 
	friend 
	Vector2	operator-( const Vector2& u , const Vector2& v );

	//----- operator * (scalar product)
	friend 
	double	operator*( const Vector2& u , const Vector2& v );

	//----- operator ==
	friend 
	int	operator==( const Vector2& u , const Vector2& v );

	//----- operator =
	void	operator=( const Vector2& v ) {	x = v.x ;  y = v.y ; }
	void	operator=( const Vector3& v ) ; // 3D --> 2D
						// x = v.x ;  y = v.y ; 

	//----- normalization
	//.....  Return normalized *this.
	//.....  *this itselt is not modified.
	//.....  If |*this| < epsilon, zero vector is returned.
	Vector2 normalize     ( double epsilon = FR_MIN_VECTOR2_LENGTH ) const ;
	void    selfNormalize ( double epsilon = FR_MIN_VECTOR2_LENGTH ) ;

}; // Vector2


	//-------------------------//
	//----- CLASS Vector3 -----//
	//-------------------------//
class Vector3 {
public:
	//----- components of 3-dimensional vector
	double	x, y, z;

	//----- constructors
		Vector3 () { x = 0.0; y = 0.0; z = 0.0; }
		Vector3	( double xx, double yy, double zz ) 
		{ x = xx; y = yy; z = zz; }
		Vector3	( const Vector3& rhs )
		{ x = rhs.x ; y = rhs.y; z = rhs.z ; }

	//----- substitution
	void	operator () ( double xx, double yy, double zz ) 
			{ x = xx; y = yy; z = zz; }

	//----- display components of vector
	void	print()    const { cerr << "( " << x << " " << y << " " << z << " )";}
	void	print(ofstream& out) const { out << "( " << x << " " << y << " " << z << " )";}
	void	printerr() const { cerr << "( " << x << " " << y << " " << z << " )";}
	void	printout() const { cout << "( " << x << " " << y << " " << z << " )";}

	//----- get length of vector
	double	getLength() const { return sqrt( x * x + y * y + z * z )  ; }

	//----- operator* ( multyplying scalar from left )
	friend 
	Vector3  operator*( const double& c, const Vector3& v );

	//----- operator* ( multyplying scalar from right )
	friend 
	Vector3  operator*( const Vector3& v ,  const double& c );


	//----- operator * ( scalar product )
	friend	
	double	operator*( const Vector3& u , const Vector3& v );

	//----- operator ^ ( vector product )
	friend	
	Vector3 operator^( const Vector3& u , const Vector3& v );

	//----- operator + 
	friend
	Vector3 operator+( const Vector3& u , const Vector3& v );

	//----- operator - 
	friend
	Vector3	operator-( const Vector3& u , const Vector3& v );

	//----- operator ==
	friend
	int	operator==( const Vector3& u , const Vector3& v );

	//----- operator += 
	void	operator+=( const Vector3& v ) ;

	//----- operator -= 
	void	operator-=( const Vector3& v ) ;

	//----- operator = 
	void	operator = ( const Vector3& v ) ;

	//----- normalization
	//.....  Return normalized *this.
	//.....  *this itselt is not modified.
	//.....  If |*this| < epsilon, zero vector is returned.
	Vector3 normalize    ( double epsilon = FR_MIN_VECTOR3_LENGTH ) const ;
	void    selfNormalize( double epsilon = FR_MIN_VECTOR3_LENGTH )  ;

	//----- to spherical coordinate
	Vector3Sph toSpherical();

	//----- to cylindrical coordinate
	Vector3Cyl toCylindrical() ;

}; // Vector3



//----- CLASS Vector3Cyl 
class Vector3Cyl {
public:
	double	r, theta, z;	// theta:x-angle

		Vector3Cyl () { r = 0.0; theta = 0.0; z = 0.0; }
		Vector3Cyl ( double r_given, \
			     double theta_given, \
			     double z_given)
			   { r = r_given; theta = theta_given; z = z_given; }
	void	operator () ( double r_given, \
			      double theta_given, \
			      double z_given)
			   { r = r_given; theta = theta_given; z = z_given; }

	void	operator = ( const Vector3Cyl& v ) 
			{ 
				this->r     = v.r ;
				this->theta = v.theta ;
				this->z     = v.z ;
			}

	void	print () const
		{ cerr	<< "( Cyl " 
			<< r << " " << theta << " " << z << " )"; 
		}

};



//----- CLASS Vector3Sph
class Vector3Sph {
 public:
		//----- data members
	double	r, phi, theta ; // phi:z-angle, theta:x-angle

		//----- constructor 1
		Vector3Sph () { r = 0.0; phi = 0.0, theta = 0.0 ; }

		//----- constructor 2
		Vector3Sph ( double r_given , 
			     double phi_given , 
			     double theta_given )
			   { r = r_given; phi = phi_given; theta = theta_given; }

		//----- substitution
	void	operator () ( double r_given, 
			      double phi_given,
			      double theta_given )
			   { r = r_given; phi = phi_given; theta = theta_given; }

		//----- operator =
	void	operator = ( const Vector3Sph& v ) 
			{ 
				this->r     = v.r ;
				this->phi   = v.phi ;
				this->theta = v.theta ;
			}
			
		//----- display components
	void	print () const
		{ cerr	<< "( Sph " 
			<< r << " " << phi << " " << theta << " )"; 
		}

		//----- spherical --> cartecian
	Vector3	toCartecian() ;

}; // Vector3Sph 


//----------------------------//
//----- Default vectors ------//
//----------------------------//
// const Vector3  THE_ORIGIN_3D (0.0, 0.0, 0.0) ;
// const Vector3  THE_UP_3D     (0.0, 0.0, 1.0) ;
// const Vector2  THE_ORIGIN_2D (0.0, 0.0)      ;
#endif
