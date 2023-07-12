#include <process.h>
#include<stdio.h>
#include<stdlib.h>
#include <error.h>

int main() 
{
		int termstat ;

		char DAWN_GUI_TCL []  = "C:\\Progra~1\\DAWN\\DAWN_GUI.tcl";

		char * WISH = getenv( "WISH");
		if(!WISH) { WISH = "wish";}

		int pid = spawnlp(_P_NOWAIT, WISH, WISH, DAWN_GUI_TCL, NULL );
		_cwait(  &termstat, pid, _WAIT_CHILD);

		return 0 ;
}
