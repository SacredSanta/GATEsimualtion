#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream.h>

#include "main_selector.h"

#include "FRCommandAnalyzer.h"
#include "FRFileServer.h"

	//----- output parameters
// #define DEBUG_MAIN2


int
MainSelector::primViewer( int argc, char* argv[] , Command& command, Parameter& parameter )
{
	cerr << "***** g4.prim viewer mode (default)" << endl;
	cerr << "***** (\"dawn -h\" for help)" << endl << endl;

		//---------------------//
		//----- variables -----//
		//---------------------//
	FRFileServer server (argv[1] );
	const	Attribute	defaultAttribute ;
	FRCommandAnalyzer	commandAnalizer( argv[1], command, parameter , defaultAttribute );
	int dummy = argc; dummy = 0 ;

		//----------------------------//	
		//----- File format test -----//	
		//----------------------------//	
	{
		ifstream   fin( argv[1] ) ;
		char ch1 = '\0';
		char ch2 = '\0';

		if ( !fin )
		{ 
			cerr << "ERROR: Cannot open file " << argv[1]  << endl;
			exit(1); 
		}  
		fin.get( ch1 ) ; fin.get( ch2 ) ;
		if( ch1 != '#' || ch2 != '#' )    
		{ 
			cerr << argv[1]  << " is not a g4.prim-format file";
			cerr << endl;
			exit(1); 
		}   

		fin.close();
	}

		//------------------------------//
		//------ parameter setting -----//
		//------------------------------//
		//..... set user parameter
	command.setParameter( parameter );


	server.ReceiveMessage(&commandAnalizer) ;


#if defined DEBUG_MAIN2
	ofstream fout("xxx.param_debug") ; 
	command.printParameter( parameter,  fout ) ;
#endif

		//----- return 
	return 1 ;

} // MainSelector::primViewer()

