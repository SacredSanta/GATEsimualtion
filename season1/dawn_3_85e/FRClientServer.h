/////////////////////////////
///// FRClientServer.h  /////
/////////////////////////////

#if !defined FR_CLIENT_SERVER_H
#define FR_CLIENT_SERVER_H

#include "dev_control.h"

#if defined USE_UNIX
#if defined USE_SOCKET

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/un.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream.h>

#include "FRBufSize.h"
#include "FRVServer.h"

	//---------------------------------//
	//-----  class FRClientServer -----//
	//---------------------------------//

class FRClientServer : public FRVServer { 
 public:
	enum { SEND_BUFMAX = FR_SEND_BUFMAX , RECV_BUFMAX = FR_RECV_BUFMAX };
	enum { SUN_PATH_MAX = 16 };

 protected:	
	const char		TERMINATOR ;
	const char		END_OF_LINE;
	char			SUN_PATH[ SUN_PATH_MAX ];
	int			PORT_NUMBER ;
	int			fSocketFd ;

	char		fReceivedMessage [ RECV_BUFMAX ];
	char		fSendingMessage  [ SEND_BUFMAX ];

 protected:

	void		Err( char* message ) { perror(message) ;}
	void		SetSendingMessage( const char* message ) 
				{ strcpy( fSendingMessage, message );}
	void		Send() ; // send string in fSendingMessage

 public:	
		//----- Server
	int		AcceptUnix(); 
	int		AcceptINET();

		//----- Client
	int		ConnectUnix(); 
	int		ConnectINET(); 

		//----- Common to server and client

		//---------- (1)
	FRClientServer (	char terminator = '.'           ,
				char end_line = '\n'              ) ;  
	virtual ~FRClientServer () {;}
	void		SetSunPath( const char* sun_path ) 
			{ strcpy     ( SUN_PATH, sun_path ); }
	void		SetPortNumber( int port_num ) 
			{ PORT_NUMBER = port_num ; }
	void		IncrementPortNumber( int incr = 1 ) 
			{ PORT_NUMBER += incr ; }
	const char*	GetSendingMessage() const
				{ return  fSendingMessage            ;}
	int		GetSendingMessageLength() const
				{ return  strlen(fSendingMessage)    ;}
	void		SetReceivedMessage( const char* message ) 
				{ strcpy( fReceivedMessage, message );}
	const char*	GetReceivedMessage() const
				{ return fReceivedMessage            ;}
	int		GetReceivedMessageLength() const    
				{ return strlen(fReceivedMessage)    ;}
	int		GetSofd() const { return fSocketFd ; }
	int		GetPortNumber () const { return PORT_NUMBER ; }
	void		ClearReceivedMessage () 
			{ memset(fReceivedMessage, '\0', RECV_BUFMAX) ;	}

	int		IsTerminator(char ch ) { return ( ch == TERMINATOR ); }
	char		GetTerminator()  const  { return TERMINATOR ; }
	int		IsEndOfLine (char ch ) { return ( ch == END_OF_LINE ); }
	char		GetEndOfLine() const { return END_OF_LINE ; }
	void		DisConnect();
	void		Clear();
	void		WaitSendBack( const char* command_string ) ;

		//---------- (2) send and receive
	virtual	void	Send     ( const char* message ) ; 
	virtual void	SendLine ( const char* message ) ; 
				// Add END_OF_LINE if the message does not
				// terminate with it. And then send the message.
	virtual	void	Receive() ;
	virtual void	ReceiveLine() ;

}; // FRClientServer

#endif // USE_SOCKET 

#endif // USE_UNIX

#endif // END of FRClientServer.h
