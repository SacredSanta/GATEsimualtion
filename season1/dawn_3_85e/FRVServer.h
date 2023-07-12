/////////////////////////
///// FRVServer.h  //////
/////////////////////////

#if !defined FR_VSERVER_H
#define FR_VSERVER_H

// #define DEBUG

class FRCommandAnalyzer ;

	//---------------------------//
	//-----  class FRVServer -----//
	//---------------------------//
class FRVServer { 
 public:
	FRVServer  (){;}
	virtual ~FRVServer (){;}

	virtual int	ReceiveMessage( FRCommandAnalyzer* command_analyzer_p ) = 0 ;
	virtual void	SendLine ( const char* message )                = 0 ; 


} ; // FRVServer

#endif
