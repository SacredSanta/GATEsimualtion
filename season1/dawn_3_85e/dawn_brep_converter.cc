#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream.h>
#include <iostream.h>
#include <strstream.h>

const int MAX = 1024 ;


int is_blank_line( char* line ) 
{
	enum {LINE, BLANK};  

	int  status = BLANK ;
	char* p = NULL  ;
	 
	if        ( line[0] == '\0' ) {
		status = BLANK ;
		return status ;
	} else if ( strlen( line )  == 0 ) {
		status = BLANK ;
		return status ;
	}

	for ( p  = line; ( *p != '\0' || *p != '\n' ) ; p++ ) 
	{
			
		char c = *p ;

		if ( !isspace( c ) ){

			status = LINE ;
			break ;		 	      
		}
	}

	return status ;
}



int main( int argc, char* argv[])
{
	char buf [MAX];

	if( argc != 2 ) {

	 cerr << "USAGE: dawn_brep_converter old-format-file";
	 cerr << endl;	
	 exit(1);	
	}


	// input stream
	ifstream input ( argv[1] ) ; 

	// states
	int in_vertex = 0 ;
	int in_facet  = 0 ;

	while( input.getline( buf, MAX ) ) 
	{

		if ( is_blank_line( buf ) ) { 

			cout << endl;

		} 
		else   if ( !strncmp( buf, "/Brep"      , 5  ) ) 
		{
			cout << "/Polyhedron" << endl;

		} else if ( !strncmp( buf, "/EndBrep"   , 8  ) ) 
		{
			cout << "/EndPolyhedron" << endl;

		} else if ( !strncmp( buf, "BeginVertex", 11 ) ||\
		            !strncmp( buf, "Vertex:"    ,  7 )    ) 
		{
			in_vertex = 1 ;	

		} else if ( !strncmp( buf, "EndVertex"  ,  9 ) ||\
		            !strncmp( buf, "end_Vertex",  10 )    ) 
		{
			in_vertex = 0 ;	

		} else if ( !strncmp( buf, "BeginFacet" , 10) ||\
		            !strncmp( buf, "Facet:"     ,  6)    ) 
		{
			in_facet  = 1 ;

		} else if ( !strncmp( buf, "EndFacet"   ,  8) ||\
		            !strncmp( buf, "end_Facet"  ,  9)    ) 
		{
			in_facet  = 0 ;

		} else if ( in_vertex ) {

			int    dummy;
			double x, y, z ;
			sscanf( buf, "%d %lg %lg %lg", &dummy, &x, &y, &z );

			cout << "/Vertex" << " " ;
			cout << x         << " " ;
			cout << y         << " " ;
			cout << z         << endl;


		} else if ( in_facet ) {

			int flabel ;	
			char* p = buf ; 

			cout << "/Facet ";

			istrstream str_in ( buf ) ;
			while ( str_in >> flabel ) {	
				cout << flabel << " " ;
			}
			cout << endl;

		} else {
			cout << buf << endl;
		}

	}
	
	input.close();

}
