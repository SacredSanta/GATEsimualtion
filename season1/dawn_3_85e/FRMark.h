////////////////////////
///// FRMark.h   /////
////////////////////////

#if !defined  MARK2D_H
#define MARK2D_H

#include "colortype.h"
#include "vector.h"
#include "FRString.h"

class FRMark {

 protected:
	enum { MAX_PARAM = 3 };

 public:

		//----- label of marks
	enum FRMarkType {	UNDEF    , 
				CIRCLE2D , 
				SQUARE2D , 
				TEXT2D   , 
				NUM_MARK } ;

		//----- constructor
	FRMark( void ) ;
	FRMark( int			type_given, 
		const Vector3&		P, 
		double			param0 ,
		const ColorType&	color_given );


		//----- destructor
	~FRMark() { ; }

		//----- get
	int		getType	     () const	{ return type       ; }
	Vector3		getPosition3D() const	{ return position3D ; }
	double		getParam     ( int i = 0 ) const ;
	ColorType	getColor     () const	{ return color      ; }		
	const char*     getCharString() const { return string.GetCharString() ; }
	
		//----- set
	void		setType( int tp ) 
			{ 
				type  = tp ;
				if( type <= UNDEF || type >= NUM_MARK ) 
				{
					type = 	CIRCLE2D; 
				}
			}
	void		setPosition3D( const Vector3& P )    
				{ position3D = P ;}
	void		setColor     ( const ColorType& color_given ) 
				{ color = color_given ; }
	void		setParam     ( double val, int i = 0 ) ;
	void		setString (const char* str ) 
				{ string.ResetString( str );}

		//----- =
	void	operator = ( const  FRMark& rhs ) ;

 protected:

	int		type;
	Vector3		position3D ;
	double		param[MAX_PARAM]; // param[0] = size_vp 
	ColorType	color ;
	FRString	string ;

};


#endif
