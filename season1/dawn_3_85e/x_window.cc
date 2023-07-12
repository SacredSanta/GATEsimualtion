/////////////////////////////////////////////////////////
/////		x_window.cc	1995/05/15         //////
/////////////////////////////////////////////////////////

#include "dev_control.h"
#if defined USE_XWIN

#include <stdio.h>
#include <stdlib.h>

#include "x_window.h"


	//----- X_Window::open() 
void
X_Window::open( const char* title , double width , double height ) 
{
		//----- set flag
	flag_open = 1 ;

		//----- Window Attributes 
	XSetWindowAttributes	xwin_att ;
	xwin_att.backing_store = Always ;

		//----- display ID
	if((dpy = XOpenDisplay(NULL)) == NULL)
	{	perror("Cannot open display!") ;
		exit(1) ;
	}

		//----- create window 
	windowID  = XCreateSimpleWindow(	dpy, RootWindow(dpy, 0),
						0, 0, (int)(width), (int)(height), 0,
						BlackPixel(dpy, 0), WhitePixel(dpy, 0) ) ;
	XChangeWindowAttributes ( dpy, windowID, CWBackingStore, &xwin_att );
	XStoreName ( dpy, windowID, title );

		//----- set Event mask of window 
	XSelectInput(	dpy, windowID,  \
			ExposureMask |	\
			ButtonPressMask  | ButtonReleaseMask |	\
			ButtonMotionMask |	\
			KeyPressMask);

		//----- Map window 
	XMapWindow( dpy, windowID ) ;	XFlush( dpy );

} // X_Window::open() 

	//----- X_Window::close() 
void
X_Window::close() 
{
		//----- destroy window 
	XDestroyWindow ( dpy, windowID );
	XCloseDisplay  ( dpy ) ;

		//----- set flag
	flag_open = 0 ;

} // X_Window::close

#endif // USE_XWIN

