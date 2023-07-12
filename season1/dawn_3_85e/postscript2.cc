////////////////////////////////////////////////////
//////////  postscript2.cc    1997/11/28   /////////
////////////////////////////////////////////////////

#include "postscript.h"


	//----- PostScript::toLandscape_vp()
Vector2 
PostScript::toLandscape_vp( const Vector2& point_vp )
{
		//----- local
	Vector2	point_rotated ;

		//----- locate (+90 deg)
	point_rotated.x = (-1.0) * (point_vp.y) ;
	point_rotated.y =  point_vp.x ;

		//----- output		
	return point_rotated ;

} // PostScript::toLandscape_vp()


		//-----PostScript::drawLineSegment_landscap_vp()
void	PostScript::drawLineSegment_landscape_vp(	const Vector2& P_vp, 
							const Vector2& Q_vp, 
							LineType* linetype_p ) 
{
		//----- skip drawing invisible line
	if( (linetype_p->getVisibility()) == 0 ) { return ; }

		//----- rotate vertices
	Vector2  P_rotated ; P_rotated = toLandscape_vp( P_vp ) ;
	Vector2  Q_rotated ; Q_rotated = toLandscape_vp( Q_vp ) ;

		//----- set line type
	setLineType( linetype_p ) ;

		//----- create a  new path
	Path_vp( P_rotated, Q_rotated ) ;
	
		//----- draw
	*out_p << "DRAWPATH" << endl;

} // PostScript::drawLineSegment_landscape_vp()


	//----- void PostScript::drawPolygon_landscape_vp()
void PostScript::drawPolygon_landscape_vp(	Vector2* point , 
						int num_point  , 
						LineType&  linetype )
{
		//----- invisible linetype?
	if( (linetype.getVisibility()) == 0 ) {
		return ;
	}

		//----- local variables
	int		i;
	Vector2*	point_rotated = new Vector2[ num_point ] ; 

		//----- set line type
	setLineType( &linetype ) ;

		//----- rotate vertices
	for( i = 0 ; i < num_point ; i++ ) {
		point_rotated[i] = toLandscape_vp( point[i] ) ;
	}

		//----- draw polygon
	Path_vp( point_rotated, num_point );
	*out_p << "DRAWPOLYGON" << endl ;		

		//----- 
	delete [] point_rotated ;

} //PostScript::drawPolygon_landscape_vp()


	//----- PostScript::drawPolygon_landscape_vp( LineTypeAddressArray )
void PostScript::drawPolygon_landscape_vp(	Vector2* point , 
						int num_point , 
						LineType** linetypeAddressArray ) 
{
		//----- local variables
	int	i, i_plus ;
	Vector2  P_rotated ;
	Vector2  Q_rotated ;

		//----- make path
	for( i = 0 ; i < num_point ; i++ ) {
		i_plus = i + 1 ;	
		if( i_plus == num_point ) { i_plus = 0 ; }
		
		P_rotated = toLandscape_vp( point[i      ] ) ;
		Q_rotated = toLandscape_vp( point[i_plus ] ) ;

		drawLineSegment_vp(	P_rotated    , Q_rotated  , 
					linetypeAddressArray[i] ) ;
	}

} // PostScript::drawPolygon_landscape_vp(LineType**)


	//----- PostScript::drawPolygon_landscape_vp( LineTypeArray )
void PostScript::drawPolygon_landscape_vp(	Vector2* point , 
						int num_point , 
						LineType* linetypeArray ) 
{
		//----- local variables
	int	i, i_plus ;
	Vector2  P_rotated ;
	Vector2  Q_rotated ;

		//----- make path
	for( i = 0 ; i < num_point ; i++ ) {
		i_plus = i + 1 ;	
		if( i_plus == num_point ) { i_plus = 0 ; }

		P_rotated = toLandscape_vp( point[i      ] ) ;
		Q_rotated = toLandscape_vp( point[i_plus ] ) ;
		
		drawLineSegment_vp(	P_rotated, Q_rotated, 
					&(linetypeArray[i])       ) ;
	}


} // PostScript::drawPolygon_landscape_vp(LineType*)


	//----- 
void	PostScript::fillPolygon_landscape_vp(	Vector2* point, 
						int num_point , 
						ColorType*  color_p )
{
		//----- invisible linetype?
	if( (color_p->getVisibility()) == 0 ) {
		return ;
	}

		//----- local variables
	int	i;
	Vector2* point_rotated = new Vector2[num_point] ;

		//----- set color
	setRGB(	color_p->getRed(), 
		color_p->getGreen(), 
		color_p->getBlue() ); 

		//----- rotate vertices
	for( i = 0 ; i < num_point ; i++ )
		point_rotated[i] = toLandscape_vp( point[i] ) ;

		//----- fill polygon
	Path_vp( point_rotated, num_point ) ;
	*out_p << "FILLPOLYGON" << endl ;		

		//----- 
	delete [] point_rotated ;

} // PostScript::fillPolygon_landscape_vp()


		//----- PostScript::drawfillPolygon_landscape_vp()
void	PostScript::drawfillPolygon_landscape_vp(	Vector2* point, 
							int num_point , 
							ColorType*  color_p )
{
		//----- invisible linetype?
	if( (color_p->getVisibility()) == 0 ) {
		return ;
	}

		//----- local variables
	int	i;
	Vector2* point_rotated = new Vector2[num_point] ;


		//----- set line type
	setRGB(	color_p->getRed(), 
		color_p->getGreen(), 
		color_p->getBlue() ); 
	setLineWidth( 0.0 ) ;
	setLineStyleToSolid();

		//----- rotate vertices
	for( i = 0 ; i < num_point ; i++ ) {
		point_rotated[i] = toLandscape_vp( point[i] ) ;
	}

		//----- draw and fill polygon
	Path_vp( point_rotated, num_point ) ;
	*out_p << "DRAWFILLPOLYGON" << endl ;		

		//-----
	delete [] point_rotated ;

} // PostScript::drawfillPolygon_landscape_vp()


		//----- PostScript::fillCircle_landscape_vp()
void	PostScript::fillCircle_landscape_vp( const Vector2& center_vp, double radius_vp , ColorType* color_p )
{
		//----- invisible linetype?
	if( (color_p->getVisibility()) == 0 ) {
		return ;
	}

		//----- local variables
	Vector2	center_ls_vp ;
	Vector2	center_pt    ;
	double	radius_pt    ;

		//----- rotate position
	center_ls_vp  = toLandscape_vp( center_vp ) ;

		//----- units
	positionVpToPt( center_ls_vp , &center_pt ) ;
	radius_pt = lengthVpToPt ( radius_vp );

		//----- set color
	setRGB(	color_p->getRed  (), 
		color_p->getGreen(), 
		color_p->getBlue () ); 

		//----- draw circle
	*out_p  << "newpath " ;
	*out_p  << (center_pt.x) << " " << (center_pt.y) << " " ;
	*out_p  << radius_pt     << " " ;
	*out_p  << "0 360  arc fill" << endl ;		

} // PostScript::fillCircle_landscape_vp()


	//----- PostScript::fillSquare_landscape_vp()
void	PostScript::fillSquare_landscape_vp(	const Vector2&	center_vp  ,
						double		dL_vp      ,
						ColorType*	color_p     )
{
		//----- invisible linetype?
	if( (color_p->getVisibility()) == 0 ) {
		return ;
	}

		//----- local variables
	Vector2	center_ls_vp ;
	Vector2	center_pt ;
	double	dL_pt ;

		//----- rotate 
	center_ls_vp  = toLandscape_vp( center_vp ) ;

		//----- units
	this->positionVpToPt     ( center_ls_vp , &center_pt ) ;
	dL_pt = lengthVpToPt ( dL_vp );

		//----- set color
	setRGB(	color_p->getRed  (), 
		color_p->getGreen(), 
		color_p->getBlue () ); 

		//----- fill square
	double xmin = center_pt.x - dL_pt ; 
	double xmax = center_pt.x + dL_pt ; 
	double ymin = center_pt.y - dL_pt ; 
	double ymax = center_pt.y + dL_pt ; 

	*out_p  << "newpath "  << endl;
	*out_p  << xmin << " " << ymin << "  moveto" << endl;
	*out_p  << xmax << " " << ymin << "  lineto" << endl;
	*out_p  << xmax << " " << ymax << "  lineto" << endl;
	*out_p  << xmin << " " << ymax << "  lineto" << endl;
	*out_p  << "fill" << endl ;		

} // PostScript::fillSquare_landscape_vp()


	//----- FRPostScript::drawColoredString_landscape_vp ()
void PostScript::drawColoredString_landscape_vp (	const char* str, 
							const ColorType& color ,
							double size_vp         ,
							double offset_x_vp     ,
							double offset_y_vp     ,
							double X_vp  ,         
							double Y_vp  )  

{
		//----- invisible linetype?
	if( ( color.getVisibility() ) == 0 ) {
		return ;
	}

		//----- local
	Vector2 P_vp( X_vp, Y_vp ), P_ls_vp, P_ls_pt;
	int	size_pt , offset_x_pt, offset_y_pt ;

		//----- rotate
	P_ls_vp  = toLandscape_vp( P_vp ) ;

		//----- units
	this->positionVpToPt     ( P_ls_vp , &P_ls_pt ) ;
	size_pt   = (int) (lengthVpToPt   ( size_vp      ) );
	offset_x_pt = (int) (lengthVpToPt   ( offset_x_vp    ) ) ;
	offset_y_pt = (int) (lengthVpToPt   ( offset_y_vp    ) ) ;

		//----- moveto starting position of string
	moveto_pt( (P_ls_pt.x + offset_x_pt) , (P_ls_pt.y+ offset_y_pt ) );

		//----- set font and color
	setFontSize ( size_pt );
	setRGB(color.getRed(), color.getGreen(), color.getBlue());

		//----- draw string
	*out_p  << "("<< str <<")" << "show" << endl;

} // FRPostScript::drawColoredString_landscape_vp ()
