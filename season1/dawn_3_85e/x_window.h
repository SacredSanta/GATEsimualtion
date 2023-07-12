////////////////////////
/////  x_window.h  /////
////////////////////////

#if !defined X_WINDOW_H
#define X_WINDOW_H

#include "dev_control.h"
#if defined USE_XWIN

	//--------------------------//
	//-----  include files -----//
	//--------------------------//
#include <X11/Xlib.h>
#include <X11/Xutil.h>


		//----- const
#include "a4size.h"
// #if !defined PARAMETER_H
//	const double  A4_WIDTH_MM  = 210.0 ;
//	const double  A4_HEIGHT_MM = 297.0 ;
//	const double  A4_WIDTH_PT  = (A4_WIDTH_MM / 25.4 * 72.0) ;
//	const double  A4_HEIGHT_PT = (A4_HEIGHT_MM / 25.4 * 72.0) ;
// #endif

	//----------------------------//
	//-----  class X_Window  -----//
	//----------------------------//

class X_Window {



		//----- display ID
	Display*	dpy ;		

		//----- window ID
	Window		windowID ;

		//----- name
	char  window_title[100] ;

		//----- status
	int	flag_open ;

 public:
		//----- Constructor and Destructor
	X_Window() { flag_open = 0 ; }
	~X_Window(){ ; }

		//----- open and close window
	void	open(	const char* title = "" , 
			double width = A4_WIDTH_PT       , 
			double height = A4_HEIGHT_PT        ) ;
	void	close() ;

		//----- erase all figures drawn in x_window
	void	clearWindow( void ) { XClearWindow( dpy, windowID ); }


		//----- get
	Display*	getDisplay()    const { return dpy      ; }
	Window		getWindow ()    const { return windowID ; }
	int		isOpen    ()    const { return flag_open ; }

		//----- set
	void		setDisplay( Display* _dpy ){ dpy = _dpy ; }
	void		setWindow( Window _win ){ windowID = _win ; }
	void		setOpen(){ flag_open = 1 ; }
}; // X_Window 


#endif // USE_XWIN

//////////////////////////////////////
#endif
//////////////////// END OF x_window.h
