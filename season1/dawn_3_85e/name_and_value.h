/////////////////////////////
///// name_and_value.h /////
/////////////////////////////

#if !defined NAME_AND_VALUE_H
#define NAME_AND_VALUE_H


#include <string.h>	


class NameAndValue {
	char*	name ;
	double	value ;
 public:
		//----- constructors
	NameAndValue()  { name = NULL , value = 0.0 ; }
	NameAndValue( char* string, double value_given )  
	{ 
		setName  ( string      ) ;
		setValue ( value_given ) ; 
	}

		//----- destructor and clear()
	~NameAndValue() { this->clear(); }
	void clear() 
	{ 
		delete [] name ; name = NULL ;
		value = 0.0 ;
	}

		//----- set name
	void setName( const char* string )
	{
		delete [] name ;
		name = new char[ (strlen(string) + 1) ] ;
		strcpy( name, string ) ;
	}

		//----- set value
	void setValue( double value_given ) { value = value_given ; }

		//----- set all data
	void set ( char* string, double value_given )  
	{ 
		setName ( string      ) ;
		setValue( value_given ) ; 
	}

		//----- get 
	char*	getName () const { return name  ; }
	double	getValue() const { return value ; }

		//----- assignment
	void operator = ( const NameAndValue& rhs )
	{
			//-----
		if( rhs.getName() == NULL ) {
			delete [] name ; name = NULL;
		} else {
				//----- set string
			char* string = rhs.getName();
			char* name_org = name ;
			name = new char[ (strlen(string) + 1) ] ;
			strcpy( name, string ) ;
			delete [] name_org ;

				//----- set value
			setValue( rhs.getValue() );
		}
	}
	
} ; // NameAndValue
	
///////////////// end of name_and_value.h
#endif
