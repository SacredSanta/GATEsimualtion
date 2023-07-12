////////////////////////////////////
///// toolstring.h  1995/12/04 /////
////////////////////////////////////

#if !defined TOOL_STRING_H
#define TOOL_STRING_H

#include<ctype.h>
#include<stdio.h>
#include<string.h>
#include "dev_control.h"
#if defined USE_VC
	#include <strstrea.h>
#else
	#include <strstream.h>
#endif
#include "FRBufSize.h"

class ToolString {

//	enum { BUFSIZE = 256 } ;
	enum { BUFSIZE = FR_DEFA_BUFMAX } ;
	char	fBuf[ BUFSIZE ];

 public:
		//----- convert extension 
		//..... e.g.
		//..... convertExtension( "polygon.data", ".data" , ".ps" ) 
		//.....   polygon.data ----> polygon.ps
		//..... convertExtension( "polygon", ".data" , ".ps" ) 
		//.....   polygon ---> polygon.ps
		//..... convertExtension( "polygon.dat", ".data" , ".ps" ) 
		//.....   polygon.dat ---> polygon.dat.ps
	static void convertExtension(char *str1, char *ext1, char *ext2)
	{
		if(!strcmp(&str1[strlen(str1) - strlen(ext1)], ext1)){
			strcpy(&str1[strlen(str1) - strlen(ext1)], ext2);
		} else {
			strcat(str1, ext2);
		}
	}
	static void convertExtension(	char *str1       ,
					const char *ext1 ,
					const char *ext2   )
	{
		if(!strcmp(&str1[strlen(str1) - strlen(ext1)], ext1)){
			strcpy(&str1[strlen(str1) - strlen(ext1)], ext2);
		} else {
			strcat(str1, ext2);
		}
	}

		//----- get first word from given string
	static int getFirstWord( char * string , char * word ) 
	{
		return sscanf( string, "%s", word );
	}

		//----- remove header string from string
		//..... RETURN: 1 (normal) or 0 (error)
		//..... Remaining value is returned to the 3rd argument
	static int removeHeader( char* string , char* header, double* num_p  ) ;


		//----- getDoubleArray3()
		//..... Return: 1 if success, 0 if error
	static int	getDoubleArray3(	const char*  string             , 
						int          dim_each_array     , 
						double* A, double* B, double* C , 
						int num_skip_word = 0            ) ;

		//----- getInt(), getDouble()
		//..... Return: 1 if success, 0 if error
		//..... index-th word in string is returned to val as int or double
		//..... index = 0, 1, 2, ...
	static int	getInt    ( const char* string, int*    val, int index = 0 ) ;
	static int	getDouble ( const char* string, double* val, int index = 0 ) ;

		//----- search address of non-white character in the given string
		//...... within the range, string[0] , ... , string[max_index] 
		//...... Usually, max_index is given as strlen( string )
	static  char* getBeginningOfString( char* string , int max_index );


	static  int isEnvironmentalVariableTrue( const char* string );


} ; // ToolString


/////////////////////// end of toolstring.h
#endif
