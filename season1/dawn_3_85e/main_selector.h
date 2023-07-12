//////////////////////////////
///// main_selector.h  //////
//////////////////////////////

#if !defined MAIN_SELECTOR_H
#define MAIN_SELECTOR_H

	//----- prototype
#include "dev_control.h"
#include "command.h"
#include "parameter.h" 


//class Command ;
//class Parameter ;

	//-------------------------// 
	//----- Main Selector -----//
	//-------------------------// 
class MainSelector {


 public:
	int standAlone         ( int argc, char* argv[] , Command& command, Parameter& parameter );
	int primViewer         ( int argc, char* argv[] , Command& command, Parameter& parameter );

#if !defined USE_VC
	int daemonProcessUnix  ( int argc, char* argv[] , Command& command, Parameter& parameter );
	int daemonProcessINET  ( int argc, char* argv[] , Command& command, Parameter& parameter );
#endif

}; // MainSelector 

/////// common include files for various main functions
#include "FRDevice.h"
#include "command.h"
#include "filereader.h"
#include "hid.h" 
#include "hidline.h" 
#include "linesegment.h"
#include "message.h"
#include "name_and_value.h"
#include "object.h"
#include "parameter.h" 
#include "perspective.h"
#include "postscript.h" 


////////////// end of main_selector.h
#endif
