#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream.h>


#include "dev_control.h"
#if defined USE_UNIX 
#if defined USE_SOCKET

#include "main_selector.h"

#include "FRCommandAnalyzer.h"
#include "FRServer.h"

	//----- output parameters
#define DEBUG_MAIN2


const int	FR_MAX_PORT_INCR = 10 ;

int
MainSelector::daemonProcessUnix( int argc, char* argv[] , Command& command, Parameter& parameter )
{
	cerr << "***** Daemon mode with Unix domain (-g option)" << endl << endl;

		//---------------------//
		//----- variables -----//
		//---------------------//
	int			command_status = 1 ;
	const	Attribute	defaultAttribute ;
	FRServer server;
	int dummy = argc; dummy = 0 ;

	while(1) {
		FRCommandAnalyzer	commandAnalizer( argv[1], command, parameter , defaultAttribute);
//		commandAnalizer.setCurrentAttribute( defaultAttribute );
		server.AcceptUnix() ; // Unix domain


		command_status = server.ReceiveMessage(&commandAnalizer) ;


		server.DisConnect();
		server.Clear();

			//---- QUIT
		if       ( command_status == 0 ) { 
			; // do nothing
		} 
			//----- TERMINATE_DAWND
		else if  ( command_status == -1 ) { 
			break ;
		} else {
			; // do nothing
		}
	}


		//----- return 
	return command_status ;

} // MainSelector::daemonProcessUnix()



int
MainSelector::daemonProcessINET( int argc, char* argv[] , Command& command, Parameter& parameter )
{
	cerr << "***** Daemon mode with INET domain (-G option)" << endl << endl;

		//---------------------//
		//----- variables -----//
		//---------------------//
	int			command_status = 1 ;
	const	Attribute	defaultAttribute ;
	FRServer server;
	int dummy = argc; dummy = 0 ;

	while(1) {
		FRCommandAnalyzer	commandAnalizer( argv[1], command, parameter , defaultAttribute );

			//----- INET domain connection
			//CERN 
		int accept_trial = 0 ;
		while(1) {
		  if ( ++accept_trial > FR_MAX_PORT_INCR ) {
		    cerr << "***** DAWN cannot accept INET connection."<< endl;
		    cerr << "      Maybe, too many DAWN's are"         << endl;
		    cerr << "      already running in the same host."  << endl;
		    exit(1);

		  } else if ( server.AcceptINET() ) { 
			    // INET domain connection is established
		    break ; 
		  } else { 
			    // Connection failed. Try the next port.
		    server.IncrementPortNumber();
		    cerr << "***** Trial server port is incremented to " ;
		    cerr << server.GetPortNumber() << endl;
		  } 
		} 

			//----- receive and execute commands
		command_status = server.ReceiveMessage(&commandAnalizer) ;


			//----- disconnection and cleaning of data
		server.DisConnect();
		server.Clear();

			//---- QUIT
		if       ( command_status == 0 ) { 
			; // do nothing
		} 
			//----- TERMINATE_DAWND
		else if  ( command_status == -1 ) { 
			break ;
		} else {
			; // do nothing
		}
	}


		//----- return 
	return command_status ;

} // MainSelector::daemonProcessINET()

#endif // USE_SOCKET
#endif // USE_UNIX
