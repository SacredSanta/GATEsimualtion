//////////////////////////
/////  command1.cc  /////
//////////////////////////


#include <stdio.h> 
#include "command.h"
#include "dev_control.h"
#include "message.h"
#include "clip_tester.h"

#include "customize.h"

// #define DEBUG_COMMAND1
// #define DISPLAY_LOG

	//----- QuickSort<NameAndValue>::compare0()
int  QuickSort<NameAndValue>::compare0( NameAndValue *a , NameAndValue *b )
{
		//----- compare
	if         ( a->getValue() < b->getValue() ) {
		return  -1 ;	// *a is smaller
	} else 	if ( a->getValue() > b->getValue() ) {
		return   1 ;	// *b is nearer to eye than *a
	} else {
		return 0 ;
	}

} // QuickSort<NameAndValue>compare0()	

	//----- QuickSort<NameAndValue>::compare1()
	//.....  EMPTY
int  QuickSort<NameAndValue>::compare1( NameAndValue *a , NameAndValue *b )
{
	return  (a == b ); // dummy
}

	//----- Command::getClippingMode ()
	// clipping_mode =  0 ( no cllipping)
	//               =  1 ( clipping) 
	//               =  2 ( landscape clipping) 
int	Command::getClippingMode ( const Parameter& parameter )
{
		//----- return value
	int	clipping_mode = CLIPPING ; // initialization
	
		//----- set landscape mode
	if(	parameter.device_label == XWIN2  ||
			parameter.device_label == PS2         ) 
	{
		clipping_mode = LANDSCAPE_CLIPPING ; 
				// landscape clipping mode 
	} else {
		clipping_mode = CLIPPING ; 
				// clipping mode
	}

		//----- return calculated clipping mode
	return clipping_mode ;

}

	//-----  Command::errorMessage()
void Command::errorMessage(	char* string1 , 
				char* string2 , 
				char terminal  ) 
{
	cerr << "ERROR: " << string1 << string2 << terminal ;
}


	//----- Command::openDawn( )
void Command::openDawn ( int& argc, char* argv[] , Parameter& parameter )
{
		//----- opening message
	cerr << "\n";
	cerr << "***************************************\n";
	cerr << "          Fukui  Renderer         \n";
	cerr << "              DAWN                \n";
	cerr << " (Drawer for Academic WritiNgs)   \n";	
	cerr << " ver " << VERSION << "   " << DATE << "\n";
	cerr << "***************************************\n";
	cerr << endl << endl;

		//----- help
//     	cerr << "USAGE : dawn [option] data_file(s) \n" ;
//	cerr << endl << endl;

/////////////////////////////////////////////////////////////////////////////////////////
//
//		//----- att file
//	cerr << "Format of object-attribute file:\n";
//	cerr << "     color                 kd_red kd_green kd_blue \n";
//	cerr << "     transparency          on/off\n";
//	cerr << "     light_angle           light_v_angle  light_h_angle\n";
//	cerr << "     real_edge_width       width \n";
//	cerr << "     outline_width         width \n";
//	cerr << "     aux_edge_width        width \n";
//	cerr << "     aux_edge_style        style \n";
//	cerr << "     aux_threshold_angle   angle \n";
//	cerr << "\n";
//	cerr << "  Note : * Filename must be \"data_file_name.att\". \n";
//	cerr << "           If attribute file does not exist, default paramters are used.\n";
//	cerr << "         * First columns are dummy. Other colums are values.\n" ; 
//	cerr << "         * At least, \"color\" must be described. Others can be omitted.\n";
//	cerr << "         * color:  0 <= R <= 1, 0 <= G <= 1, 0 <= B <= 1.\n" ;  
//	cerr << "         * transparency: \"on\" or \"off\"\n" ; 
//	cerr << "         * Line style:  1->solid, 2->dashed, 3->invisible . \n" ; 
//	cerr << "\n\n" ;
//
/////////////////////////////////////////////////////////////////////////////////////////

		//----- local
	ifstream	fin ;
	int	i , j ;
	int	num_data_files = 0 ;

		//----- error checking
	if( argc <= 1 ) { exit(1) ; }

		//----- check if files exist
	for( i = 1 ; i < argc ; i++ ) {
		if( argv[i][0] != '-' ) 
		{	// file

				//----- data file name
			num_data_files++ ;
			fin.open ( argv[i] ) ;
			if( !fin ) {
				cerr << "ERROR: Data file " << argv[i] ;
				cerr << " cannot be opened!\n" ;
				exit(1) ;
			} else {
				fin.close();
			}

		} else { // option
				
				//----- analyze option 
				//.....  and modify parameter if necessary
			analyzeOption( argv[i], parameter ) ;

				//----- shift args
			argc-- ;  
			for( j = i ; j < argc ; j++ ) {
				argv[j] = argv[(j+1)];
			}
			i-- ;

		}

	} // for

		//----- no data filename
//	if( num_data_files == 0 && parameter.flag_IP_mode == 0 ) 
	if( num_data_files == 0 && parameter.flag_IP_mode <= 1 ) 
	{
		cerr << "ERROR: Null data" << endl;
        	exit(1) ;
	}


} // Command::openDawn ( int argc, char* argv[] )



	//-----	Command::analyzeOption()
int	Command::analyzeOption( char* string, Parameter& parameter ) 
{
		//----- local
	int	status = 0 ;

		//----- analyze option
	if( string[0] != '-' ) {
		status = 0 ; // this arg is not an option
	} else {

		status = 1 ; // this arg is an option

		switch ( string[1] ) {
		 case 'h':
			cerr << "Option:\n" << endl;

			cerr << " -c    Delete an old/irrerevant parameter file.\n";
			cerr << "       (Try to rescue DAWN from punic, anyway!)\n";
			cerr << " -d    Direct mode, i.e., skip invoking the GUI panel,\n";
			cerr << "       and use the previous parameters stored in the parameter file.\n";
			cerr << "       in the current directory.\n";

			cerr << " -g    Daemon mode (Unix domain).\n";
			cerr << " -G    Daemon mode (INET domain).\n";

			cerr << " -h    Help 1 (options).\n";
			cerr << " -H    Help 2 (env variables).\n";
			cerr << " -t    swap on/off of forceible wireframe attribute of each 3D primitive.\n";

			cerr << " -v    g4.prim-format viewer mode (default). \n";
			cerr << endl;
			cerr << "Information: http://geant4.kek.jp/~tanaka " << endl; 
			cerr << endl;
			exit(1);
		 case 'H':
			cerr << DAWN_ENV_VAR1 << endl;
			cerr << DAWN_ENV_VAR2 << endl;
			cerr << DAWN_ENV_VAR3 << endl;
			cerr << DAWN_ENV_VAR4 << endl;
			cerr << DAWN_ENV_VAR5 << endl;
			cerr << DAWN_ENV_VAR6 << endl;
			cerr << endl;
			cerr << "For more details, see DOC/CUSTOMIZE in the DAWN package, or " << endl; 
			cerr << "http://geant4.kek.jp/~tanaka . " << endl; 
			cerr << endl;
			exit(1);

#if defined DISPLAY_LOG
			cerr << "***** Option \'H\' is set to the parameter database" << endl;
#endif
			break ;
		 case 'o':
			parameter.flag_output_polygon = 1 ;
#if defined DISPLAY_LOG
			cerr << "***** Option \'o\' is set to the parameter database" << endl;
#endif
			break ;
		 case 'd':
			parameter.flag_skip_gui = 1 ;
#if defined DISPLAY_LOG
			cerr << "***** Option \'d\' is set to the parameter database" << endl;
#endif
			break ;
		 case 'b':
			parameter.flag_IP_mode = 0 ; // brep viewer mode 
#if defined DISPLAY_LOG
			cerr << "***** Option \'b\' is set to the parameter database" << endl;
#endif
			break ;
		 case 'v':
			parameter.flag_IP_mode = 1 ; // solid viewer mode 
#if defined DISPLAY_LOG
			cerr << "***** Option \'v\' is set to the parameter database" << endl;
#endif
			break ;
#if defined USE_SOCKET
		 case 'g':
			parameter.flag_IP_mode = 2 ; // unix domain
#if defined DISPLAY_LOG
			cerr << "***** Option \'g\' is set to the parameter database" << endl;
#endif
			break ;
		 case 'G':
			parameter.flag_IP_mode = 3 ; // inet domain
#if defined DISPLAY_LOG
			cerr << "***** Option \'G\' is set to the parameter database" << endl;
#endif
			break ;
#endif // USE_SOCKET
		 case 't': 
			parameter.flag_swap_transparency = 1 ;
#if defined DISPLAY_LOG
			cerr << "***** Option \'t\' is set to the parameter database" << endl;
#endif
			break ;

		 case 'c': {
#if defined DISPLAY_LOG
			cerr << "***** Option \'c\' is set to the parameter database" << endl;
#endif

			remove (  DEFAULT_HISTORY_FILE  ) ;
			remove (  DEFAULT_DEF_HIST_FILE ) ;
			remove (  "*.history" ) ;
			remove (  "*.default" ) ;

			cerr << "Parameter files in current directory are deleted.\n" << endl;
			exit(1);
		 }
		 default:
			cerr << "WARNING: Unknown option \'"<< string[1] << "\' is read but ignored" << endl;
		} // switch

	} // if--else

		//----- return status
	return status ;

} //Command::analyzeOption()


	//----- Command::setCamera(), 1
void	Command::setCamera( const Parameter& parameter , Perspective& camera ) 
{
	this->setCamera(	parameter.camera_distance , 
				parameter.camera_v_angle  , 
				parameter.camera_h_angle  , 
				Vector3( parameter.target_x, parameter.target_y, parameter.target_z ),
				parameter.focal_distance ,
				parameter.epsilon3d       , 
				camera                      ); 

} // Command::setCamera(), 1


	//----- Command::setCamera(), 2
void	
Command::setCamera(	double		camera_distance , 
			double		v_angle  , 
			double		h_angle  , 
			Vector3		target   ,
			double		focal_distance ,
			double		epsilon3d       ,
			Perspective&	camera           ) 
{
	camera.setTarget ( target );			// set origin of camera-world coord
	camera.setRho               ( camera_distance ) ; 
							// distance 
	camera.setPhi               ( v_angle  ) ;	// vertical  angle
	camera.setTheta             ( h_angle  ) ;	// horiontal angle
	camera.setFocalDistance    ( focal_distance , epsilon3d ) ;
							// distance to "film" from eye
} // Command::setCamera(), 2


	//----- Command::setCameraAuto()
void	Command::setCameraAuto(	const Box3D&	totalBoundingBox, 
				Parameter&	parameter, 
			        Perspective&	camera ) 
{
				//--------------------------//
				//----- initialization -----//
				//--------------------------//
		this->setCamera( parameter, camera ) ;

				//----------------------------//
				//----- auto positioning -----//
				//----------------------------//
				//..... reset: parameter.camera_distance
				//.....        camera.setRho()
		if( parameter.camera_distance <= 0.0 ) {
				//----- if auto  ...
			double	box_size = totalBoundingBox.getSize();
			this->autoPositioning( box_size, parameter, camera );
		}


				//--------------------------//
				//----- auto targeting -----//
				//--------------------------//
				//..... reset: parameter.target_xyz
				//.....        camera.setTarget()
		if( parameter.flag_auto_target ) {
				//----- if auto  ...
			Vector3  box_center ;
			box_center = totalBoundingBox.getCenter();
			this->setTarget( box_center , parameter , camera ) ;
		}

				//------------------------//
				//----- auto zooming -----//
				//------------------------//
				//..... reset: parameter.focal_distance
				//.....        camera.setFocalDistance
		if(	!parameter.flag_focal_distance_given || \
			parameter.focal_distance <= 0.0 ) 
		{
				//----- if auto  ...
			this->autoZoom( totalBoundingBox, parameter, camera );
		} 


} // Command::setCameraAuto()


	//-----Command::autoPositioning()
void	Command::autoPositioning( double total_rough_size , Parameter& parameter, Perspective& camera )
{	
		//----- set parameter.camera_distance by auto
	if( total_rough_size > 0.0  ) {

#if defined DISPLAY_LOG
			//----- message
		cerr<< "\n\n";
		cerr << "------------------------------\n";
		cerr << " Auto camera-positioning ...  \n";
		cerr << "------------------------------" << endl;
#endif


		parameter.camera_distance = \
			CAMERA_DISTANCE_RATIO * total_rough_size ;
		camera.setRho( parameter.camera_distance );

#if defined DISPLAY_LOG
			//----- message
		cerr<< "\n\n";
		cerr << "-----------------------------------\n";
		cerr << " Camera Distance is set to " << parameter.camera_distance << endl;
		cerr << "-----------------------------------" << endl;
#endif

	} // if 


} // Command::autoPositioning()



	//----- Command::setTarget()
void	
Command::setTarget( const Vector3& target , Parameter& parameter, Perspective& camera )
{
#if defined DISPLAY_LOG
		//----- message
	cerr << endl << endl;
	cerr<< "-----------------------\n";
	cerr << " Auto targeting  ...  \n";
	cerr<< "-----------------------" << endl;
#endif

		//----- set parameter
	parameter.target_x = target.x ;
	parameter.target_y = target.y ;
	parameter.target_z = target.z ;

		//----- set center as target of camera
	camera.setTarget( target );

#if defined DISPLAY_LOG
		//----- message
	cerr << endl << endl;
	cerr << "-----------------------------------\n";
	cerr << " Camera Target is set to "; target.print(); cerr << "\n";
	cerr << "-----------------------------------" << endl;
#endif

} // Command::setTarget()


	//----- Command::autoZoom()
void	
Command::autoZoom( const Box3D& total_bounding_box, Parameter& parameter, Perspective& camera )
{
#if defined DISPLAY_LOG
		//----- message
	cerr<< "\n\n";
	cerr << "---------------------\n";
	cerr << " Auto zooming   ...  \n";
	cerr << "---------------------" << endl;
#endif

		//----- local
	int		i ;
	int		num_corner = 8 ;
	Vector3		corner[8];
	double		distance ;

		//----- calc max and min point
	for( i = 0 ; i < num_corner ; i++ ) {
		ToolBox3D::GetCorner( i, total_bounding_box, (corner[i]) ) ;
	}

/////////////////////
//	for( i = 0 ; i < num_corner ; i++ ) {
//		corner[i] = total_bounding_box.getCorner( i ) ;
//	}
////////////////////////
////////////////////////////////////
//	{
//		double	xmin = total_bounding_box.getXmin () ;
//		double	xmax = total_bounding_box.getXmax () ;
//
//		double	ymin = total_bounding_box.getYmin () ;
//		double	ymax = total_bounding_box.getYmax () ;
//
//		double	zmin = total_bounding_box.getZmin () ;
//		double	zmax = total_bounding_box.getZmax () ;
//
//
//		 (corner[ 0 ])( xmin, ymin, zmin ) ; 
//		 (corner[ 1 ])( xmax, ymin, zmin ) ; 
//		 (corner[ 2 ])( xmax, ymax, zmin ) ; 
//		 (corner[ 3 ])( xmin, ymax, zmin ) ; 
//		 (corner[ 4 ])( xmin, ymin, zmax ) ; 
//		 (corner[ 5 ])( xmax, ymin, zmax ) ; 
//		 (corner[ 6 ])( xmax, ymax, zmax ) ; 
//		 (corner[ 7 ])( xmin, ymax, zmax ) ; 
//	}
//////////////////////////////////////////////////

		//----- initialize screen distance to "very far"
	camera.setFocalDistance(1.0e+99);

		//----- set screen distance
	camera.setScreenRange( corner, num_corner, parameter.epsilon3d );

		//----- rescaling
	distance = camera.getFocalDistance();
	distance = distance * parameter.scale ;
	camera.setFocalDistance(distance, parameter.epsilon3d );


		//----- parameter setting
	parameter.focal_distance = camera.getFocalDistance();

#if defined DISPLAY_LOG
		//----- message
	cerr << endl << endl;
	cerr << "-----------------------------------\n";
	cerr << " Screen Distance is set to " << distance << "\n";
	cerr << "-----------------------------------" << endl;
#endif

} // Command::autoZoom()


	//----- Command::setCameraPositionToParameter()
void	Command::setCameraPositionToParameter(	double camera_distance , 
						double camera_v_angle ,
						double camera_h_angle ,
						Parameter& parameter ) 
{
	parameter.camera_distance = camera_distance ;
	parameter.camera_v_angle  = camera_v_angle  ; 
	parameter.camera_h_angle  = camera_h_angle  ;
}


	//----- Command::setTargetPointToParameter()
void	Command::setTargetPointToParameter(	double x, 
						double y,
						double z,
						Parameter& parameter ) 
{
	parameter.target_x = x ;
	parameter.target_y = y ;
	parameter.target_z = z ;
}


	//----- Command::setScaleToParameter()
void	Command::setScaleToParameter(	double scale_given, 
					Parameter& parameter )
{
	parameter.scale = scale_given ;
}

	//----- Command::setFocalDistanceToParameter()
void	Command::setFocalDistanceToParameter(	double focal_distance, 
						Parameter& parameter )
{
	parameter.focal_distance = focal_distance ; 
}

void	Command::setAutoTargetFlagToParameter(	int        is_auto, 
						Parameter& parameter )
{
	if( is_auto ) {
		parameter.flag_auto_target = 1 ;
	} else { 
		parameter.flag_auto_target = 0 ;
	}
}

	//----- Command::setDeviceToParameter()
void	Command::setDeviceToParameter(	int        dev ,
					Parameter& parameter )
{
  if( dev >= PS || dev < DEVICE_END ) {
    parameter.device_label = dev ;
  }
}

	//----- Command::setTotalBoundingBox()
void	Command::setTotalBoundingBox(	double xmin, double ymin, double zmin, 
					double xmax, double ymax, double zmax, 
					Box3D* total_bounding_box_p            )
{
	total_bounding_box_p->set(	xmin, ymin, zmin,
					xmax, ymax, zmax ) ;
} // Command::setTotalBoundingBox()


	//----- Command::setTotalBoundingBoxToParameter()
void	Command::setTotalBoundingBoxToParameter( const Box3D& total_bounding_box , Parameter* parameter_p )
{
	parameter_p->xmin = total_bounding_box.getXmin();
	parameter_p->ymin = total_bounding_box.getYmin();
	parameter_p->zmin = total_bounding_box.getZmin();
	parameter_p->xmax = total_bounding_box.getXmax();
	parameter_p->ymax = total_bounding_box.getYmax();
	parameter_p->zmax = total_bounding_box.getZmax();

} // Command::setTotalBoundingBoxToParameter()


	//----- Command::readDataFile()
void	
Command::readDataFile(	const Parameter&	parameter          , 
			FileReader&		filereader         , 
			List<NameAndValue>&	object_name_server ,
			List<Object>&		object_server      , 
			List<LineSegment>&	lineseg_server     ,
			DataSize&		total_data_size     )	
{
		//----- message

		//----- local
	int		num_read_object = 0 ;
	Object*		object_p ;
	const	int	HEADER_SIZE = 100 ;
	char		header[HEADER_SIZE];
	NameAndValue	object_name ;

		//----- make winged_edge structure ?
//	int		mode_winged_edge = parameter.winged_edge_mode ;

		//-----  get vector sum of points
	object_name_server.toTail();	num_read_object = 0 ;
	while( object_name_server.getItem( object_name ) )
	{
			//----- local
		Object		empty_object ;
		char*	cur_name  = object_name.getName();
		int	cur_label = object_server.getNItem();

			//----- set current name and label
		object_server.push( empty_object ) ;		
		object_p = object_server.getItemAddress();
		object_p->setName ( cur_name  ) ; 
		object_p->setLabel( cur_label ) ; 

#if defined DISPLAY_LOG
		cerr<< endl << endl;
		cerr<< "-----------------------------------------\n";
		cerr << " Reading data file \"" << (object_p->getName())<< "\"  ..." << endl;
		cerr<< "-----------------------------------------\n";
#endif

			//----- open current data file
		filereader.open( ( object_p->getName() ) );

			//----- get header of data file
		filereader.getLine( header, HEADER_SIZE, '\n' );

		if(	strncmp(header, FORMAT_VERSION_TITLE, FORMAT_VERSION_TITLE_LENGTH ) ) 
		{
				//----- bad data format
			cerr << "ERROR (Command::readDataFile(), 1)" << endl;
			cerr << "   Data file \"" << cur_name << "\" is not of the format version " << FORMAT_VERSION << endl;
//			exit(1);
		} 

		 if (	'0' == header[ FORMAT_VERSION_TITLE_LENGTH     ]   &&\
			'0' == header[ FORMAT_VERSION_TITLE_LENGTH +1  ]       )
		{
				//----- old data format
			filereader.close();
			filereader.open( ( object_p->getName() ) );
			filereader.readOldPlanData( *object_p, &lineseg_server , 
						    parameter.winged_edge_mode , total_data_size );
			filereader.close();
		} else 	if( 	FORMAT_VERSION[0] == header[ FORMAT_VERSION_TITLE_LENGTH     ]  ||\
				FORMAT_VERSION[1] == header[ FORMAT_VERSION_TITLE_LENGTH + 1 ]       )
		{
				//----- valid data format
			filereader.close();
			filereader.open( ( object_p->getName() ) );
			filereader.readPlanData( *object_p, &lineseg_server , parameter , total_data_size );
			filereader.close();

		} else 	if( 	'P' == header[ FORMAT_VERSION_TITLE_LENGTH     ]  ||\
				'O' == header[ FORMAT_VERSION_TITLE_LENGTH + 1 ]       )
		{
				//----- polygon.out file
			filereader.close();
			filereader.open( ( object_p->getName() ) );
			filereader.readPolygonOut( *object_p, &lineseg_server , total_data_size );
			filereader.close();

		} else {
				//----- bad data format
			cerr << "ERROR (Command::readDataFile(), 2)" << endl;
			cerr << "   Data file \"" << cur_name << "\" is not of the format version " << FORMAT_VERSION << endl;
//			exit(1);
		}

			//----- updation 
		object_name_server.upward(); num_read_object++ ;

	} // while

#if defined DISPLAY_LOG
		//----- message
	cerr << endl << endl;
	cerr << "-------------------------------\n";
	cerr << " Number of read data files here = " << num_read_object << endl;
	cerr << " The last object label is  "   << ( object_server.getItemAddress()->getLabel()) << endl;
	cerr << "--------------------------------" << endl ;
#endif

} // Command::readDataFile()


	//-----	scanDataFile()
void	Command::scanDataFile( 	char*			argv[],
				int			argc , 
				FileReader&		filereader ,
				Perspective&		camera     ,
				List<NameAndValue>&	object_name_server ,
				Box3D&			total_bounding_box ,
			 	Parameter&		parameter     ) 
{

		//----- local
	int		flag_bounding_box_initialized = 0 ;
	int		i = 0 ;
	NameAndValue*	object_p = NULL ;
	const	int	HEADER_SIZE = 100 ;
	char		header[HEADER_SIZE];
	int		num_data_file = 0 ;
	Box3D		bounding_box ;

		//----- initialize object_name_server
	object_name_server.clear();

		//-----  scan each object
		//.....   i: index for each argument
		//.....   i = 0 is the executable filename
	for( i = 1 ; i < argc ; i++ ) {

			//----- local to this block
		NameAndValue	empty_object ;


			//----- add an empty object to object_name_server
		object_name_server.push( empty_object ) ;		

			//----- set name to the current (empty) object 
		object_p = object_name_server.getItemAddress();
		object_p->setName ( argv[i] ) ; 

#if defined DISPLAY_LOG
			//----- message
		cerr<< endl << endl;
		cerr<< "-----------------------------------------\n";
		cerr << " Scanning data file \"" << (object_p->getName())<< "\" ..." << endl;
		cerr<< "-----------------------------------------" << endl;
#endif

			//----- check header comment of the current data file
		filereader.open( argv[i] );
		filereader.getLine( header, HEADER_SIZE, '\n' );
		if(	strncmp(header, FORMAT_VERSION_TITLE, FORMAT_VERSION_TITLE_LENGTH ) ) 
		{
				//----- bad data format
			cerr << "ERROR (Command::scanDataFile()), 1" << endl;
			cerr << "   Data file \"" << (argv[i]) << "\" is not of the format version " << FORMAT_VERSION << endl;
//			exit(1);
		} 
		filereader.close();

			//----- Scan data file and
			//..... (1) calc ( or read ) bounding box                  (in world coord)
			//..... (2) set BoundingBox.center.position.eyeZ to object (in world coord)

		 if (	'0' == header[ FORMAT_VERSION_TITLE_LENGTH     ]   &&\
			'0' == header[ FORMAT_VERSION_TITLE_LENGTH +1  ]       )
		{
				//----- old data format
			filereader.open( argv[i] );
			filereader.scanOldPlanData( camera, *object_p , bounding_box );
			filereader.close();
		} else 	if( 	FORMAT_VERSION[0] == header[ FORMAT_VERSION_TITLE_LENGTH     ]  ||\
				FORMAT_VERSION[1] == header[ FORMAT_VERSION_TITLE_LENGTH + 1 ]       )
		{
				//----- standard data format
			filereader.open( argv[i] );
			filereader.scanPlanData( camera , *object_p , bounding_box );
			filereader.close();

		} else 	if( 	'P' == header[ FORMAT_VERSION_TITLE_LENGTH     ]  ||\
				'O' == header[ FORMAT_VERSION_TITLE_LENGTH + 1 ]       )
		{
				//----- polygon.out file format
			parameter.flag_read_polygon_out = 1 ;	// set flag to show that 
								// polygon.out data is read

			filereader.open( argv[i] );
			filereader.scanPolygonOut( camera, *object_p , bounding_box , parameter );
			filereader.close();

				//----- do not sort data files if a polygon.out file is included
			parameter.flag_sort_data = 0 ;				
			
		} else {
				//----- bad data format
			cerr << "ERROR (Command::scanDataFile()), 2" << endl;
			cerr << "   Data file \"" << (argv[i]) << "\" is not of the format version " << FORMAT_VERSION << endl;
//			exit(1);
		}


				//----- calc and update total_bounding_box
		if( !flag_bounding_box_initialized ) 
		{
			total_bounding_box.set(	bounding_box.getXmin(),
						bounding_box.getYmin(),
						bounding_box.getZmin(),
						bounding_box.getXmax(),
						bounding_box.getYmax(),
						bounding_box.getZmax() ) ;

			flag_bounding_box_initialized = 1 ; 

		} else {
			total_bounding_box.update( bounding_box );
		}
		
	} // for( each arg )

		//----- calc number of data files
	num_data_file        = object_name_server.getNItem();
	parameter.num_object = num_data_file ;

		//----- set PS file name
	if( num_data_file > 0 ) 
	{
		strcpy(parameter.ps_file, (object_name_server[0]).getName() ) ;
		ToolString::convertExtension( parameter.ps_file, ".data", ".eps" );
//		ToolString::convertExtension( parameter.ps_file, ".data", ".ps" );  
	} 

		//----- sort data in object_name_server
	if( parameter.flag_sort_data ) {
			//----- local
		QuickSort<NameAndValue> qsort ;

			//----- make a temporary array
		NameAndValue*	object_name_array_tmp = NULL ;
		object_name_array_tmp = new NameAndValue [ num_data_file ] ;
		if ( object_name_array_tmp == NULL ) {
			cerr << "ERROR (Command::scanOldPlanData()), 3" << endl;
			exit(1);
		}

#if defined DISPLAY_LOG
			//----- sort objects
		cerr<< endl << endl;
		cerr<< "------------------------\n";
		cerr << " Sorting data files ..." << endl;
		cerr<< "------------------------" << endl;
#endif

		object_name_server.toArray( object_name_array_tmp , num_data_file );
		qsort.doSort( object_name_array_tmp , 0, (num_data_file - 1 ) );

			//----- replace old data with sorted data
		object_name_server.clear();
		object_name_server.addArrayReverse( object_name_array_tmp, num_data_file );

			//----- delete temporary array
		delete [] object_name_array_tmp ;

	} else {

#if defined DISPLAY_LOG
			//----- message
		cerr<< endl << endl;
		cerr<< "--------------------------\n";
		cerr <<" Data files are not sort."  << endl;
		cerr<< "--------------------------" << endl;
#endif
	}

}// Command::scanDataFile()


	//----- Command::doHiddenLineElimination()
void	Command::doHiddenLineElimination(	const	Parameter&	parameter,
						HidLine&		hidline   , 
						double			epsilon3d ,
						double			epsilon2d   ) 
{

	if( parameter.flag_hidline_removal == 0 ) { 

			//----- message
		cerr<< "----------------------------------------\n";
		cerr << " Hidden-line removal is made off. \n";
		cerr<< "-----------------------------------------\n";
		cerr << endl << endl;

		hidline.doNotHiddenPartElimination2( epsilon3d, epsilon2d );

	} else if( parameter.flag_haloing && !parameter.flag_swap_transparency ) {
#if defined DISPLAY_LOG
			//----- message
		cerr << "-------------------------------------------------------\n";
		cerr << " Eliminating hidden parts of lines (with haloing) .... \n";
		cerr << "  number_of_linesegments = " << hidline.getNumLineSegment() << "\n";
		cerr<< "-------------------------------------------------------\n";
		cerr << endl << endl;
#endif

		hidline.doHiddenPartElimination( epsilon3d, epsilon2d );
	} else {
#if defined DISPLAY_LOG
			//----- message
		cerr<< "----------------------------------------\n";
		cerr << " Processing lines .... \n";
		cerr << "  number_of_linesegments = " << hidline.getNumLineSegment() << "\n";
		cerr<< "-----------------------------------------\n";
		cerr << endl << endl;
#endif

		hidline.doNotHiddenPartElimination( epsilon3d, epsilon2d );
	}

} // Command::doHiddenLineElimination()


	//----- Command::passFacetToHidCMD()
void	
Command::passFacetToHidCMD(	List<Object>&		object_server , 
				Perspective&		camera        ,
				Hid&			hid           ,
				HidLine&		hidline       ,
				Parameter&		parameter       )
{
		//----- local
	Object*		object_p      ;

		//-----  pass facets of objects to hid
	object_server.toTail();	
	while( (object_p = object_server.getItemAddress())  ) 
	{
			//----- pass facet of current object to hid
		passFacetToHidCMD(	object_p      , 
					camera        ,
					hid           ,
					hidline       ,
					parameter       ) ;

			//----- updation
		object_server.upward();	

	} // while( object_p ... )

}// Command::passFacetToHidCMD()


#if defined USE_VC

	//----- Command::passFacetToHidCMD(Object *)
void	
Command::passFacetToHidCMD(	Object*			object_p      , 
				Perspective&		camera        ,
				Hid&			hid           ,
				HidLine&		hidline       ,
				const Parameter&	parameter     ,
				int			backface_culling_mode )
{
		//----- local
	int		double_edge_draw_mode ;
	Parameter	parameter_tmp ;
	Shading		shading       ;
	LineType	linetype ;


		//----- no att file
		//.....  set shading parameter based on 
		//.....  the input parameters (light etc)
	parameter_tmp = parameter ;
	setShading ( parameter_tmp , &shading );
	object_p->setFlagAttFileExist( 0 ) ; 

		//----- set linetype to object
		//.....  decide types of read, aux, and outline edges
	setLineTypeToObject( parameter_tmp , object_p ) ;

		//----- set double_edge_draw_mode
	if( parameter_tmp.aux_edge_style == DASHED ) {
		double_edge_draw_mode = 0 ;
	} else {
		double_edge_draw_mode = 1 ;
	}

		//----- set shading mode
	switch( parameter_tmp.drawing_mode ) {
		case HID:
			parameter_tmp.shading_mode = LAMBERT ;
			break ;
		case HID2:
			break ; // draw with linesegments
		case HID3:
			parameter_tmp.shading_mode = PHONG ;
			break ;
		default:
			break ;
	}

		//----- set landscape mode
	int	clipping_mode = getClippingMode ( parameter_tmp );

		//----- pass edges and/or facets of objects to 
		//..... hiden-line or hidden-surface removal routine
	if( parameter_tmp.transparency ) 
	{
			//----- This object is transparent

#if defined DISPLAY_LOG
		cerr<< "\n\n";
		cerr << "-------------------------------------------------------------------------------- \n" ;
		cerr << " OBJECT " << (object_p->getLabel()) << " \"" << (object_p->getName() ) ;
		cerr << "\": Passing edges to hidden line removal routine  ... \n" ;
		cerr << "-------------------------------------------------------------------------------- " << endl;
#endif

			//----- color --> linetype
		ColorType	color;	
		color.setRGB( parameter_tmp.kd_red, parameter_tmp.kd_green, parameter_tmp.kd_blue ) ; 
		linetype.setColor( color ) ; // line color is the color of light for this object

			//----- width --> linetype
		Parameter	default_param ;
		double		edge_width ;
		if( parameter_tmp.real_edge_width < 0.0 || parameter_tmp.real_edge_style == INVISIBLE ) {
			edge_width = default_param.real_edge_width ;
		} else {
			edge_width = parameter_tmp.real_edge_width ;
		}
		linetype.setWidth( edge_width ) ; 

			//----- object_p->(edges with linetype) --> hidline
		object_p->passEdgeToHidLine (	camera                            ,
						&hidline                          ,
						parameter_tmp.aux_threshold_angle ,
						parameter.epsilon3d	          ,
						double_edge_draw_mode,
						clipping_mode        ,  
						parameter.flag_back_edge_dashed  ); 

	} else {
			//----- This object is not transparent

#if defined DISPLAY_LOG
		cerr<< "\n\n";
		cerr << "-------------------------------------------------------------------------------- \n" ;
		cerr << " OBJECT " << (object_p->getLabel()) << " \"" << (object_p->getName() ) ;
		cerr << "\": Passing polygons to hidden surface removal routine  ... \n" ;
		cerr << "-------------------------------------------------------------------------------- " << endl;
#endif

			//----- pass object to Hid class
		object_p->passFacetToHid (	camera                                       , 
						shading                                      , 
						&hid                                         , 
						&hidline                                     ,
						parameter_tmp.aux_threshold_angle            , 
						parameter_tmp.epsilon3d                      , 
						double_edge_draw_mode                        ,
						backface_culling_mode			     ,	
						parameter_tmp.shading_mode                   ,
						clipping_mode  );
	}

}// Command::passFacetToHidCMD(Object*)


#else 

	//----- Command::passFacetToHidCMD(Object *)
void	
Command::passFacetToHidCMD(	Object*			object_p      , 
				Perspective&		camera        ,
				Hid&			hid           ,
				HidLine&		hidline       ,
				const Parameter&	parameter     ,
				int			backface_culling_mode )
{
		//----- local
//	char*		ATT = ".att" ;
	const char	ATT[] = ".att" ;
	char		data_file[256] ;
	char		attribute_file[256]   ;
	int		double_edge_draw_mode ;

	Parameter	parameter_tmp ;
	Shading		shading       ;
	LineType	linetype ;


		//----- data file name and attribute file name
	strcpy( data_file, (object_p->getName() ) ) ;
	strcpy( attribute_file, data_file ) ;
	strcat( attribute_file, ATT ) ;

		//----- open attribute file
	ifstream fin( attribute_file ) ;

		//----- read parameters from attribute file
		//..... and set them to "shading"
	if( !fin ) {
			//----- no att file
			//.....  set shading parameter based on 
			//.....  the input parameters (light etc)
		parameter_tmp = parameter ;
		setShading ( parameter_tmp , &shading );
		object_p->setFlagAttFileExist( 0 ) ; 
	}
	else {
			//----- att file  exist
			//.....  set shading parameter based on 
			//.....  the attribute file
		parameter_tmp = parameter ;
		readObjectRenderingParameter( fin, parameter_tmp );
		setShading ( parameter_tmp , &shading );
		object_p->setFlagAttFileExist( 1 ) ; 
		object_p->setTransparency( parameter_tmp.transparency );

			//----- set reflection coefficients 
		ColorType	color ;

		color.setRGB( parameter_tmp.kd_red, parameter_tmp.kd_green, parameter_tmp.kd_blue );
		object_p->setKd( color ) ;  // full reflection of source light

		color.setRGB( parameter_tmp.ks_red, parameter_tmp.ks_green, parameter_tmp.ks_blue );
		object_p->setKs( color ) ;  // full specular reflection 

		fin.close();
	}

		//----- set linetype to object
		//.....  decide types of read, aux, and outline edges
	setLineTypeToObject( parameter_tmp , object_p ) ;

		//----- set double_edge_draw_mode
	if( parameter_tmp.aux_edge_style == DASHED ) {
		double_edge_draw_mode = 0 ;
	} else {
		double_edge_draw_mode = 1 ;
	}

		//----- set shading mode
	switch( parameter_tmp.drawing_mode ) {
		case HID:
			parameter_tmp.shading_mode = LAMBERT ;
			break ;
		case HID2:
			break ; // draw with linesegments
		case HID3:
			parameter_tmp.shading_mode = PHONG ;
			break ;
		default:
			break ;
	}

		//----- set landscape mode
	int	clipping_mode = getClippingMode ( parameter_tmp );

		//----- pass edges and/or facets of objects to 
		//..... hiden-line or hidden-surface removal routine
	if( parameter_tmp.transparency ) 
	{
			//----- This object is transparent

#if defined DISPLAY_LOG
		cerr<< "\n\n";
		cerr << "-------------------------------------------------------------------------------- \n" ;
		cerr << " OBJECT " << (object_p->getLabel()) << " \"" << (object_p->getName() ) ;
		cerr << "\": Passing edges to hidden line removal routine  ... \n" ;
		cerr << "-------------------------------------------------------------------------------- " << endl;
#endif

			//----- color --> linetype
		ColorType	color;	
		color.setRGB( parameter_tmp.kd_red, parameter_tmp.kd_green, parameter_tmp.kd_blue ) ; 
		linetype.setColor( color ) ; // line color is the color of light for this object

			//----- width --> linetype
		Parameter	default_param ;
		double		edge_width ;
		if( parameter_tmp.real_edge_width < 0.0 || parameter_tmp.real_edge_style == INVISIBLE ) {
			edge_width = default_param.real_edge_width ;
		} else {
			edge_width = parameter_tmp.real_edge_width ;
		}
		linetype.setWidth( edge_width ) ; 

			//----- object_p->(edges with linetype) --> hidline
//		int	clipping_mode = getClippingMode ( parameter_tmp );

		object_p->passEdgeToHidLine (	camera                            ,
						&hidline                          ,
						parameter_tmp.aux_threshold_angle ,
						parameter.epsilon3d	          ,
						double_edge_draw_mode,
						clipping_mode        ,  
						parameter.flag_back_edge_dashed  ); 

	} else {
			//----- This object is not transparent

#if defined DISPLAY_LOG
		cerr<< "\n\n";
		cerr << "-------------------------------------------------------------------------------- \n" ;
		cerr << " OBJECT " << (object_p->getLabel()) << " \"" << (object_p->getName() ) ;
		cerr << "\": Passing polygons to hidden surface removal routine  ... \n" ;
		cerr << "-------------------------------------------------------------------------------- " << endl;
#endif

			//----- pass object to Hid class
		object_p->passFacetToHid (	camera                                       , 
						shading                                      , 
						&hid                                         , 
						&hidline                                     ,
						parameter_tmp.aux_threshold_angle            , 
						parameter_tmp.epsilon3d                      , 
						double_edge_draw_mode                        ,
						backface_culling_mode			     ,	
						parameter_tmp.shading_mode                   ,
						clipping_mode  );
	}

}// Command::passFacetToHidCMD(Object*)

#endif // USE_VC --- else


	//----- Command::passLineSegmentToHidLine ()
void 
Command::passLineSegmentToHidLine (	List<LineSegment>&	linesegServer , 
					Perspective&		camera        ,  
					HidLine& 		hidline       , 
					Parameter&		parameter      ) 
{ 
		//----- local variables
	LineType 	linetype  ;
	LineSegment*	lineseg_p ;

#if defined DISPLAY_LOG
		//----- message
	cerr << endl <<  endl;
	cerr << "-----------------------------------\n";
	cerr << " Installing linesegment data  ...  \n";
	cerr << "-----------------------------------"  << endl;
#endif

		//----- Note: in addition of edge data -----//

		//----- main loop
	linesegServer.toTail();
	while(	(lineseg_p = linesegServer.getItemAddress()) ) 
	{
			//----- set 2d image of linesegment
		lineseg_p->setImage( &camera , parameter.epsilon3d );

			//----- modify linetype of linesegment
		linetype = lineseg_p->getLineType();
		linetype.setWidth( parameter.line_width );
		lineseg_p->setLineType( linetype ); 

			//----- pass linesegment to hidline
		hidline.addItem_lineSegmentList( *lineseg_p );

			//------ updation
		linesegServer.upward();
	}

#if defined DISPLAY_LOG
			//----- message
	cerr << endl <<  endl;
	cerr << "-----------------------------------\n";
	cerr << " Linesegment data installed.       \n";
	cerr << "-----------------------------------\n";
	cerr << endl <<  endl;
#endif


} // Command::passLineSegmentToHidLine ()


	//----- Command::savePolygon()
void Command::savePolygon( const Parameter& parameter , Hid& hid ) 
{
		
		//----- local
	char   polygon_output_file[256] ; strcpy( polygon_output_file , DEFAULT_POLYGON_OUTPUT_FILE );
	char   begin_facet_string [32] ;  strcpy( begin_facet_string  , BEGIN_FACET                 );
	char   terminal_char       = TERMINAL_CHAR ;

#if defined DISPLAY_LOG
			//----- message
		//------ message
	cerr << "------------------------------------------\n";
	cerr << " Saving polygon data  to file " << polygon_output_file << "\n";
	cerr << "------------------------------------------\n\n";
	cerr << endl ;
#endif

		//----- output
	hid.writePolygonOut( polygon_output_file , begin_facet_string , terminal_char , parameter ) ;	

} // Command::savePolygon()


	//----- Command::readDataFile_doHiddenSurfaceElimination()
void	Command::readDataFile_doHiddenSurfaceElimination(	const Parameter&	parameter          ,
								FileReader&		filereader         , 
								List<NameAndValue>&	object_name_server , 
								List<Object>&		object_server      , 
								List<LineSegment>&	lineseg_server     , 
								Hid&			hid                , 
								HidLine&		hidline            ,
								DataSize&		total_data_size      )	
{
		//----- local
	Perspective&	camera = *(hid.getCamera());
	Object*		object_p  = NULL  ; // initialization
	NameAndValue	object_name ;
	int		object_label ;

	int		beginning_object_label_per_session ;
	int		ending_object_label_per_session ;

	const	int	HEADER_SIZE = 256 ;
	char		header[HEADER_SIZE]; // buffer of a read line from file
	
		//----- winged-edge-structure mode
//	int		mode_winged_edge = parameter.winged_edge_mode ;

		//-----  outside loop : for each session
	object_name_server.toTail();	object_label = 0 ;
	while( object_name_server.getItem( object_name )  ) 
	{
			//----- local 
		int		num_non_transparent_facet_per_session = 0 ;

			//----- labels for beginning and ending object
		beginning_object_label_per_session = object_label ;
		ending_object_label_per_session    = object_label ;

			//----- inside loop: for each object_name
		while(1) 
		{
				//----- local
			Object		empty_object ;

				//----- get current object_name
			object_name_server.getItem( object_name ) ;

				//----- set a new empty object to object_server
				//..... and set it to name and label
			object_server.push( empty_object ) ;		
			object_p = object_server.getItemAddress();
			object_p->setName ( ( object_name.getName())  ) ; 
			object_p->setLabel( object_label ) ; 

#if defined DISPLAY_LOG
			cerr << "\n\n";
			cerr<< "-----------------------------------------\n";
			cerr << " Reading data file \"" << (object_p->getName())<< "\"  ...\n";
			cerr<< "-----------------------------------------" << endl;
#endif


				//----- open current data file
			filereader.open( ( object_p->getName() ) );

				//----- get header of data file
			filereader.getLine( header, HEADER_SIZE, '\n' );

				//----- read data file
			if(	strncmp(header, FORMAT_VERSION_TITLE, FORMAT_VERSION_TITLE_LENGTH ) ) 
			{
					//----- bad data format
				cerr << "ERROR (Command::readDataFile_doHiddenSurfaceElimination(), 1)" << endl;
				cerr << "   Data file \"" << ( object_name.getName()) << "\" is not of the format version " << FORMAT_VERSION << endl;
//	DDD			exit(1);
			} 

				//----- 
			 if (	'0' == header[ FORMAT_VERSION_TITLE_LENGTH     ]   &&\
				'0' == header[ FORMAT_VERSION_TITLE_LENGTH +1  ]       )
			{
					//----- old data format
				filereader.close();
				filereader.open( ( object_p->getName() ) );
				filereader.readOldPlanData( *object_p, &lineseg_server , 
							    parameter.winged_edge_mode  , total_data_size );
				filereader.close();
			} else 	if( 	FORMAT_VERSION[0] == header[ FORMAT_VERSION_TITLE_LENGTH     ]  ||\
					FORMAT_VERSION[1] == header[ FORMAT_VERSION_TITLE_LENGTH + 1 ]       )
			{	
					//----- valid data format
				filereader.close();
				filereader.open( ( object_p->getName() ) );
				filereader.readPlanData( *object_p, &lineseg_server , parameter , total_data_size );
				filereader.close();

			} else 	if( 	'P' == header[ FORMAT_VERSION_TITLE_LENGTH     ]  ||\
					'O' == header[ FORMAT_VERSION_TITLE_LENGTH + 1 ]       )
			{
				filereader.close();
				filereader.open( ( object_p->getName() ) );
				filereader.readPolygonOut( *object_p, &lineseg_server , total_data_size );
				filereader.close();

			} else {
					//----- bad data format
				cerr << "ERROR (Command::readDataFile_doHiddenSurfaceElimination(), 2)" << endl;
				cerr << "   Data file \"" << ( object_name.getName()) << "\" is not of the format version " << FORMAT_VERSION << endl;
// DDD				exit(1);
			}

				//----- end of reading data file
				

				//----- pass facets of current object to hid or pass edges to hidline
				//.....  (1) non-transparent object: Facets are passed to hid.
				//.....  (2) transparent     object: Edges  are passed to hidline
			if( (object_p->getNumFacet()) > 0 ) 
			{
				passFacetToHidCMD(	object_p      , 
							camera        ,
							hid           ,
							hidline       ,
							parameter    ) ;
			}


				//----- count 	num_non_transparent_facet_per_session 
			if( !(object_p->getTransparency())  ) {
				num_non_transparent_facet_per_session += (object_p->getNumFacet()) ;
			}

				//----- clear arrays in current object 
				//..... except for position data in world coordinate data
			object_p->clearArray_VertexEdgeFacet();

				//----- updation or exit from loop 
			if( (hid.getNItem_Polygon3DList()) >= parameter.max_polygon_per_session ) {
				ending_object_label_per_session    = object_p->getLabel();
				break ; // Enough number of polygons are passed.
					// Go to hidden part elimination.

			} else if ( (object_p->getLabel()) == 0 && (object_p->getConvexity()) == 1 ) {
				ending_object_label_per_session    = object_p->getLabel();
				break ; // If the first object is convex, 
					// process it with in one sesseion

			} else {	// Not enough number of polygons are passed.
					// go to next object
					
				//----- updation -----//
				object_name_server.upward();	 object_label++ ;
				//--------------------//

					//----- no more objects: make the cuurent one the last object
				if( object_name_server.isCurrentNodeNull() ) {
					object_name_server.toHead(); object_label-- ;
					ending_object_label_per_session    = object_p->getLabel();
					break ;
				}
			}

		} // while, inside loop

			//----- perform hidden surface elimination
			//.....  Position data in world coordinate data is copied to Polygon3D.
		if(	beginning_object_label_per_session == ending_object_label_per_session  &&\
			( ( object_p->getNumFacet()) == 0  || (object_p->getTransparency()) == 1 )      )
		{
			; // do nothing 
		} else if ( num_non_transparent_facet_per_session == 0 ) { 

			; // do nothing 
		} else if (	beginning_object_label_per_session == 0 &&\
				ending_object_label_per_session    == 0 &&\
				(object_p->getConvexity())         == 1  ) 
		{
#if defined DISPLAY_LOG
			//----- message
			cerr << "\n\n";
			cerr << "-------------------------------------------------------------------------------- \n" ;
			cerr << " upto OBJECT " << (object_p->getLabel()) << " \"" << (object_p->getName()) << "\": ";
			cerr << " Back-face culling is being performed ... \n" ;
			cerr << "--------------------------------------------------------------------------------" << endl;
#endif

			hid.doNotHiddenPartElimination(	parameter.epsilon3d );

		} else if ( parameter.flag_BFC_mode ) { 
#if defined DISPLAY_LOG
			//----- message
			cerr << "\n\n";
			cerr << "-------------------------------------------------------------------------------- \n" ;
			cerr << " upto OBJECT " << (object_p->getLabel()) << " \"" << (object_p->getName()) << "\": ";
			cerr << " Back-face culling is being performed ... \n" ;
			cerr << "--------------------------------------------------------------------------------" << endl;
#endif

			hid.doNotHiddenPartElimination(	parameter.epsilon3d );

		} else {

#if defined DISPLAY_LOG
			//----- message

			cerr << "\n\n";
			cerr << "-------------------------------------------------------------------------------- \n" ;
			cerr << " upto OBJECT " << (object_p->getLabel()) << " \"" << (object_p->getName()) << "\": ";
			cerr << " Eliminating hidden parts of facets ... \n" ;
			cerr << "-------------------------------------------------------------------------------- \n" ;
#endif

			hid.doHiddenPartElimination(	beginning_object_label_per_session, 
							ending_object_label_per_session, 
							parameter.epsilon3d , 
							parameter.epsilon2d );
		}

			//----- clear all arrays in current object 
		{
				//----- local to this block
			Object*	object_p_tmp ;

				//----- store state
			object_server.setMark();

				//----- clear arrays
			object_server.toTail();
			while( ( object_p_tmp = object_server.getItemAddress() ) ) { 
				object_p_tmp->clearArray();
				object_server.upward();	
			}
				//----- recover state
			object_server.toMark();
		}

			//----- clear temporary position data stored in hid
			//.....  Position data has already been copied to Polygon3D
		hid.clearAuxiliaryVertexPositionList()	;

			//----- updation
		object_name_server.upward();	object_label++ ;
			// go to the next object

	} // while, outside loop


		//----- set polygon images to array polygon2d[], and calc numPolygon2D
		//..... (WARNING: polygon2DList is cleared.)
		//..... (WARNING: Old data is polygon2d[] is abandonned.)
	hid.movePolygon2DListToArray();

		//----- debuggung information
#if defined DISPLAY_LOG
			//----- message
	cerr << "\n" << endl;
	cerr << "  " <<  hid.getNumPolygon2D() << " polygon images are created\n";
	cerr << endl << endl;
#endif

} // Command::readDataFile_doHiddenSurfaceElimination()




	//----- Command::setShading()
void
Command::setShading( Parameter& parameter , Shading* shading_p ) 
{
		//----- local 
	ColorType	light_color   ;
	ColorType	amb_color     ;
		
		//----- set light
	light_color.setRGB ( parameter.Ip_red, parameter.Ip_green, parameter.Ip_blue ) ;
	amb_color.setRGB   ( parameter.Ia_red, parameter.Ia_green, parameter.Ia_blue ) ;

		//----- set shading routine
	shading_p->set(	parameter.light_v_angle ,  parameter.light_h_angle , 
			light_color,               amb_color                );
	
} // Command::setShading()


	//----- Command::setLineTypeToObject()
void
Command::setLineTypeToObject( const Parameter& parameter , Object* object_p ) 
{
		//----- local
	LineType	line_real, line_aux , line_outline;
	ColorType	crystal ;	crystal.invisible() ;
	LineType	linesolid   ( ColorType(0.0,0.0,0.0) );
	LineType	linedash    ( ColorType(0.0,0.0,0.0) , UNIT_LINE_WIDTH , FR_LINE_ON_OFF_DASH ) ;
	LineType	linecrystal ( ColorType(0.0,0.0,0.0, 0.0) );

		//----- set to real edge and outline
		//.....  If width < 0.0 , default width is set insterd.
		//.....  See class "LineType".
	line_real    = linesolid ; line_real.setWidth   ( parameter.real_edge_width );
	line_outline = linesolid ; line_outline.setWidth( parameter.outline_width   );

		//----- set to aux edge
		//.....  If width < 0.0 , default width is set insterd.
		//.....  See class "LineType".
	switch ( parameter.aux_edge_style ) {
	 case SOLID:
		line_aux = linesolid   ; line_aux.setWidth ( parameter.aux_edge_width );
		break;
	 case DASHED:
		line_aux = linedash    ; line_aux.setWidth ( parameter.aux_edge_width );
		break;
	 case INVISIBLE:
		line_aux = linecrystal ; line_aux.setWidth ( parameter.aux_edge_width );
		break;
	 default:
		line_aux = linesolid   ; line_aux.setWidth ( parameter.aux_edge_width );
		break;
	}

		//----- negative width ---> invsisible
	if( parameter.real_edge_width < 0.0 ) { line_real.setColor( crystal )    ; }
	if( parameter.outline_width   < 0.0 ) { line_outline.setColor( crystal ) ; }
	if( parameter.aux_edge_width  < 0.0 ) { line_aux.setColor( crystal )     ; }

		//----- install linetypes to object
	object_p->setLineType( REAL,        line_real );
	object_p->setLineType( AUXILIARY,   line_aux  );
	object_p->setLineType( AUXILIARY_H, line_aux  );
	object_p->setLineType( OUTLINE,     line_outline );

} //  Command::setLineTypeToObject() 

