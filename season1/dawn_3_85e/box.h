///////////////////////////////
///// box.h   1995.11.23  /////
///////////////////////////////

#if !defined BOX_H
#define BOX_H


#include "vector.h"


	//---------------------//
	//----- class Box -----//
	//---------------------//
class Box {
	double left ;
	double bottom ;
	double right ;
	double top ;

 public:

		//----- constructor 1 
	Box(	double xmin = 0.0 , 
		double ymin = 0.0 , 
		double xmax = 0.0 , 
		double ymax = 0.0  ) 
	{ 
		set( xmin, ymin , xmax, ymax ) ;
	}

		//----- constructor 2 
		//..... corner0: left-bottom point
		//..... corner2: right-top   point
	Box(	const Vector2&	corner0 , const Vector2& corner2 )
	{ 
		set( corner0, corner2 );
	}

		//----- define box 1
	void	set(	double xmin = 0.0 , 
			double ymin = 0.0 , 
			double xmax = 0.0 , 
			double ymax = 0.0  ) 
		{
			setLeft   ( xmin );	setBottom ( ymin );	
			setRight  ( xmax );	setTop    ( ymax );
		}


		//----- define box 2
		//..... corner0: left-bottom point
		//..... corner2: right-top   point
	void	set(	const Vector2& corner0 , const Vector2& corner2 )
		{
			setLeft   ( corner0.x );  setBottom ( corner0.y );
			setRight  ( corner2.x );  setTop    ( corner2.y );
		}

		//----- set each parameter of box
	void	setLeft  ( double x ) { left   = x ;}
	void	setBottom( double y ) { bottom = y ;}
	void	setRight ( double x ) { right  = x ;}
	void	setTop   ( double y ) { top    = y ;}

		//----- get each parameter of box
	double	getLeft  () const { return left   ; }
	double	getBottom() const { return bottom ; }
	double	getRight () const { return right  ; }
	double	getTop   () const { return top    ; }

		//----- calc width and height of box
	double	getWidth ()  const { return ( right - left   ) ; }
	double	getHeight()  const { return ( top   - bottom ) ; }

		//----- calc center of of box
	double	getCenterX() const { return ( 0.5 * ( right + left   ) ) ; }
	double	getCenterY() const { return ( 0.5 * ( top   + bottom ) ) ; }
	Vector2	getCenter()  const { return Vector2( getCenterX(), getCenterY() ); } 

		//----- calc each corner of box
		//..... corner0: left-bottom
		//..... corner1: right-bottom
		//..... corner2: right-top
		//..... corner3: left-top
	Vector2	getCorner0() const { return Vector2( left , bottom ) ; }
	Vector2	getCorner1() const { return Vector2( right, bottom ) ; }
	Vector2	getCorner2() const { return Vector2( right, top    ) ; } 
	Vector2	getCorner3() const { return Vector2( left , top    ) ; } 


		//----- get positions or corners
	Vector2	getCorner( int label ) const 
	{ 
		Vector2	P ;

		switch( label ) {
		 case 0 : P( left , bottom ) ; break ;
		 case 1 : P( right, bottom ) ; break ;			
		 case 2 : P( right, top    ) ; break ;
		 case 3 : P( left , top    ) ; break ;

		 default:
			cerr << "ERROR (Box::getCorner():\n";
			cerr << "      invalid label " << label << endl;
			break;
		}

		return P ;
	}

}; // Box

typedef Box Box2D ;



	//-----------------------//
	//----- class Box3D -----//
	//-----------------------//
class Box3D {
	double xmin, ymin, zmin ;
	double xmax, ymax, zmax ;

 public:

		//----- constructor  
	Box3D(	double xmin_given = 0.0 , 
		double ymin_given = 0.0 , 
		double zmin_given = 0.0 , 
		double xmax_given = 0.0 , 
		double ymax_given = 0.0 ,
		double zmax_given = 0.0   )
	{ 
		set(	xmin_given, ymin_given , zmin_given , 
			xmax_given, ymax_given , zmax_given ) ;
	}

		//----- define box 
	Box3D(	const Vector3&  min  ,
		const Vector3&  max    )
	{
		setXmin( min.x );
		setYmin( min.y );
		setZmin( min.z );

		setXmax( max.x );
		setYmax( max.y );
		setZmax( max.z );
	}

		//----- define box 
	void	set(	double xmin_given = 0.0 , 
			double ymin_given = 0.0 , 
			double zmin_given = 0.0 ,
			double xmax_given = 0.0 , 
			double ymax_given = 0.0 ,
			double zmax_given = 0.0  ) 
		{
			setXmin( xmin_given );
			setYmin( ymin_given );
			setZmin( zmin_given );

			setXmax( xmax_given );
			setYmax( ymax_given );
			setZmax( zmax_given );
		}


		//----- define box 
	void	set(	const Vector3&  min  ,
			const Vector3&  max    )
		{
			setXmin( min.x );
			setYmin( min.y );
			setZmin( min.z );

			setXmax( max.x );
			setYmax( max.y );
			setZmax( max.z );
		}


		//----- set each parameter of box
	void	setXmin  ( double x ) { xmin   = x ;}
	void	setXmax  ( double x ) { xmax   = x ;}

	void	setYmin  ( double y ) { ymin   = y ;}
	void	setYmax  ( double y ) { ymax   = y ;}

	void	setZmin  ( double z ) { zmin   = z ;}
	void	setZmax  ( double z ) { zmax   = z ;}


		//----- get each parameter of box
	double	getXmin () const { return xmin  ; }
	double	getXmax () const { return xmax  ; }

	double	getYmin () const { return ymin  ; }
	double	getYmax () const { return ymax  ; }

	double	getZmin () const { return zmin  ; }
	double	getZmax () const { return zmax  ; }



		//----- calc width and height of box
	double	getSize () const  
		{
			double	dx = xmax - xmin ;
			double	dy = ymax - ymin ;			
			double	dz = zmax - zmin ;	
			double	size = sqrt( dx*dx +  dy*dy + dz*dz );
			return size ;
		}
			

		//----- calc center of of box
	double	getCenterX() const { return ( 0.5 * ( xmin + xmax ) ) ; }
	double	getCenterY() const { return ( 0.5 * ( ymin + ymax ) ) ; }
	double	getCenterZ() const { return ( 0.5 * ( zmin + zmax ) ) ; }
	Vector3	getCenter()  const { return Vector3( getCenterX(), getCenterY(), getCenterZ() ); } 


		//----- updation
	void update( const Box3D& box3d )
	{
			//----- local
		double x , y , z ;

			//----- updat min
		x = box3d.getXmin();
		y = box3d.getYmin();
		z = box3d.getZmin();
		if( x  < xmin ) { xmin = x ; }
		if( y  < ymin ) { ymin = y ; }
		if( z  < zmin ) { zmin = z ; }

			//----- updat max
		x = box3d.getXmax();
		y = box3d.getYmax();
		z = box3d.getZmax();
		if( x  > xmax ) { xmax = x ; }
		if( y  > ymax ) { ymax = y ; }
		if( z  > zmax ) { zmax = z ; }
	}

		//----- updation
	void update( double x, double y, double z )
	{
		if( x  < xmin ) { xmin = x ; }
		if( y  < ymin ) { ymin = y ; }
		if( z  < zmin ) { zmin = z ; }

		if( x  > xmax ) { xmax = x ; }
		if( y  > ymax ) { ymax = y ; }
		if( z  > zmax ) { zmax = z ; }
	}


		//----- shift by ...
	void shiftBy( const Vector3& shift )
	{
		xmin += shift.x ;
		ymin += shift.y ; 
		zmin += shift.z ; 

		xmax += shift. x ; 
		ymax += shift. y ;
		zmax += shift. z ; 
	}


///////////////////////////////////////
//		//----- get positions or corners
//	Vector3	getCorner( int label ) const 
//	{ 
//		Vector3	P ;
//
//		switch( label ) {
//		 case 0 : P( xmin, ymin, zmin ) ; break ;
//		 case 1 : P( xmax, ymin, zmin ) ; break ;			
//		 case 2 : P( xmax, ymax, zmin ) ; break ;
//		 case 3 : P( xmin, ymax, zmin ) ; break ;
//		 case 4 : P( xmin, ymin, zmax ) ; break ;
//		 case 5 : P( xmax, ymin, zmax ) ; break ;			
//		 case 6 : P( xmax, ymax, zmax ) ; break ;
//		 case 7 : P( xmin, ymax, zmax ) ; break ;
//		 default:
//			cerr << "ERROR (Box3D::getCorner():\n";
//			cerr << "      invalid label " << label << endl;
//			break;
//		}
//
//		return P ;
//	}
////////////////////////////////

		//----- 
	void operator = ( const Box3D& rhs )
	{
		this->setXmin  ( rhs.xmin );
		this->setXmax  ( rhs.xmax );

		this->setYmin  ( rhs.ymin );
		this->setYmax  ( rhs.ymax );

		this->setZmin  ( rhs.zmin );
		this->setZmax  ( rhs.zmax );
	}


		//----- 
	void clear() { this->set(); }


}; // Box3D


class ToolBox3D 	
{

public:

	static void GetCorner (	int		label , 
				const Box3D&	box3d , 
				Vector3&	P)  
	{
		double	xmin = box3d.getXmin () ;
		double	xmax = box3d.getXmax () ;

		double	ymin = box3d.getYmin () ;
		double	ymax = box3d.getYmax () ;

		double	zmin = box3d.getZmin () ;
		double	zmax = box3d.getZmax () ;

		if        ( label == 0 ) {
			P( xmin, ymin, zmin ) ;

		} else if ( label == 1 ) {
			P( xmax, ymin, zmin ) ;

		} else if ( label == 2 ) {
			P( xmax, ymax, zmin ) ; 

		} else if ( label == 3 ) {
			P( xmin, ymax, zmin ) ;	

		} else if ( label == 4 ) {
			P( xmin, ymin, zmax ) ; 

		} else if ( label == 5 ) {
			P( xmax, ymin, zmax ) ; 

		} else if ( label == 6 ) {
			P( xmax, ymax, zmax ) ; 

		} else if ( label == 7 ) {
			P( xmin, ymax, zmax ) ; 
		} else {
			P( 0.0, 0.0, 0.0 ) ; 
			cerr << "ERROR (ToolBox3D::GetCorner():\n";
			cerr << "      invalid label " << label << endl;
		}

///////////////////////// HP aCC cannot accept below???
//		switch( label ) {
//		 case 0 : P( xmin, ymin, zmin ) ; break ;
//		 case 1 : P( xmax, ymin, zmin ) ; break ;			
//		 case 2 : P( xmax, ymax, zmin ) ; break ;
//		 case 3 : P( xmin, ymax, zmin ) ; break ;
//		 case 4 : P( xmin, ymin, zmax ) ; break ;
//		 case 5 : P( xmax, ymin, zmax ) ; break ;			
//		 case 6 : P( xmax, ymax, zmax ) ; break ;
//		 case 7 : P( xmin, ymax, zmax ) ; break ;
//		 default:
//			cerr << "ERROR (ToolBox3D::GetCorner():\n";
//			cerr << "      invalid label " << label << endl;
//			break;
//		}
////////////////////////////////////////////////////////
	}
};


///////////////// end of bounding_box.h
#endif
