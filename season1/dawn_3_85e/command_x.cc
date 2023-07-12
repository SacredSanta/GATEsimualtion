//////////////////////////
/////  command_x.cc  /////
//////////////////////////

#include "command.h"

#if defined USE_XWIN

	//----- Command::open_X()
int	
Command::open_X( PostScript& ps , X_Window& xwin, PS2X& ps2x, Parameter& parameter , int flag_immediate ) 
{

		//----- local
	char	title[100] ;
	
	if( parameter.flag_IP_mode == 1 ) {
		strcpy( title, "FR SUB Window  (Click Button or Hit Key to Proceed!)");
	} else {
		strcpy( title, "FR MAIN Window (Click Button or Hit Key to Proceed!)");
	}


		//----- open device
	if( !(xwin.isOpen()) ) 
	{
			//----- open devices
		xwin.open( title );

	} else if ( !flag_immediate ) {
		xwin.clearWindow();
		XFlush( xwin.getDisplay() );
	}


	ps2x.Open( xwin.getDisplay() , xwin.getWindow () )  ;
	ps.open(	NULL , 0 ,  
			Vector2( parameter.viewport_center_x , parameter.viewport_center_y ) );

		//----- return status 
	return 1 ;

} // Command::open_X()


	//----- Command::close_X()
int	Command::close_X( PostScript& ps , X_Window& xwin, PS2X& ps2x ) 
{ 
	cerr <<  flush; // flush many times for some systems

		//----- Note:
		//.....  If open_X() is called for the first time,
		//.....  xwin, ps, ps2x are open, and xwin.isOpen() becomes 1
	if( xwin.isOpen() ) {
		cerr <<  flush;

			//----- flush
		ps.output("showpage\n") ;
		cerr <<  flush;
		ps.flush() ;
		cerr <<  flush;

			//----- 
		XFlush( xwin.getDisplay() );
		cerr <<  flush;
		if( ps.isOpen()   ) { ps.close()   ; }
		cerr <<  flush;
		if( ps2x.isOpen() ) { ps2x.Close() ; }
		cerr <<  flush;
	}
	cerr <<  flush;

		//-----
	return 1 ;

}// Command::close_X()


	//----- Command::showpage_X()
int	Command::showpage_X( PostScript& ps , X_Window& xwin )
{ 
		//----- draw
	ps.output("showpage\n") ;
	XFlush( xwin.getDisplay() );
	ps.flush() ; 
	cerr << flush ;

		//-----
	return 1 ;

}// Command::showpage_X()

	//----- void FRCommandAnalyzer::waitEvent()
void Command::waitEvent( const Parameter& parameter , X_Window& xwin ) 
{
		//----- local
	XEvent	event ;


		//-----  local
	if( parameter.device_label == PS || parameter.device_label == PS2 ) {
		return ;
	}

		//----- message
	cerr<<  "----------------------------------------------------------\n";
	cerr << " Drawing completed. " << endl;
	cerr << " Push right mouse button or hit any key on vieing window!" << endl;
	cerr<<  "----------------------------------------------------------\n";
	cerr<< endl << endl;

	while(1) {
		XNextEvent( xwin.getDisplay() , &event ) ;

		if( event.type == KeyPress    ) { 
			break ; 
		}
		else if( event.type == ButtonPress ) { 

			int flag_exit = 1 ;

			switch   ( event.xbutton.button ) {
			 case  Button1:
				flag_exit = 0 ;
				break ;
			 case  Button2:
				flag_exit = 0 ;
				break ;
			 case  Button3:
				flag_exit = 1 ;
				break ;
			 default: ;
			}

			if( flag_exit ) { break ; }
				// exit from while loop
		}
	}// while

}// void Command::waitEvent()



#endif // USE_XWIN

