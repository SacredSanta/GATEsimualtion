//////////////////////////////////
///// toolio.h   1995/08/20  /////
//////////////////////////////////

#if !defined _TOOLIO_H
#define _TOOLIO_H


#include<iostream.h>
#include<fstream.h>
#include<stdio.h>
#include <stdlib.h>	

class ToolIO {

 public:
	enum{ COPY, MOVE };

		//----- convine file1 and file2 and make file_sum
		//.....  if mode = MOVE, file1 and file2 are removed, else they remain.
 	static int convineFile( char* file1, char* file2 , char* file_sum , int mode = MOVE ) ;
 	
		//----- message
	static  void coutLine     ( char * string ) { cerr << string << endl ; }
	static  void message      ( char * string ) { cerr << string << endl ; }
	static  void message      ( int    num    ) { cerr << num    << endl ; }

		//----- errMessage
	static  void errorMessage   ( char* function_name , char* label = NULL , char * message = NULL ) 
		{ cerr << "ERROR   ("<< function_name << "), " << label << ": " << message << endl; exit(1) ; }
	static  void warningMessage ( char* function_name , char* label = NULL , char * message = NULL ) 
		{ cerr << "WARNING ("<< function_name << "), " << label << ": " << message << endl; }
	static  void cerrLine     ( char * string ) { cerr << string << endl ; }


} ; // ToolIO


#endif

//////////////// end of toolio.h
