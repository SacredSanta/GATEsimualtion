///////////////////////
///// FRServer.h  /////
///////////////////////

#if !defined FR_SERVER_H
#define FR_SERVER_H

#include "dev_control.h"
#if defined USE_UNIX 
#if defined USE_SOCKET

// #define DEBUG

#include "FRClientServer.h"
#include "FRCommandList.h"
#include "FRCommandAnalyzer.h"

	//---------------------------//
	//-----  class FRServer -----//
	//---------------------------//
class FRServer: public FRClientServer { 
 public:
	FRServer (	char terminator = '.'           ,
			char end_line = '\n'              ) 
		: FRClientServer ( terminator, end_line ) {;}
	~FRServer (){;}

	int	ReceiveMessage( FRCommandAnalyzer* command_analyser_p ) ; 

} ; // FRServer 



	//----- FRServer::ReceiveMessage()
inline int
FRServer::ReceiveMessage( FRCommandAnalyzer* command_analyzer_p )  
{

	int	index = 0 ;
	int	status = 1 ;
	int	num_read_char = 0 ;
	char	ch ;
	char	buf[ FRClientServer::RECV_BUFMAX ];

	while( status >0  ) {

			//-----
		index = 0 ;
		while( 1 ) {
			num_read_char = read( fSocketFd, &ch, 1 );

			if        ( num_read_char == 1 ) {
				buf[index++] = ch ; 

				if( IsEndOfLine(ch) ) { 
					buf[index++] = '\0' ; 
					status = command_analyzer_p->analyzeAndExecute ( buf , this ) ;
					index = 0 ;
		
#if defined DEBUG
					cerr << "ReceivedMessageByServer = " << buf << endl; 
#endif

					if( status <= 0 ) { break;}
				} 

			} else if ( num_read_char == 0 ) {
				index = 0 ;
#if defined DEBUG
				cerr << "WARNING (FRServer::ReceiveMessage()), 1a" << endl; 
#endif

			} else {
				index = 0 ;
#if defined DEBUG
				cerr << "WARNING (FRServer::ReceiveMessage()), 1b" << endl; 
#endif
			}

		} // while(1)

	} // while( status )

	return status ;

} // FRServer::ReceiveMessage()


#endif // USE_SOCKET
#endif // USE_UNIX

#endif
