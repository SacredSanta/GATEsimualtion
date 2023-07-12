////////////////////////////
/////	perspective.h  /////
////////////////////////////


//////////////////////////////////////////////////////////////////////////
///// NOTE:
/////    * In the following we often use a term "camera-world coordinate"
/////      Its definition is as follows:
/////
/////  	 camera-world coordinate:
/////	  	Origin is the target point.
/////	  	Base vectors are the same as world coordinate.
/////  	 eye coordinate:
/////		Obtained by performing viewing transformation to camera-world coordinate.
/////	  	Origin is eye position.
/////		Z-direction is from eye position to target.
/////  	 2D screen coordinate ( = viewport coordinate ):
/////	  	2D coordinate on screen, whose distance from eye position is 
/////	  	"focalDistance".
/////  	 3D screen coordinate:
/////	  	x = x of 2d screen coordinate
/////	  	y = y of 2d screen coordinate
/////	  	z = z of eye
/////
/////    * In this class, "theta" is the azimuthal angle (angle from x-axis),
/////      and "phi" is the polar angle (angle from z-axis).
/////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////


#if !defined __PERSPECTIVE_H__
#define __PERSPECTIVE_H__

	//--------------------------------//
	//----- INCLUDE HEADER FILES -----//
	//--------------------------------//
#include	"toolmath.h"
#include	"vector.h"
#include	"dev_control.h"

	//-----------------------------------------//	
	//----- default parameters for camera -----//
	//-----------------------------------------//	

	//-----  default "d"  (distance between eye & screen)
const double	DEFAULT_SCREEN_DISTANCE	      = 1.0;	

	//-----  default "rho"  (distance between eye
	//.....                  and origin of world coordinate)
const double	DEFAULT_CAMERA_POSITION_RHO   = 10000.0;

	//-----  default "phi" ( angle from z-axis of world coordinate)
const double	DEFAULT_CAMERA_POSITION_PHI   = 0.0;

	//-----  default "theta" ( angle from x-axis of world coordinate)
const double	DEFAULT_CAMERA_POSITION_THETA = 0.0;

	//-----
// class Perspective;
// typedef	void( Perspective::*viewing_func     )( Vector3 *) ;
// typedef	void( Perspective::*viewing_rev_func )( Vector3 *) ;


	//-----------------------------//
	//----- CLASS Perspective -----//
	//-----------------------------//
class Perspective {

 protected:

		//-----  d = distance( eye, screen )
	double		focalDistance ;

		//----- Target point of camera.
		//.....  Default is the origin of world coordinate.
		//.....  It is modified when the camera rotates.
		//.....  Target point is expressed with world coordinate.
		//.....  Target point is the origin of 
		//.....  "camera-world coordinate".
	Vector3		target ;

		//-----  Position of camera (eye) in polar cooldinate.
		//.....   Origin is the target point, i.e.,
		//.....   camera-world coordinate is used to express it.
	double		rho   ;		//	distance
	double		phi   ;		// 	polar angle (angle from z-axix)
	double		theta ;		// 	azimuthal angle (angle from x-axix)

		//----- cos & sin of angles phi and theta
		//----- calculated through setPhi() and setTheta() ;
	double		cos_phi   , sin_phi   ;
	double		cos_theta , sin_theta ;

		//----- Viewing matrix elements (Z up)
		//.....  v11 = - sin_theta ;
		//.....  v12 = - cos_phi * cos_theta ;
		//.....  v13 = - sin_phi * cos_theta ;
		//.....  ( v14 = 0 )
		//.....
		//.....  v21 =	cos_theta ;
		//.....  v22 = - cos_phi * sin_theta ;
		//.....  v23 = - sin_phi * sin_theta ;
		//.....  ( v24 = 0 )
		//.....
		//.....  ( v31 = 0 )
		//.....  v32 =   sin_phi ;
		//.....  v33 = - cos_phi ;
		//.....  ( v34 = 0 )
		//.....
		//.....  ( v41 = v42 = 0 )
		//.....  v43 = rho ;
		//.....  ( v44 = 1 )
	double		v11 , v12 , v13 , \
			v21 , v22 , v23 , \
			      v32 , v33 , \
				    v43 ; \

		//----- Viewing matrix elements (Y up)
	double		w11 , w12 , w13 , \
			      w22 , w23 , \
			w31 , w32 , w33 , \
			w41 , w42 , w43 ; \

		//----- eye vector
		//..... Vector indicating direction to target point 
		//..... from camera position.
		//..... It is expressed in world coordinate 
		//..... (or, equevalently, in camera-world coordinate).
	Vector3		eyeVector ;

		//----- eye position in camera_world coord
	Vector3		E         ; 

		//----- upward direction (Y or Z)
		//...... Set with SetYup() or SetZup();
	char		upDirection ;

		//----- pointer to current viewing transformation function
////////////////////////////////////
// #if defined USE_VC
////////////////////////////////////

	void	cur_viewing_func     (Vector3*) ;
	void	cur_viewing_rev_func (Vector3*) ;	

//////////////////////////////////////////////////////
//#else 
//	void( Perspective::*cur_viewing_func     )( Vector3 *) ;
//	void( Perspective::*cur_viewing_rev_func )( Vector3 *) ;
//#endif
//////////////////////////////////////////////////////	

	//----- mathematical tools
		//..... sin, cos in degrees etc.
	ToolMath	tool ;

 protected:

		//----- access to upDirection
	void	setYup() ;
	void	setZup() ;
	int	isYup() { return (upDirection == 'Y'); }
	int	isZup() { return (upDirection == 'Z'); }

		//----- calculate vij, wij, E, and eyeVector
	void  setViewingMatrix    () ; // (*) 
	void  setViewingMatrix_Yup() ; // called only from (*)
	void  setViewingMatrix_Zup() ; // called only from (*)
	void  setEyePosition      () ; // called only from (*)
	void  setEyeVector        () ; // called only from (*)


		//----- worldToScreen() = (1) + (2) + (3)

		//----- (1) world coordinate --> camera-world coordinate
		//.....   reset origin of world coordinate 
		//.....   to target point of camera.  
		//.....   Arg   : world coordinate
		//.....   Return: camera-world coordinate
	Vector3	resetWorldOrigin ( const Vector3& world ) ;

		//----- (2) viewing transformation
		//.......... Arg: camera-world coord --> eye coord
	void  viewing     ( Vector3* coord_3 ) ;
	void  viewing_Yup ( Vector3* coord_3 ) ;
	void  viewing_Zup ( Vector3* coord_3 ) ;

		//----- (3) perspective transformation
		//........Arg   : eye coord --> 3D screen coord
		//.....   Return: 1 if  normal
		//.....           0 if  error (	if coord_3.z < epsilon ) 
	int  perspective ( Vector3* coord_3 , double epsilon3d = 1.0e-6 ) ;

		//----- Arg: eye coord --> camera_world coord 
	void  viewingReverse     ( Vector3* coord_3 );
	void  viewingReverse_Yup ( Vector3* coord_3 ) ;
	void  viewingReverse_Zup ( Vector3* coord_3 ) ;

 public:
		//----- set protected data of class
	Perspective (
			double  focalDistanceGiven	= DEFAULT_SCREEN_DISTANCE , \
			double  target_x			= 0.0 , \
			double  target_y			= 0.0 , \
			double  target_z			= 0.0 , \
			double	rhoGiven		= DEFAULT_CAMERA_POSITION_RHO , \
			double	phiGiven		= DEFAULT_CAMERA_POSITION_PHI , \
			double	thetaGiven		= DEFAULT_CAMERA_POSITION_THETA \
			) ;

		//----- set focalDistance
		//..... if argument is less than epsilon,
		//..... focalDistance is set to DEFAULT_SCREEN_DISTSNCE
		//..... if argument < epsilon, 
		//..... focalDistance = 1.0
	void  setFocalDistance ( double focalDistanceGiven, double epsilon3d = 1.0e-6 ) ;

		//----- Set focalDistance 
		//..... suth that the given point P is drawn 
		//..... inside screen (viewport) of width * height
		//..... if calculated focalDistance is less than epsilon,
		//..... it is overwrited with DEAULT_SCREEN_DISTANCE.
		//..... Return: resultant focalDistance
	double 	setScreenRange (	Vector3&  P , double epsilon3d , \
					double width = 1.0, double  height = 1.0, \
					double epsilon = 1.0e-6 );

	double 	setScreenRange (	Vector3*  P , int num_point , \
					double    epsilon3d         , \
					Vector3*  initial_point = NULL , \
					double width = 1.0, double  height = 1.0, \
					double epsilon = 1.0e-6 );
						// This function sets minimal focal distance to member 
						// "focalDistance" such that 
						// all given points are drawn inside the screen.
						// In calculating the minimum, d = setSceenRange(*initial_point) is 
						// used as initial_distance value.  If initial_point is NULL, 
						// current focalDistance is used as d instead.
	double 	setScreenRange (	Vector3**  P , int num_point , \
					double    epsilon3d          , \
					Vector3*  initial_point = NULL , \
					double width = 1.0, double  height = 1.0, \
					double epsilon = 1.0e-6 );
						// Same function except that addresses of points P[i] are given.

		//----- set target point
	void  setTarget ( Vector3 targetGiven ) ;

		//----- set camera position
		//..... if rhoGiven < epsilon, rho is set to
		//..... DEFAULT_CAMERA_POSITION_RHO 
	void  setRho   ( double rhoGiven , double epsilon = 1.0e-6 ) ;
	void  setPhi   ( double phiGiven )	 ;
	void  setTheta ( double thetaGiven ) ;
	void  setCameraPosition ( double rhoGiven , \
				  double phiGiven , \
				  double thetaGiven ) ;

		//----- get protected data of class
	double  getFocalDistance()  const { return focalDistance ; }
	Vector3 getTarget()         const { return target ; }
	double  getRho()            const { return rho   ; }
	double  getPhi()            const { return phi   ; }
	double  getTheta()          const { return theta ; }
	Vector3 getCameraPosition() const ;	// Return: camera position in
					//         world coordinate

		//-----  world coordinate --> eye coordinate
	void  worldToEye ( const Vector3& world , Vector3& eye ) ;

		//-----  world coordinate --> screen coordinate (3D, 2D)
		//.....   Return: 1 if  normal
		//.....           0 if  error ( if eye.z < epsilon )
	int  worldToScreen ( const Vector3& world , Vector3& screen_3d, double epsilon3d   = 1.0e-6 ) ;
	int  worldToScreen ( const Vector3& world , Vector2& screen_2d, double epsilon3d   = 1.0e-6 ) ;
	int  worldToViewport ( const Vector3& world , Vector2& screen_2d, double epsilon3d = 1.0e-6 ) 
		{ return worldToScreen( world, screen_2d, epsilon3d );}

		//-----  eye coordinate   --> screen coordinate
		//.....   Return: 1 if  normal
		//.....           0 if  error ( if eye.z < epsilon )
	int  eyeToScreen( const Vector3& eye , Vector3& screen, double epsilon = 1.0e-6 ) ;

		//-----  eye coordinate   --> world coordinate
	void  eyeToWorld( const Vector3& eye , Vector3& world ) ;

		//-----  screen coordinate --> world coordinate
		//.....   Return: 1 if  normal
		//.....           0 if  error ( focalDistance <= 0)
	int  screenToWorld ( const Vector3& screen , Vector3& world, double epsilon = 1.0e-6 ) ;

		//-----  screen coordinate --> eye coordinate
		//.....   Return: 1 if  normal
		//.....           0 if  error ( focalDistance <= 0)
	int  screenToEye( const Vector3& screen , Vector3& eye, double epsilon = 1.0e-6 ) ;

		//----- get z-value of given point
		//.......... Get z-coordinate in eye coordinate
		//.......... for a point given in world coordinate
	double  eyeZ ( const Vector3& world ) ;

		//----- get direction of given point seeing from camera
		//.....  Get vector EP with E camera position and 
		//.....  P given point in the argument.
		//.....  The given P shold be expressed in world coordinate.
		//.....  
		//.....  Return: vector EP in world coordinate 
		//.....  
	Vector3	    getDirectionFromCamera ( Vector3 P ) ;

		//----- return eye vector of camera
	Vector3     getEyeVector() const { return eyeVector ; }

		//----- Decide if a given plane with given normal vector "n" 
		//.....  and with point P on it
		//.....  shows frontside, backside or only edges.
		//.....  point P and vector n are to given in world coord.
		//.....  Return  1 : front 
		//.....         -1 : back  
		//.....          0 : edges 
	int	frontOrBack( Vector3 n, Vector3 P , double epsilon = 1.0e-6 );


		//-----
	double	mapSphereSizeToScreen( double size_3d , Vector3 P, double epsilon3d );


} ; // Perspective

////////////////////////////////////////
#endif
///////////////////// end of camera.h
