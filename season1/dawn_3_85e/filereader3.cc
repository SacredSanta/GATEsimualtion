///////////////////////////////////////
///// filereader3.cc  1996.03.06 //////
///////////////////////////////////////

// #define DEBUG_FILE_READER

#include "filereader.h"
#include "message.h"


	//----- FileReader::readPlanData( object , linseg_list )
int FileReader::readPlanData( Object&             object , 
			      List<LineSegment>* linseg_set_p , 
			      const Parameter&    parameter    ,
			      DataSize&           total_data_size )
{

		//----- local
	int		num_read_linesegment = 0 ;
	int		num_read_vertex      = 0 ;
	int		num_read_attribute   = 0 ;
	int		num_read_facet       = 0 ;
	List<Vertex>   vertex_list ;
	char*	buf      = NULL ;
	char*	buf2     = NULL ;
	int*	vertices = NULL ;
	Attribute*		attribute_array = NULL ;
	Attribute	att_default ;

	int		mode_winged_edge = parameter.winged_edge_mode ;


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
	buf      = new char [ LINEBUF    ] ;  strcpy( buf,  "");
	buf2     = new char [ LINEBUF    ] ;  strcpy( buf2, "");
	vertices = new int  [ VERTEX_MAX ] ;
	
	
		//----- BLOCK 0: data format
	getLine( buf , LINEBUF ,'\n') ;
	if(	strncmp(buf, FORMAT_VERSION_TITLE, FORMAT_VERSION_TITLE_LENGTH ) ||\
			FORMAT_VERSION[0] != buf[ FORMAT_VERSION_TITLE_LENGTH     ]      ||\
			FORMAT_VERSION[1] != buf[ FORMAT_VERSION_TITLE_LENGTH + 1 ]          )
	{
		cerr << "Data file \"" << filename << "\" is not the format version " << FORMAT_VERSION << endl;
		cerr << "ERROR in header comment" << endl;
		exit(1);
	}

		//----- BLOCK 1: ObjectOrigin block
	{
			//----- local
		Vector3		origin ;
		double		x, y, z ;	// vertex position

			//----- STEP 1-1: search beginning of block
		while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
			if( strstr( buf, BEGIN_OBJECT_ORIGIN ) != NULL ) {
				break ;
			} 
		}
			//----- STEP 1-2: read data
			//---------- skip blank lines
		while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
			if( strlen( buf ) != 0  ) {
				break ;
			} 
		}
			//---------- read data
		sscanf( buf, "%lg %lg %lg", &x, &y, &z ) ;

			//---------- set data to object
		origin( x, y, z ) ;	object.setOrigin( origin );
		origin = object.getOrigin();

#if defined DISPLAY_LOG
		cerr << " *** object_origin = " ; origin.print() ; cerr << endl ;
#endif		
			//----- STEP 1-3: search end of block
		while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
			if( strstr( buf, END_OBJECT_ORIGIN ) != NULL ) {
				break ;
			}
		}

	} // BLOCK 1

		//----- BLOCK 2: Read Bounding Box
		//.....  Only checking of format (Data is read in scanPlanData().)
	{
			//----- local to this block
		double xmin, ymin, zmin ;
		double xmax, ymax, zmax ;

			//----- STEP 2-1: search beginning of block
		while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
			if( strstr( buf, BEGIN_BOUNDING_BOX ) != NULL ) {
				break ;
			}
		}

			//----- STEP 2-2: read data
		if( !(input >> xmin >> ymin >> zmin >> xmax >> ymax >> zmax ) ) {
			cerr << "Data file \"" << filename << "\" is not the format version " << FORMAT_VERSION << endl;
			cerr << "ERROR in BoundingBox block" << endl;
			exit(1);
		}

			//----- STEP 2-3: search end of block
		while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
			if( strstr( buf, END_BOUNDING_BOX ) != NULL ) {
				break ;
			}
		}

	} // BLOCK 2


		//----- BLOCK 3: Vertex Block
	{
			//----- local to this block
		double		x, y , z ;
		int		vertex_label_tmp ;
		Vertex		vertex_item_tmp ;

			//----- STEP 3-1: search beginning of block
		while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
			if( strstr( buf, BEGIN_VERTEX ) != NULL ) {
				break ;
			}
		}

			//----- STEP 3-2: read data
		num_read_vertex = 0 ; // clear counter 
		while( getLine( buf, LINEBUF ,'\n' ) > 0 )
		{
				//----- search end of block
			if( strstr( buf, END_VERTEX ) != NULL ) {
				break ;
			}

				//----- read label, x, y, z of a vertex
				//.....  Note: Irregular lines, e.g. blak lines
				//.....        are skipped
			if ( sscanf(	buf, "%d %lg %lg %lg", \
					&vertex_label_tmp, &x, &y, &z ) != EOF ) 
			{ 		
					//----- add label and vertex to object
				addVertex( vertex_label_tmp, Vector3( x, y, z ) , object ) ;
				num_read_vertex++ ; // increment counter

					//----- store the vertex to list 
				vertex_item_tmp = Vertex( vertex_label_tmp, Vector3( x, y, z ) ) ;
				vertex_list.addItem( vertex_item_tmp ) ;
			} // if

		} // while
#if defined DISPLAY_LOG
		cerr << " *** number of vertices : " << num_read_vertex << '\n' ;
#endif
		total_data_size.incrementNumVertex( num_read_vertex );

	} // BLOCK 3

		//----- BLOCK 4: Attribute block
	{
			//----- local
		char* 	word   = new char   [LINEBUF];
		char*	dummy1 = new char [LINEBUF] ;
		char*	dummy2 = new char [LINEBUF] ;

		Attribute	att_tmp ;
		ColorType	color_tmp ;
		int		label_tmp = 0 ;
		double		red_tmp=1.0, green_tmp=1.0, blue_tmp=1.0 ;
		int		phong_power_tmp = 0 ;
		char		on_off[10];
		int		on_off_val ;
		List<Attribute>	attribute_list ;
	
			//----- search beginning of block
		while( getLine( buf, LINEBUF , '\n') > 0 ) {
			if( strstr( buf, BEGIN_ATTRIBUTE ) != NULL ) {
				break ;
			}
		}
	
			//----- read attributes
		while( getLine( buf, LINEBUF ,'\n') > 0 )
		{
				//----- initialize word
			strcpy(word, "");

				//----- get first word of read line
			if( ToolString::getFirstWord( buf, word ) == EOF ) {
				continue ; // skip blank line etc
			}
	
				//===== beginning of one attribute
			if     ( !strcmp( word, ATT_DEF ) ) {
				sscanf( buf, "%s %d", dummy1, &label_tmp );
				att_tmp = Attribute(); // initialization
				att_tmp.setLabel(label_tmp); // set label
			} 
				//----- kd
			else if( !strcmp( word, ATT_DIFFUSE ) ) {
				sscanf( buf, "%s %s %lf %lf %lf", \
					dummy1, dummy2, \
					&red_tmp, &green_tmp, &blue_tmp );
				color_tmp.setRGB( red_tmp, green_tmp, blue_tmp );
				att_tmp.setKd(color_tmp);
			} 
				//----- ks
			else if( !strcmp( word, ATT_SPECULAR ) ) {
				sscanf( buf, "%s %s %lf %lf %lf", \
					dummy1, dummy2, \
					&red_tmp, &green_tmp, &blue_tmp );
				color_tmp.setRGB( red_tmp, green_tmp, blue_tmp );
				att_tmp.setKs(color_tmp);
			} 
				//----- phong_power n
			else if( !strcmp( word, ATT_PHONG_POWER  ) ) {
				sscanf( buf, "%s %d", dummy1, &phong_power_tmp );
				att_tmp.setPhongPower( phong_power_tmp );
			} 
				//----- transparency
			else if( !strcmp( word, ATT_TRANSPARENCY  ) ) {
				sscanf( buf, "%s %s", dummy1, on_off );
				if( !strcmp(on_off, "on") || !strcmp(on_off, "ON") )
				{
					on_off_val = 1 ;
				} else {
					on_off_val = 0 ;
				}

				if( parameter.flag_swap_transparency && !parameter.flag_IP_mode ) {
				  on_off_val = ( on_off_val == 0 );
				}

				att_tmp.setTransparency(on_off_val);
			} 
				//----- visibility
			else if( !strcmp( word, ATT_VISIBILITY  ) ) {
				sscanf( buf, "%s %s", dummy1, on_off );
				if( !strcmp(on_off, "on") || !strcmp(on_off, "ON") )
				{
					on_off_val = 1 ;
				} else {
					on_off_val = 0 ;
				}
				att_tmp.setVisibility(on_off_val);
			} 
				//===== end of one attribute
			else if( !strcmp( word, ATT_DEF_END ) ) {
	
#if defined DEBUG_FILE_READER
				cerr << "lb: " << (att_tmp.getLabel()) << endl;
				cerr << "kd: " ; (att_tmp.getKd()).printRGB(); cerr << endl;
				cerr << "ks: " ; (att_tmp.getKs()).printRGB();cerr << endl;
				cerr << "n : " ; cerr << att_tmp.getPhongPower() << endl;
				cerr << "t : " ; cerr << att_tmp.getTransparency() << endl;
				cerr << "v : " ; cerr << att_tmp.getVisibility() << endl;
				cerr << endl;
#endif
				attribute_list.push( att_tmp ) ;
			}
			else if( !strcmp( word, END_ATTRIBUTE ) ) {
				break ; // end of att block ;
			} else {
				; // do nothing if unknown word
			}

		} // while

			//----- move attribute from list to array
		if( attribute_list.getNItem() > 0 ) {
			num_read_attribute = attribute_list.getNItem() ;
			attribute_array = new Attribute [num_read_attribute];
			if( attribute_array == NULL ) {
				cerr << "ERROR in Attribute block" << endl;		
				exit(1) ;
			}

			while( attribute_list.pop( att_tmp ) ) {
				attribute_array[ (att_tmp.getLabel()) ] \
					= att_tmp;
			}

				//----- store 0-th attribute (if any) as a dafault attribute
			object.setAttribute( attribute_array[0] ) ; 
			att_default = attribute_array[0] ;

		} else {
			att_default = Attribute();
			object.setAttribute( Attribute() ) ; 
			delete [] attribute_array ;
			attribute_array = NULL; 
		}

		delete [] word ;
		delete [] dummy1 ;
		delete [] dummy2 ;

	} // BLOCK 4


		//----- BLOCK 5: Facet block
	{	
			//----- local
		Attribute	att_tmp ;
		int		att_label_tmp ;
		int		flag_att_set = 0 ;
		int		status ;
		int		vnum = 0 ; // initialized to dummy value
		LineType	linetype_tmp;
		Vertex		vertex_item_tmp ;
		Vector3		lineseg_begin , lineseg_end ;

			//----- STEP 5-1: search beginning of block
		while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
			if( strstr( buf, BEGIN_FACET ) != NULL ) {
				break ;
			}
		}

			//----- STEP 5-2: read data
		num_read_facet		= 0 ; // reset counter for facets
		num_read_linesegment	= 0 ; // reset counter for linesegments
		while( (status = getLine( buf, LINEBUF, TERMINAL_CHAR )) ) 
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

				//----- read latter part of one-facet data
			if( getLine( buf2, LINEBUF, TERMINAL_CHAR2 ) < 0 ) {
				cerr << "ERROR in Facet block 1" << endl;	
				exit(1) ;
			}

				//----- NOTE
				//.....  Here, buf is vertex labels, and buf2 is attribute label if any

				//----- read attribute 
			if( sscanf( buf2, "%d", &att_label_tmp ) == EOF ) {
					//----- failed to reading label, or
					//..... no attribute label is described
				flag_att_set  = 0 ; // attribute is not read

			} else if ( att_label_tmp < 0 || att_label_tmp >= num_read_attribute ) {

					//----- undefined attribute label
				flag_att_set = 0 ; // attribute is not read
				cerr << "WARNING: Undefined attribute label, " << att_label_tmp ;
				cerr << ", in data \"" << (object.getName()) << "\"" << endl;
			} else {
					//----- valid attribute label
					//.....  ---> read it!
				flag_att_set = 1 ; // attribute IS read
				att_tmp = attribute_array[ att_label_tmp ] ;
			}

				//----- set default attribute (attribute of object)
				//..... to att_tmp
				//..... Note that flag_att_set is kept unchanged.
			if( !flag_att_set ) {
				att_tmp = att_default ; // default att is used
				att_label_tmp = 0     ; // label of the default att
			}

				//----- vertices: integer array of vertex lables
				//....  vnum     : number of returned items in the array
			getVertices( buf, vertices, VERTEX_MAX, vnum ) ;


#if defined DEBUG_FILE_READER
			{
				int ii ;
				for( ii = 0 ; ii < vnum ; ii++ ) {
					if ( abs(vertices[ii]) >= num_read_vertex ) {
						cerr << "ERROR (FileReader::readPlanData() \n";
						cerr << "     Undefined vertex label " << vertices [ii] << endl;

						vertices[ii] = num_read_vertex - 1 ;


						cerr << "     Replaced with the maximum label " << vertices [ii] << endl;
						}
				}
			}
#endif
				//----- read facet data
			if( vnum >= 3 ) {	// polygon data
					//----- local to vnum >=3 block
				int	status_tmp = 0 ; // initialization

					//----- invisible facet
					//.....   do nothing and simply abandon this facet
				if( !att_tmp.getVisibility() ) 
				{
					status_tmp = 1 ; 
				} 
				else {

						//----- add this facet to object with attatched attribute
						//..... Note: The first arg is used as a label of 
						//.....       the facet. So, the incrementation must be 
						//.....       done afterwards.
					status_tmp = addFacetToObject(	num_read_facet++, vertices, vnum, 
									&att_tmp, object                ) ;

				} // if( !att_tmp.getVisibility() )--- else

					//----- error?
				if ( status_tmp == ERROR ) { 
					cerr << "ERROR in Facet block 2" << endl;	
					exit(1) ; 
				}

			} else if ( vnum == 2 )  { // line data

					//------ starting point
				if( linseg_set_p != NULL ) { 
						//------ starting point of linesegment
					vertex_item_tmp = vertex_list[vertices[0]];
					lineseg_begin = vertex_item_tmp.getPosition() + (object.getOrigin());	

						//------ ending point of linesegment
					vertex_item_tmp = vertex_list[vertices[1]];
					lineseg_end = vertex_item_tmp.getPosition() + (object.getOrigin()) ;	

						//----- set linetype:
						//.....  If no attribute is set, 
						//.....  or if att_label_tmp is 0, i.e. 
						//.....  the default attribute, 
						//.....  then line color is set to black.
					linetype_tmp = LineType(); // initialized to default

					if( !flag_att_set ) 
					{ 
						linetype_tmp.setColor( ColorType(0.0, 0.0, 0.0) );  
					} else {
						linetype_tmp.setColor( (att_tmp.getKd()) );
					} 

						//------ output
					linseg_set_p->addItem( LineSegment(lineseg_begin, lineseg_end, linetype_tmp ) );
					num_read_linesegment++ ;

				} // if( linseg_set_p != NULL )

			} else { // vnum <= 1
				cerr << "WARNING: Too few vertices in a polygon of \"";
				cerr << (object.getName()) << "\"" << endl;
				continue ;	// too few labels 
						// ---> skip this line

			}

				//----- clear buf
			strcpy( buf , "");
			strcpy( buf2, "");

		} // while( getLine(...)  )

			//----- debugging information
		total_data_size.incrementNumFacet( num_read_facet       );
		total_data_size.incrementNumLine ( num_read_linesegment );
#if defined DISPLAY_LOG
		cerr << " *** number of facets       : " << num_read_facet       << '\n' ;
		cerr << " *** number of linesegments : " << num_read_linesegment << '\n' ;
		cerr << " *** Constructing Object .....\n";
#endif
	} // BLOCK 5


		//----- info
#if defined DISPLAY_LOG
	cerr << " *** total number of vertices     : " << total_data_size.num_vertex  << endl;
	cerr << " *** total number of facets       : " << total_data_size.num_facet   << endl;
	cerr << " *** total number of linesegments : " << total_data_size.num_line    << endl;
#endif

		//----- set the default attribute to object
	object.setAttribute( att_default ) ;

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

} // readPlanData( object, line )


	//----- FileReader::addFacetToObject()
int FileReader::addFacetToObject(	int label                    , 
					int *v                       , 
					int n                        , 
					const Attribute* attribute_p , 
					Object& object                )
{
	int	i;
	int	edge;					// current edge
	int*	edge_data	= new int [n] ;		// edge list

	int	startEdge = object.getNumEdgeInList();	// start edge
	int	endEdge   = startEdge + n - 1;		// end   edge

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

		next = (edge == endEdge)   ? startEdge : edge + 1;

		prev = (edge == startEdge) ? endEdge   : edge - 1;

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
	if( attribute_p != NULL ) {
		facetItem.setAttribute( *attribute_p );
		facetItem.setColorMode( Facet::SOFT_COLOR );
				// color of this attribute is used in rendering
	} else {
		facetItem.setColorMode( Facet::OBJECT_COLOR );
				// color of object is used in rendering
	}

		//---------- add facet to object
	object.addFacetItem(facetItem);

		//----- delete
	delete [] edge_data ;

		//----- return
	return NORMAL ;

}// FileReader::addFacetToObject()
