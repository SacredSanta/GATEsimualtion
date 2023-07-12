#include <stdio.h>
#include <stdlib.h>
#include <fstream.h>
#include <unistd.h>

#include "FRCommandList.h"
#include "FRClientServer.h"
#include "FRCommandAnalyzer.h"

	//--------------------------------//
	//-----  class FROuterClient -----//
	//--------------------------------//
class FROuterClient : public FRClientServer { 

	int	flag_use_gui;

 public:
		FROuterClient (	char terminator = '.' ,
			        char end_line = '\n'    ) \
		: FRClientServer ( terminator, end_line ) { flag_use_gui = 0;}

		~FROuterClient () {;}

	void	SendMessage( ifstream& fin ) ;	
	int	ReceiveMessage( FRCommandAnalyzer* command_analyzer_p ) 
		{ FRCommandAnalyzer* dummy = command_analyzer_p; dummy = 0 ; return 0 ;} 

} ;


void	FROuterClient::SendMessage( ifstream& fin ) 
{
	char buf[ SEND_BUFMAX ] ;

	while( fin.getline( buf, SEND_BUFMAX ) )
	{
			//----- send message (newline is added)
		SendLine( buf );
		if( !strncmp( buf, FR_GUI, strlen(FR_GUI) ) ) { 
			WaitSendBack( FR_GUI );
		}
		if( !strncmp( buf, FR_WAIT, strlen(FR_WAIT) ) ) { 
			WaitSendBack( FR_WAIT );
		}
	}
} // SendMessage()



int main( int argc, char* argv[] ) 
{
	FROuterClient	Client ;
	ifstream	fin;
	const int	MAX_CONNECT_TRIAL = 10 ;

	if( argc != 2 ) {
		cerr << "USAGE: g4test filename" << endl;
		exit(1);
	}


		//----- establish connection
	int connect_trial = 0 ;
	while(1) {
	  if ( ++connect_trial > MAX_CONNECT_TRIAL ) {
	    cerr << "***** INET Connection failed."                 << endl;
	    cerr << "      Maybe, you have not invoked DAWN yet,"   << endl;		
	    cerr << "      or too many DAWN's are already running " << endl;
	    cerr << "      in the server host."                     << endl;
	    exit(1);

	  } else if ( Client.ConnectINET() ) { 
		    // INET domain connection is established
	    cerr << "***** g4test_inet is connected to port  " ;
	    cerr << Client.GetPortNumber() ;
	    cerr << "  of server " << endl;
	    break ; 
	  } else { 
		    // Connection failed. Try the next port.
	    Client.IncrementPortNumber();
	    cerr << "***** Client incremented targeting port to " ;
	    cerr << Client.GetPortNumber() << endl;
	  } 
	} 

		//----- send data
	while (1) {

		int   status = 1 ;
		char  input[8];


		fin.open( argv[1] );

		if(!fin) {
			cerr << "Cannot open file " << argv[1] << endl;
			exit(1);
		} 


		if( ( getenv( "G4DAWN_GUI_ALWAYS" ) != NULL        ) && \
		   ( strcmp( getenv( "G4DAWN_GUI_ALWAYS" ),"0"  ) )      ) 
		{
				//----- send data
			Client.SendLine    ( FR_GUI );
			Client.WaitSendBack( FR_GUI );
		} 


		Client.SendMessage( fin );

			//----- 
		fin.close();
		Client.SendLine( FR_WAIT );
		Client.WaitSendBack( FR_WAIT );

			//-----
		cerr << "Input number ( < 0 to exit ): ";
		gets( input );
		sscanf( input, "%d", &status );

		if( status < 0 ) { break ;}
	}

	Client.SendLine( FR_CLEAR_DATA );
	Client.SendLine( FR_TERMINATE_DAWND );
	Client.DisConnect();

//	exit(0);
	return 0 ;
}
