#include <stdlib.h>
#include "toolstring.h"



	//----- ToolString::removeHeader()
int 
ToolString::removeHeader( char* string , char* header, double* num_p  ) 
{
		//----- local
	int	status	= 0    ; // initialized to error
	char*	p	= NULL ;

		//----- header found 
	if( (p = strstr( string , header )) != NULL ) {
		p += strlen(header);
		if ( sscanf( p ,"%lg", num_p ) != EOF ) {
			status = 1 ; //  success
		} else {
			status = 0 ; //  error
		}

		//----- header not found
	} else {
		status = 0 ; //  error 					
	}

		//----- return status
	return status;

} // ToolString::removeHeaderFromString()




	//----- ToolString::getDoubleArray3()
int	ToolString::getDoubleArray3(	const char*  string              , 
					int          dim_each_array      , 
					double* A, double* B, double* C  , 
					int num_skip_word                  ) 
{
	int	errorA = 0 , errorB = 0 , errorC = 0;  // status
	int	i;

	char str_tmp [ BUFSIZE ]; strcpy( str_tmp, string ) ;
	istrstream	str_in( str_tmp ) ;
	char buf [ BUFSIZE ];

		//----- skip
	for( i = 0 ; i < num_skip_word ; i++ ) {
		str_in >> buf ;
	}

		//----- make A[]
	for( i = 0 ; i < dim_each_array ; i++ ) {
		if( !(str_in >> A[i]) ) { errorA = 1 ; }
	}
	
		//----- make B[]
	for( i = 0 ; i < dim_each_array ; i++ ) {
		if( !(str_in >> B[i]) ) { errorB = 1 ; }
	}

		//----- make C[]
	for( i = 0 ; i < dim_each_array ; i++ ) {
		if( !(str_in >> C[i]) ) { errorC = 1 ; }
	}


		//----- return status
	return !(errorA ||  errorB ||  errorC );

} // ToolString::getDoubleArray3()

// string

		//----- ToolString::getInt()
int	ToolString::getInt( const char* string, int* val, int index ) 
{
	int	status = 1 ;
	int	i;
	char str_tmp [ BUFSIZE ]; strcpy( str_tmp, string ) ;
	istrstream	str_in( str_tmp ) ;
	char buf[ BUFSIZE ];
	
		//----- skip
	for( i = 0 ; i < index ; i++ ) {
		str_in >> buf ;
	}

		//----- read index-th word as int value
		//..... (index = 0, 1, 2, ...)
	if(!(str_in >> *val ) ) { status = 0 ; }
	
		//----- return status
	return status ;

}

// string

		//----- ToolString::getDouble()
int	ToolString::getDouble( const char* string, double* val, int index ) 
{
	int	status = 1 ;
	int	i;

	char str_tmp [ BUFSIZE ]; strcpy( str_tmp, string ) ;
	istrstream	str_in( str_tmp ) ;
	char buf[ BUFSIZE ];
	
		//----- skip
	for( i = 0 ; i < index ; i++ ) {
		str_in >> buf ;
	}

		//----- read index-th word as double value
		//..... (index = 0, 1, 2, ...)
	if(!(str_in >> *val ) ) { status = 0 ; }
	

		//----- return status
	return status ;

}

// string


char* ToolString::getBeginningOfString( char* string , int max_index )
{
	char* p       = NULL      ;
	 
	if        ( string[0] == '\0' ) {
		p = NULL ;

	} else if ( strlen( string )  == 0 ) {
		p = NULL ;

	} else {

		int n = 0 ;
		for ( p  = &(string[0]) ; (*p != '\0' && n < max_index ) ; p++ , n++ ) 
		{
			if ( !isspace( *p ) ){
				break ;		 	      
			}
		}
	}

	return p ;
}


int ToolString::isEnvironmentalVariableTrue( const char* string )
{
	int status = 0;

	if( getenv( string ) != NULL ) {
		if( !strcmp( getenv( string ),"0" ) ) 
		{ 
			status = 0 ;
		} else {
			status = 1 ;
		}
	} 

	return status ;
}
