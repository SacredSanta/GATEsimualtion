///////////////////////////////
/////     qsort.h      ////////
///////////////////////////////

#if !defined QSORT_H
#define QSORT_H

#include <iostream.h>
#include <stdlib.h>

template <class Type>
class QuickSort {
		//----- mode of comparinig function
		//.....  mode = 0:  use compare0()
		//.....       = 1:  use compare1()
	int	mode ;

 public:
		//----- constructor
	QuickSort( int mode_given = 0 ) { setMode ( mode_given ); }

		//----- destructor
	~QuickSort() {;}

		//----- set mode
	void	setMode( int mode_given = 0 ) { mode = mode_given ; }

		//----- get mode at present
	int	getMode() { return mode ; }

		//----- comparing function
		//.....  Return:
		//.....  +1 if  a ">" b
		//.....  -1 if  a "<" b
		//.....   0 if  a "=" b
	int	compare0( Type* a, Type* b ) ;
	int	compare1( Type* a, Type* b ) ;

		//----- quick sort function
	void doSort( Type* a, int left, int right ) ;

} ; // class QuickSort



template <class Type>
inline void QuickSort<Type>::doSort( Type* a, int left, int right ) 
{
		//----- local variables
	int	i, j ;
	Type	axis, tmp ;

		//-----  axis
	axis = a [ (left + right ) /2 ] ; 

		//----- starting i and j
	i = left ;	j = right ;

	while(1) {
		if        ( mode == 0 ) { 
			while ( compare0( &a[ i ] , &axis ) == -1 ) { i++ ; }
			while ( compare0( &a[ j ],  &axis ) == +1 ) { j-- ; }
		} else if ( mode == 1 ) { 
			while ( compare1( &a[ i ] , &axis ) == -1 ) { i++ ; }
			while ( compare1( &a[ j ],  &axis ) == +1 ) { j-- ; }
		} else {
			cerr << "ERROR ( QuickSort::doSort() )\n";
			cerr << "     mode = " << mode << endl ;
			exit(1) ;
		}

			//----- termination condition
		if( i >= j ) { 	break ; }

			//----- a[i] <--> a[j]
		tmp  = a[i] ;	a[i] = a[j] ;	a[j] = tmp  ; 

			//----- updation of i and j 
		i++ ; j-- ;

	} // while(1)

		//----- recursive call for left partial series
	if( (i - left) >= 2 ) { this->doSort( a, left, i - 1   ); }

		//----- recursive call for right partial series
	if( (right -j) >= 2 ) { this->doSort( a, j + 1,  right ); }

} // QuickSort<Type>::doSort()



#endif


