///////////////////////
///// FRDevice.h  /////
///////////////////////

#if !defined  FRDEVICE_H
#define FRDEVICE_H

#include "dev_control.h"

#if defined USE_XWIN
  #include "x_window.h"
  #include "ps2x.h"
#endif

#if defined USE_OPEN_GL
  #include "FROpenGL.h"
#endif

#endif // END of FRDevice.h
