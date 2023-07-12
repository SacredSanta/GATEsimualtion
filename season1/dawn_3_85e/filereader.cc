//////////////////////////////////////
///// filereader.cc  1995.05.15 //////
//////////////////////////////////////

// #define DEBUG_FILE_READER

#define RECALC_BOUNDING_BOX
#define SKIP_SCANNING_LABEL0

#include "filereader.h"
#include "message.h"

	//----- FileReader::FileReader(void) 
FileReader::FileReader(void) 
{
	filename = NULL ; 
}

	//----- FileReader::getLine()
int
FileReader::getLine( char* buf, int BUF_LENGTH, char END_LINE )
{
		//----- local
	int	status = 0 ; // initialized to "read EOF"
	int	num_ch = 0 ; // num of read character 
	char	ch ;

		//----- read charaters until END_LINE appears
	while( input.get(ch) ) 
	{
		if( ch == END_LINE ) {
			status = 1 ; // read END_LINE
			break ;
		} 

		if( num_ch < ( BUF_LENGTH - 1 ) ) 
		{
			buf[num_ch++] = ch ;
		} else {
			status = -1 ; // buffer overflow
			break ;
		}
	}

		//----- return status
	buf[ num_ch ] = '\0';
	return status ;

} // FileReader::getLine()


	//----- set vertex
int FileReader::addVertex(int label, Vector3 vect, Object& object)
{
	Vertex	item( label, vect );
	object.addVertexItem(item);

	return NORMAL ;
}// FileReader::addVertex()



	//----- FileReader::getVertices()
	//.....  Get labels of vertex (type is int) from string.
void FileReader::getVertices( char *s, int *vertices, int VERTEX_MAX, int& num )
{
	int length = strlen(s);
	int i ;
	char*	p = 0 ;
	num = 0 ;

	for ( i = 0 , p = s ; i < length ; i++, p++ )
	{
		if( isdigit( *p ) || *p == '-' ) { 	
			sscanf( p, "%d", &(vertices[ num ]) ); num++ ;
			while ( 1 ) {
				i++; ++p ;
				if ( isspace( *p ) ) { break ;}
			} 
		}

		if( num >= VERTEX_MAX ) { break ; }
	}

} // FileReader::getVertices()


	//----- FileReader::open()
int FileReader::open( const char *fname )
{
	input.open( fname ) ;

	if( !input ) {
		cerr << "ERROR (FileReader::open()), 1 :\n";
		cerr << "   Cannot open file " << fname << endl;
		return ERROR ;
	}

	delete [] filename ; 
	filename = new char [ strlen(fname) + 1];
	if( !filename ) {
		cerr << "ERROR (FileReader::open()), 2 :\n";
		cerr << "   Cannot open file " << fname << endl;
		return ERROR ;
	}

	strcpy(filename, fname);

	return NORMAL ;

} // FileReader::open()


	//----- FileReader::close()
int FileReader::close()
{
	input.close() ;
	delete [] filename; filename = NULL ;
	return NORMAL ;
}


	//----- FileReader::scanPlanData()
void	FileReader::scanPlanData   ( Perspective& camera, NameAndValue& object , Box3D& bounding_box )
{
		//----- local
	double		x, y, z ;	// vertex position
	Box3D		box3d ;
	Vector3		origin ;
	Vector3		center ;
	const	int	LINEBUF = 256 ;
	char		buf[LINEBUF];
	double		object_value ;


		//----- BLOCK 0: data format
	getLine( buf , LINEBUF ,'\n') ;
	if(	strncmp(buf, FORMAT_VERSION_TITLE, FORMAT_VERSION_TITLE_LENGTH ) ||\
			FORMAT_VERSION[0] != buf[ FORMAT_VERSION_TITLE_LENGTH     ]      ||\
			FORMAT_VERSION[1] != buf[ FORMAT_VERSION_TITLE_LENGTH + 1 ]          )
	{
		cerr << "ERROR (FileReader::scanOldPlanData()), 1\n";
		cerr << "Data file \"" << (this->getFileName()) << "\" is not the format version " << FORMAT_VERSION << endl;
		exit(1);
	}

		//----- BLOCK 1: ObjectOrigin block
	{
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

			//----- STEP 1-3: set data to "origin"
		sscanf( buf, "%lg %lg %lg", &x, &y, &z ) ;
		origin( x, y, z ) ;
		
			//----- STEP 1-3: search end of block
		while( getLine( buf, LINEBUF ,'\n' ) > 0 ) {
			if( strstr( buf, END_OBJECT_ORIGIN ) != NULL ) {
				break ;
			}
		}

	} // BLOCK 1


		//----- BLOCK 2: Read Bounding Box
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


			//----- set bounding box
		box3d.set( xmin, ymin, zmin, xmax, ymax, zmax );

	} // BLOCK 2


		//----- BLOCK 3: Vertex Block
		//..... Executed if the bounding box was invalid.
	if(	( box3d.getXmin() ) >= ( box3d.getXmax() ) ||\
		( box3d.getYmin() ) >= ( box3d.getYmax() ) ||\
		( box3d.getZmin() ) >= ( box3d.getZmax() )      )
	{

#if defined DISPLAY_LOG
		cerr << "*** Bounding Box data is recalculated." << '\n' ;
#endif

			//----- local to this block
		int	label ;
		int	num_read_vertex ;

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
			if ( sscanf( buf, "%d %lg %lg %lg", &label, &x, &y, &z ) == EOF ) 
			{ 		
				continue ;
			} else {

#if defined SKIP_SCANNING_LABEL0
				if( label == 0 ) { continue ; }
#endif
					//----- count number of vertices
				num_read_vertex++;
			}
	
				//----- set bounding box
			if( num_read_vertex == 1 ) {
				box3d.set   ( x, y, z, x, y, z );
			} else {
				box3d.update( x, y, z );
			}
		}

	} // BLOCK 3


		//----- calc value center.z_eye
	center = box3d.getCenter();	// center in body coord

#if defined DEBUG_FILE_READER
	cerr << "xmax_body   = " << (box3d.getXmax()) << endl;
	cerr << "ymax_body   = " << (box3d.getYmax()) << endl;
	cerr << "zmax_body   = " << (box3d.getZmax()) << endl;
	cerr << "xmin_body   = " << (box3d.getXmin()) << endl;
	cerr << "ymin_body   = " << (box3d.getYmin()) << endl;
	cerr << "zmin_body   = " << (box3d.getZmin()) << endl;
	cerr << "center_body = "; center.print(); cerr << endl;
	cerr << "origin = "; origin.print(); cerr << endl;

#endif
	center += origin ;		// center in world coord 
	object_value = camera.eyeZ( center );
	object.setValue( object_value ); 

		//----- return bounding box in world coord
	bounding_box = box3d ;	bounding_box.shiftBy( origin ) ;


} // FileReader::scanPlanData()



