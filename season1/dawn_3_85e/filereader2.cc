///////////////////////////
///// filereader2.cc  /////
///////////////////////////


// #define DEBUG_FILE_READER2

#include "filereader.h" 

	//----- FileReader::readPolygonOut()
int FileReader::readPolygonOut( Object& object , List<LineSegment>* linseg_set_p , DataSize& total_data_size )
{
		//----- local
	const	int	mode_winged_edge = 0 ; // no winged-edge structure
	int	vnum = 0 ; // initialized to dummy value
	int	label ;
	int	status ;
	int	num_read_linesegment ;
	int	num_read_vertex ;
	int	num_read_facet ;
	double		x, y, z ;	// vertex position
	Vector3		origin ;
	Vertex		vertex_item ;
	List<Vertex>   vertex_list ;
	Vector3		lineseg_begin , lineseg_end ;
	char	ch       ;
	char*	buf      = NULL    ;
	char*	buf2     = NULL    ;
	int*	vertices = NULL;
	char	dummy[256];
	
		//----- size of arrays
	int	LINEBUF     = DEFAULT_LINEBUF   ;
	int	VERTEX_MAX  = DEFAULT_VERTEX_MAX  ;

	if( getenv( "DAWN_BUF_SIZE" ) != NULL ) {
		sscanf( getenv("DAWN_BUF_SIZE" ), "%d", &LINEBUF ) ;
	} 
	if( getenv( "DAWN_MAX_FACET_SIZE" ) != NULL ) {
		sscanf( getenv("DAWN_MAX_FACET_SIZE" ), "%d", &VERTEX_MAX ) ;
	} 

		//----- dynamical memory allocation
	buf      = new char [ LINEBUF    ] ;
	buf2     = new char [ LINEBUF    ] ;
	vertices = new int  [ VERTEX_MAX ] ;

		//----- abandon header comment
	getLine( buf , LINEBUF ,'\n') ;
	if( buf[0] != '%' ) {
			//----- bad data format
		cerr << "ERROR (Filereader::readPolygonOut(), 1:" << endl;
		cerr << "     Data file \"" << (this->getFileName()) << "\" is not of format version PO" << endl;
		exit(1);		
	}

		//----- set convexity to object
	object.setConvexity(1) ; // convex object!

		//----- Read Origin
		//---------- (1) skip null lines
	while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
		if( strlen( buf ) != 0  ) {
			break ;
		} 
	}
		//---------- (2) parse data
	sscanf( buf, "%s %lg %lg %lg", dummy , &x, &y, &z ) ;

		//---------- (3) set origin of object
	origin( x, y, z ) ;	object.setOrigin( origin );
	origin = object.getOrigin();
	cerr << " *** object_origin = " ; origin.print() ; cerr << endl ;

		//----- skip lines until vertex block begins
	while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
		if( strstr( buf, BEGIN_VERTEX ) != NULL ) {
			break ; // ---> to facet part
		}
	}

		//----- read vertices
	num_read_vertex = 0 ; // clear counter 
	while( input.getline( buf, LINEBUF ) )
	{
			//----- If  BEGIN_FACET is found,
			//..... go to definition area of faces.
		if( strstr( buf, BEGIN_FACET ) != NULL ) {
			break ;
		}

			//----- read label, x, y, z of a vertex
		if ( sscanf( buf, "%d %lg %lg %lg", &label, &x, &y, &z ) == EOF ) 
		{ 		
			continue ;
		}

			//----- add label and vertex to object
		addVertex( label, Vector3( x, y, z ) , object ) ;
		vertex_item = Vertex( label, Vector3( x, y, z ) ) ;
		vertex_list.addItem( vertex_item ) ;
		
			//----- increment counter
		num_read_vertex++ ;
	}
	total_data_size.incrementNumVertex( num_read_vertex );
	cerr << " *** number of vertices : " << num_read_vertex << '\n' ;

		//----- read faces
	num_read_facet = 0 ;     // reset counter for facets
	num_read_linesegment = 0 ; // reset counter for linesegments
	while( ( status = getLine( buf, LINEBUF, TERMINAL_CHAR )) ) 
	{
			//----- local
		LineType	linetype_tmp;
		Vertex		vertex_item_tmp ;		
		double		red_tmp = 1.0 , green_tmp = 1.0, blue_tmp = 1.0 ;

			//----- buffer overflow?
		if( status < 0 ) {
			cerr << "ERROR (Filereader::readPolygonOut(), 2:" << "\n" ;
			cerr << "  There exists a too long line in data file.\n";
			cerr << "  Reset environmental variable DAWN_BUF_SIZE to a larger value.\n";
			cerr << "  e.g.  setenv DAWN_BUF_SIZE " << (2 * LINEBUF ) << "\n";
			cerr << "  Current value is " << LINEBUF << "." << endl;
			exit(1);
		}

			//----- skip to beginning of color 
		while( 1 ) {
			input.get(ch) ;
			if( ch == '{' ) { break ;} 
		}

			//----- read color
		getLine( buf2, LINEBUF, '}' ) ;
		sscanf( buf2, "%lf %lf %lf", &red_tmp, &green_tmp, &blue_tmp ) ;

			//----- vertices: integer array of vertex lables
			//....  num     : number of items in the array
		getVertices( buf, vertices, VERTEX_MAX, vnum ) ;

			//----- read facet data
		if( vnum >= 3 )  { 
			addFacetToObject(	num_read_facet, vertices, vnum, 
						ColorType( red_tmp, green_tmp, blue_tmp ), 
						object )  ;
			num_read_facet++ ; 
		} else if ( vnum == 2 )  { // line data

				//------ starting point
			if( linseg_set_p != NULL ) { 
				num_read_linesegment++ ;

					//------ starting point of linesegment
				vertex_item_tmp = vertex_list[vertices[0]];
				lineseg_begin = vertex_item_tmp.getPosition() + (object.getOrigin());	

					//------ ending point of linesegment
				vertex_item_tmp = vertex_list[vertices[1]];
				lineseg_end = vertex_item_tmp.getPosition() + (object.getOrigin()) ;	

					//----- set color
				linetype_tmp = LineType(); // initialized to default
				linetype_tmp.setColor( ColorType( red_tmp, green_tmp, blue_tmp ) );

					//------ output
				linseg_set_p->addItem( LineSegment(lineseg_begin, lineseg_end, linetype_tmp ) );

			} // if( linseg_set_p != NULL )

		} else  {
			continue ;	// too few labels 
					// ---> skip this line
		}

	} // while( input.getline(...)  )

		//----- debugging information
	total_data_size.incrementNumFacet( num_read_facet       );
	total_data_size.incrementNumLine ( num_read_linesegment );
	cerr << " *** number of facets       : " << num_read_facet       << '\n' ;
	cerr << " *** number of linesegments : " << num_read_linesegment << '\n' ;
	cerr << " *** Constructing Object .....\n";

		//----- make half edge structure
		//.....  (1) setOtherEdgeAndRightFacet() ; // only if mode_winged edge is true
		//.....  (2) setAllArray() ;
		//.....  (3) setWorldCoordData();
	if( num_read_vertex > 0 && num_read_facet > 0 ) {
		object.install( mode_winged_edge );
	}
		//----- delete dynamical memories
	delete [] buf ;
	delete [] buf2 ;
	delete [] vertices ;


		//----- return 
	return NORMAL ;

} // readPolygonOut()


	//----- FileReaderscan::scanPolygonOut()
void FileReader::scanPolygonOut( Perspective& camera, NameAndValue& object , Box3D& bounding_box , Parameter& parameter )
{
		//----- local
	Box3D		box3d ;
	Vector3		origin ;
	Vector3		center ;
	const	int	LINEBUF = 256 ;
	char		buf[LINEBUF];
	double		object_value ;
	char		dummy[256];

		//----- abandon header comment
	getLine( buf , LINEBUF ,'\n') ;
	if( buf[0] != '%' ) {
			//----- bad data format
		cerr << "ERROR (FileReader::scanPolygonOut()), 1\n";
		cerr << "Data file \"" << (this->getFileName()) << "\" is not of format version PO " << endl;
		exit(1);		
	}

		//----- skip null lines
	while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
		if( strlen( buf ) != 0  ) {
			break ;
		} 
	}

		//---------- (1) set object origin to local variable "origin"
	sscanf( buf, "%s %lg %lg %lg", dummy , &(origin.x), &(origin.y), &(origin.z) ) ;
		//---------- (2) set camera target to parameter
	getLine( buf, LINEBUF ,'\n' ) ; 
	sscanf( buf, "%s %lg %lg %lg", dummy ,  &(parameter.target_x), \
						&(parameter.target_y), \
						&(parameter.target_z) ) ;

		//---------- (3) set camera position to parameter
	getLine( buf, LINEBUF ,'\n' ) ; 
	sscanf( buf, "%s %lg %lg %lg", dummy ,  &(parameter.camera_distance),\
						&(parameter.camera_v_angle) ,\
						&(parameter.camera_h_angle ) ) ;

		//---------- (4) set screen distance to parameter
	getLine( buf, LINEBUF ,'\n' ) ; 
	sscanf( buf, "%s %lg",         dummy , &(parameter.focal_distance) );
		//---------- (5) min position (in body coord)
	getLine( buf, LINEBUF ,'\n' ) ; 
	sscanf( buf, "%s %lg %lg %lg", dummy ,  &(parameter.xmin),\
						&(parameter.ymin) ,\
						&(parameter.zmin) ) ;
		//---------- (6) max position (in body coord)
	getLine( buf, LINEBUF ,'\n' ) ; 
	sscanf( buf, "%s %lg %lg %lg", dummy ,  &(parameter.xmax),\
						&(parameter.ymax) ,\
						&(parameter.zmax) ) ;

		//----- calc bounding box (body coord)
	box3d.set(	parameter.xmin, 
			parameter.ymin, 
			parameter.zmin, 
			parameter.xmax, 
			parameter.ymax, 
			parameter.zmax ); 

		//----- calc center.z_eye (body coord, world coord)
	center = box3d.getCenter();	// center in body coord
	center += origin ;		// center in world coord 
	object_value = camera.eyeZ( center );
	object.setValue( object_value ); 

		//----- return bounding box (world coord)
	bounding_box = box3d ;	bounding_box.shiftBy( origin ) ;

} // FileReader::scanPolygonOut()


	//----- FileReader::addFacetToObject()
int FileReader::addFacetToObject(int label, int *v, int n, const ColorType& color, Object& object )
{
	int	i;
	int	edge;					// current edge
//	int	edge_data[n];				// edge list
	int*	edge_data	= new int [n] ;		// edge list

	int	startEdge = object.getNumEdgeInList();	// start edge
	int	endEdge = startEdge + n - 1;		// end   edge

		//----- data which constructs half-edge data
	int	vertex;					// vertex
	int	pVertex = v[n-1];			// previous vertex
	int	prev;					// previous edge
	int	next;					// next edge
	int	other;					// other-half edge
	int	facet = object.getNumFacetInList();	// left-facet
	int	rFacet;					// right-facet

		//----- too few vertices for a facet?
	if( n < 3 ) {
		cerr << "facet definition error\n" ;
		return ERROR ;
	}


		//----- add edge data to object
	for(i=0, edge=startEdge; i < n; i++, edge++)
	{
		vertex = v[i];

		next = (edge == endEdge) ? startEdge : edge + 1;

		prev = (edge == startEdge) ? endEdge : edge - 1;

		other = rFacet = NO_ITEM ;

			//----- add Edge to list in Object
		const int INVISIBLE = 0 ;
		if( vertex < 0 ) { 
				//----- add INVISIBLE edge
			object.addEdgeItem(Edge( edge, \
						 abs(vertex), abs(pVertex), \
						 next, prev, other, \
						 rFacet, facet,\
						 INVISIBLE                 ));
		} else {
				//----- add VISIBLE edge
			object.addEdgeItem(Edge( edge, 
						 abs(vertex), abs(pVertex), 
						 next, prev, other, 
						 rFacet, facet             ));
		}

			//----- add edge's label to list in Facet
		edge_data[i] = edge;

		pVertex = vertex;
	}


		//----- add facet data to object
		//----------- declare facet 
	Facet	facetItem(label );

		//----------- edge
	for(i=0 ; i<n; i++) { 
		facetItem.addEdge( edge_data[i] ) ; 
	}
	facetItem.setEdgeArray() ; 
		// do nothing in current version of class Facet

		//----------- color
	facetItem.setColor    ( color );
	facetItem.setColorMode( Facet::HARD_COLOR );

		//---------- add facet to object
	object.addFacetItem(facetItem);

		//----- delete
	delete [] edge_data ;

		//----- return
	return NORMAL ;

}// FileReader::addFacetToObject()
