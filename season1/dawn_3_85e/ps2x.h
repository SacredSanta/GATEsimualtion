/***********************************************************
 *	ps2x.h                                             *
 *                                                         *
 *      1996/1/11        Yoshitaka Kawabata                *
 ***********************************************************/

#ifndef PS2X_H
#define PS2X_H

#include "dev_control.h"
#if defined USE_XWIN

#include <X11/Xlib.h>
#include "x_window.h"


typedef struct
{
	int	orient ;
	float	llx, lly, urx, ury, xdpi, ydpi ;
//	unsigned int width, height ;
} GSContext_t, *GSContext ;


	//----------------------//
	//----- class PS2X -----//
	//----------------------//
class PS2X {

	GSContext	ctx ;

	Bool		Start( Display*, Window ) ;
	Bool		End() ;

	int		flag_open ;

 public:
	PS2X() ;
	~PS2X(){ ; }

	void		Open (	Display	*dpy ,
				Window	win  ,
				double	xwin_width_pt = A4_WIDTH_PT, 
				double	xwin_height_pt = A4_HEIGHT_PT ) ;
	void		Close()	;

	GSContext	CreateContext( int, float, float, float, float, float, float ) ;
	void		DestroyContext() ;

	int		isOpen() const { return flag_open ; }

} ;

#endif // USE_XWIN

#endif // END of ps2x.h

