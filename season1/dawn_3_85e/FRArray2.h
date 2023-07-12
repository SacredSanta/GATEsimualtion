//////////////////////
///// FRArray2.h /////
//////////////////////

#if !defined  FR_ARRAY2_H
#define FR_ARRAY2_H

#include <iostream.h>

template <class Type> 
class FRArray2 {
 
public:
	FRArray2( unsigned int MAX1, unsigned int MAX2 ) ;
	~FRArray2() { delete [] m_value ; }

	Type& operator() ( unsigned int i, unsigned int j ) ;
		
private:
	unsigned int m_MAX1 ; 
	unsigned int m_MAX2 ; 

	Type* m_value ;
};

template <class Type> 
inline 	FRArray2<Type>::FRArray2( unsigned int MAX1, unsigned int MAX2 ) 
{ 
	m_MAX1 = MAX1 ;
	m_MAX2 = MAX2 ;
	m_value = new  Type [ (m_MAX1 * m_MAX2)  ];
}

template <class Type> 
inline Type&  FRArray2<Type>::operator() ( unsigned int i,  unsigned int j )
{
	if( i  >= m_MAX1 ) {
		cerr << "WARNING (FRArray2): Irrelevant array index 1: " ;
		cerr << i << endl;
		return m_value[0];
	} 

	if( j  >= m_MAX2 ) {
		cerr << "WARNING (FRArray2): Irrelevant array index 2: " ;
		cerr << j << endl;
		return m_value[0];
	}

	return m_value [  (m_MAX2 * i +  j ) ] ;
}

#endif
