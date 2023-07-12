// #include<unistd.h>
#include "command.h"
#include "main_selector.h"
#include "parameter.h"
#include "FRCommandList.h"

// #define DEBUG_MAIN

int
main( int argc, char* argv[] )
{
		//----- variables
	MainSelector	main_selector ;
	Command		command ;
	Parameter	parameter ;

		//---------------------------//
		//----- option analysing-----//
		//---------------------------//
		//  Display opening and analyze options
	command.openDawn( argc , argv , parameter ); 

#if defined USE_UNIX 

		//----- select main
	if       ( parameter.flag_IP_mode == 1 ) {
			// -v option (default): solid viewer mode
		main_selector.primViewer( argc, argv , command, parameter );

#if defined USE_SOCKET

	} else if( parameter.flag_IP_mode == 2 ) {
			// -g option: daemon mode (Unix domain)
		main_selector.daemonProcessUnix( argc, argv , command, parameter );

	} else if( parameter.flag_IP_mode == 3 ) {

			// -G option: daemon mode (INET domain)
		main_selector.daemonProcessINET( argc, argv , command, parameter );

#endif // USE_SOCKET

	} else {
			// -v option (default): solid viewer mode
		main_selector.primViewer( argc, argv , command, parameter );
	}

#else  // USE_UNIX 
		//----- select main
	if       ( parameter.flag_IP_mode == 1 ) {
			// -v option (default): solid viewer mode
		main_selector.primViewer( argc, argv , command, parameter );

	} else {
			// -b : b-rep mode (for debugging)
		main_selector.standAlone( argc, argv , command, parameter );
	}

#endif // USE_UNIX 


		//----- debugging information
#if defined DEBUG_MAIN
	command.printParameter( parameter , cerr )    ; cerr << "\n\n";
#endif

		//----- terminating the process
//	exit(0) ;
	return 0 ;

} // main()
