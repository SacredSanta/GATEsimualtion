//////////////////////////
/////  command4.cc  /////
//////////////////////////


/////////////////
static int flag_device_open = 0 ;
////////////////

#include "command.h"

#if   defined USE_OPEN_GL
 #include "command4a.icc"
#elif defined USE_XWIN
 #include "command4b.icc"
#else 
 #include "command4c.icc"
#endif
