//////////////////////
///// version.h  /////
//////////////////////

#if !defined VERSION_H
#define VERSION_H

#include "dev_control.h"

#if   defined USE_OPEN_GL
const	char	VERSION[] = "3.85e-GL" ;
#elif defined USE_XWIN
const	char	VERSION[] = "3.85e-X" ;
#elif defined USE_VC 
const	char	VERSION[] = "3.85e-Win (VC++)" ;
#elif defined USE_CYGNUS
const	char	VERSION[] = "3.85e-CygnusB19" ;
#else 
const	char	VERSION[] = "3.85e (Dev. indep. Mode)" ;
#endif

const	char	DATE[]    = "October 15, 2003" ;

#endif
