//////////////////////////
/////  command3.cc  /////
//////////////////////////


#include "command.h"
#include "message.h"


// #define DEBUG_COMMAND3
// #define DISPLAY_LOG


	//-----  Command::drawLine()
void Command::drawLine(	const Parameter&        parameter    ,
			PostScript&		ps           ,
			HidLine&		hidline       ) 
{
	if( parameter.device_label == PS2 || parameter.device_label == XWIN2 ) {
		this->drawLine_landscape_PS(	ps            ,
						hidline      ,
						parameter.blank_scale    );
	} else {
		this->drawLine_PS(	ps            ,
					hidline      ,
					parameter.blank_scale    );
	}

}  // Command::drawLine()


	//-----  Command::drawLine()
void Command::drawLine(	const Parameter&        parameter    ,
			PostScript&		ps           ,
		        Perspective&		camera       ,
			List<LineSegment>&	lineseg_server  ) 
{
		//----- local
	HidLine		hidline_tmp ;
		
		//----- set images of lineseg's
	setImage( parameter, lineseg_server , camera );  

		//----- pass lineseg's to hidline_tmp
	hidline_tmp.setVisibleLineSegment ( lineseg_server ) ;

		//----- draw
	drawLine(  parameter , ps, hidline_tmp );

} // Command::drawLine()



	//-----  Command::setImage()
void Command::setImage(	const Parameter&        parameter       ,
			List<LineSegment>&	lineseg_server  , 
			Perspective&		camera           )
{
		//----- local
	LineSegment*	lineseg_p   ;
		
		//----- set images of lineseg's
	lineseg_server.toTail();
	while( (lineseg_p = lineseg_server.getItemAddress() ) )
	{
		lineseg_p->setImage( &camera, parameter.epsilon3d ) ;
		lineseg_server.upward();
	}

} // Command::setImage()


	//-----  Command::drawObject_WireFrame()
void Command::drawCurrentObject_WireFrame(	const Parameter&  parameter    ,
						PostScript&       ps           ,
						List<Object>&    object_server ,
						Perspective&      camera        )
{
		//----- local
	Object*		object_p  = object_server.getItemAddress() ;
	LineType	linetype ;
	double		epsilon3d = parameter.epsilon3d ;
	double          aux_threshold_angle = parameter.aux_threshold_angle ;
	int		clipping_mode = getClippingMode( parameter );


		//----- set linetype to object
		//.....  decide types of read, aux, and outline edges
	setLineTypeToObject( parameter , object_p ) ;

		//----- draw facets of object 
	if ( !object_p ) {
		; // do nothing 
	} else 	if( parameter.device_label == PS2 || parameter.device_label == XWIN2 ) 
	{
		object_p->drawWireFrame_landscape_PS( camera, ps, linetype , aux_threshold_angle , epsilon3d , clipping_mode );
	} else {
		object_p->drawWireFrame_PS( camera, ps, linetype , aux_threshold_angle, epsilon3d , clipping_mode );
	}

} // Command::drawCurrentObject_WireFrame()




	//----- Command::clearObject( List<Object>& object_server )
void Command::clearCurrentObject( List<Object>& object_server )
{
		//----- local
	Object*		object_p ;

	object_server.toHead();   
	if ( (object_p = object_server.getItemAddress()) )
	{
		object_p->clearArray();
	}

} // Command::clearObject()



	//----- Command::readDataFile_DrawWireFrame_Clear()
void	
Command::readDataFile_DrawWireFrame_Clear(	const Parameter&	parameter          , 
						FileReader&		filereader         , 
						List<NameAndValue>&	object_name_server ,
						List<Object>&		object_server      , 
						List<LineSegment>&	lineseg_server     ,
						DataSize&		total_data_size    ,
						PostScript&		ps           ,
						Perspective&		camera        )
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
	object_name_server.toTail();
	num_read_object = 0 ;
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
			cerr << "ERROR (Command::readDataFile_DrawWireFrame_Clear(), 1)" << endl;
			cerr << "   Data file \"" << (object_name.getName()) << "\" is not of the format version " << FORMAT_VERSION << endl;
//			exit(1);
		} 


			//----- 
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
			cerr << "ERROR (Command::readDataFile_DrawWireFrame_Clear(), 2)" << endl;
			cerr << "   Data file \"" <<  (object_name.getName()) << "\" is not of the format version " << FORMAT_VERSION << endl;
//			exit(1);
		}


			//----- draw object and clear
		drawCurrentObject_WireFrame(	parameter    ,
						ps           ,
						object_server ,
						camera        ) ;
		clearCurrentObject( object_server ) ;


			//----- draw lines and clear
		drawLine(	parameter ,
				ps        ,
				camera    ,
				lineseg_server  ) ;
		lineseg_server.clear();


			//----- updation 
		object_name_server.upward(); num_read_object++ ;

	} // while


#if defined DISPLAY_LOG
		//----- message
	cerr << "\n\n" ;
	cerr << "-------------------------------\n";
	cerr << " Number of read data files here = " << num_read_object << endl;
	cerr << " The last object label is  "   << ( object_server.getItemAddress()->getLabel()) << endl;
	cerr << "--------------------------------\n";
	cerr << endl << endl;
#endif

} // Command::readDataFile_DrawWireFrame_Clear()


	//----- Command::doIncrementalHiddenSurfaceElimination()
void	Command::doIncrementalHiddenSurfaceElimination(	const Parameter&	parameter          ,
						        List<Object>&		object_server      , 
						        Hid&			hid                , 
						        HidLine&		hidline            , 
							int			flush_mode           )
{
		//----- no polygons to draw
	if( object_server.getNItem() == 0 ) { return ; }

		//----- backup cur of objectServer and then go to head
	int	cur_org = object_server.whereIsCurrentNode();
	object_server.toHead ();

		//----- static variables
//	static int		beginning_object_label_per_session    = 0 ;
//	static int		num_non_transparent_facet_per_session = 0 ;
	int	beginning_object_label_per_session    = hid.getBeginningObjectLabelPerSession()  ;
	int	num_non_transparent_facet_per_session = hid.getNumNonTransparentFacetPerSession() ;


		//----- auto variables
	Perspective&	camera = *(hid.getCamera());
	Object*		object_p  = object_server.getItemAddress(); // head
	int		object_label = object_p->getLabel() ;
	int		ending_object_label_per_session = object_label ;


		//----- BLOCK 1 -----//
		//----- All hidden surface removals have already been performed.
		//...... That is, Block 3 has already been executed 
		//...... in the former calling of this function.
		//...... Create polygon2d[] array and return.
		//...... All arrays in objects have already been cleared.
	if(	flush_mode && \
		ending_object_label_per_session < beginning_object_label_per_session )
	{

			//----- set polygon images to array polygon2d[], and calc numPolygon2D
			//..... (WARNING: polygon2DList is cleared.)
			//..... (WARNING: Old data is polygon2d[] is abandonned.)
		hid.movePolygon2DListToArray();

#if defined DISPLAY_LOG
			//----- message
		cerr << "\n" << endl;
		cerr << "  All 3D polygons have already been processed.\n";
		cerr << "  Polygon images are copied to array.\n" ;
		cerr << "  " <<  hid.getNumPolygon2D() << " polygon images are created\n";
		cerr << "\n" << endl;
#endif

			//----- reset object server to original state
		object_server.goTo( cur_org ) ;

			//------ RETURN 
		return ;
	}


		//----- BLOCK 2 -----//
		//----- pass facets of current object to hid or pass edges to hidline
		//.....  (1) non-transparent object: Facets are passed to hid.
		//.....  (2) transparent     object: Edges  are passed to hidline
	if( !flush_mode ) {
		if(	(object_p->getNumFacet()) > 0 )	
		{
			passFacetToHidCMD(	object_p      , 
						camera        ,
						hid           ,
						hidline       ,
						parameter    ) ;

				//----- count number of non-transparent facet in this session
			if( !(object_p->getTransparency())  ) {
				num_non_transparent_facet_per_session += (object_p->getNumFacet()) ;
				hid.setNumNonTransparentFacetPerSession( num_non_transparent_facet_per_session ) ;
			}
		}


			//----- clear arrays in current object 
			//..... except for position data in world coordinate data
			//..... These data are unnesessary after passsing them to hid
#if defined DISPLAY_LOG
		cerr << endl;
		cerr << " *** Clearing unnesessary data from memory (Phase 1)..." << endl;
#endif
		object_p->clearArray_VertexEdgeFacet();

	}


		//----- BLOCK 3 -----//
		//----- Hidden surface removal
		//.....  Execution of this block means ending of one session.
		//.....
		//.....  Note: Passed facets from objects to hid are stored in
		//.....        the Hid::polygon3DList
		//.....        If items in the list is greater than 
		//.....        parameter.max_polygon_per_session , 
		//.....        hidden surface removal is performed.
		//.....        The hidden surface removal is also performed forciblly
		//.....        when flush_mode is non-zero.
	if( ( (hid.getNItem_Polygon3DList()) >= parameter.max_polygon_per_session ) \
	    ||\
	    flush_mode ) 
	{
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
			cerr << endl << endl;
			cerr << "-------------------------------------------------------------------------------- " << endl;
			cerr << " upto OBJECT " << (object_p->getLabel()) << " \"" << (object_p->getName()) << "\": ";
			cerr << " Back-face culling is being performed ... " << endl ;
			cerr << "-------------------------------------------------------------------------------- " << endl;
			cerr << endl << endl;

			hid.doNotHiddenPartElimination(	parameter.epsilon3d );

		} else if ( parameter.flag_BFC_mode ) { 

			cerr << endl << endl;
			cerr << "-------------------------------------------------------------------------------- " << endl;
			cerr << " upto OBJECT " << (object_p->getLabel()) << " \"" << (object_p->getName()) << "\": ";
			cerr << " Back-face culling is being performed ... " << endl ;
			cerr << "-------------------------------------------------------------------------------- " << endl;
			cerr << endl << endl;

			hid.doNotHiddenPartElimination(	parameter.epsilon3d );

		} else {

			cerr << endl << endl;
			cerr << "-------------------------------------------------------------------------------- " << endl;
			cerr << " upto OBJECT " << (object_p->getLabel()) << " \"" << (object_p->getName()) << "\": ";
			cerr << " Eliminating hidden parts of facets ... " << endl;
			cerr << "-------------------------------------------------------------------------------- " << endl;
			cerr << endl << endl;

			hid.doHiddenPartElimination(	beginning_object_label_per_session, 
							ending_object_label_per_session, 
							parameter.epsilon3d , 
							parameter.epsilon2d );
		}

			//----- clear all arrays in current object 
		{

				//----- local to this block
			Object*	object_p_tmp ;

#if defined DISPLAY_LOG
			cerr << endl << endl;
			cerr << "  *** Clearing unnesessary data from memory (Phase 2) ..." << endl;
#endif

				//----- clear arrays
			object_server.toTail();
			while( ( object_p_tmp = object_server.getItemAddress() ) ) { 
				object_p_tmp->clearArray();
				object_server.upward();	
			}
		}

			//----- clear temporary position data stored in hid
			//.....  Position data has already been copied to Polygon3D
		hid.clearAuxiliaryVertexPositionList()	;

			//----- update controlling parameters for the next sesseion
		beginning_object_label_per_session =  ending_object_label_per_session + 1 ;
		hid.setBeginningObjectLabelPerSession ( beginning_object_label_per_session ) ;

		num_non_transparent_facet_per_session = 0 ;
		hid.setNumNonTransparentFacetPerSession( num_non_transparent_facet_per_session ) ;

	}// if large number of items in hid.polygon3DList, or flush mode is true



	if( flush_mode ) {
			//----- set polygon images to array polygon2d[], and calc numPolygon2D
			//..... (WARNING: polygon2DList is cleared.)
			//..... (WARNING: Old data is polygon2d[] is abandonned.)
		hid.movePolygon2DListToArray();


#if defined DISPLAY_LOG
			//----- message
		cerr << endl << endl;
		cerr << "  Polygon images are copied to array.\n" ;
		cerr << "  " <<  hid.getNumPolygon2D() << " polygon images are created\n";
		cerr << endl << endl;
#endif

	}


		//----- reset object server to original state
	object_server.goTo( cur_org ) ;


} // Command::doIncrementalHiddenSurfaceElimination()


//////////////////////////////////
////////////////////////////////////



	//----- Command::doHiddenLineElimination()
void Command::doHiddenLineElimination(	Parameter&		parameter , 
					List<LineSegment>&	lineseg_server       ,
					HidLine&		hidline_remover      ,
					Hid&			hid_surface_remover  ,
					Perspective&		camera                )
{

			//------------------------------------//
			//----- Hidden line elimination  -----//
			//------------------------------------//
			//----- Final set up before doing hidden line removal
			//..... (1) list ---> array
			//..... (2) pass address of hid
		this->passLineSegmentToHidLine (	lineseg_server   , 
							camera           ,  
							hidline_remover  , 
							parameter          ) ;
		hidline_remover.install();
		hidline_remover.setHid( &hid_surface_remover );

			//----- Do hidden line removal
		if (	parameter.drawing_mode >= WF_STORED && \
			parameter.drawing_mode <= HID3          )		
		{
			this->doHiddenLineElimination( parameter, hidline_remover, parameter.epsilon3d, parameter.epsilon2d ) ;
		}
		

} // Command::doHiddenLineElimination()



	//----- Command::passEdgeToHidLineCMD()
void	
Command::passEdgeToHidLineCMD(	Object*			object_p      , 
				Perspective&		camera        ,
				HidLine&		hidline       ,
				const Parameter&	parameter      )
{

		//----- local
	int		double_edge_draw_mode = 0 ;
	LineType	linetype                  ;
	int	clipping_mode = getClippingMode ( parameter );

		//----- linetype_aux
	linetype.setWidth ( parameter.line_width );

		//----- set linetype to object 
		//.....  decide types of read, aux, and outline edges
	setLineTypeToObject( parameter , object_p ) ;

	object_p->passEdgeToHidLine (	camera                            ,
					&hidline                          ,
					parameter.aux_threshold_angle ,
					parameter.epsilon3d	          ,
					double_edge_draw_mode	          ,
					clipping_mode                     ,
					parameter.flag_back_edge_dashed  );

#if defined DISPLAY_LOG
	cerr << endl ;
	cerr << "  *** Clearing unnesessary data from memory (Type 2, Phase 1) ..." << endl;
	cerr << endl ;
	object_p->clearArray();
#endif


}// Command::passEdgeToHidLineCMD()


	//----- Command::passEdgeToHidLineCMD()
void	
Command::passEdgeToHidLineCMD(	List<Object>&		object_server , 
				Perspective&		camera        ,
				HidLine&		hidline       ,
				const Parameter&	parameter      )
{

		//----- local
	Object*		object_p      ;

		//-----  get vector sum of points
	object_server.toTail();	
	while( (object_p = object_server.getItemAddress())  ) 
	{

		this->passEdgeToHidLineCMD(	object_p      , 
						camera        ,
						hidline       ,
						parameter      );

		object_server.upward();	

	} // while( object_p ... )

}// Command::passEdgeToHidLineCMD()

