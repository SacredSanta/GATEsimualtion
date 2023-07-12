//////////////////////////
///// brepformat.h  //////
//////////////////////////

#ifndef  BREPFORMAT_H
#define  BREPFORMAT_H


	//----- buffer sizes
const	int	DEFAULT_LINEBUF    = 4000 ;
const	int	DEFAULT_VERTEX_MAX = 1000 ;

	//----- for header comment
const	char	FORMAT_VERSION_TITLE[]		= "%dawn data format version-";
const	int	FORMAT_VERSION_TITLE_LENGTH	= 26 ;
const	char	FORMAT_VERSION[]		= "02" ;
const	char	FR_BREP_HEADER[] = "%dawn data format version-02" ;

	//----- for ObjectOrigin block
const	char	BEGIN_OBJECT_ORIGIN[]    = "ObjectOrigin:" ;
const	char	END_OBJECT_ORIGIN[]      = "end_ObjectOrigin" ;

	//----- for BoundingBox block
const	char	BEGIN_BOUNDING_BOX[]  = "BoundingBox:" ;
const	char	END_BOUNDING_BOX[]    = "end_BoundingBox" ;

	//----- for Vertex block
const	char	BEGIN_VERTEX[]      = "Vertex:" ;
const	char	END_VERTEX[]        = "end_Vertex" ;
const	char	BEGIN_VERTEX2[]     = "BeginVertex" ;
const	char	END_VERTEX2[]       = "EndVertex" ;

	//----- for Attribute block
const	char	BEGIN_ATTRIBUTE[]      = "Attribute:" ;
const	char	END_ATTRIBUTE[]        = "end_Attribute" ;
const	char	ATT_DEF[]           = "define"      ;
const	char	ATT_DEF_END[]       = "enddef"      ;
const	char	ATT_DIFFUSE[]       = "diffuse"     ;
const	char	ATT_SPECULAR[]      = "specular"    ;
const	char	ATT_RGB     []      = "rgb"         ;
const	char	ATT_PHONG_POWER []  = "phong_power" ;
const	char	ATT_TRANSPARENCY[]  = "transparency";
const	char	ATT_VISIBILITY  []  = "visibility"  ;

	//----- for Facet block
const	char	BEGIN_FACET_OLD[]  = "Faces:" ;
const	char	BEGIN_FACET[]      = "Facet:" ;
const	char	END_FACET[]        = "end_Facet" ;
const	char	BEGIN_FACET2[]     = "BeginFacet"  ;
const	char	END_FACET2[]       = "EndFacet" ;
const	char	TERMINAL_CHAR      = '#'  ;
const	char	TERMINAL_CHAR2     = ';'  ;

#endif
