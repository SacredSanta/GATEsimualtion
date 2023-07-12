///////////////////////////
///// FRVisText2D.h   /////
///////////////////////////

#include "FRString.h"
#include "colortype.h"
#include "vector.h"


class FRVisText2D {

 public:
	FRVisText2D ( void ) : \
		m_string(), m_color(), m_P_mm(), m_size_pt(0.0) {}

	void SetString( const FRString& string ) 
		{ m_string = string ; }
	void SetColor( const ColorType& color ) 
		{ m_color = color ; }
	void SetPosition_mm( const Vector2& P_mm ) 
		{ m_P_mm = P_mm ; }
	void SetSize_pt    ( double size_pt ) 
		{ m_size_pt = size_pt ; }

	const FRString& GetString      () const { return m_string  ; }  
	ColorType       GetColor       () const { return m_color   ; }
	Vector2         GetPosition_mm () const { return m_P_mm    ; }
	double          GetSize_pt     () const { return m_size_pt ; }

	void  operator = ( const FRVisText2D& rhs )
	{ 
		if( this != (&rhs) ) { 
			m_string  = rhs.m_string ;    
			m_color   = rhs.m_color  ;
			m_P_mm    = rhs.m_P_mm   ;
			m_size_pt = rhs.m_size_pt ;
		}
	}

 protected:

	FRString  m_string      ;
	ColorType m_color       ;
	Vector2   m_P_mm ;
	double	  m_size_pt     ;
};
