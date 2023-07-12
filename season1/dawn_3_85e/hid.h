//////////////////////////////////////////
//////////  hid.h  1995/06/01   //////////
//////////////////////////////////////////


#if !defined HID_H
#define HID_H

#include <fstream.h>	
#include <stdlib.h>	
#include "diamond.h"
#include "linetype.h"
#include "parameter.h" 
#include "perspective.h"
#include "list.h"
#include "qsort.h"
#include "tool2d.h"
#include "tool3d.h"
#include "vector.h"

//---------------------//
//----- constants -----//
//---------------------//

//---------------------//
//----- proto types -----//
//---------------------//
class Vertex2WithLineType ;
class Polygon2D ;
class Polygon3D ;
class Hid       ;

//-------------------------------------//
//----- class Vertex2WithLineType -----//
//-------------------------------------//

class Vertex2WithLineType {
 public:
	Vector2		position ;
	LineType*	linetype ;
	Vector3*	position3D_p ;
	int		label ;
 public: 	
	
		//----- constructor 1
	Vertex2WithLineType(void) : position() 
	{ linetype = NULL ;  position3D_p = NULL ; label = -1 ;} 

		//----- constructor 2
	Vertex2WithLineType(	Vector2& P, 
				LineType* lt , 
				Vector3* P3D_p = NULL , 
				int label_given = -1   ) 
	{ 
		position.x = P.x ;  position.y = P.y ;
		linetype = lt ;
		position3D_p = P3D_p ;
		label = label_given ;
	}

	//----- operator =
	void	operator=( const Vertex2WithLineType& rhs ) 
	{	
		position.x = (rhs.position).x ;
		position.y = (rhs.position).y ;
		linetype   = rhs.linetype ;
		position3D_p = rhs.position3D_p ;
		label = rhs.label ;
	}

}; // Vertex2WithLineType 



//-------------------------//
//----- class Polygon2D -----//
//-------------------------//

class Polygon2D {
 public:
	ColorType*	color ;

		//----- line types of edges
	LineType**	edgeType ;

		//----- number of vertices
	int		numVertex ;	

		//----- vertex position in world coordinate,
	Vector2*	vertex2D ;

		//----- object label
	int		objectLabel ;

		//----- parent Polygon3D
	Polygon3D*	parent3D_p ;

 public:
		//----- constructor 1
	Polygon2D ( void ) ;

		//----- constructor 2
	Polygon2D (	Vector2*	position     ,
			LineType**	linetype     ,
			int		num          , 
			ColorType*	color_p      ,
			int		object_label ,
			Polygon3D* 	parent_polygon3D_p ) ;

		//----- set data
	void set (	Vector2*	vertex2D_given  ,
			LineType**	edgeType_given  ,
			int		numVertex_given ,
			ColorType*	color_p         ,
			int		object_label    ,
			Polygon3D* 	parent_polygon3D_p );

		//----- set data
	void set (	List<Vertex2WithLineType>&	list_vlt ,
			ColorType*			color_p   ,
			int				object_label ,
			Polygon3D* 			parent_polygon3D_p );

		//----- destructor
	~Polygon2D () ;

		//----- clear all data members
	void clear(void);

		//----- copy
	//----- Polygon2D::opeartor = ()
	void	operator = ( const Polygon2D& rhs );
	void	operator = ( Polygon2D& rhs );

		//----- remove zero-length edge if any
		//..... Return: number of vertices in resultant polygon
	int	removeZeroLengthEdge( double epsilon = 1.0e-6 );

		//-----
	int	getObjectLabel() const { return objectLabel ; }
	int	getNumVertex  () const { return numVertex   ; }	
	Vector2*    getVertex2D () { return vertex2D   ; }	
	ColorType*  getColor    () { return color      ; }	
	LineType**  getEdgeType () { return edgeType   ; }

} ; // Polygon2D


//-------------------------------//
//----- type Polygon2D_p   -----//
//-------------------------------//
typedef Polygon2D*	Polygon2D_p ;



//-------------------------//
//----- class Polygon3D -----//
//-------------------------//

class Polygon3D {
 private:
	enum { DEFAULT_VERTEX_NUMBER = 4 } ;

 public:
		//----- integer label for general purpose
	int		label ;
	
		//----- object label
	int		objectLabel ;

		//----- facet label
	int		facetLabel ;
	int*		neighboringFacet ;
	int		numNeighboringFacet ;
	int		flag_convex ;

		//----- color
	ColorType	color ;

		//----- line types of edges
	LineType**	edgeType ;

		//----- number of vertices
	int		numVertex ;	

		//----- vertex position in world coordinate,
	Vector3**	vertex ;	
	Vector3*	vertex3D ;	// usually null

		//----- normal vector in world coordinate
	Vector3		normalVector;	

		//----- vertex position in 2D screen coordinate
	Vector2*	vertex2D ;

		//----- boxel ( in 3D screen coordinate )
	double		xmin, xmax ;
	double		ymin, ymax ;
	double		zmin, zmax ;
	double		umin, umax ;	// not used (initialization only )
	double		vmin, vmax ;	// not used (initialization only )

		//----- convex sub polyons
		//.....   Expressed in 2D screen coord ( = viewport coord).
	List<Polygon2D> subPolygonList ;

		//----- data for root polygon (as a facet of object)
	Vector3*	vertex3D_org  ;
	LineType**	edgeType_org  ;
	int		numVertex_org ;

 public:
		//----- constructor 
	Polygon3D( void );

		//----- desctuctor
	~Polygon3D(){ this->clear() ; }

		//----- clear dynamical memories
	void clear();

		//----- set all data members except for "label", "flag_convex", 
		//.....  and "subPolygonList"
		//.....  * "subPolygonList" is cleared.
		//.....  * If normal_vector_p == NULL, 
		//.....    member normalVector is automatically 
		//.....    calculated inside this function
		//.....  * if vertex3D_given != NULL, vertex3D[]'s are set 
		//.....    and vertex[]'s  are  reset to their address.
		//.....
		//.....  Return: error (0) or normal (1) 
		//.....
	int set(	Vector3**		vertex_given     , 
			LineType**		edgeType_given   ,
			int			numVertex_given  , 
			ColorType		color_given      , 
			Vector3*		normal_vector_p  ,
			Perspective*		camera_p         ,
			double			epsilon3d        , 
			int			object_label     ,
			int			facet_label      ,
			int*			neighboring_facet ,
			int			num_neighboring_facet_given , 
			Vector3*		vertex3D_given = NULL         ) ;

	int  set(	List<Vertex2WithLineType>&	list_vlt        ,
			ColorType			color_given      , 
			Vector3*			normal_vector_p  ,
			Perspective*			camera_p         ,
			double				epsilon3d        ,
			int				object_label     ,
			int				facet_label      ,
			int*				neighboring_facet ,
			int				num_neighboring_facet_given ,
			Vector3*			vertex3D_given = NULL          ) ;

		//----- own vertex position data by itself
	void	setVertex3D();

		//----- access functions
	void	setLabel( int label_given ) { label = label_given ; }
	int	        getLabel        () const { return label        ; }
	int		getObjectLabel  () const { return objectLabel  ; }
	int		getFacetLabel   () const { return facetLabel   ; }

	ColorType	getColor        () const { return color        ; }

	LineType**	getEdgeType     () const { return edgeType     ; }
	const LineType&	getEdgeType     ( int index ) { return *(edgeType[index]) ; }

	int	        getNumVertex    () const { return numVertex    ; }

	Vector3**       getVertex       () const { return vertex       ; }
	const Vector3&  getVertex       ( int index ) { return *(vertex[ index ]) ; }

	Vector3		getNormalVector () const { return normalVector ; }

	Vector2*        getVertex2D     () const { return vertex2D     ; }
	const Vector2&  getVertex2D     ( int index ) { return vertex2D[ index ] ; }


		//----- convexity
	void	setConvexity( int on_off ) 
		{  
			if( on_off == 0 ) { flag_convex = 0 ;} else { flag_convex = 1 ; } 
		}
	int	getConvexity() const  { return flag_convex ; }

		//----- substitution
	void	operator = ( const Polygon3D& rhs ) ;

		//----- set data members vertex2D, xmin, xmax, ymin, ymax, zmin, zmax
		//..... Return: error (0) or normal (1)
	int	setImage( Perspective* camera_p, double epsilon = 1.0e-6 );

		//----- set subPolygonList
	void	setSubPolygonList();

		//----- see if given polygon is neigboring one
	int	isNeighborOf( const Polygon3D& polygon3d ) ;


		//----- removeZeroLengthImageEdge()
	int	removeZeroLengthImageEdge( double epsilon2d , double epsilon3d , Perspective* camera_p ) ;


		//----- from2DTo3D()
		//.....  Return: normal (1) or error (0)
	int	from2DTo3D( const Polygon2D& polygon2d , Hid* hid_p, double epsilon3d );

		//----- set vertex3D_org and numVertex_org 
		//..... (data for original polygon as a facet of object)
	void	setOriginalData( Vector3*   vertex3D_given , LineType** edgetype_given, int num_vertex_given );
	void	setOriginalData( Vector3**  vertex_given   , LineType** edgetype_given, int num_vertex_given );
	int	isOriginalDataSet() const ;

} ; // Polygon3D


//-------------------------------//
//----- type Polygon3D_p   -----//
//-------------------------------//
typedef Polygon3D*	Polygon3D_p ;


//------------------------//
//----- class Hid    -----//
//------------------------//

class Hid {
	
private:
		//---- const
	enum { NOT_HIDDEN2 = -1 , NOT_HIDDEN = 0 , HIDDEN = 1 };
	enum { INDEPENDENT = 0 } ;

		//----- 3D data
	List<Polygon3D>	polygon3DList ;
	List<Polygon3D>	convexPolygon3DList ;
 	int			numPolygon3D ;
	Polygon3D*		polygon3d ;
	List<Vector3>		auxiliaryVertexPosition ;

		//----- 2D data
 	int			numPolygon2D ;
	Polygon2D*		polygon2d ;

		//----- linetype for perforation line
		//.....  Used in decomposition into convex polygon
	LineType	perforationLineType ;

		//----- camera
	Perspective*	camera_p ;
	Vector3		eyePosition ;

		//----- polygon label
	int	polygon_counter ;

		//-----
	List<Polygon2D>	polygon2DList;

		//----- bounding diamonds for parts of polygon2d[]
	int		boundingDiamondInterval  ;
	Diamond*	boundingDiamondArray     ;

		//----- 
	int		beginning_object_label_per_session    ;
	int		num_non_transparent_facet_per_session ;

 public:

		//----- Initialize parameters used in incremental hidden-surface removal
	void	initializeIncrementalHid();

	int	getBeginningObjectLabelPerSession()   const { return beginning_object_label_per_session    ;}
	int	getNumNonTransparentFacetPerSession() const { return num_non_transparent_facet_per_session ;}

	void	setBeginningObjectLabelPerSession  ( int label ) { beginning_object_label_per_session    = label ;}
	void	setNumNonTransparentFacetPerSession( int num   ) { num_non_transparent_facet_per_session = num   ;}


		//----- see if polygon a is hidden by polygon b
		//.....  RETURN:
		//.....  		  NOT_HIDDEN    = 0	if  P is not hidden by Q
		//.....  		  NOT_HIDDEN2   = -1	if  P is not hidden by Q,
		//.....	                                        and P.zmax < Q.zmin
		//.....			  HIDDEN        =  1	if  P may be hidden by Q
	int	isHidden( Polygon2D* sub_a, Polygon3D_p a , Polygon3D_p b , int* relation_2d , 
			  double epsilon3d = 1.0e-6 ,  double epsilon2d = 1.0e-6 );

		//----- max min test for 8 direction
		//.....  Return
		//.....		0:  if a is independent of b
		//.....		    on screen.
		//.....		1:  else
	int	maxMinTest( Polygon3D*   a , Polygon3D*   b  , double epsilon2d = 1.0e-6 ) ;
	int	maxMinTest( Polygon3D_p* a , Polygon3D_p* b  , double epsilon2d = 1.0e-6 ) ;

		//----- Polygon3D::depthOverlapTest()
		//.....  Depth overlapping test
		//.....  Return
		//.....		 1: if (*a)->zmax < (*b)->zmin.
		//.....		-1: if (*b)->zmax < (*a)->zmin.
		//.....		 0: else (ovrlaps in z direction)
		//..... (zmax, zmin are eye coordinate.)
	int	depthOverlapTest( Polygon3D_p* a , Polygon3D_p* b  , double epsilon3d = 1.0e-6 ) ;

		//------ relative position of polygons  
		//..... Note: Available only if polygon( vertex3d, num_vertex) is 
		//......included by or equal to *polygon_p
		//..... 
		//.....  Return:  Tool3D::POSITIVE_SIDE, Tool3D::NEGATIVE_SIDE, Tool3D::UNKNOWN 
	int	includedPolygonVsPlaneTest(	Vector3* vertex3d_a,  Vector2* vertex2d_a , 
						const Vector3& noraml_a , int num_vertex_a , 
						const Vector3& normal_b, const Vector3& P_on_b , 
						double epsilon3d = 1.0e-6 );

		//-----  included vertex points vs plane test
		//.....  See if vertices of polygon a, which are included by polygon b is
		//.....  in positive or negative side of plane b.
		//.....
		//.....  Return:  Tool3D::POSITIVE_SIDE, Tool3D::NEGATIVE_SIDE, Tool3D::UNKNOWN 
		//.....
		//.....  Argument: 
		//.....		vertex3d_a:	3-dim vertex positions of polygon a
		//.....		vertex2d_a:	2-dim vertex positions of polygon a on screen
		//.....		normal_a:	normal vector of plane a including polygon a
		//.....		num_vertex_a:	number of vertices of polygon a
		//.....		normal_b:	normal vector of plane b including polygon b
		//.....		P_on_b  :	an arbitrary position on plane b
		//.....		vertex2d_b:	2-dim vertex positions of polygon b on screen
		//.....		num_vertex_a:	number of vertices of polygon b
	int  crossedPolygonVsPlaneTest(	Vector3*       vertex3d_a,  Vector2*       vertex2d_a , 
					const Vector3& normal_a ,   int            num_vertex_a , 
					const Vector3& normal_b ,   const Vector3& P_on_b, 
					Vector2*       vertex2d_b,  int            num_vertex_b, 
					double         epsilon3d = 1.0e-6 ,  
					double         epsilon = 1.0e-6                          ) ;

		//----- polygon-polygon test
		//.....  -1	if polygon a is further from eye
		//.....   1	if polygon b  is further from eye
		//.....   0	undecided
		//.....   -2	images do not intersect on screen
	int	polygonPolygonTest(	Vector3* vertex3d_a, Vector2* vertex2d_a, int num_vertex_a, 
					Polygon3D* b , 
					int max_intersection, 
					double epsilon3d = 1.0e-6 , double epsilon = 1.0e-6 );


		//----- subtraction and devision to convex subpolygons: 
		//.....  a_polygon - b_polygon
	int   	hide_A(		const Polygon2D&	a_polygon, 
				const Polygon2D&	b_polygon, 
				int		b_polygon_label  ,
				List<Polygon2D>*  output_list_p   , 
				List<int>*        list_label_p  , 
				double epsilon = 1.0e-6               );


 public:
			//----- constructor 1
	Hid( Perspective*  camera_p_given = NULL );

		//----- destructor 1
	~Hid() { this->clear() ; }

		//----- clear all
	void	clear() ;

		//----- clear polygon data
		//.....  camera data etc are not cleared
	void	clearPolygon();

		//----- clear polygon lists
	void	clearPolygon2DList()               { polygon2DList.clear();          }
	void	clearPolygon3DList()               { polygon3DList.clear();          }
	void	clearConvexPolygon3DList()         { convexPolygon3DList.clear();    }
	void	clearAuxiliaryVertexPositionList() { auxiliaryVertexPosition.clear(); }

		//----- clear polygon arrays
	void	clearPolygon2DArray() { delete [] polygon2d ; polygon2d = NULL ; numPolygon2D = 0 ; }
	void	clearPolygon3DArray() { delete [] polygon3d ; polygon3d = NULL ; numPolygon3D = 0 ; }

			//----- manipulation of polygon3DList
	int	addItem_Polygon3DList  ( Polygon3D& poly3d , int clipping_mode = 1 ) ;
					// mode_clipping =  0 ( no cllipping)
					//               =  1 ( clipping) 
					//               =  2 ( landscape clipping) 					

	int	getItem_Polygon3DList  ( Polygon3D& poly3d ) ;
	int	getNItem_Polygon3DList () ;
	void	toTail_Polygon3DList   () ;
	int	upward_Polygon3DList   () ;

		//----- convert list to array
		//.....  Return:   number of substituted items to Array.
		//.....  Note:  After this function is called, polygon3DList is cleared.
	int setPolygon3DToArray( List<Polygon3D>& polygon3d_list_given ) ;

		//----- install polygon3D array "polygon3d"
	void	install(){ return  ; }

		//----- set and get camera 
	void		setCamera( Perspective* camera_p_given ) ;
	Perspective*	getCamera( ) { return camera_p ; }

		//----- get  eye position
	Vector3		getEyePosition() { return eyePosition ; }

		//----- get number of items in array polygon3d
	int	getNumPolygon3D(){ return numPolygon3D ;}

		//----- get label-th item in the array "polygon3d"
	Polygon3D&	getPolygon3D( int label ) ;

		//----- get number of items in array polygon2d
	int		getNumPolygon2D(){ return numPolygon2D ;}

		//----- get label-th item in the array "polygon2d"
	Polygon2D&	getPolygon2D( int label ) ;

		//----- equivalent to getPolygon2D()
	Polygon2D&	operator [] ( int label ) ;


		//----- point: viewport_coord ---> world coordinate inside given plane
		//..... Return: 0 if error, 1 if normal
		//..... Argument:
		//.....        point2d: viewport coord  ( 2-dim )
		//.....        normal : normal vector of plane,
		//.....        P_on   : a point on plane
		//.....        point3d_p : output
	int	point2DToPoint3D (	const Vector2&	point2d, 
					const Vector3&	normal ,
					const Vector3&	P_on ,
					Vector3*	point3d_p ,
					double		epsilon3d = 1.0e-6 );

		//----- get visible part of subpolygons and set them
		//.....  to array "polygon2d"
	void	extractVisiblePart(	int		beginning_object_label ,    
					int		ending_object_label       , 
					double epsilon3d = 1.0e-6 , 
					double epsilon = 1.0e-6 );

		//----- perform hidden part elimination
		//.....  Return:   numPolygon2D
		//.....  Note:  After this function is called, polygon3DList is cleared.
	void	doHiddenPartElimination(	int		beginning_object_label       , 
						int		ending_object_label       , 
						double 		epsilon3d = 1.0e-6 ,
						double		epsilon = 1.0e-6 );

	void	doNotHiddenPartElimination(	double 		epsilon3d = 1.0e-6 ) ;


		//----- convert polygon3DList to Polygon3DArray() 
		//..... (1) Sorted 
		//..... (2) Convexated
		//..... (2) polygon3DList is deleted
	void	movePolygon3DListToArray( double epsilon3d , double epsilon2d );

		//----- move polygonn2DList to polygon2d  array
		//.....  Polygon2DList is cleared.
		//.....  Old data stored in polygon2d array is abandoned.
	int	movePolygon2DListToArray(void) ;

		//----- Hid::removeZeroLengthEdge()
		//..... Return: number of vertices in resultant polygon
	int	removeZeroLengthEdge(	List<Vertex2WithLineType>& list_vlt , 
						double epsilon = 1.0e-6 );

		//----- get first convave vertex label
		//..... Return: vertex label ( if found     )
		//.....         -1	     ( if not found )
	int	getFirstConcaveVertex(	List<Vertex2WithLineType>& list_vlt ,
						double epsilon = 1.0e-6                ) ;

		//----- convexate Polygon3D
		//..... Return : num obtained convex polygon
		//..... WARNING:  After the former function is called, 
		//.....        input_Polygon3D_list is
	int	convexate(	List<Polygon3D>&  input_Polygon3D_list ,
				List<Polygon3D>*  output_Polygon3D_list_p ,
				double epsilon3d = 1.0e-6 , 
				double epsilon2d = 1.0e-6 );
	int	convexate(	Polygon3D& polygon3D_input , 
				List<Polygon3D>*  output_Polygon3D_list_p ,
				double epsilon3d = 1.0e-6 , 
				double epsilon2d = 1.0e-6 ) ;
	void	resetRemovedVertexForConvexation(	const Polygon3D&		polygon3D_input , 
							List<Vertex2WithLineType>&	list_vlt       ,
							double				epsilon2d       ,
							double				epsilon_nodim     ) ;

		//----- Hid::halfLineVsPolygon()
		//.....  get intersection "X" between halfline AB and 
		//.....  polygon list_vlt.
		//.....  If plural intersections exist, the one with 
		//.....  mininum line parameter t_AB of halfline AB
		//.....  is returned.
		//..... Arguments:
		//.....  A: starting point of halfline AB
		//.....  B: another point on  halfline AB
		//.....  list_vlt: polygon
		//.....  starting_label: from here searching starts
		//.....  max_search: searching is done until 
		//.....              starting_label + max_search -1 
		//....	 *X_p:    intersection if exists, 
		//.....  *t_AB_p: line paramater of intersection on halfline AB
		//.....         ( t = 0 for A and t = 1 for B )
		//.....  *t_edge: line paramater of intersection on edge
		//.....  *edge_label_p: integer label of edge where intersection occurs,
		//..... Return:	 
		//.....  1  if halfline PQ intersects with polygon
		//.....  0  if halfline PQ does not intersects with polygon
	int	halfLineVsPolygon(	Vector2&  A,      Vector2&  B           ,
				List<Vertex2WithLineType>& list_vlt   ,
				int  starting_label, int     max_search , 
				Vector2*  X_p                           ,  
				double*   t_AB_p                        ,
				double*   t_edge_p                      ,
				int*      edge_label_p                  ,
				double epsilon = 1.0e-6                    );

	int	halfLineVsPolygonForConvexation(	Vector2&  A,      Vector2&  B  , Vector2& C   ,
				List<Vertex2WithLineType>& list_vlt   ,
				int  starting_label, int     max_search , 
				Vector2*  X_p                           ,  
				double*   t_AB_p                        ,
				double*   t_edge_p                      ,
				int*      edge_label_p                  ,
				double epsilon = 1.0e-6                    );

		//----- get number of vertices of given type of orientation
	int	getNumVertexOfGivenOrt	(	List<Vertex2WithLineType>& list_vlt ,
					int	ort_type ,
					double epsilon = 1.0e-6                      );
	int	getNumOrtZeroVertex	(	List<Vertex2WithLineType>& list_vlt ,
					double epsilon = 1.0e-6                      );
	int	getNumConcaveVertex	(	List<Vertex2WithLineType>& list_vlt ,
					double epsilon = 1.0e-6                      );
	int	getNumConvexVertex	(	List<Vertex2WithLineType>& list_vlt ,
					double epsilon = 1.0e-6                      );

		//----- write result of hidden line/surface removal to file
	int  	writePolygonOut(	char*			output_file   ,
					char*			begin_facet   ,
					char			terminal_char ,
					const Parameter& 	parameter	) ;

	int  	writePolygonOut(	const  Polygon2D&	poly2d        ,
					ofstream&		vertex_out    ,
					ofstream&		facet_out    ,
					int&			vertex_label  ,			
					char			terminal_char ,
					double			epsilon3d      ); 

		//----- recoverOriginalPolygon3D()
	void	recoverOriginalPolygon3D( Polygon3D& polygon3D_input,  Polygon3D* polygon3D_output_p , double epsilon3d );

		//----- total bounding diamond of polygon2d[]
	int	getBoundingDiamond(	double*		xmin ,
					double*		xmax ,
					double*		ymin ,
					double*		ymax ,
					double*		umin ,
					double*		umax ,
					double*		vmin ,
					double*		vmax  );

		//----- bounding diamonds for parts of polygon2d[]
	int	getBoundingDiamondInterval() const { return boundingDiamondInterval  ;}
	void	clearBoundingDiamondArray() 
		{
			delete [] boundingDiamondArray ; 
			boundingDiamondArray = NULL ; 
			boundingDiamondInterval = 0 ; 
		}
	int	setBoundingDiamondArray( int diamond_interval  ) ;

		//----- multiple max-min test
		//.....  RETURN: 
		//.....   0: Given area is independent of 
		//.....      polygons from polygon3d[index] 
		//.....      to polygon3d[index_end].
		//.....      The index_end is returned to the last argument.
		//.....   1: Else or unknown.
	int	multiMaxMinTest(	double xmin_given , double xmax_given ,
					double ymin_given , double ymax_given ,
					double umin_given , double umax_given ,
					double vmin_given , double vmax_given ,
					int index_given   , double epsilon2d, 
					int* index_end_p     ) ;

} ; // Hid



#endif
////////////////////////////// end of hid.h
