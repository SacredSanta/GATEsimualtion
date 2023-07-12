////////////////////////////
///// FRSocketMacro.h  /////
////////////////////////////


#if !defined SOCKET_MACRO_H
#define SOCKET_MACRO_H

// MACRO for portability

	//----- argument of accept() system call
#if defined SOCKET_AIX
	#define         CLIENT_ADDR_LENGTH    size_t
#elif defined SOCKET_REDHAT61
	#define         CLIENT_ADDR_LENGTH    socklen_t
#else 
	#define         CLIENT_ADDR_LENGTH    int
//	#define         CLIENT_ADDR_LENGTH    size_t
#endif

	//----- gethostname
#if defined SOCKET_IRIX
 #include <sys/systeminfo.h>
 #define  GET_HOSTNAME( hostname, length )  sysinfo( SI_HOSTNAME, hostname, length ) 
#elif defined SOCKET_SOLARIS
 #include <sys/systeminfo.h>
 #define  GET_HOSTNAME( hostname, length )  sysinfo( SI_HOSTNAME, hostname, length ) 
#else 
 #define  GET_HOSTNAME( hostname, length )  gethostname( hostname, length ) 
#endif			

#endif

