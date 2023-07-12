/////////////////////////////////////////////////
/////	HEADER FILE: toolMath.h
/////////////////////////////////////////////////

#if !defined  TOOL_MATH_H
#define       TOOL_MATH_H 

	//----- INCLUDE
#include <iostream.h>
#include <math.h>	
#include "FRMath.h"

	//----------------------//
	//----- constants ------//
	//----------------------//

	//----------------------------//
	//----- CLASS ToolMath ------//
	//----------------------------//
class ToolMath {

 public:
		//----- constructor
	ToolMath( void ){ ; }

	double Sin ( double angleInDegree ) ;
	double Cos ( double angleInDegree ) ;
	double ArcSin ( double realNumber ) ;
	double ArcCos ( double realNumber ) ;
}; // ToolMath


	//-------------------------------------------------------//
	//----- inline member functions of class ToolMath ------//
	//-------------------------------------------------------//

	//----- sin & cos for angles in degrees
inline
double ToolMath::Sin ( double angleInDegree )
{
	return  sin( angleInDegree * FR_D_PI / 180.0 ) ;
}

inline
double ToolMath::Cos ( double angleInDegree )
{
	return  cos( angleInDegree * FR_D_PI / 180.0 ) ;
}

inline
double ToolMath::ArcSin ( double realNumber )
{
	return  ( asin( realNumber ) * 180.0 / FR_D_PI ) ;
}

inline
double ToolMath::ArcCos ( double realNumber )
{
	return  ( acos( realNumber ) * 180.0 / FR_D_PI ) ;
}



//////////////////////////////////////
#endif
//////////////////// END OF toolmath.h
