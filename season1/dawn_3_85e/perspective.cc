//////////////////////////////
/////	perspective.cc   /////
//////////////////////////////


// #define DEBUG_PERSPECTIVE



	//--------------------------------//
	//----- INCLUDE HEADER FILES -----//
	//--------------------------------//
#include <iostream.h>	
#include <stdlib.h>	
#include <string.h>
#include "perspective.h"

	//-----------------------------------------------------------//
	//----- protected member functions of class Perspective -----//
	//-----------------------------------------------------------//

	//----- Perspective::setYup()
void	Perspective::setYup() 
{
	upDirection = 'Y' ;

/////////////////////////////////////////////////////////////////
// #if !defined USE_VC
//	cur_viewing_func     = &(Perspective::viewing_Yup);
//	cur_viewing_rev_func = &(Perspective::viewingReverse_Yup);
// #endif
//////////////////////////////////////////////////////////////////

}

	//----- Perspective::setZup()
void	Perspective::setZup()
{
	upDirection = 'Z' ;
////////////////////////////////////////////////////////////////////
//#if !defined USE_VC
//	cur_viewing_func     = &(Perspective::viewing_Zup);
//	cur_viewing_rev_func = &(Perspective::viewingReverse_Zup);
//#endif
////////////////////////////////////////////////////////////////////
}

	//----- Perspective::setViewingMatrix()
void  Perspective::setViewingMatrix()
{
		//----- set eyeVector 
	setEyeVector() ;

		//----- set eye position E (camera_world coord)
	setEyePosition();

		//----- set viewing matrix elements
	if( isYup() ) {
		setViewingMatrix_Yup();
			// set wij

		v11 = v12 =  v13 = 0.0 ;
		v21 = v22 =  v23 = 0.0 ;
		      v32 =  v33 = 0.0 ;
		             v43 = 0.0 ;
	} else {
		setViewingMatrix_Zup();
			// set vij

		w11 = w12 = w13 = 0.0 ;
		      w22 = w23 = 0.0 ;
		w31 = w32 = w33 = 0.0 ;
		w41 = w42 = w43 = 0.0 ;
	} 

} 



	//----- Perspective::setEyeVector()
void  Perspective::setEyeVector() 
{
	eyeVector( (- sin_phi * cos_theta ) , \
		   (- sin_phi * sin_theta ) , \
		   (- cos_phi             )  );
	eyeVector = eyeVector.normalize() ; 
}


void	Perspective::setEyePosition() 
{
	E.x = rho * sin_phi * cos_theta ;
	E.y = rho * sin_phi * sin_theta ;
	E.z = rho * cos_phi             ;
}

void Perspective::setViewingMatrix_Yup()
{
	const	double plus_zero = 1.0e-6 ;
	double	rho_xz \
		= sqrt ( sin_phi * sin_phi * cos_theta * cos_theta + cos_phi * cos_phi );

	double	cos_a =  0.0 ;   // a = 270 deg if rho_xz = 0.0
	double	sin_a = -1.0 ; 

	if ( fabs (rho_xz ) >  plus_zero ) { cos_a = cos_phi / rho_xz ; }
	if ( fabs (rho_xz ) >  plus_zero ) { sin_a = sin_phi * cos_theta / rho_xz; }

	double	cos_b = sin_phi * sin_theta ;
	double	sin_b = rho_xz ;

	w11 =   cos_a ;
	w12 = - sin_a * cos_b ;
	w13 = - sin_a * sin_b ;

	w22 =           sin_b ;
	w23 =         - cos_b ;

	w31 = - sin_a ;
	w32 = - cos_a * cos_b ;
	w33 = - cos_a * sin_b ;

	w41 = - E.x * cos_a                       + E.z * sin_a ;
	w42 =   E.x * sin_a * cos_b - E.y * sin_b + E.z * cos_a * cos_b ;
	w43 =   E.x * sin_a * sin_b + E.y * cos_b + E.z * cos_a * sin_b ;

}


void  Perspective::setViewingMatrix_Zup()
{
		//----- calc viewing matrix elements
	v11 = -	sin_theta ;
	v12 = - cos_phi * cos_theta ;
	v13 = - sin_phi * cos_theta ;

	v21 =	cos_theta ;
	v22 = - cos_phi * sin_theta ;
	v23 = - sin_phi * sin_theta ;

	v32 =   sin_phi ;
	v33 = - cos_phi ;

	v43 = rho ;
}



	//----- worldToScreen() = (1) + (2) + (3)

	//-----  (1) world coordinate --> camera-world coordinate
Vector3	Perspective::resetWorldOrigin ( const Vector3& world )
{
	Vector3	w = world ;
	return( w - target ) ;

} // Perspective::resetWorldOrigin()


	//----- (2) viewing transformation
	//.....     camera-world coordinate --> eye coordinate
void  Perspective::viewing ( Vector3* coord_3 )
{
/////////////////////////////
//#if defined USE_VC
/////////////////////////////

	cur_viewing_func( coord_3 ) ;

/////////////////////////////////////////////
//#else
//	(this->*cur_viewing_func)( coord_3 ) ;
//#endif
////////////////////////////////////////////
}


void  Perspective::viewing_Yup ( Vector3* coord_3 )
{
		//----- camera-world coordinate
	Vector3	wc = *coord_3 ;

		//----- eye_coord = camera_world_coord * V
	coord_3->x = w11 * wc.x                 +  w31 * wc.z  +  w41 ;
	coord_3->y = w12 * wc.x  +  w22 * wc.y  +  w32 * wc.z  +  w42 ;
	coord_3->z = w13 * wc.x  +  w23 * wc.y  +  w33 * wc.z  +  w43 ;
}

void  Perspective::viewing_Zup ( Vector3* coord_3 )
{
		//----- camera-world coordinate
	Vector3	wc = *coord_3 ;

		//----- eye_coord = camera_world_coord * V
	coord_3->x = v11 * wc.x  +  v21 * wc.y			      ;
	coord_3->y = v12 * wc.x  +  v22 * wc.y  +  v32 * wc.z         ;
	coord_3->z = v13 * wc.x  +  v23 * wc.y  +  v33 * wc.z  +  v43 ;

} // Perspective::viewing_Zup()


	//----- (3) perspective transformation
	//.....     eye coordinate --> 3D screen coordinate
	//.....     This function is not the construntor Perspective().
int  Perspective::perspective ( Vector3* coord_3 , double epsilon3d )
{
		//----- local variables
	int		status ;
	Vector3		eye = *coord_3 ;	// eye coordinate

		//----- error checking
	if( eye.z < epsilon3d ) { 
		status = 0 ; 
		cerr << "ERROR: You tried to draw an object behind your camera position." << endl;
		cerr << "       Try again with a larger (or AUTO) camera distance." << endl;
		cerr << "       Good luck!" << endl;
		exit(1);

	} else {
			//----- eye coord --> 3D screen coord
		status = 1 ; 
		coord_3->x = focalDistance * eye.x / eye.z ;
		coord_3->y = focalDistance * eye.y / eye.z ;
		coord_3->z = eye.z			  ;
	}
	
		//----- return
	return status ;
	
} // Perspective::perspective()


	//----- eye coordinate --> camera_world coordinate 
void  Perspective::viewingReverse ( Vector3* coord_3 )
{
////////////////////////////////////////////
//#if defined USE_VC
////////////////////////////////////////////

	cur_viewing_rev_func         ( coord_3 ) ;

//////////////////////////////////////////////////////
//#else
//	(this->*cur_viewing_rev_func)( coord_3 ) ;
//#endif
//////////////////////////////////////////////////////
}

void  Perspective::viewingReverse_Yup ( Vector3* coord_3 )
{
		//----- local variables
	Vector3	eye = *coord_3 ;	// eye coordinate

		//----- camera_world_coord = eye_coord * V^{-1}
	coord_3->x = eye.x * w11 + eye.y * w12 + eye.z * w13  + E.x ;
	coord_3->y =               eye.y * w22 + eye.z * w23  + E.y ;
	coord_3->z = eye.x * w31 + eye.y * w32 + eye.z * w33  + E.z ;

}

void  Perspective::viewingReverse_Zup ( Vector3* coord_3 )
{
		//----- local variables
	Vector3	eye = *coord_3 ;	// eye coordinate
	
		//----- camera_world_coord = eye_coord * V^{-1}
	coord_3->x = eye.x * v11 + eye.y * v12 + (eye.z - rho) * v13 ;
	coord_3->y = eye.x * v21 + eye.y * v22 + (eye.z - rho) * v23 ;
	coord_3->z =               eye.y * v32 + (eye.z - rho) * v33 ;

} // Perspective::viewingReverse()


	//---------------------------------------------------------//
	//-----  public member functions of class Perspective -----//
	//---------------------------------------------------------//

	//----- constructor
Perspective::Perspective (
				double		focalDistanceGiven , \
				double		target_x , \
				double		target_y , \
				double		target_z , \
				double		rhoGiven , \
				double		phiGiven , \
				double		thetaGiven \
) :	target ( target_x , target_y , target_z ) , \
	eyeVector(), E(), upDirection('Y'), \
	tool() 
{
//////////////////////////////////////////////////////////
//#if !defined USE_VC
//	cur_viewing_func = 0 ; cur_viewing_rev_func = 0;
//#endif
////////////////////////////////////////////////////////

		//----- Decide up direction
	if( getenv( "DAWN_UP_DIRECTION" ) != NULL ) {
		if(	!strcmp(getenv( "DAWN_UP_DIRECTION" ),"z") || \
			!strcmp(getenv( "DAWN_UP_DIRECTION" ),"Z")      )
		{
			setZup();
		} else {
			setYup();
		}
	} else {
		setYup();
	}

		//---- set focalDistance
	setFocalDistance( focalDistanceGiven ) ;

		//----- set following data members
		//.....  rho, phi, theta, 
		//.....  cos_phi, sin_phi, cos_theta, sin_theta
		//.....  vij, wij, E, eyeVector
	setCameraPosition ( rhoGiven , phiGiven , thetaGiven ) ;

	
} // Perspective::Perspective()


	//----- set screen-eye distance
void  Perspective::setFocalDistance (	double focalDistanceGiven, double epsilon )
{
	if( focalDistanceGiven <= epsilon ) { 
		focalDistance = DEFAULT_SCREEN_DISTANCE ;
	} else {
		focalDistance = focalDistanceGiven ;
	}

} // Perspective::setFocalDistance()


	//----- Perspective::setScreenRange (Vector3& )
double Perspective::setScreenRange (	Vector3&  P , double epsilon3d , \
					double   width, double  height, \
					double   epsilon )
{
		//----- local variables
	double 		d1 = 1.0, d2 = 1.0 ; // temporary screen distance
	double 		d ;		// resultant screen distance
	Vector3		P_eye ;		// P in eye coordinate
	int		flag_x = 0 ;	// Is d1 calculated?
	int		flag_y = 0 ;	// Is d2 calculated?

		//----- calc eye coordinate
	worldToEye( P, P_eye ); 

		//----- calc candidates of screen distance
		//----------- calc d1 from P_eye.x
	if( fabs( P_eye.x ) > epsilon ) { 
		d1 = P_eye.z * 0.5 * width  / fabs( P_eye.x ) ;
		flag_x = 1 ; // d1 is calculated
	} 

		//----------- calc d2 from P_eye.y
	if( fabs( P_eye.y ) > epsilon ) { 
		d2 = P_eye.z * 0.5 * height / fabs( P_eye.y ) ;
		flag_y = 1 ; // d2 is calculated
	}

		//----- select resultant screen distance
	if( flag_x == 1 && flag_y == 1 ) {
		d  = ( d1 <= d2 ? d1 : d2 );
		setFocalDistance( d,  epsilon3d );
	} else if ( flag_x == 1 && flag_y == 0 ) {
		setFocalDistance( d1, epsilon3d );
	} else if ( flag_x == 0 && flag_y == 1 ) {
		setFocalDistance( d2, epsilon3d );
	} else { // flag_x == 0 && flag_y == 0
		// Do nothing, and keep 
		// old focalDistance
	}

		//----- return screen distance
	return focalDistance ;
	
} // Perspective::setScreenRange ( Vector3& )


	//----- Perspective::setScreenRange ( Vector3* point , int num_point )
double Perspective::setScreenRange (	Vector3*  P , int num_point , \
					double    epsilon3d         , \
					Vector3*  initial_point_p , \
					double   width, double  height, \
					double   epsilon )
{
		//----- local variables
	int	i ;
	double	d, d_tmp ;
		
		//----- initialize screen distance
	if( initial_point_p != NULL ) {
		d = this->setScreenRange( *initial_point_p, epsilon3d, width, height, epsilon );
	} else {
			//----- if initial_point_p is NULL...
		d = focalDistance ;
	}

		//----- calc minimal screen distance
	for( i = 0 ; i < num_point ; i++ ) {
		d_tmp = setScreenRange( P[i], epsilon3d, width, height , epsilon );
		if( d_tmp < d ) { d = d_tmp ;}
	}
	
		//-----	set screen distance
	setFocalDistance( d, epsilon3d );

		//----- return screen distance
	return focalDistance ;

} // Perspective::setScreenRange ( Vector3* point , int num_point ))


	//----- Perspective::setScreenRange ( Vector3** point , int num_point )
double Perspective::setScreenRange (	Vector3**  P , int num_point ,\
					double    epsilon3d         , \
					Vector3*  initial_point_p , \
					double   width, double  height, \
					double   epsilon )
{
		//----- local variables
	int	i ;
	double	d, d_tmp ;
		
		//----- initialize screen distance
	if( initial_point_p != NULL ) {
		d = this->setScreenRange( *initial_point_p, epsilon3d , width, height, epsilon );
	} else {
			//----- if initial_point_p is NULL...
		d = focalDistance ;
	}

		//----- calc minimal screen distance
	for( i = 0 ; i < num_point ; i++ ) {
		d_tmp = setScreenRange( *P[i], epsilon3d , width, height , epsilon );
		if( d_tmp < d ) { d = d_tmp ;}
	}
	
		//-----	set screen distance
	setFocalDistance( d, epsilon3d );

		//----- return screen distance
	return focalDistance ;

} // Perspective::setScreenRange ( Vector3** point , int num_point ))


	//----- set target point of camera
void  Perspective::setTarget ( Vector3 targetGiven )
{
	target   = targetGiven ;

} // Perspective::setTarget()


	//----- set camera position ( camera distance from target point )
void  Perspective::setRho ( double rhoGiven, double epsilon )
{
	if( rhoGiven < epsilon ) { 
		rho = DEFAULT_CAMERA_POSITION_RHO ;
	} else {
		rho = rhoGiven ;
	}

	setViewingMatrix() ;

} // Perspective::setRho()


	//----- set camera position ( camera angle from z-axis of
	//.....                       world coordinate )
void  Perspective::setPhi ( double phiGiven )
{
	phi = phiGiven ;
	cos_phi = tool.Cos( phi ) ;		// Cos: cosine function in degrees
	sin_phi = tool.Sin( phi ) ;		// Sin: sine function in degrees
	setViewingMatrix() ;

} // Perspective::setPhi()


	//----- set camera position ( camera angle from x-axis of
	//.....                       world coordinate )
void  Perspective::setTheta ( double thetaGiven )
{
	theta = thetaGiven ;
	cos_theta = tool.Cos( theta ) ;		// Cos: cosine function in degrees
	sin_theta = tool.Sin( theta ) ;		// Sin: sine function in degrees
	setViewingMatrix() ;

} // Perspective::setTheta()


	//----- set camera position (viewpoint)
	//.....  rho  : camera distance from target point
	//.....  phi  : camera angle from z-axis of world coordinate
	//.....  theta: camera angle from x-axis of world coordinate
	//.....
	//.....  Modified:
	//.....   rho, phi, theta, cos_phi, sin_phi, cos_theta, sin_theta,
	//.....   vij, eyeVector.
void  Perspective::setCameraPosition (	double rhoGiven , \
					double phiGiven , \
					double thetaGiven )
{
		//----- set distance( camera, target )
	rho = rhoGiven ;	

		//----- set angel from z-axis of camera-world coordinate
	phi = phiGiven ;	
	cos_phi = tool.Cos( phi ) ;		// Cos: cosine function in degrees
	sin_phi = tool.Sin( phi ) ;		// Sin: sine function in degrees

		//----- set angel from x-axis of camera-world coordinate
	theta = thetaGiven ;
	cos_theta = tool.Cos( theta ) ;		// Cos: cosine function in degrees
	sin_theta = tool.Sin( theta ) ;		// Sin: sine function in degrees

		//----- calc viewing matrix elements "vij" and "eyeVector"
	setViewingMatrix() ;

} // setCameraPosition()


	//----- getCameraPosition()
Vector3 Perspective::getCameraPosition() const 
{
		//----- local variables
	Vector3 camera_position ;

		//----- camera position in camera-world coordinate
	camera_position = (-1.0) * rho * eyeVector ;

		//----- camera position in world coordinate
	camera_position = camera_position + target ;

		//----- return camera position in world coordinate
	return camera_position ;

} // Perspective::getCameraPosition()
	

	//----- world coordinate --> eye coordinate
void  Perspective::worldToEye ( const Vector3& world , Vector3& eye ) 
{
						// world coord
	eye = resetWorldOrigin ( world ) ;	// --> camera-world coord
	viewing( &eye ) ;			// --> eye coord

} // Perspective::worldToEye ( world , eye )


	//----- world coordinate --> 3D screen coordinate
int  Perspective::worldToScreen ( const Vector3& world , Vector3& screen_3d, double epsilon3d )
{
		//----- local variables
	int status ;	// return value

		//----- coordinate transformations
						       // world coord
	screen_3d = resetWorldOrigin ( world ) ;       // --> camera-world coord
	viewing	( &screen_3d ) ; 		       // --> eye          coord
	status = perspective ( &screen_3d, epsilon3d );// --> 3D screen    coord

		//----- error message
	if( !status ) {
#if defined DEBUG_PERSPECTIVE
		cerr	<< "WARNING (Perrspective::worldToScreen()):\n" ;
		cerr    << "    Some vertices are behind the given camera position.\n";
#endif
//		exit(1) ;
	}

		//----- RETURN
	return status ;

} // Perspective::worldToScreen ( world , screen_3d )


	//----- world coordinate --> 2D screen coordinate
int  Perspective::worldToScreen ( const Vector3& world , Vector2& screen_2d, double epsilon3d )
{
		//----- local variables
	int		status ;	// return value
	Vector3		eye ;	

		//----- coordinate transformations
					   // world coord
	eye = resetWorldOrigin ( world ) ; // --> camera-world coord
	viewing	( &eye ) ; 		   // --> eye          coord

		//----- error checking and set result
	if( eye.z < epsilon3d ) { 
		status = 0 ; 

#if defined DEBUG_PERSPECTIVE
		cerr	<< "WARNINNG: (Perspective::worldToScreen( Vector2& ))\n";
		cerr	<< "     eye_coord.z = " << eye.z << endl ;
		cerr	<< "     Cannot calc screen coord.\n " ;
#endif

	} else {
			//----- eye coord --> 2D screen coord
		status = 1 ; 
		screen_2d.x = focalDistance * eye.x / eye.z ;
		screen_2d.y = focalDistance * eye.y / eye.z ;
	}

		//----- return
	return status ;

} // Perspective::worldToScreen ( world , screen_2d )



	//-----  eye coordinate   --> screen coordinate
int  Perspective::eyeToScreen( const Vector3& eye , Vector3& screen, double epsilon ) 
{
		//----- local variables
	int status ;	// return value

		//----- coordinate transformations
	screen = eye ;					// eye coord
	status = perspective( &screen, epsilon );	// screen coord
				
		//----- error message
	if( !status ) {
		cerr  << "ERROR in  Perspective::eyeToScreen() \n";
		cerr  << "     Cannot calc screen coordinate\n" ;
	}

	return status ;
}


	//----- eye coordinate --> world coordinate 
void  Perspective::eyeToWorld ( const Vector3& eye , Vector3& world ) 
{
		//----- local variables
	Vector3		camera_world = eye ;	// eye coord

		//----- eye coord --> camera-world coord 
		//.....  camera_world_coord = eye_coord * V^{-1}
	viewingReverse( &camera_world );	// camera-world coord

		//----- camera_world coord --> world coord 
	world = camera_world + target ;		// world coord

} // Perspective::eyeToWorld ( eye , world )



	//-----  screen coordinate --> world coordinate
int  Perspective::screenToWorld ( const Vector3& screen , Vector3& world , double epsilon )
{
		//----- local variables 
	int	status ;
	Vector3 eye ;

		//----- screen_coord --> eye_coord 
	status = screenToEye( screen , eye , epsilon );	// eye coord

		//----- eye_coord    --> world coord
	if( status ) { 
		eyeToWorld ( eye , world ) ; 		// world coord
	} else {
		cerr	<< "ERROR in  Perspective::screenToWorld() \n";	
		cerr	<< "     Cannot calc world coordinate\n" ;
	}

		//----- return 
	return status ;
} 


	//----- screen coordinate --> eye coordinate
int  Perspective::screenToEye( const Vector3& screen , Vector3& eye, double epsilon )
{
		//----- local variables
	double	ratio ; // z_e / d
	int	status ;

		//----- error checking
	if( focalDistance <= epsilon ) { 
		cerr << "ERROR in  Perspective::screenToEye()\n";
		cerr << "     Wrong screen distance from view point.\n";
		cerr << "     focalDistance = " << focalDistance << endl;
		status =  0 ;
	} else {
			//----- x_e = (z_e / d ) * x_s 
			//..... y_e = (z_e / d ) * y_s 
			//..... z_e = z_s ;
		eye.z = screen.z ;
		ratio = ( eye.z / focalDistance );
		eye.x = ratio * screen.x ;
		eye.y = ratio * screen.y ;
		status = 1 ;
	}

		//----- RETURN
	return status;

} // Perspective::screenToEye()



	//----- Get z-coordinate :
	//.....  Get z-coordinate in eye coordinate
	//.....  for a point given in world coordinate
double Perspective::eyeZ ( const Vector3& world )
{
	Vector3	wc = resetWorldOrigin( world ) ;

	double	ze ;

	ze =	 (- sin_phi * cos_theta ) * wc.x  \
		+(- sin_phi * sin_theta ) * wc.y  \
		+(- cos_phi )             * wc.z 
		+  rho ;

	return ze ;

} // Perspective::eyeZ ( Vector3 )


	//----- getDirectionFromEye()
Vector3	    Perspective::getDirectionFromCamera ( Vector3 P )
{
    //----- local variables 
    //.....  E: position of camera ( eye )
    //.....  T: target point of camera
    //.....  P: given point
    Vector3 vecET, vecTP ;

    //----- calc vector ET and vector TP
    vecET = rho * eyeVector ;
    vecTP = P - target ;

    //----- return vector EP = ET + EP
    return ( vecET + vecTP ) ;

} // Perspective::getDirectionFromCamera()


	//-----	frontOrBack()
int	Perspective::frontOrBack( Vector3 n, Vector3 P , double epsilon )
{
	//----- local constants and variables
	Vector3	vecEP, vecEP_normalized ;
	double	prod ;
	
	//----- direction vector pointing given point from eye
	vecEP = this->getDirectionFromCamera ( P ) ;
	vecEP_normalized = vecEP.normalize();

	//----- decide side of the given plane
	//.....  seeing from camera
	prod  = vecEP_normalized * n ;
	
	if( prod < -epsilon ) {
		return 1 ;	// shows front side
	} else if( prod > epsilon ) {
		return -1 ;	// shows back side
	} else {
		return 0 ;	// shows only edges
	}

}  // Perspective::frontOrback()


	//----- Perspective::mapSphereSizeToScreen()
double	Perspective::mapSphereSizeToScreen( double size_3d , Vector3 P, double epsilon3d )
{
  double size_on_screen = 0.0 ;
  double ze = eyeZ(P) ;
  
  if ( ze > epsilon3d ) {
    size_on_screen = size_3d * focalDistance / ze ;
  }

  return size_on_screen ;
}

////////////////////////
// #if defined USE_VC
/////////////////////////

	//----- Perspective::cur_viewing_func()
void	Perspective::cur_viewing_func     (Vector3* P )
{
	if (isYup()) {
		viewing_Yup( P ) ;
	} else {
		viewing_Zup ( P );
	}
}

	//----- Perspective::cur_viewing_rev_func()
void	Perspective::cur_viewing_rev_func     ( Vector3* P )
{
	if (isYup()) {
		viewingReverse_Yup ( P );
	} else {
		viewingReverse_Zup ( P );
	}
}
////////////////////////////
//#endif
///////////////////////////
