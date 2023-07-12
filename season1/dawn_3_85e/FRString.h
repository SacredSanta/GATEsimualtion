//////////////////////////////
///// FRString.h   /////
//////////////////////////////

#if !defined FR_STRING_H
#define      FR_STRING_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>

class FRString {
 public:  
   FRString( void ) : m_string(NULL){ }

   FRString( const char* str ) : m_string(NULL)
     {  
	if( str != NULL ) {
		m_string = new char [ strlen( str ) + 1 ] ;
		strcpy( m_string , str );
	}
     }

   void Clear( void )
     {
	delete [] m_string ; 
	m_string = NULL ; 
     }

  virtual ~FRString( void ) 
     {
	Clear(); 
     }

   unsigned int Length( void ) const 
     { 
	if( m_string )
	{
		return strlen( m_string ) ; 
	} else {
		return 0; 
	}
     }

   void ResetString( const char* str ) 
     { 
	Clear(); 
	if( str != NULL ) {
		m_string = new char [ strlen( str ) + 1 ] ;
		strcpy( m_string , str );
	}
     }

   const char* GetCharString( void ) const 
     { 
	return  m_string ; 
     }

   operator   const char* () const { return m_string; }

   void  operator = ( const FRString& rhs )
     { 
	if( this != (&rhs) ) { 
		ResetString( rhs.m_string );
	}
     }
   
   void  operator += ( const FRString& rhs )
     { 
		//----- backup old string
	char * org_string = m_string ;

		//----- remake space for new string
	m_string
	 = new char [ strlen( org_string) + strlen(rhs.m_string) + 1 ];

		//----- make the new string
	strcpy( m_string, org_string );
	strcat( m_string, rhs.m_string );

		//----- delete old string
	delete [] org_string ;
     }

   int  operator == ( const FRString& rhs ) const 
     { 
	return ( !strcmp( m_string, rhs.m_string ) );
     }

   int  Find ( const FRString& string ) 
     { 
	return ( strstr( m_string, string.m_string ) != NULL ) ;
     }

   int  operator != ( const FRString& rhs ) const 
     { 
	return ( strcmp( m_string, rhs.m_string ) );
     }

   void  Decompose( int*    array, int* num_p, int MAX_INDEX = 1000 ) const ;

   void  RemoveFirstWord();
   void  ReplaceTerminalNewlineWithNull();

 protected:
   char*  m_string ;
   
} ; // FRString
   
inline 
void FRString::Decompose(int* array, int* num_p, int MAX ) const 
{
	int     length = strlen( m_string );
	char*   p ;
	int     i ;

	*num_p = 0 ;
	for ( i = 0 , p = m_string ; i < length ; i++, p++ )
	{
		if( isdigit( *p ) || *p == '-' ) { 	
			sscanf( p, "%d", &(array[ *num_p ]) ); (*num_p)++ ;
			while ( 1 ) {
				i++; ++p ;
				if ( isspace( *p ) ) { break ;}
			} 
		}

		if( *num_p >= MAX ) { break ; }
	}
}

inline 
void FRString::RemoveFirstWord()
{
	//
	char*  tmp = new char [(Length() + 1 )];
	char*  p = m_string ;
	
	// skip first blank if any
	while(  isspace(*p) && p != '\0' ) {p++;} 

	// skip one word
	while( !isspace(*p) && p != '\0' ) {p++;} 

	// skip second blank if any
	while(  isspace(*p) && p != '\0' ) {p++;} 

	// reset string
	strcpy( tmp, p  );
	ResetString (tmp);

	//
	delete [] tmp ;
}

inline 
void FRString::ReplaceTerminalNewlineWithNull()
{
	const int max_index = strlen( m_string ) - 1 ; 

	if ( m_string[ max_index ] == '\n' ) {
		m_string[ max_index ] = '\0' ;
	}

}

#endif
