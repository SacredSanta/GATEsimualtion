//////////////////////////////////////
///// fileReader.h   1995.05.15 //////
//////////////////////////////////////


#ifndef __FILEREADER_H
#define __FILEREADER_H

#include <fstream.h>

#include "dev_control.h"
#if defined USE_VC
	#include <strstrea.h>
#else
	#include <strstream.h>
#endif

#include <stdio.h>		
#include <stdlib.h>	
#include <string.h>

#include "attribute.h"
#include "linesegment.h"
#include "name_and_value.h"
#include "object.h"
#include "parameter.h"
#include "list.h"
#include "toolstring.h"
#include "brepformat.h"

	//----- proto type
class PolygonAttribute ; // for old data format

	//------------------------------//
	//-----  class DataSize  -------//
	//------------------------------//
struct DataSize {
 public:
	int	num_vertex ;
	int	num_facet  ;
	int	num_line   ;

	DataSize() { num_vertex = num_facet = num_line = 0 ; }
	void	incrementNumVertex( int n = 1 ) { num_vertex += n  ; }
	void	incrementNumFacet ( int n = 1 ) { num_facet  += n  ; }
	void	incrementNumLine  ( int n = 1 ) { num_line   += n  ; }

	void	operator = ( const DataSize& rhs )
		{
			num_vertex = rhs.num_vertex ;
			num_facet  = rhs.num_facet  ;
			num_line   = rhs.num_line   ;
		}

	void	clear() 
		{ num_vertex = num_facet = num_line = 0 ; }

}; // DataSize

	//--------------------------------//
	//-----  class FileReader  -------//
	//--------------------------------//

class	FileReader {

		//----- const
	enum { ERROR, NORMAL } ;

		//----- input file stream of data
	char*		filename;
	ifstream	input ;


	void	getVertices( char *s, int* vertices, int VERTEX_MAX , int& num ) ;

	int	addVertex( int label, Vector3 pos, Object& obj );

		//----- add facet to object
		//	'v' ... vertex list which composes facet
		//	'n' ... number of vertex
	int	addFacetToObject(int label, int *v, int n, const Attribute* attribute_p, Object& object );
					// if attribulte_p is NULL, no attribute is set to facet
	int	addFacetToObject(int label, int *v, int n, const ColorType& color, Object& object );
					// The facet shall have a fixed color.

public:
		// constructor
	FileReader(void) ;

		// destructor
	~FileReader()	{ close() ; }

		// functions

	int		open( const char *filename ) ;
	int		readPlanData ( Object& obj , List<LineSegment>* lineseg_set_p  ,
                                       const Parameter& parameter, DataSize& total_data_size ) ;
	char*		getFileName() { return filename ; }

		//-----  Read a line from data member "input".
		//.....  Here line means a stream of characters ended with "END_LINE" characters.
		//.....  Read line is returned to "buf" with size "LINE_BUF,
		//.....  in which character "END_LINE" is replaced with '\0'.
		//.....  Return:
		//.....		1: success, 0: EOF is read, -1: buf overflow
	int	getLine( char* buf, int BUF_LENGTH, char END_LINE );

		//----- read attributes of polygon from "input"
		//.....  Return:
		//.....   1: normal, 0:EOF is read, -1: buffer overflow, -2: stream err
	int	readOldPlanData( Object& object , List<LineSegment>* linseg_set_p , int mode_winged_edge  , DataSize& total_data_size );

	int	readPolygonOut( Object& object , List<LineSegment>* linseg_set_p , DataSize& total_data_size ) ;


		//----- Scan data file and
		//..... (1) calc ( or read ) bounding box                  (in world coord)
		//..... (2) set BoundingBox.center.position.eyeZ to object (in world coord)
	void	scanPlanData   ( Perspective& camera, NameAndValue& object , Box3D& bounding_box );
	void	scanOldPlanData( Perspective& camera, NameAndValue& object , Box3D& bounding_box );
	void	scanPolygonOut ( Perspective& camera, NameAndValue& object , Box3D& bounding_box , Parameter& parameter );

	int	close( void ) ;


		//----- used only for old data format
 public:
	int	readAttribute( PolygonAttribute& polygon_attribute );
			//   1: normal, 0:EOF is read, -1: buffer overflow, -2: stream err
 private:
	int	addFacet( int label, int* vertex, int n, PolygonAttribute* attribute_p, Object& obj );
					// if attribulte_p is NULL, no attribute is set to facet

} ; // FileReader

#endif
