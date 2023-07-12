/////////////////////////////////////////
//////////  hidA.cc  1995/04/  //////////
/////////////////////////////////////////

// #define DEBUG_HIDE_A


#include "hid.h" 

const	double	LARGE_EPSILON2D_RATIO = 100.0 ;
const	double	MAX_LARGE_EPSILON2D   = 1.1e-5 ;


		//----- Hid::hide_A()
int   Hid::hide_A(	const Polygon2D&	a_polygon, 
			const Polygon2D&	b_polygon, 
			int			b_polygon_label  ,
			List<Polygon2D>*	output_list_p   , 
			List<int>*		list_label_p  , 
			double			epsilon             )
{
		//-------------------------//
		//----- local const   -----//
		//-------------------------//
	const double ONE_MINUS_EPSILON = 1.0 - epsilon ;


		//----------------------------//
		//----- local variables  -----//
		//----------------------------//

		//----- tolerance
	double	epsilon_large = LARGE_EPSILON2D_RATIO * epsilon ;
	if( epsilon_large > MAX_LARGE_EPSILON2D ) { epsilon_large = MAX_LARGE_EPSILON2D ; }

		//----- loop vairable
	int		i;		

		//----- output value
	int		num_output_polygon = 0 ;	// initialized

		//----- output to list_label_p
	int		b_polygon_label_output ;

		//----- num vertices
	int	a_num_vertex = a_polygon.numVertex ;	// initialized
	int	b_num_vertex = b_polygon.numVertex ;	// initialized

		//----- veretx position of a_polygon
	Vector2*	a_vertex_position = NULL ; // initialized

		//----- starting and ending points of an edge of b_polygon 
	Vector2		A_b, B_b;	

		//----- vertex labels of points A_b and B_b above
	int		vertex_label_A_of_b, vertex_label_B_of_b;

		//----- edge label of b_polygon connecting points A_b and B_b
	int		edge_label_AB_of_b ;

		//----- number of intersection between a_ and b_polygons
	int		num_intersection ;

		//----- intersection points of a_ and b_polygons
	Vector2		X_array[2] ;	// 0: for smaller AB_b_t_array[i]
					// 1: for larger  AB_b_t_array[i]

		//----- edge label of a_polygon
	int		a_edge_label_array[2] ;	// 0: edge where X_array[0] is
						// 1: edge where X_array[1] is

		//----- line parameters for line including 
		//..... an edge of a_polygon
	double		a_t_array [2] ; // 0: for X_array[0]
					// 1: for X_array[1]

		//----- line parameters for line including 
		//..... an edge of b_polygon
		//.....  AB_b_t_array[0] < AB_b_t_array[1]
	double		AB_b_t_array [2] ;	// 0: for X_array[0]
						// 1: for X_array[1]

		//----- vertex labels of a_polygon
		//.....   These vertices come from intersection
		//.....   with b_polygon .
	int		a_vertex_label_X[2] ;	// 0: for X_array[0]
						// 1: for X_array[1]

		//----- item got from a_polygon_list 
	Vertex2WithLineType		item_vlt ;

		//----- copy of a member of item_vlt
	LineType*	linetype_tmp ;

		//----- list initially equivalent to a_polygon,
		//...... and later to be devided
	List<Vertex2WithLineType>	a_polygon_list , a_polygon_list_backup ;

		//----- output list devided from a_polygon_list 
	List<Vertex2WithLineType>	list_vlt_output ;

		//----- Polygon2D type value equivalent to list_vlt_output
	Polygon2D			polygon2D_output ;

		//----- working variable for test of dividing a_polygon_list
//	Polygon2D			polygon2D_tmp ;	

		//----- flags... 1: division performed, 0: division not performed
	int	flag_is_devided ;

		//----- bounding box of subpolygon of a
//	double	left_a  , right_a  , top_a  , bottom_a  ;
//	double	left_AB , right_AB , top_AB , bottom_AB ;
	double	xmin_a  , xmax_a  , ymin_a  , ymax_a  , umin_a  , umax_a  , vmin_a  , vmax_a  ;
	double	xmin_AB , xmax_AB , ymin_AB , ymax_AB , umin_AB , umax_AB , vmin_AB , vmax_AB ;
	
		//-----------------------------------//
		//----- end of local variables  -----//
		//-----------------------------------//


		//---------------------------------------------//
		//----- error checking and initialization  -----//
		//---------------------------------------------//

		//----- error checking: too few vertices for given polygons?
	if( a_num_vertex < 3 || b_num_vertex < 3 ) {
		cerr << "ERROR: (Hid::hide_A() ) 1: ";
		cerr <<	"too few number of vertices of a_polygon\n";
		cerr << "a_num_vertex = " << a_num_vertex << endl;
		cerr << "b_num_vertex = " << b_num_vertex << endl;
		exit(1) ;
	}

		//----- make initial list equivalent to a_polygon 
	for ( i = 0 ; i < a_num_vertex ; i++ ) {
		a_polygon_list.addItem( Vertex2WithLineType(	a_polygon.vertex2D[i], 
								a_polygon.edgeType[i] ) );
	}

		//----- initial setting of a_vertex_position
	a_vertex_position = new Vector2 [ a_num_vertex ];
	if( a_vertex_position == NULL ) {
		cerr << "ERROR: (Hid::hide_A() ) 2: ";
		exit(1) ;
	}
	for ( i = 0 ; i < a_num_vertex ; i++ ) {
		a_vertex_position[i]  = a_polygon.vertex2D[i] ;
	}

		//----- bounding box of a_polygon
	Tool2D::getBoundingDiamond(	a_vertex_position, 
					a_num_vertex , 
					&xmin_a  , &xmax_a , 
					&ymin_a  , &ymax_a , 
					&umin_a  , &umax_a , 
					&vmin_a  , &vmax_a   );


		//-----------------------------------------------//
		//----- cut off sub_polygons from a_polygon -----//
		//-----------------------------------------------//

	for(	edge_label_AB_of_b = 0 ; 
		( (edge_label_AB_of_b  < b_num_vertex) && ( a_num_vertex >= 3 ) ) ; 
		edge_label_AB_of_b++                                                ) 
	{

			//----- end points of edges AB of b_polygon
		vertex_label_A_of_b = edge_label_AB_of_b ;   
		vertex_label_B_of_b = edge_label_AB_of_b + 1 ;
		if( vertex_label_B_of_b == b_num_vertex ) { vertex_label_B_of_b = 0 ; }

		A_b = b_polygon.vertex2D [ vertex_label_A_of_b ] ;
		B_b = b_polygon.vertex2D [ vertex_label_B_of_b ] ;

			//----- bounding box of edge AB of b_polygon
		{
				//----- local
			Vector2	AB_tmp[2];
			AB_tmp[0] = A_b ; AB_tmp[1] = B_b ; 
			Tool2D::getBoundingDiamond(	AB_tmp , 2 ,
							&xmin_AB  , &xmax_AB , 
							&ymin_AB  , &ymax_AB , 
							&umin_AB  , &umax_AB , 
							&vmin_AB  , &vmax_AB   );
		}

			//----- max-min test of edgeAB vs a_polygon
		if( xmax_AB  < ( xmin_a - epsilon ) ) { continue ; } // -x 
		if( xmin_AB  > ( xmax_a + epsilon ) ) { continue ; } // +x 
		if( ymax_AB  < ( ymin_a - epsilon ) ) { continue ; } // -y 
		if( ymin_AB  > ( ymax_a + epsilon ) ) { continue ; } // +y 
		if( umax_AB  < ( umin_a - epsilon ) ) { continue ; } // -u 
		if( umin_AB  > ( umax_a + epsilon ) ) { continue ; } // +u 
		if( vmax_AB  < ( vmin_a - epsilon ) ) { continue ; } // -v 
		if( vmin_AB  > ( vmax_a + epsilon ) ) { continue ; } // +v 

			//----- get intersections between directed line AB and a_polygon
			//.....  Edge AB of b_polygon is a part of the directed line AB.
		num_intersection = \
		Tool2D::directedLineVsConvexPolygon(	A_b, B_b , 
							a_vertex_position , a_num_vertex ,
							X_array ,
							AB_b_t_array,
 							a_t_array, 
							a_edge_label_array,
							epsilon );

			//----- Tests for calculated intersections:


			//---------- TEST 1:
			//.......... If num intersection is less than 2 ,
			//.......... skip this edge AB of b_polygon.
		if(	num_intersection < 2      ) { 
			continue ; // skip this edge AB of b_polygon
		}

			//----------  In the following, num_intersection = 2.

			//---------- TEST 2:
			//.......... If edge_label_array[] is in bad range,
			//.......... skip this edge AB of b_polygon.
		if(	a_edge_label_array[0] <  0             || 
			a_edge_label_array[0] >= a_num_vertex  || 
			a_edge_label_array[1] < 0 || 
			a_edge_label_array[1] >= a_num_vertex     ) 
		{
#if defined DEBUG_HID2
			cerr << "WARNING: (Hid::hide_A() ) 3: ";
			cerr << "   Inconsistent edge labels " << endl;
			cerr << "   a_num_vertex        = " << a_num_vertex          << endl ;  
			cerr << "   edge_label_array[0] = " << a_edge_label_array[1] << endl ;  
			cerr << "   edge_label_array[1] = " << a_edge_label_array[1] << endl ;  
#endif
			continue ; // skip this edge AB of b_polygon
		}


			//---------- TEST 3:
			//..... If edge AB of b_polygon is outside or on a edge of
			//..... a_polygon, skip this edge AB of b_polygon.
		if(	 AB_b_t_array[ 0 ] > ONE_MINUS_EPSILON && 
			 AB_b_t_array[ 1 ] > ONE_MINUS_EPSILON    )
		{
			continue ;  // skip this edge AB of b_polygon
		}
		if(	 AB_b_t_array[ 0 ] <  epsilon    && 
			 AB_b_t_array[ 1 ] <  epsilon       ) 
		{
			continue ;  // skip this edge AB of b_polygon
		}

			//---------- TEST 4:
			//.......... If two interactions are on the same edge of a_polygon,
			//.......... skip this edge AB of b_polygon, 
		if(    a_edge_label_array[0] == a_edge_label_array[1] ) { 
			continue ;  // skip this edge AB of b_polygon
		}
		
			//---------- TEST 5:
			//----- ERROR checking: 
			//.....  if AB_b_t_array[0] > AB_b_t_array[1], 
			//.....  there is something wrong in a_polygon 
			//.....  (a polygon with crossing edges?)
		if( AB_b_t_array[0] > AB_b_t_array[1]	) {  
#if defined DEBUG_HID2
			cerr << "WARNING: (Hid::hide_A() ) 4:\n ";
			cerr << "        Produced polygon with zero area is abandoned.\n ";
#endif
//			continue ; // @@@@@@
			break    ; // get out of for( edge_label_AB_of_b ) loop ---> exeptional treatment block
		}


			//----- Tests for calculated intersections completed.


			//----- BACKUP A_POLYGON LIST
			//.....  before intersection points are inserted to a_polygon,
			//.....  and so a_polygon is modified.
		a_polygon_list_backup = a_polygon_list ;

			//----- Set mark to the starting vertex of edge a_edge_label_array[0] 
			//.....  of a_polygon, where the 1st intersection is.
		a_polygon_list.goTo( a_edge_label_array[0] );
		a_polygon_list.setMark();

			//----- Set mark2 to the starting vertex of edge a_edge_label_array[1] 
			//.....  of a_polygon, where the 2nd intersection is.
		a_polygon_list.goTo( a_edge_label_array[1] );
		a_polygon_list.setMark2();
		
			//----- insert first intersection X_array[0] to a_polygon
			//..... as a new vertex, and mark to the new vertex.
			//..... A_POLYGON_LIST MODIFIED!
		a_polygon_list.toMark();
		a_polygon_list.getItem( item_vlt );	linetype_tmp = item_vlt.linetype ;
		a_polygon_list.insertItem( Vertex2WithLineType( X_array[0], linetype_tmp ) ) ;
		a_polygon_list.setMark();

			//----- insert 2nd intersection X_array[1] to a_polygon
			//..... as a new vertex, and mark to the new vertex
			//..... A_POLYGON_LIST MODIFIED!
		a_polygon_list.toMark2();
		a_polygon_list.getItem( item_vlt );	linetype_tmp = item_vlt.linetype ;
		a_polygon_list.insertItem( Vertex2WithLineType( X_array[1], linetype_tmp ) ) ;
		a_polygon_list.setMark2();

			//----- get vertex labels of added new vertices of a_polygon
		a_vertex_label_X[0] = a_polygon_list.whereIsMark();
		a_vertex_label_X[1] = a_polygon_list.whereIsMark2();

			//----- The two intersections are on the same edge of a_polygon?
			//.....  If true, cancel modification of a_polygon_list, 
			//.....  and skip this edge AB of b_polygon
		if( a_vertex_label_X[0] == a_vertex_label_X[1] ) {
#if defined DEBUG_HID2
			cerr << "WARNING: (Hid::hide_A() ) 5:\n ";
#endif
			a_polygon_list = a_polygon_list_backup ;
			continue ; // skip this edge AB, and cancel modification of a_polygon
		}

			//----- Copy  a_vertex_label_X[0] -->.....-->a_vertex_label_X[1] 
			//.....  to list_vlt_output ( temporary list ),
			//.....  and remove "....." from a_polygon.
			//.....  Note that "a_vertex_label_X[0]" and 
			//.....  "a_vertex_label_X[1]" are only copied, not moved.
			//..... A_POLYGON_LIST MODIFIED!
		list_vlt_output.clear();
		flag_is_devided = \
		a_polygon_list.devideCyclic(	a_vertex_label_X[0], 
						a_vertex_label_X[1] , 
						&list_vlt_output    ) ;

			//----- In the following, (1) tests of division and (2) output
			//..... are performed.


			//----- TEST D-1
			//----- If bad parameters were passed in calling
			//..... a_polygon_list.devideCyclic() in the above,
			//..... cancel the division and skip this edge AB of b_polygon.
		if( flag_is_devided == 0 ) { 
			a_polygon_list = a_polygon_list_backup ;
			continue ; // skip this edge AB, and cancel modification of a_polygon
		}
					
			//----- reset perforation line types
			//---------- 1.  remaining a_polygon list
			//.......... A_POLYGON_LIST MODIFIED!
		a_polygon_list.toMark(); 
		a_polygon_list.overwriteItem( Vertex2WithLineType( X_array[0], &perforationLineType ) ) ;
			//---------- 2.  output polygon list
		list_vlt_output.toHead();
		list_vlt_output.overwriteItem( Vertex2WithLineType( X_array[1], &perforationLineType ) ) ;


			//----- TEST D-2: test of left (remaining) sub-polygon
			//..... If no area exists in the left part of line AB,
			//..... cancel the division and skip this edge AB of b_polygon.
		this->removeZeroLengthEdge( a_polygon_list , epsilon ); // remove zero-length edge
		if( ( a_polygon_list.getNItem() ) < 3 ) 
		{
			a_polygon_list = a_polygon_list_backup ;
			continue ;  // skip this edge AB, and cancel modification of a_polygon
		} else {

				//----- more strong ristriction to avoid creation of area-zero a_polygon

				//----- local 
			int				num_vertex_left_tmp ;
			int				num_ort_zero_vertex_left_tmp ;
			List<Vertex2WithLineType>	list_vlt_tmp ;		

				//----- test with large epsilon2d
			list_vlt_tmp = a_polygon_list ; //  make a copy of left polygon to check it
			this->removeZeroLengthEdge( list_vlt_tmp , epsilon_large ); // USE "epsilon_large" !!!

			num_vertex_left_tmp          = list_vlt_tmp.getNItem(); 
			num_ort_zero_vertex_left_tmp = this->getNumOrtZeroVertex( list_vlt_tmp, epsilon );

			if( num_vertex_left_tmp < 3 ) { 
				a_polygon_list = a_polygon_list_backup ;
				continue ; // skip this edge AB, and cancel modification of a_polygon
			} else { 
				if( ( num_vertex_left_tmp - num_ort_zero_vertex_left_tmp ) < 3 ) {
					a_polygon_list = a_polygon_list_backup ;
					continue ; // skip this edge AB, and cancel modification of a_polygon
				}
			}

		} // TEST D-2


			//----- TEST D-3: test of right (output) sub-polygon
			//..... If no area exists in the right part of line AB,
			//..... cancel the division and skip this edge AB of b_polygon.
		this->removeZeroLengthEdge( list_vlt_output , epsilon ); // remove zero-length edge
		if( ( list_vlt_output.getNItem() ) < 3 ) 
		{
			a_polygon_list = a_polygon_list_backup ;
			continue ;  // skip this edge AB, and cancel modification of a_polygon
		} else {
				//----- more strong ristriction to avoid creation of area-zero output polygons

				//----- local 
			int				num_vertex_right_tmp ;
			int				num_ort_zero_vertex_right_tmp ;
			List<Vertex2WithLineType>	list_vlt_tmp ;	

				//----- test with large epsilon2d
			list_vlt_tmp = list_vlt_output ; //  make a copy of right polygon to check it
			this->removeZeroLengthEdge( list_vlt_tmp , epsilon_large );  // USE "epsilon_large" !!!

			num_vertex_right_tmp          = list_vlt_tmp.getNItem(); 
			num_ort_zero_vertex_right_tmp = this->getNumOrtZeroVertex( list_vlt_tmp, epsilon );

			if( num_vertex_right_tmp < 3 ) { 
				a_polygon_list = a_polygon_list_backup ;
				continue ; // skip this edge AB, and cancel modification of a_polygon
			} else { 
				if( ( num_vertex_right_tmp - num_ort_zero_vertex_right_tmp ) < 3 ) {
					a_polygon_list = a_polygon_list_backup ;
					continue ; // skip this edge AB, and cancel modification of a_polygon
				}
			}

		} // TEST D-3


			//----- list_vlt_output --> polygon2D_output 
		polygon2D_output.set( list_vlt_output, a_polygon.color , a_polygon.objectLabel , a_polygon.parent3D_p );


			//----- Now all tests for divisions are passed. Go to output.

			//----- OUTPUT polygon2D_output to output_list_p 
		output_list_p->addItem( polygon2D_output );
		b_polygon_label_output = b_polygon_label + 1 ;
		list_label_p->addItem( b_polygon_label_output );
		num_output_polygon++ ;	// count number of output polygon


			//----- reset a_num_vertex and a_vertex_positions
		a_num_vertex = a_polygon_list.getNItem(); // count remaining vertices
		delete [] a_vertex_position ;
		a_vertex_position = new Vector2 [ a_num_vertex ];
		if( a_vertex_position == NULL ) {
			cerr << "ERROR: (Hid::hide_A() ) 6: ";
			exit(1) ;
		}
		a_polygon_list.toTail(); i = 0 ;
		while( a_polygon_list.getItem( item_vlt ) ) {
			a_vertex_position[i++]  = item_vlt.position ;
			a_polygon_list.upward();
		}


			//----- update bounding diamond of remaining a_polygon
		Tool2D::getBoundingDiamond(	a_vertex_position, 
						a_num_vertex , 
						&xmin_a  , &xmax_a , 
						&ymin_a  , &ymax_a , 
						&umin_a  , &umax_a , 
						&vmin_a  , &vmax_a   );

	} // for( edge_label_AB_of_b )


		//-------------------------------------------------//
		//----- delete dinamically allocated memories -----//
		//-------------------------------------------------//
	delete  []  a_vertex_position ;


		//--------------------------------//
		//----- exeptional treatment -----//
		//--------------------------------//

	if( a_polygon_list.getNItem() >= 3 ) {

			//----- variables local to this block
		int		flag_inside ;
		int		status_of_remaining_a_polygon;
		Vector2		center ;


			//----- make remaining sub polygon
		polygon2D_output.set( a_polygon_list, a_polygon.color , a_polygon.objectLabel, a_polygon.parent3D_p );
		polygon2D_output.removeZeroLengthEdge( epsilon );

			//----- investigate remaining sub polygon
		if( polygon2D_output.numVertex >= 3 ) {
			status_of_remaining_a_polygon \
			= Tool2D::polygonVsPolygon(	polygon2D_output.vertex2D, 
							polygon2D_output.numVertex, 
							b_polygon.vertex2D, 
							b_polygon.numVertex, 
							epsilon );

			if ( status_of_remaining_a_polygon == Tool2D::INDEPENDENT )
			{
					//----- INDEPENDENT: 
					//.....     OUTPUT remaining sub polygon to output list
				output_list_p->addItem( polygon2D_output );
				b_polygon_label_output = b_polygon_label + 1 ;
				list_label_p->addItem( b_polygon_label_output );
				num_output_polygon++ ;	// count number of output polygon

			} else if ( status_of_remaining_a_polygon == Tool2D::INCLUDED ||
				    status_of_remaining_a_polygon == Tool2D::EQUAL       )
			{
					//----- INCLUDED or EQUAL: 
					//.....     abondon remaining sub polygon to output list
				; // do nothing
			} else {	//----- CROSS or INCLUDE

					//----- get  rough center of remaining a_polygon
				center = Tool2D::getRoughPolygonCenter(	polygon2D_output.vertex2D , 
									polygon2D_output.numVertex ); 

					//----- see if the rough center is inside b_polygon
				flag_inside \
				= Tool2D::isPointInsidePolygon(		center , 
								  	b_polygon.vertex2D , 
									b_polygon.numVertex , 
									epsilon ) ;

					//----- if outside , we regard that the remaining a_polygon is INDEPENDENT, 
					//..... of b_polygon, and add it to output list, else abandon it
				if( flag_inside == 0 ) {
						//----- rough center is outside: OUTPUT
					output_list_p->addItem( polygon2D_output );
					b_polygon_label_output = b_polygon_label + 1 ;
					list_label_p->addItem( b_polygon_label_output );
					num_output_polygon++ ;	// count number of output polygon
				} else {
					//----- rough center is inside or on edge
					;	// do nothing
				}

			} // if ( status_of_remaining_a_polygon == ...) -- else

		} // if( polygon2D_output.numVertex... ) 

	} // if( a_polygon_list.getNItem()...)


		//-------------------------------------------------//
		//----- return number of output polygons      -----//
		//-------------------------------------------------//
	return		num_output_polygon ;

} // Hid::hide_A()
