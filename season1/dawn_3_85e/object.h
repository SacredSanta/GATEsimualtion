////////////////////////////////////////////////////////////
///// object.h                                         /////
/////                                                  /////
///// This header file includes specification part of  /////
///// the following classes:                           /////
///// 	class Object                                   /////
/////   class Facet                                    /////
///// 	class Edge                                     /////
///// 	class Vertex                                   /////
////////////////////////////////////////////////////////////

#if !defined __OBJECT_H
#define __OBJECT_H

/////////////////////////////
///////  include files  /////
/////////////////////////////
#include <iostream.h>
#include <stdlib.h>	
#include <string.h>
#include <math.h>	
#include "attribute.h"
#include "box.h"
#include "list.h"
#include "vector.h"
#include "perspective.h"
#include "tool3d.h"
#include "linetype.h"
#include "colortype.h"
#include "shading.h"
#include "hid.h" 
#include "hidline.h" 
#include "postscript.h" 

////////////////////////////
///////  constants     /////
////////////////////////////

		//----- initial dummy value of belonging attributes
	const	int	NO_ITEM   = -1 ;

		//----- initial dummy value of label
	const	int	NO_LABEL  = -1 ;

		//----- default line types used to draw objects
	const	int	REAL 		= 0 ;	
	const	int	OUTLINE		= 1 ;
	const	int	AUXILIARY	= 2 ;
	const	int	AUXILIARY_H	= 3 ;
	const	int	CRYSTAL 	= 4 ;
	const	int	NUM_LINETYPE 	= 5 ;

////////////////////////////
///////  proto type    /////
////////////////////////////
class	Object ;

////////////////////////////
///////  class Vertex  /////
////////////////////////////
//----- Specification part of class Vertex
class  Vertex {

		//----- constants
	enum { ERROR, NORMAL } ;

		//----- Label
	int 	label ;

		//----- position of this edge ( in body coordinate )
	Vector3	position ;

 public:
		//----- constructor
	Vertex ( int label_given  , Vector3 postion_given              ) ;
	Vertex ( int label_given                                       ) ;
	Vertex ( void ) ;

		//----- get private data of class vertex
	int	getLabel()           const { return label ;}
	Vector3	getPosition()        const { return position ; }
	
		//----- set private data of class Vertex
	void	setLabel( int   label_ )         { label = label_ ; }
	void	setPosition( Vector3 position_ ) { position = position_ ; }

		//----- substitution
	int 	operator = ( const Vertex& rhs ) ;

} ;	// Vertex



/////////////////////////////
///////  class Edge    /////
/////////////////////////////
//----- Specification part of class Edge
class Edge {
		//----- constants
	enum { ERROR, NORMAL } ;
	enum { INVISIBLE = 0 , VISIBLE = 1 } ; 

		//----- Label
	int 	label ;

		//----- Connected vertices
	int 	vertex	   ;	// v
	int 	prevVertex ;	// cweh.vertex
				//  ... Not a part of half-edge
				//  ... structure.
				//  ... Added for convenience.

		//----- Connected edges
	int 	nextEdge  ;		// ccweh
	int 	prevEdge  ;		// cweh
	int 	otherEdge ;		// oeh

		//----- Connected Faces
	int 	rightFacet ;		// f
	int 	leftFacet  ;		// oeh.face
					//  ... not a part of half-edge
					//  ... structure
					//  ... Added for convenience.

		//----- working variables for drawing
	int		visibility  ;

		//---------- linetype pointer
	LineType*	lineType_p  ;

		//---------- working flag
	int		flag ;

 public:
		//----- constructor
	Edge( int label_          = NO_LABEL,
		int vertex_ 	= NO_ITEM , int prevVertex_ 	= NO_ITEM ,
		int nextEdge_	= NO_ITEM , int prevEdge_   	= NO_ITEM ,
		int otherEdge_	= NO_ITEM , int rightFacet_ 	= NO_ITEM ,
		int leftFacet_	= NO_ITEM , 
		int visibility_ = VISIBLE ,
		LineType* linetype_p = NULL                                 );

	~Edge(){;}

		//----- draw edge (PS)
	int	draw_PS (	Object&		objItem   , 
				Perspective&	camera    ,
				PostScript&	ps        ,
		                const LineType& linetype  , 
				double		epsilon3d = 1.0e-6 ) ;

		//----- draw edge (PS)
	int	draw_landscape_PS (	Object&		objItem   , 
					Perspective&	camera    ,
					PostScript&	ps        ,
			                const LineType&	linetype  , 
					double		epsilon3d = 1.0e-6 ) ;

		//----- get private data of class Edge
	int	getLabel()	  const { return label          ; }
	int 	getVertex()	  const { return vertex		; }
	int 	getPrevVertex()	  const { return prevVertex	; }
	int 	getNextEdge()	  const { return nextEdge	; }
	int 	getPrevEdge()	  const { return prevEdge	; }
	int 	getOtherEdge()	  const { return otherEdge	; }
	int 	getRightFacet()	  const { return rightFacet	; }
	int 	getLeftFacet()	  const { return leftFacet	; }
	int	getVisibility()   const { return visibility	; }
	const LineType*  getLineType()  const { return lineType_p	; }
	int 	getFlag()	  const { return flag	        ; }	

		//----- set private data of class Edge
		//---------- label and name
	void	setLabel( int   label_ )        { label = label_ ; }

		//---------- vertices
	void	setVertex( int vertex_ )	{ vertex = vertex_ ;}
	void	setPrevVertex( int vertex_ )	{ prevVertex = vertex_ ;}

		//---------- edges
	void	setNextEdge( int edge_ )	{ nextEdge = edge_ ;}
	void	setPrevEdge( int edge_ )	{ prevEdge = edge_ ;}
	void	setOtherEdge( int edge_ )	{ otherEdge = edge_ ; }

		//---------- facets
	void	setRightFacet( int facet_ )	{ rightFacet = facet_ ; }
	void	setLeftFacet( int facet_ )	{ leftFacet  = facet_ ; }

		//---------- working variables
	void	setLineType( LineType* linetype_p ){ lineType_p = linetype_p ;}
	void	setFlag( int status )		{ flag = status ; }

		//----- substitution
	int 	operator = ( const Edge& rhs ) ;

} ;	// Edge


/////////////////////////////
///////  class Facet    /////
/////////////////////////////
//----- Specification part of class Facet
class Facet {
		//----- constants
	enum { ERROR, NORMAL } ;
	enum { NAME_LENGTH = 16 } ;
	enum { INVISIBLE , VISIBLE } ;

		//----- Label
	int 	label ;

		//----- list of edges
	List<int> edgeList ;

		//----- color
	int		flag_color_set ; 
		//	enum { OBJECT_COLOR, SOFT_COLOR , HARD_COLOR } ;
	ColorType	color ; // used if HARD_COLOR

		//----- Attributes
	Attribute	attribute ;

		//----- working datas for drawing

		//---------- normal vector in world coordinate
	Vector3	normalVector ; // world coordinate

		//---------- first vertex position in world coordinate
	Vector3	oneVertexPosition ; // world coordinate

		//---------- array equivalent to edge list
	int*	edgeArray ;

		//---------- working flag
	int	flag ;

 public:
		//----- constants
	enum { OBJECT_COLOR, SOFT_COLOR , HARD_COLOR } ;

		//----- constructor
	Facet ( int label_ = NO_LABEL ) ;

		//----- destructor
	~Facet() { clear() ; }

		//----- clear edge list-list
	void	clear() ;

		//----- color
	void		setColorMode ( int mode ) ;
	void		setColor ( const ColorType& color_given ) { color = color_given ; }
	void		setColor ( const ColorType& color_given , int mode ) 
			{ 
				color = color_given ; 
				flag_color_set = mode ; 
				if( flag_color_set <= OBJECT_COLOR || flag_color_set > HARD_COLOR ) 
				{
					flag_color_set = SOFT_COLOR ;
				}
			}
	ColorType	getColor() const { return color ; }
	int		isColorSet() const { return flag_color_set ; }


		//----- set and get attribute
	void		setAttribute( const Attribute& att ) { attribute.set  ( att ) ; }
	Attribute	getAttribute()	const {    return attribute         ; }

		//----- get private data of class Facet
	int		getLabel()	       const { return label ; }
	Vector3		getNormalVector()      const { return normalVector ; }
	Vector3		getOneVertexPosition() const { return oneVertexPosition ; }
			
	int		getNumEdgeInList()    const { return edgeList.getNItem() ; }

	int		getEdgeLabel( int n ) { return edgeList[ n ] ; }
				// return integer label of n_th edge
	int		getEdgeItem( int edge_label , int& edge_item ) const ;
	int		getFlag()              const { return flag      ; }	


		//----- set private data of class Facet
	void	setLabel( int   label_ ) { label = label_ ; }
	int 	setNormalVector( Object* objItem ) ; // return: ERROR or NORMAL
	void	setFlag( int status ){ flag = status ; }	

		//----- add edge-label to list
		//..... Return 1  if successfully added.
		//.....        0  if failed to add
	int 	addEdge ( int edge_ )
			{ return edgeList.addItem ( edge_ ) ; }

		//----- set facet, edge and vertex lists
		//.....  Return: NORMAL = 1 if success
		//.....          ERROE  = 0 if error
	int 	setEdgeArray();

		//----- substitution
	int 	operator = ( const Facet& rhs ) ;

} ; // Facet


/////////////////////////////
///////  class Object   /////
/////////////////////////////
class Object {

		//----- constants
	enum { ERROR, NORMAL } ;
	enum { NAME_LENGTH = 32 } ;
	enum { DEFAULT = 0 , NOT_DEFAULT = 1 };
	enum { NOT_DRAWN , DRAWN } ;
	enum { INVISIBLE , VISIBLE } ;

		//----- Label
	int 	label ;

		//----- Name
	char	name[ NAME_LENGTH ] ;

		//----- attribute reset by .att file
	int		flag_att_file_exist ;
	int		flag_convex ;
	
		//----- Attributes
	Attribute	attribute ;

		//----- Temporary lists of facets, edges, and vertices composing object
		//.....  Items are deleted after they are copied to  ...Array's.
	List<Facet>	facetList	;
	List<Edge> 	edgeList	;
	List<Vertex>	vertexList ;

		//----- number of items in ...Array
	int		numFacet ;
	int		numEdge ;
	int		numVertex ;

		//----- arrays equivalent to the above lists
	Facet*		facetArray	;
	Edge*		edgeArray	;
	Vertex*		vertexArray	;

		//----- body coordinate ( difined with world coordinate)
	Vector3		origin;		// origin
	Vector3		e1 ;		// base vector for x-direction
	Vector3		e2 ;		// base vector for y-direction
	Vector3		e3 ;		// base vector for z-direction

 public:
		//----- vertex positions in world coordinate (temporary data)
	Vector3*	vertexPosition;

		//----- bounding box in 3D
		//...... set by setBoundingBox().
		//...... setBoundingBox() is called in setWorldCoordinateData()
	Vector3		boundingBox_min , boundingBox_max ;

		//----- Five LineType of edge
		//..... REAL, OUTLINE, AUXILIARY, AUXILIARY_H, CRYSTAL
		//.....  0     1        2          3            4     
	LineType	lineType[ NUM_LINETYPE ] ;

 public:
		//----- constructor
	Object (	int label_ = NO_LABEL , char* name_ = "no_name" );

		//----- destructor
	~Object() { clear() ; }

		//----- clear 
	void	clear() ;
	
		//----- clear lists
	void	clearList() ;

		//----- clear arrays 
		//.....   Arrays  facetArray, edgeArray, vertexArray 
		//.....   are cleared.
		//.....   Note:  vertexPosition is not cleared.
	void	clearArray_VertexEdgeFacet();

		//----- clear all arrays 
		//.....   Arrays  facetArray, edgeArray, vertexArray 
		//.....   and vertexPosition are cleared.
	void	clearArray() ;

		//----- move origin to given position
	void	moveTo( Vector3& new_position ) 
		{
			setOrigin( new_position ) ;
			setWorldCoordData();
		}

		//----- draw object with wireframe (PS)
	void	drawWireFrame_PS (	Perspective&	camera ,
					PostScript&	ps     ,
				        const LineType&	linetype ,
					double		threshold_angle ,
					double		epsilon3d ,
					int		clipping_mode ) ;

		//----- draw object with wireframe (PS)
	void	drawWireFrame_landscape_PS (	Perspective&	camera ,
						PostScript&	ps     ,
					        const LineType&	linetype ,
						double		threshold_angle ,
						double		epsilon3d, 
						int		clipping_mode ) ;

		//-----  pass polygons ( facets) to Hid class
		//.....  The following lines are automalitally selected intelligently for each edge:
		//.....		(1) REAL edge
		//.....		(2) AUXILIARY edge, 
		//.....		(3) OUTLINE
		//....
		//.....  Argument:
		//.....		threshold_angle:
		//.....			if an angle between normal vector of
		//.....                 two neighboring facets 
		//.....			are greater than threshold_angle, 
		//.....			the edge between them is regarded as auxiliary.
	void  passFacetToHid (	Perspective&	camera                         ,
				Shading&	shading                        ,
				Hid*		hid_output_p                   , 
				HidLine*	hidline_p                      , 
				double		threshold_angle                ,
				double		epsilon3d	               ,     
				int		double_edge_draw_mode          ,
				int		backface_culling_mode          ,
				int		shading_mode                   ,
				int		clipping_mode                     );
	void  passEdgeToHidLine (	Perspective&	camera           ,
					HidLine*	hidline_p        , 
					double		threshold_angle  ,
					double		epsilon3d	 ,     
					int		double_edge_draw_mode ,
					int		clipping_mode    ,
					int		back_edge_dashed       );

		//----- get label and name of class Object
	int 		getLabel()	const { return label	; }
	int		getConvexity()  const { return flag_convex ; }
	const char*	getName()	const { return name	; }
	LineType 	getLineType( int type ) const ;

		//----- get definition of body coordinate
	Vector3	getOrigin()	const { return origin ; }
	Vector3	getE1() 	const { return e1 ; }
	Vector3	getE2() 	const { return e2 ; }
	Vector3	getE3() 	const { return e3 ; }

		//----- get item with given label
		//..... Return : 1 if proper item is found in the list
		//.....          0 if not found
	int	getVertexItem ( int vertex_label , Vertex& vertex_item ) const ;
	int	getEdgeItem   ( int edge_label   , Edge&   edge_item )   const ;
	int	getFacetItem  ( int facet_label  , Facet& facet_item )   const ;
	const Facet&	getFacetItem  ( int facet_label ) { return facetArray[ facet_label ] ; }
	int	getFacet      ( int		facet_label  , 
				Vector3_p*	vertex_position_p , 
				Vector3* normal_vector_p )               const ;
	int	getNumEdgeOfFacet( int index )                           const ;

		//----- get number of facets, edges or vertices in ...List's
	int 	getNumFacetInList () const { return facetList.getNItem()  ; }
	int 	getNumEdgeInList  () const { return edgeList.getNItem()   ; }
	int 	getNumVertexInList() const { return vertexList.getNItem() ; }

		//----- get number of facets, edges or vertices in ...Array's
	int 	getNumFacet () const	{ return  numFacet  ; }
	int 	getNumEdge  () const	{ return  numEdge   ; }
	int 	getNumVertex() const	{ return  numVertex ; }

		//----- get bounding box 
	Vector3		getBoundingBoxMin() const { return boundingBox_min  ; }
	Vector3		getBoundingBoxMax() const { return boundingBox_max  ; }
	const Vector3*	getBoundingBoxMinAddress() const ; // return NULL if not calc yet
	const Vector3*	getBoundingBoxMaxAddress() const ; // return NULL if not calc yet

		//----- get attributes
	Attribute	getAttribute()	const {    return attribute         ; }
	ColorType	getKd()		const {    return attribute.getKd() ; }
	ColorType	getKs()		const {    return attribute.getKs() ; }
	int		getPhongPower()	const {    return attribute.getPhongPower() ; }
	int		getTransparency () const { return attribute.getTransparency() ; }
	int		getVisibility()	const {    return attribute.getVisibility() ; }

		//----- set private data of class Object
	void	setLabel  ( int label_given  )  { label = label_given ;}
	void	setConvexity( int on_off ) 
		{  
			if( on_off == 0 ) { flag_convex = 0 ;} else { flag_convex = 1 ; } 
		}

	void	setName   ( const char* name_given ) ;
	void	setOrigin ( Vector3 origin_given ) { origin = origin_given ; }
	void	setBaseVector (Vector3 e1_given , Vector3 e2_given ) ;
	int 	setWorldCoordData();
	void	setLineType ( int type , LineType& linetype );
	void	setDefaultLineType ();
	void	setBoundingBox() ;

		//----- set otherEdge member to each edge
	void	setOtherEdgeAndRightFacet();

		//----- set facet, edge and vertex lists
		//.....  Return: NORMAL = 1 if success
		//.....          ERROE  = 0 if error
	int	setFacetArray();
	int	setEdgeArray();
	int	setVertexArray();
	int	setAllArray();

		//----- set attributes
	void    setAttribute( const Attribute& att ) { attribute.set  ( att    ) ; }
	void    setKd( const ColorType& kd_given )   { attribute.setKd(kd_given) ; }
	void    setKs( const ColorType& ks_given )   { attribute.setKs(ks_given) ; }
	void	setPhongPower   ( int power ) 
		{
			if( power >= 0 ) { 
				attribute.setPhongPower(power) ;
			} else { 
				attribute.setPhongPower(0);
			} 
		}
	void	setTransparency ( int on_off )  { attribute.setTransparency (on_off); }
	void	setVisibility   ( int on_off )  { attribute.setVisibility   (on_off); }

		//----- install data to half-edge structure
		//.....  (1) setOtherEdgeAndRightFacet() ; 
		//.....		(only if mode_winged_edge is true, i.e. non zero )
		//.....  (2) setAllArray() ;
		//.....  (3) setWorldCoordData();
	int	install( int mode_winged_edge = 1 ) ;

		//----- add element of list
	int	addFacetItem ( const Facet&  facetItem )
		{ return facetList.addItem ( facetItem ) ; }
	int	addEdgeItem  ( const Edge&   edgeItem  )
		{ return edgeList.addItem ( edgeItem ) ;}
	int	addVertexItem( const Vertex& vertexItem)
		{ return vertexList.addItem( vertexItem ) ;}

		//----- substitution
	int 	operator = ( const Object& rhs ) ;

		//----- comparison 
	int 	operator == ( const Object& rhs ) 
		{ 
			return ( !strcmp( name, rhs.getName() ) ) ;
		}

		//----- coordinate transformation:
		//.....  body coordinate --> world coordinate
	void	bodyToWorld( const Vector3 body_coord, Vector3& world_coord );

		//----- coordinate transformation:
		//.....  world coordinate --> body coordinate
	void	worldToBody( const Vector3 world_coord, Vector3& body_coord );

		//----- get & set flag_att_file_exist
	int	isAttFileExist() const { return flag_att_file_exist ;}
	void	setFlagAttFileExist( int flag_given ) 
		{
			if( flag_given ) {
				flag_att_file_exist = 1 ;
			} else {
				flag_att_file_exist = 0 ;
			}

		}

		//----- see if bounding box of this object is
		//..... clipped out of the screen
	int	isBoundingBoxClippedOut(	Perspective&	camera,
						double		epsilon3d,        
						int		clipping_mode ) ;     


} ;	// Object

//////////////////////////////////////////////////
#endif
////////////////////////////////// end of object.
