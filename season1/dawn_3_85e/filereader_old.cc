//////////////////////////////////////////
///// filereader_old.cc  1995.05.15 //////
//////////////////////////////////////////


#include "filereader.h"
#include "polygon_attribute.h"


	//----- FileReaderscan::OldPlanData()
void FileReader::scanOldPlanData( Perspective& camera, NameAndValue& object , Box3D& bounding_box )
{
		//----- local
	int		num_read_vertex = 0 ;
	int		label ;
	double		x, y, z ;	// vertex position
	Box3D		box3d ;
	Vector3		origin ;
	Vector3		center ;
	const	int	LINEBUF = 256 ;
	char		buf[LINEBUF];
	double		object_value ;

		//----- abandon header comment
	getLine( buf , LINEBUF ,'\n') ;
	if( buf[0] != '%' ) {
			//----- bad data format
		cerr << "ERROR (FileReader::scanOldPlanData()), 1\n";
		cerr << "Data file \"" << (this->getFileName()) << "\" is not of format version 00 or PO " << endl;
		exit(1);		
	}

		//----- Read Origin
		//---------- skip blank lines
	while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
		if( strlen( buf ) != 0  ) {
			break ;
		} 
	}
		//---------- read data
	sscanf( buf, "%lg %lg %lg", &x, &y, &z ) ;
	origin( x, y, z ) ;

		//----- read vertices
	num_read_vertex = 0 ;
	while( input.getline( buf, LINEBUF ) )
	{
			//----- If  BEGIN_FACET_OLD is found,
			//..... get out of vertex block
		if( strstr( buf, BEGIN_FACET_OLD ) != NULL ) {
			break ;
		}

			//----- read label, x, y, z of a vertex
		if ( sscanf( buf, "%d %lg %lg %lg", &label, &x, &y, &z ) == EOF ) 
		{ 		
			continue ;
		} else {
				//----- count number of vertices
			num_read_vertex++;
		}
			//----- set bounding box
		if( num_read_vertex == 1 ) {
			box3d.set   ( x, y, z, x, y, z );
		} else {
			box3d.update( x, y, z );
		}
	} // while

		//----- calc value center.z_eye
	center = box3d.getCenter();	// center in body coord
	center += origin ;		// center in world coord 
	object_value = camera.eyeZ( center );
	object.setValue( object_value ); 

		//----- return bounding box in world coord
	bounding_box = box3d ;	bounding_box.shiftBy( origin ) ;

} // FileReader::scanOldPlanData()


	//----- FileReader::readOldPlanData( object , linseg_list )
int FileReader::readOldPlanData( Object& object , List<LineSegment>* linseg_set_p , int mode_winged_edge , DataSize& total_data_size )
{

		//----- local
	int	vnum = 0 ; // initialized to dummy value
	int	label ;
	int	status ;
	int	num_read_linesegment ;
	int	num_read_vertex ;
	int	num_read_facet ;
	double	x, y, z ;	// vertex position
	Vector3		origin ;
	LineType	linetype , linetype_tmp;
	Vertex		vertex_item_tmp ;
	List<Vertex>   vertex_list ;
	Vector3		lineseg_begin , lineseg_end ;
	char	ch       ;
	char*	buf      = NULL    ;
	int*	vertices = NULL;
	PolygonAttribute	polygon_attribute ;
	int			flag_att_set = 0 ;
	int			int_dummy = -1 ;
	
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
	vertices = new int  [ VERTEX_MAX ] ;


		//----- abandon header comment
	getLine( buf , LINEBUF ,'\n') ;
	if( buf[0] != '%' ) {
			//----- bad data format
		cerr << "Data file \"" << (this->getFileName()) << "\" is not of format version 00" << endl;
		cerr << "ERROR in header comment (3)" << endl;
		exit(1);		
	}

		//----- Read Origin
		//---------- skip blank lines
	while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
		if( strlen( buf ) != 0  ) {
			break ;
		} 
	}
		//---------- read data
	sscanf( buf, "%lg %lg %lg", &x, &y, &z ) ;

		//---------- set origin
	origin( x, y, z ) ;	object.setOrigin( origin );
	origin = object.getOrigin();
	cerr << " *** object_origin = " ; origin.print() ; cerr << endl ;

		//----- read vertices
	num_read_vertex = 0 ; // clear counter 
	while( input.getline( buf, LINEBUF ) )
	{
			//----- If  BEGIN_FACET_OLD is found,
			//..... go to definition area of faces.
		if( strstr( buf, BEGIN_FACET_OLD ) != NULL ) {
			break ;
		}

			//----- read label, x, y, z of a vertex
		if ( sscanf( buf, "%d %lg %lg %lg", &label, &x, &y, &z ) == EOF ) 
		{ 		
			continue ;
		}

			//----- add label and vertex to object
		addVertex( label, Vector3( x, y, z ) , object ) ;
		vertex_item_tmp = Vertex( label, Vector3( x, y, z ) ) ;
		vertex_list.addItem( vertex_item_tmp ) ;
		
			//----- increment counter
		num_read_vertex++ ;
	}
	total_data_size.incrementNumVertex( num_read_vertex );
	cerr << " *** number of vertices : " << num_read_vertex << '\n' ;

		//----- read faces
	num_read_facet = 0 ;     // reset counter for facets
	num_read_linesegment = 0 ; // reset counter for linesegments
	while( ( status = getLine( buf, LINEBUF, TERMINAL_CHAR ) ) ) 
	{

			//----- buffer overflow?
		if( status < 0 ) {
			cerr << "ERROR:\n";
			cerr << "  There exists a too long line in data file.\n";
			cerr << "  Reset environmental variable DAWN_BUF_SIZE to a larger value.\n";
			cerr << "  e.g.  setenv DAWN_BUF_SIZE " << (2 * LINEBUF ) << "\n";
			cerr << "  Current value is " << LINEBUF << "." << endl;
			exit(1);
		}
		

			//----- read attribures
		flag_att_set = 0 ;
		polygon_attribute( int_dummy, 1.0, 1.0, 1.0 ) ; // initialize to white
		input.get(ch) ;
		switch (ch) { 
		 case ' ':
		 case '\n':
		 case '\t':
			break ; // if white, do nothing
		 case '{':
				//----- read  attributes of facet
				//..... ( exit if error )
			flag_att_set = 1 ;
			readAttribute( polygon_attribute ) ;
//			if( readAttribute( polygon_attribute ) < 0 ) {  exit(1) ; }
				
			
			break ; // if white, do nothing
		 default:
			input.putback(ch);
		}

			//----- vertices: integer array of vertex lables
			//....  num     : number of items in the array
		getVertices( buf, vertices, VERTEX_MAX, vnum ) ;

			//----- read facet data
		if( 0 == vnum  || 1 == vnum )  {
			continue ;	// too few labels 
					// ---> skip this line
		} else if ( 2 == vnum )  { // line data

				//------ starting point
			if( linseg_set_p != NULL ) { 
				num_read_linesegment++ ;

					//------ starting point of linesegment
				vertex_list.toTail() ;
				while( vertex_list.getItem( vertex_item_tmp ) ) { 
					label = vertex_item_tmp.getLabel() ;
					if( label == vertices[0] ) {
						lineseg_begin = vertex_item_tmp.getPosition() + origin ;	
						break ; // go to search ending point
					} else {
						vertex_list.upward();
					}
				}

					//------ ending point of linesegment
				vertex_list.toTail() ;
				while( vertex_list.getItem( vertex_item_tmp ) ) { 
					label = vertex_item_tmp.getLabel() ;
					if( label == vertices[1] ) {
						lineseg_end = vertex_item_tmp.getPosition() + origin ;	
						break ; // go to outputting part
					} else {
						vertex_list.upward();
					}
				}

					//----- set color
				if( flag_att_set ) {
					ColorType color_tmp( polygon_attribute.red, polygon_attribute.green, polygon_attribute.blue );  
					linetype_tmp = linetype ;
					linetype_tmp.setColor( color_tmp );
				} else {
					linetype_tmp = linetype ;
				}

					//------ output
				linseg_set_p->addItem( LineSegment(lineseg_begin, lineseg_end, linetype_tmp ) );

			} // if( linseg_set_p != NULL )

		} else { 
			if( flag_att_set ) {
				polygon_attribute.setColorMode( Facet::SOFT_COLOR );
				status = addFacet( num_read_facet, vertices, vnum, &polygon_attribute, object )  ;
			} else {
				status = addFacet( num_read_facet, vertices, vnum, NULL, object )  ;
			}
			if ( ERROR == status ) { return ERROR ; }
			else { num_read_facet++ ;  }
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
	delete [] vertices ;


		//----- return 
	return NORMAL ;

} // readOldPlanData( object, line )




	//----- make edge-half and facet item from vertex list
	//	'v' ... vertex list which composes facet
	//	'n' ... number of vertex
int FileReader::addFacet(int label, int *v, int n, PolygonAttribute* attribute_p, Object& object)
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
		// no nothing in current version of class Facet

		//----------- color
	if( attribute_p != NULL ) {
		ColorType color( attribute_p->red, attribute_p->green, attribute_p->blue ) ; 
		facetItem.setColor( color , (attribute_p->getColorMode()) );
	}

		//---------- add facet to object
	object.addFacetItem(facetItem);

		//----- delete
	delete [] edge_data ;

		//----- return
	return NORMAL ;

}// FileReader::addFacet()


	//----- FileReader::readAttribute( )
int FileReader::readAttribute( PolygonAttribute& polygon_attribute )
{
		//----- local
	enum { size = 128 } ;
	char	buf[size];
	int	status ;
	
		//----- read string 
	status = getLine( buf, size, '}' ) ;

		//----- error checking
	if( status < 0 ) {
		cerr << "WARNING: (Filereader::readAttribute(), 1\n";
		cerr << "  ( status = " << status << endl;
	}


		//----- read  attribute
	istrstream	str(buf) ;
	if( !(str >> polygon_attribute.red >> polygon_attribute.green >> polygon_attribute.blue ) ) 
	{
		status = -2 ;
		cerr << "WARNING: (Filereader::readAttribute(), 2 \n";
		cerr << "  ( status = " << status << endl;
	}

		//----- return status 
	return status ;

} // FileReader::readAttribute()
