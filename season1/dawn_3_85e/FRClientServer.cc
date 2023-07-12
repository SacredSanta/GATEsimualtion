/////////////////////////////
///// FRClientServer.cc /////
/////////////////////////////

#include "dev_control.h"
#if defined USE_UNIX 
#if defined USE_SOCKET

#include "FRSocketConfig.h"
#include "FRSocketMacro.h"
#include "FRClientServer.h"

// #define DEBUG_CLIENT_SERVER

#include<stddef.h>
#include<sys/param.h>



	//----- const 	
const  char	DEFAULT_SUN_PATH[]		= "FR_TMP3"        ;
const  int	DEFAULT_PORT_NUMBER		= 40701            ;
const  char	ENV_SERVER_HOST_NAME[]		= "G4DAWN_HOST_NAME" ;
const  int	MAX_BINDING_TRIAL		= 10               ;
const  int	BINDING_TRIAL_INTERVAL		= 5                ;
const  int	MAX_CONNECT_TRIAL		= 10               ;


	//----- FRClientServer::FRClientServer ()
FRClientServer::FRClientServer ( char terminator , char end_line ) :  
	TERMINATOR ( terminator ) , 
	END_OF_LINE( end_line   ) ,
	fSocketFd  ( -1 )           
{ 
	SetSunPath    ( DEFAULT_SUN_PATH ) ; // for Unix domain
	SetPortNumber ( DEFAULT_PORT_NUMBER ) ;
	ClearReceivedMessage () ;
}


	//----- FRClientServer::ConnectUnix()
int FRClientServer::ConnectUnix()
{
		//----- local
	int			flag_connected = 0 ; 
	struct sockaddr_un	server_address ;

		//----- make socket
	fSocketFd = socket( AF_UNIX, SOCK_STREAM, 0 );
	if( fSocketFd < 0 ) { Err("FRClientServer::ConnectUnix(),socket"); }

		//----- set server address
	memset( (char *)&server_address, '\0', sizeof(server_address)) ;
	server_address.sun_family = AF_UNIX ;
	strcpy( server_address.sun_path, SUN_PATH );

		//----- connection
	int	connection_status = -1  ;
	int	num_trial         = 0 ;
	while( connection_status < 0 && num_trial <= MAX_CONNECT_TRIAL ) {
		num_trial++ ; 
		connection_status = connect( fSocketFd, (struct sockaddr * )(&server_address), sizeof( server_address ) ) ;
		if( connection_status  <0 ) 
		{
#if defined DEBUG_CLIENT_SERVER
			Err("FRClientServer::ConnectUnix(),connect => RETRY");
#endif
			flag_connected = 0 ;
		} else {
			flag_connected = 1 ;
			break ;
		}

		sleep(1);

	} // while(connection_status...)

		//----- return status of connection
	return flag_connected ;

} // FRClientServer::ConnectUnix()


	//----- FRClientServer::AcceptUnix()
int
FRClientServer::AcceptUnix()
{
		//----- local 
        int      		flag_accept = 1 ; // unused
	sockaddr_un		server_address ;
	sockaddr_un		client_address ;
//	int			client_addr_length ;
	int			fd_socked_waiting ;

		//----- clear PIPE
	unlink(SUN_PATH);

		//----- make socket
	fd_socked_waiting = socket( AF_UNIX, SOCK_STREAM, 0 );
	if( fd_socked_waiting < 0 ) { Err("FRClientServer::AcceptUnix(), socket"); }

		//----- binding
	memset( (char *)&server_address, '\0', sizeof(server_address)) ; 
				// clear server_address
	server_address.sun_family = AF_UNIX ;
	strcpy( server_address.sun_path, SUN_PATH );
	if( bind( fd_socked_waiting, (struct sockaddr * )(&server_address), sizeof( server_address ) ) < 0 ) {
		Err("FRClientServer::AcceptUnix(),bind");
	}

		//----- listen (wait connection)
	if( listen( fd_socked_waiting, 1 ) == -1 ) { 
		Err("FRClientServer::AcceptUnix(),listen"); 
	}

		//----- accept connection
	CLIENT_ADDR_LENGTH	client_addr_length ;

	client_addr_length = sizeof(client_address);
	fSocketFd = accept( fd_socked_waiting, 
			(struct sockaddr * )(&client_address), 
			&client_addr_length ) ;
	if( fSocketFd <0 ) { Err("FRClientServer::AcceptUnix(), accept");}
	close( fd_socked_waiting );

#if defined DEBUG_CLIENT_SERVER
	cerr << "***** SERVER: AcceptUnix\n";
#endif

	return flag_accept ;

} // FRClientServer::AcceptUnix()


	//-----	FRClientServer::Receive()
void	FRClientServer::Receive()
{
		//-----
	ClearReceivedMessage () ;
	if( recv( fSocketFd, fReceivedMessage, FRClientServer::RECV_BUFMAX , 0 ) < 0 ) 
	{
		Err("FRClientServer::Receive(), recv");
	}

#if defined DEBUG_CLIENT_SERVER
	cerr << ">>>>> receivedMessage = " << fReceivedMessage << endl;
#endif

}


	//-----	FRClientServer::ReceiveLine()
void	FRClientServer::ReceiveLine()
{
		//----- local
	char    buf[1];
	int index = 0 ;

		//----- receive a line (until newline)
	memset(fReceivedMessage, '\0', RECV_BUFMAX) ;	
	while( read( fSocketFd, buf, 1 ) == 1 ) {
		fReceivedMessage[index++] = buf[0];
		if( IsEndOfLine(buf[0]) ) { break ;}
	}
} // FRClientServer::ReceiveLine()


	//-----	FRClientServer::Send()
void	FRClientServer::Send()
{
	if( send( fSocketFd, fSendingMessage, strlen(fSendingMessage) , 0 ) < 0 ) 
	{
		Err("FRClientServer::Send(), send");
	}

#if defined DEBUG_CLIENT_SERVER
	cerr << "<<<<< SentMessage = " << fSendingMessage << endl;
#endif

} // FRClientServer::Send()


	//-----	FRClientServer::Send( message )
void	FRClientServer::Send( const char* message ) 
{
	this->SetSendingMessage( message )      ;
	this->Send();

} // FRClientServer::Send( message )


	//-----	FRClientServer::SendLine()
void	FRClientServer::SendLine( const char* message ) 
{
		//----- local
	int	smsg_length ;

		//----- set message to sending buf
	this->SetSendingMessage( message )      ;
	smsg_length = GetSendingMessageLength() ;

		//----- add newline if necessary
	if( !IsEndOfLine( fSendingMessage[ (smsg_length - 1)] ) ) {
		fSendingMessage[ smsg_length ]      = GetEndOfLine() ;
		fSendingMessage[ (smsg_length +1) ] = '\0' ;
		smsg_length = GetSendingMessageLength();
	}

		//----- send
	this->Send();

}// FRClientServer::SendLine()


	//----- FRClientServer::DisConnect()
void FRClientServer::DisConnect()
{
		//----- close connection
	if( shutdown(fSocketFd,2) < 0 ) { 
		Err("FRClientServer::DisConnect,shutdown");
	}
	close( fSocketFd );

	this->Clear();
}



	//----- FRClientServer::Clear()
void FRClientServer::Clear()
{
	unlink(SUN_PATH) ;
	fSocketFd = -1   ;
}



	//----- FRClientServer::AcceptINET()
int
FRClientServer::AcceptINET()
{
		//----- local 
  	int			flag_accept = 1;
	int			num_binding_trial = 0 ;
	sockaddr_in		server_address, client_address ;
//	int			client_addr_length ;
	int			fd_socked_waiting ;
	char			server_hostname[32] ;
	hostent*		server_host_p ;


  while(1) 
  {
		//----- controlling retry
	num_binding_trial++ ;
	if( ( num_binding_trial % BINDING_TRIAL_INTERVAL ) == 0 ) 
	{ 
		cerr << "***** Trying to bind soket. Please wait." << endl; 
	}
	if( num_binding_trial > MAX_BINDING_TRIAL ) 
	{ 
	  cerr << "***** Cannot use port " << PORT_NUMBER << endl; 
	  flag_accept = 0 ;		
	  return flag_accept ;
	}

		//----- make socket
	fd_socked_waiting = socket( AF_INET , SOCK_STREAM , 0 );
	if( fd_socked_waiting < 0 ) { Err("FRClientServer::AcceptINET(), socket"); }

		//----- get server name
	if( GET_HOSTNAME( server_hostname, sizeof( server_hostname ) ) < 0 )
	{ Err("FRClientServer::AcceptINET(), gethostname");}

		//----- get IP address of server from its name
	if( !(server_host_p = gethostbyname( server_hostname ) ) )
	{ Err("FRClientServer::AcceptINET(), gethostbyname");}


		//----- binding
	memset( (char *)&server_address, '\0', sizeof(server_address)) ; 
				// clear server_address
	server_address.sin_family = AF_INET ;
	server_address.sin_port   = htons( PORT_NUMBER );
	memcpy( (char *)(&server_address.sin_addr ), 
		(char *)( server_host_p->h_addr   ), 
		server_host_p->h_length             ); 

	if( bind( fd_socked_waiting, (struct sockaddr * )(&server_address), sizeof( server_address ) ) < 0 ) {
#if defined DEBUG_CLIENT_SERVER
		Err("FRClientServer::AcceptINET(),bind ==> RETRY");
#endif
		close (fd_socked_waiting);

	} else {
		cerr << "***** Socket is bound!" << endl; 
		break ;
	}

  }// while


		//----- listen (wait connection)
	if( listen( fd_socked_waiting, 1 ) == -1 ) { 
		Err("FRClientServer::AcceptINET(),listen"); 
	}

		//----- accept connection
	CLIENT_ADDR_LENGTH	client_addr_length ;

	client_addr_length = sizeof(client_address);
	fSocketFd = accept( fd_socked_waiting, 
			(struct sockaddr * )(&client_address), 
			&client_addr_length ) ;
	if( fSocketFd <0 ) { Err("FRClientServer::AcceptINET(), accept");}
	close( fd_socked_waiting );

#if defined DEBUG_CLIENT_SERVER
	cerr << "***** SERVER: AcceptINET\n";
#endif

	return flag_accept ;


} // FRClientServer::AcceptINET()



	//----- FRClientServer::ConnectINET()
int FRClientServer::ConnectINET()
{
		//----- local
	int			flag_connected = 0 ;
	sockaddr_in		server_address ;
	char			server_hostname[32] ;
	hostent*		server_host_p ;

		//----- make socket
	fSocketFd = socket( AF_INET, SOCK_STREAM, 0 );
	if( fSocketFd < 0 ) { 
#if defined DEBUG_CLIENT_SERVER
	  Err("FRClientServer::ConnectINET(),socket"); 
#endif
	}


		//----- get IP address of server from its name
	if( getenv( ENV_SERVER_HOST_NAME ) != NULL ) 
	{

			//----- get server name
		strcpy( server_hostname, getenv( ENV_SERVER_HOST_NAME ) );

			//----- get IP address of server from its name,
			//..... reading /etc/hosts
		server_host_p = gethostbyname( server_hostname ) ;
			
			//----- If the host specified by ENV_SERVER_HOST_NAME
			//.....  is not written in /etc/hosts, 
			//...... server host is set to the same as the
			//...... client host
		if( !server_host_p ) {
#if defined DEBUG_CLIENT_SERVER
			Err("FRClientServer::ConnectINET(), gethostbyname");
#endif
				//----- get server name
			if( GET_HOSTNAME( server_hostname, sizeof( server_hostname ) ) < 0 )
			{ Err("FRClientServer::ConnectNET(), gethostname");}

				//----- get IP address of server from its name
			if( !(server_host_p = gethostbyname( server_hostname ) ) )
			{ Err("FRClientServer::ConnectINET(), gethostbyname");}
		}

	} else {
			//----- If ENV_SERVER_HOST_NAME is not set, 
			//...... server host is set to the same as the
			//...... client host.

			//----- get server name
		if( GET_HOSTNAME( server_hostname, sizeof( server_hostname ) ) < 0 )
		{ Err("FRClientServer::ConnectNET(), gethostname");}

			//----- get IP address of server from its name
		if( !(server_host_p = gethostbyname( server_hostname ) ) )
		{ Err("FRClientServer::ConnectINET(), gethostbyname");}
	}



// #if defined DEBUG_CLIENT_SERVER
	cerr << "***** Trying connection to server host  " << server_hostname << endl;
// #endif 
	

		//----- connection and binding 
	memset( (char *)&server_address, '\0', sizeof(server_address)) ; 
				// clear server_address
	server_address.sin_family = AF_INET ;
	server_address.sin_port   = htons( PORT_NUMBER );
	memcpy( (char *)(&server_address.sin_addr ), 
		(char *)( server_host_p->h_addr   ), 
		server_host_p->h_length             ); 

	int	connection_status = -1  ;
	int	num_trial         = 0 ;
	while( connection_status < 0 && num_trial <= MAX_CONNECT_TRIAL ) {
		num_trial++ ; 
		connection_status = connect( fSocketFd, (struct sockaddr * )(&server_address), sizeof( server_address ) ) ;
		if( connection_status  <0 ) 
		{
			flag_connected  = 0 ;
#if defined DEBUG_CLIENT_SERVER
			Err("FRClientServer::ConnectINET(),connect => RETRY");
#endif 
		} else {
			flag_connected  = 1 ;
			break ;
		}

		sleep(1);

	} // while(connection_status...)

		//----- return status of connection
	return flag_connected ;

} // FRClientServer::ConnectINET()


	//----- FRClientServer::WaitSendBack()
void	FRClientServer::WaitSendBack( const char* command_string ) 
{
		//----- wait for sending back
	while(1) { 
		this->ReceiveLine();

		if( !strncmp(	this->GetReceivedMessage(), \
				command_string                , \
				(strlen(command_string))       )   )
		{
			break;
		} else {
			sleep(2);
		}

	} // while

		//----- clear buffer to receive message
	this->ClearReceivedMessage();	

} // FRClientServer::WaitSendBack()

#endif // USE_SOCKET
#endif // USE_UNIX

