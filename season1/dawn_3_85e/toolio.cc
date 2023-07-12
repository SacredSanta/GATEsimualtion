#include "toolio.h" 

int
ToolIO::convineFile( char* file1, char* file2 , char* file_sum , int mode )
{
		//----- local
	char ch ;
	
		//----- open files with error checking
	ifstream	input1( file1 ) ;
	if( !input1 ) {
		cerr << "WARNING (ToolIO::convineFile() ) 1\n";
		return 0 ; // error
	}
	ifstream	input2( file2 ) ;
	if( !input2 ) {
		cerr << "WARNING (ToolIO::convineFile() ) 2\n";
		return 0 ;  // error
	}
	ofstream	output( file_sum ) ;
	if( !output ) {
		cerr << "WARNING (ToolIO::convineFile() ) 3\n";
		return 0 ;  // error
	}


		//----- file1---> file_sum 
	while ( input1.get( ch ) ) {
		output.put(ch);
	}

		//----- file2 ---> file_sum 
	while ( input2.get( ch ) ) {
		output.put(ch);
	}

		//----- close files
	input1.close();
	input2.close();
	output.close();

		//----- remove source files if mode is MOVE
	if( mode == MOVE ) {
		remove( file1 ) ;
		remove( file2 ) ;
	}
	
		//----- return status
	return 1 ; // normal return 

}// ToolIO::convineFile()

