#include <iostream.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	char command[256];

	if( argc != 2 ) { 
		cerr << "USAGE: g4mini file.prim" << endl;
		exit(1);
	}

	strcpy(command, "dawn ");
	strcat(command, argv[1]);

	system (command);
	
	return 0;
}
