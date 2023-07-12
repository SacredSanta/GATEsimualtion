///////////////////////////////////////////
//////////  hidB.cc  1995/11/16////////////
///////////////////////////////////////////

#include "hid.h"

// #define DEBUG_CONVEXATE

const	double	LARGE_EPSILON2D_RATIO = 100.0 ;
const	double	MAX_LARGE_EPSILON2D   = 1.1e-5 ;

	//----- Hid::convexate( List<Polygon3D>& )
int	Hid::convexate(	List<Polygon3D>& input_Polygon3D_list ,
			List<Polygon3D>* output_Polygon3D_list_p ,
			double epsilon3d  , 
			double epsilon2d                            )
{
		//----- local
	int			num_output_polygon = 0 ; // initialize
	Polygon3D		poly3D ;

		//----- convexation
//	input_Polygon3D_list.toTail() ;
	while( input_Polygon3D_list.pop( poly3D ) ) {
		num_output_polygon += \
		convexate(	poly3D, 
				output_Polygon3D_list_p ,
				epsilon3d, epsilon2d      );        
	}

		//----- return number of output convex polygons
	return num_output_polygon;

} // Hid::convexate( List<Polygon3D>& )


	//----- Hid::convexate( Polygon3D& )
int	Hid::convexate(		Polygon3D&  polygon3D_input , 
				List<Polygon3D>* output_Polygon3D_list_p,
				double epsilon3d  , 
				double epsilon2d  )
{
		//------------------------------------------//
		//----- local variables  and constants -----//
		//------------------------------------------//

		//----- loop vairable
	int		i ;

		//----- tolerance
	double	epsilon2d_large = LARGE_EPSILON2D_RATIO * epsilon2d ;
	if( epsilon2d_large > MAX_LARGE_EPSILON2D ) { epsilon2d_large = MAX_LARGE_EPSILON2D ; }

		//----- numbers 
	int		num_output_polygon ;	// return value
	int		num_vertex ;
	int		num_vertex_left ;
	int		num_vertex_right ;
	int		num_convex_vertex  ;
	int		num_ort_zero_vertex ;

		//----- vertex and edge labels
	int		concave_vertex_label ;
	int		concave_vertex_prev_label ;
	int		concave_vertex_next_label ;
	int		X_edge_label ;
	int		perforation_edge_label[2] ;
				// [0]:begin, [1]:end

		//----- flags
	int	flag_X_found ;
	int	flag_is_devided ;
	int	flag_2DTo3D_succeeded  ;
	int	flag_left_polygon_exist ;
	int	flag_right_polygon_exist ;

		//----- const
	const	int	NONE = -1 ; // no concave vertex

		//----- line parameters 
	double		t_AB, t_edge ;

		//----- points concerned with perforation line AB
		//.....  A: a concave vertex of polygon
	Vector2		A, B, C;	

		//----- intersection point between 
		//..... perforation line AB and polygon 
	int		X_3D_index ;
	Vector2		X ;
	Vector3		X_3D, *X_3D_p ;

		//----- item got from list_vlt 
	Vertex2WithLineType		item_vlt ;

		//----- stack for temporary (may be concave)
		//..... polygon3D
	List<Polygon3D>	polygon_stack ;

		//----- list initially equivalent to polygon,
		//...... and later to be devided
	List<Vertex2WithLineType>	list_vlt ;

		//----- right sub-polygonoutput devided from list_vlt 
	List<Vertex2WithLineType>	list_vlt_right ;

		//----- Polygon3D 
	Polygon3D			polygon3D  ;

		//-----------------------------------//
		//----- end of local variables  -----//
		//-----------------------------------//


		//------------------------//
		//----- preparation  -----//
		//------------------------//

		//----- initialize return value
	num_output_polygon = 0 ;

		//----- TEST 1: convex polygon? --- part 1
	num_vertex = polygon3D_input.numVertex ; 
	if( num_vertex  >= 3 && polygon3D_input.getConvexity() ) {
		output_Polygon3D_list_p->push( polygon3D_input  );	num_output_polygon++ ;	
		return num_output_polygon ;	// CONVEX polygon ---> RETURN 
	}

		//----- Remove edges with zero-length image on screen
	polygon3D_input.removeZeroLengthImageEdge( epsilon2d, epsilon3d, camera_p );
	num_vertex = polygon3D_input.numVertex ; 

		//----- TEST 2: too few vertices for given polygons?
	if( num_vertex  < 3 ) { 
#if defined DEBUG_CONVEXATE
		cerr << "WARNING (Hid::convexate()) 1A : ";
		cerr << "Too few vertices of a polygon" << endl;
#endif
		return 0 ; // ERROR ---> RETURN 
			   // Number of output polygon is zero.
	} 

//		//----- TEST 3: irregular object label?
//	if ( polygon3D_input.objectLabel < 0 ) {
//#if defined DEBUG_CONVEXATE
//		cerr << "WARNING (Hid::convexate()) 1B : "; 
//		cerr << "Polygon with irregular object label" << endl;
//#endif
//		return 0 ; // ERROR ---> RETURN 
//			   // Number of output polygon is zero.
//	}

		//----- TEST 3: convex polygon? --- part 2
	num_convex_vertex  = \
	Tool2D::getNumConvexVertex(	polygon3D_input.vertex2D  , 
					num_vertex                ,
					epsilon2d );
	if( num_vertex == num_convex_vertex ) {
		output_Polygon3D_list_p->push( polygon3D_input  );	num_output_polygon++ ;	
		return num_output_polygon ;	// CONVEX polygon ---> RETURN 
	}

		//----- initialize polygon stack
	polygon_stack.clear();
	polygon_stack.push( polygon3D_input ) ;

		//--------------------------//
		//----- do convexation -----//
		//--------------------------//
	while( polygon_stack.pop( polygon3D ) ) 
	{
			//----- local to this block
		Polygon3D	right_subpolygon3D ;
		Polygon3D	left_subpolygon3D ;	

			//----------------------------------------------//
			//----- 1.Initialize list_vlt             -----//
			//-----   (equivalent to popped Polygon3D) -----//
			//----------------------------------------------//

			//----- number of vertices for the popped polygon
			//..... It will be reset if zero-length edges exist.
		num_vertex =	polygon3D.numVertex ;

			//----- make list equivalent to popped polygon3D
		list_vlt.clear();
		for ( i = 0 ; i < num_vertex ; i++ ) {
			list_vlt.push( Vertex2WithLineType(	polygon3D.vertex2D[ i ] , 
								polygon3D.edgeType[ i ] ,
								polygon3D.vertex  [ i ]	) ) ;
		}

			//----- remove  zero-length-edge and recalc number of vertices
		this->removeZeroLengthEdge( list_vlt, epsilon2d );
		num_vertex =	list_vlt.getNItem();	// Reset number of vertices 
							// for the popped polygon.
		num_ort_zero_vertex = this->getNumOrtZeroVertex( list_vlt, epsilon2d );
		if( num_vertex < 3 ) { 
			continue ; // Abandon this polygon3D
				   // and pop next one.
		} else { 

			if( ( num_vertex - num_ort_zero_vertex ) < 3 ) {
				continue ; 	// Aolygon with zero area.
						// Abandon this polygon3D, and pop the next one
			}
		}

			//------------------------------------//
			//----- 2.search concave vertex  -----//
			//------------------------------------//

			//----- search first concave vertex
		concave_vertex_label = \
		this->getFirstConcaveVertex( list_vlt , epsilon2d );
		
			//------ no concave vertices ?
			//..... If yes, output it.
		if( NONE == concave_vertex_label  ) { 
				//----- OUTPUT
			output_Polygon3D_list_p->push( polygon3D );	num_output_polygon++ ;	
			continue ;	// Polygon was convex, and so output .
					// Pop the next one.
		} 

			//-------------------------------------------------------//
			//----- 3.store vertex labesl of the concave vertex -----//
			//-----   and its neighboring vertices              -----//
			//-------------------------------------------------------//

			//------ neighboring vertex labels
		concave_vertex_prev_label  =  concave_vertex_label - 1 ;
		if( -1 == concave_vertex_prev_label ) { 
			concave_vertex_prev_label = num_vertex - 1 ;
		}
		concave_vertex_next_label  =  concave_vertex_label + 1 ;
		if( num_vertex == concave_vertex_next_label ) { 
			concave_vertex_next_label = 0 ;
		}

			//------------------------------------------------//
			//----- 4.definde perforation half line AB,  -----//
			//------------------------------------------------//

			//----- perforation line AB
		list_vlt.goTo( concave_vertex_label ) ;

		list_vlt.getDownwardItemCyclic( item_vlt ) ;
		A = item_vlt.position ;		// prev to convave vertex

		list_vlt.getItem( item_vlt ) ;
		B      = item_vlt.position ;	// convave vertex

		list_vlt.getUpwardItemCyclic( item_vlt ) ;
		C = item_vlt.position ;		// next to convave vertex

			//---------------------------------------------------------//
			//----- 5.search intersection "X" between perforation -----//
			//----- line AB and polygon (with minimum t_AB)     -------//
			//---------------------------------------------------------//

			//----- search intersection
		flag_X_found = \
		this->halfLineVsPolygonForConvexation(	A, B , C,
							list_vlt ,
							concave_vertex_next_label ,
							(num_vertex - 2) ,
							&X ,
							&t_AB, 
							&t_edge ,
							&X_edge_label ,
							epsilon2d );
		
			//----- no intersection?
			//..... If yes, regard the polygon as a convex one
		if( !flag_X_found ) {
#if defined DEBUG_CONVEXATE
			cerr << "WARNING (Hid::convexate) 3 : ";
			cerr << " Cannot find intersection.\n";
			cerr << " Abandon this polygon.\n "; 
#endif
			output_Polygon3D_list_p->push( polygon3D );	num_output_polygon++ ;	
			continue ; // skip this polygon3D and pop next one
		} 

			//------------------------------------------------//
			//------ 6.  calc X_3D and store it          -----//
			//------     in Hid::auxiliaryVertexPosition -----//
			//------------------------------------------------//

			//------ calc X_3D 
		flag_2DTo3D_succeeded = \
		point2DToPoint3D (	X                            , 
					polygon3D_input.normalVector ,
					*(polygon3D_input.vertex[0]) ,
					&X_3D                        ,
					epsilon3d                      );

		if( !flag_2DTo3D_succeeded ) {
#if defined DEBUG_CONVEXATE
			cerr << "WARNING: (Hid::convexate() ) 6:\n ";
			cerr << " Cannot calc 3D position. "        ;
			cerr << " Maybe, normal_vector * eye_vector = 0.\n"  ;
			cerr << " Abandon this polygon.\n "         ; 
#endif
			continue ; // skip this polygon3D and pop next one
		}
		auxiliaryVertexPosition.push( X_3D ) ;
		X_3D_index = ( auxiliaryVertexPosition.getNItem() ) - 1 ;
		X_3D_p = &(auxiliaryVertexPosition[ X_3D_index ] ) ;
			// WARNING!!! : Direct address to private member
			//              through pointer.


			//--------------------------------------------------//
			//----- 7. Add the intersection X to list_vlt -----//
			//-----  * mark  is set to the concave vertex  -----//
			//-----  * mark2 is set to added X             -----//
			//--------------------------------------------------//

			//------ examine calculated intersection, X
			//----- TEST 1:
		if(	X_edge_label == concave_vertex_label      \
				||                                \
			X_edge_label == concave_vertex_prev_label \
				||                                \
			X_edge_label <   0                        \
				||                                \
			X_edge_label >=  num_vertex                  )
		{
#if defined DEBUG_CONVEXATE
			cerr << "WARNING (Hid::convexation) 4A :\n ";
			cerr << " Inconsistent vertex label: " << X_edge_label << endl ;
			cerr << " Abandon this polygon.\n "; 
#endif
			continue ;  // skip this polygon3D and pop next one
		}

			//----- Set mark to concave vertex, 
		list_vlt.goTo( concave_vertex_label );
		list_vlt.setMark();

			//----- Insert intersection X to polygon and set mark2 there
			//..... A_POLYGON_LIST MODIFIED!
		list_vlt.goTo( X_edge_label );
		list_vlt.getItem(item_vlt) ;
		list_vlt.insertItem( Vertex2WithLineType( X, item_vlt.linetype,X_3D_p ) ) ;
		list_vlt.setMark2();

			//-------------------------------------------------//
			//----- 8. devide list_vlt to left and right -----//
			//-----    sub-polygons                       -----//
			//-------------------------------------------------//

			//----- vertex labels on starting and ending points of
			//..... perforation edge
		perforation_edge_label[0] = list_vlt.whereIsMark();
		perforation_edge_label[1] = list_vlt.whereIsMark2();

			//----- TEST 2:
		if( perforation_edge_label[0] == perforation_edge_label[1] ) {
#if defined DEBUG_CONVEXATE
			cerr << "WARNING: (Hid::convexate() ) 4B:\n ";
			cerr << " Inconsistent vertex label:\n ";
			cerr << " Abandon this polygon.\n "; 
#endif
			continue ; // skip this polygon3D and pop next one
		}

			//----- Copy  perforaton_edge_label[0] -->.....-->perforaton_edge_label[1] 
			//.....  to list_vlt_right ( temporary list ),
			//.....  and remove "....." from list_vlt
			//.....  Note that "perforaton_edge_label[0]" and 
			//.....  "perforaton_edge_label[1]" are only copied, not moved.
		list_vlt_right.clear();
		flag_is_devided = \
		list_vlt.devideCyclic(	perforation_edge_label[0], 
					perforation_edge_label[1] , 
					&list_vlt_right    ) ;


			//----- TEST D-1
			//----- If bad parameters were passed in calling
			//..... list_vlt.devideCyclic() in the above,
			//..... cancel the division and skip this edge AB of b_polygon.
		if( !flag_is_devided ) { 
#if defined DEBUG_CONVEXATE
			cerr << "WARNING: (Hid::convexate() ) 5:\n ";
			cerr << " Division of list failed.\n ";
			cerr << " Abandon this polygon.\n "; 
#endif
			continue ; // skip this polygon3D and pop next one
		}

			//----- set porforation linetype to each sub-polygon
		list_vlt.toMark(); 
		list_vlt.getItem( item_vlt ) ;
		item_vlt.linetype     = &perforationLineType  ;
		list_vlt.overwriteItem( item_vlt );

		list_vlt_right.toHead(); 
		list_vlt_right.getItem( item_vlt ) ;
		item_vlt.linetype     = &perforationLineType  ;
		list_vlt_right.overwriteItem( item_vlt );

			//----------------------------------------------------//
			//------ 9A.  Make the left sub-polygon complete -----//
			//---------------------------------------------------//

			//----- remove zero-length edge
		this->removeZeroLengthEdge( list_vlt, epsilon2d );		
		num_vertex_left          = list_vlt.getNItem(); 

			//----- TEST D-2: test of left (remaining) sub-polygon
		if( num_vertex_left < 3 ) 
		{
			flag_left_polygon_exist = 0 ;

		} else {
				//----- more strong ristriction to avoid creation of area-zero a_polygon

				//----- local 
			int				num_vertex_left_tmp ;
			int				num_ort_zero_vertex_left_tmp ;
			List<Vertex2WithLineType>	list_vlt_tmp ;		

				//----- test with large epsilon2d
			list_vlt_tmp = list_vlt ;
			this->removeZeroLengthEdge( list_vlt_tmp , epsilon2d_large );	// USE "epsilon2d_large" !!!

			num_vertex_left_tmp          = list_vlt_tmp.getNItem(); 
			num_ort_zero_vertex_left_tmp = this->getNumOrtZeroVertex( list_vlt_tmp, epsilon2d );

			if( num_vertex_left_tmp < 3 ) { 
				flag_left_polygon_exist = 0 ;
			} else { 
				if( ( num_vertex_left_tmp - num_ort_zero_vertex_left_tmp ) < 3 ) {
					flag_left_polygon_exist = 0 ;
				} else {
					flag_left_polygon_exist = 1 ;
				}
			}

		} // TEST D-2


			//----- make Polygon3D type value for left sub-polygon
		if( flag_left_polygon_exist ) {
				//----- make Polygon3D type value
			left_subpolygon3D.set( 	list_vlt                        , 
						polygon3D_input.color            ,
						&(polygon3D_input.normalVector)  ,
						this->camera_p                    ,
						epsilon2d                         ,
						polygon3D_input.objectLabel       ,
						polygon3D_input.facetLabel        ,						
						NULL,  0                            );
		}



			//-----------------------------------------------------//
			//------ 9B.  Make the right sub-polygon complete -----//
			//-----------------------------------------------------//

			//----- remove zero-length edge
		this->removeZeroLengthEdge( list_vlt_right, epsilon2d );		
		num_vertex_right = list_vlt_right.getNItem();


			//----- TEST D-3: test of right (devided-out) sub-polygon
		if( num_vertex_right < 3 ) 
		{
			flag_right_polygon_exist = 0 ;

		} else {
				//----- more strong ristriction to avoid creation of area-zero a_polygon

				//----- local 
			int				num_vertex_right_tmp ;
			int				num_ort_zero_vertex_right_tmp ;
			List<Vertex2WithLineType>	list_vlt_right_tmp ;		

				//----- test with large epsilon2d
			list_vlt_right_tmp = list_vlt_right ;
			this->removeZeroLengthEdge( list_vlt_right_tmp , epsilon2d_large ); 	// USE "epsilon2d_large" !!!

			num_vertex_right_tmp          = list_vlt_right_tmp.getNItem(); 
			num_ort_zero_vertex_right_tmp = this->getNumOrtZeroVertex( list_vlt_right_tmp, epsilon2d );

			if( num_vertex_right_tmp < 3 ) { 
				flag_right_polygon_exist = 0 ;
			} else { 
				if( ( num_vertex_right_tmp - num_ort_zero_vertex_right_tmp ) < 3 ) {
					flag_right_polygon_exist = 0 ;
				} else {
					flag_right_polygon_exist = 1 ;
				}
			}

		} // TEST D-3


			//----- make Polygon3D type value for right sub-polygon
		if( flag_right_polygon_exist ) {		
				//----- make Polygon3D type value for right sub-polygon
			right_subpolygon3D.set( list_vlt_right                  , 
						polygon3D_input.color            ,
						&(polygon3D_input.normalVector)  ,
						this->camera_p                   ,
						epsilon2d                        ,
						polygon3D_input.objectLabel      ,
						polygon3D_input.facetLabel       ,						
						NULL,  0                            );
		}


			//----------------------------------------------//
			//------ 10. delete X_3D if it is not used -----//
			//----------------------------------------------//
//
//		if(	(!flag_left_polygon_exist  ) && (!flag_right_polygon_exist ) && flag_2DTo3D_succeeded ) 
//		{
//			auxiliaryVertexPosition.deleteItem();
//		}
//

			//----------------------------------------------------------//
			//----- 11.  push left and right sub-polygons to stack -----//
			//-----      Note that they might be concave again.    -----//
			//----------------------------------------------------------//

			//----- push left and right sub polygons to stack
		if(	flag_left_polygon_exist && flag_right_polygon_exist ) 
		{	
			if( left_subpolygon3D.numVertex >= 3 ) {
				polygon_stack.push( left_subpolygon3D );
			}
			if( right_subpolygon3D.numVertex >= 3 ) {
				polygon_stack.push( right_subpolygon3D );
			}
		} else if (flag_left_polygon_exist && (!flag_right_polygon_exist) ) 
		{
				//----- push left sub polygon to stack
			if(	left_subpolygon3D.numVertex < num_vertex &&\
				left_subpolygon3D.numVertex >= 3             ) 
			{
				polygon_stack.push( left_subpolygon3D );
			} else {
#if defined DEBUG_CONVEXATE
				cerr << "WARNING: (Hid::convexate() ) 6A:\n ";
#endif
			}
		} else if ( (!flag_left_polygon_exist) && flag_right_polygon_exist ) 
		{
				//----- push right sub polygon to stack
			if(	right_subpolygon3D.numVertex < num_vertex &&\
				right_subpolygon3D.numVertex >= 3             ) 
			{
				polygon_stack.push( right_subpolygon3D );
			} else {
#if defined DEBUG_CONVEXATE
				cerr << "WARNING: (Hid::convexate() ) 6B:\n ";
#endif
			}
		} else {
			; // do nothing
		}

	} // while( pop )


		//-------------------------------------------------//
		//----- return number of output polygons      -----//
		//-------------------------------------------------//
	return		num_output_polygon ;

} // Convexator::convexation()

// num_vertex_left
// num_vertex_right

	//----- Hid::halfLineVsPolygonForConvexation()
int
Hid::halfLineVsPolygonForConvexation(	Vector2&  A,      Vector2&  B  ,  Vector2& C  ,
				List<Vertex2WithLineType>& list_vlt   ,
				int  starting_label, int     max_search , 
				Vector2*  X_p                           ,  
				double*   t_AB_p                        ,
				double*   t_edge_p                      ,
				int*      edge_label_p                  ,
				double    epsilon                         )
{
		//----- local constants
	int			cur_backup = list_vlt.whereIsCurrentNode() ;
	int			status_tmp ;
	int			num_vertex = list_vlt.getNItem();
	int			num_intersection ;
	int			num_search_done ;
	int			flag_X_found ;
	double			t_AB_tmp , t_edge_tmp ;
	Vector2			X_tmp ;
	Vector2			E_begin_prev ;
	Vector2			E_begin, E_end ;  // edge position
	Vertex2WithLineType	item_vlt ;


		//----- error checking
	if( starting_label < 0 || starting_label >= num_vertex ) {
		return  0 ;
	}
	if( max_search <= 0 ) {	return  0 ; }
	if( max_search > num_vertex   ) {
		max_search = num_vertex ;
	}

		//----- search intersection with minimum t_AB
	list_vlt.goTo( starting_label ) ; // initialize
	num_search_done  = 0 ; // initialize
	num_intersection = 0 ; // initialize	
	flag_X_found     = 0 ; // initialize
	while( num_search_done < max_search ) 
	{ 
			//----- previous vertex for begining point of edge
		list_vlt.getDownwardItemCyclic( item_vlt ) ;
		E_begin_prev = item_vlt.position ;

			//----- begining point of edge
		list_vlt.getItem( item_vlt ) ;
		E_begin = item_vlt.position ;

			//----- ending point of edge
		list_vlt.getUpwardItemCyclic( item_vlt ) ;
		E_end = item_vlt.position ;

			//----- get intersections between 
			//..... directed half line AB and polygon
		status_tmp = \
		Tool2D::halfLineVsLineseg(	A, B , 
						E_begin, E_end , 
						&X_tmp ,
						&t_AB_tmp, 
						&t_edge_tmp ,
						epsilon );		

			//----- Test1:  Edge with X goes from right to left?
		if(	status_tmp == 1  && \
			Tool2D::isLeftToRight( A, B, E_begin, E_end ) >=0 )
		{
			status_tmp = 0 ; // skip this X
		}

			//----- Test2: Is X in on linesegment AB ?
		if(  status_tmp == 1  && t_AB_tmp < (1 + epsilon ) ) 
		{
			if( t_AB_tmp < (1 - epsilon ) ) { // t_AB < 1.0 
				status_tmp = 0 ; // skip this X
			} else {                          // t_AB = 1.0 
					//----- if edge is in right of half region ABC, 
					//..... skip this X
				if(	Tool2D::orientation( A, B, E_begin , epsilon ) <= 0 
					&&  
					Tool2D::orientation( B, C, E_begin , epsilon ) <= 0 
					&&				
					Tool2D::orientation( A, B, E_end , epsilon )   <= 0 
					&&  
					Tool2D::orientation( B, C, E_end , epsilon )   <= 0 ) 
				{
					status_tmp = 0 ;
				}
			}
		}


			//----- store line parameter and vertex label
		if( status_tmp == 1 ) {
			flag_X_found = 1 ;
			num_intersection++ ;
			if( 1 ==  num_intersection || t_AB_tmp < *t_AB_p ) {
				*X_p          = X_tmp ;
				*t_AB_p       = t_AB_tmp ;
				*t_edge_p     = t_edge_tmp ;
				*edge_label_p = list_vlt.whereIsCurrentNode() ;
			}
		}

			//----- updation
		list_vlt.toCounterClockWise();
		num_search_done++ ;

	} // while( list_vlt)


		//----- reset current node
	list_vlt.goTo( cur_backup ) ;

		//----- return number of vertices for the revised polygon
	return flag_X_found ;

} // Hid::halfLineVsPolygonForConvexation()
