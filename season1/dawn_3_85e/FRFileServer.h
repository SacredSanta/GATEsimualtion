////////////////////////////
///// FRFileServer.h  //////
////////////////////////////

#if !defined FR_FILE_SERVER_H
#define FR_FILE_SERVER_H

// #define DEBUG

#include "dev_control.h"
#if defined USE_VC
	#include <strstrea.h>
#else
	#include <strstream.h>
#endif

#include <string.h>
#include "FRCommandList.h"
#include "FRCommandAnalyzer.h"
#include "FRBufSize.h"
#include "FRVServer.h"

	//---------------------------//
	//-----  class FRServer -----//
	//---------------------------//
class FRFileServer : public FRVServer { 
 public:
	FRFileServer  (const char* filename ){ strcpy(m_filename,filename) ;}
	~FRFileServer (){;}

        // overloaded virtual functions
	int	ReceiveMessage( FRCommandAnalyzer* command_analyzer_p ) ;
	void	SendLine ( const char* message ) 
		{ char dummy[FR_SEND_BUFMAX] ; strcpy(dummy,message); }

	// access function
	void  SetFileName ( const char* filename ) { strcpy( m_filename, filename );}
	const char* GetFileName () { return m_filename ;}

 private:
	char m_filename[256] ;

} ; // FRFileServer 



	//----- FRServer::ReceiveMessage()
inline int
FRFileServer::ReceiveMessage( FRCommandAnalyzer* command_analyzer_p )  
{
	int	status = 1 ;
	char	buf[ FR_RECV_BUFMAX ];

	ifstream	fin( m_filename ) ;
	if (!fin) { 
		cerr << "WARNING: Cannot open file " << m_filename << endl; 
		return status ; 
	}

	while( status >0  ) {
		while(1)
		{
			 if( fin.getline( buf, FR_RECV_BUFMAX ) ) {
					//----- add newline if not
				int msglen = strlen(buf) ;
				if( buf[ msglen - 1 ] != '\n' ) {
					strcat( buf, "\n");
					msglen++ ;
				}
					//----- execute read command
				status = command_analyzer_p->analyzeAndExecute ( buf , this ) ;	

					//----- termination condition
				if( status <= 0 ) { break; }
			 } else {
				status = 0 ; // equiv to FR_QUIT
				break ;
			 }
		}
	} // while( status )

	fin.close();

#if defined USE_XWIN
	command_analyzer_p->analyzeAndExecute ( FR_PAUSE       , this ) ; // wait with no sendback
#endif

	command_analyzer_p->analyzeAndExecute ( FR_CLEAR_DATA  , this ) ;	

	return status ;

} // FRFileServer::ReceiveMessage()
 
#endif
