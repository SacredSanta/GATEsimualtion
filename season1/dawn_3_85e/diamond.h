/////////////////////
///// diamond.h /////
/////////////////////

#if !defined DIAMOND_H
#define DIAMOND_H

#include <iostream.h>

class Diamond {
		//-----  bounding diamond
	double	xmin,	xmax ;
	double	ymin,	ymax ;
	double	umin,	umax ; // u = y + x  ;
	double	vmin,	vmax ; // v = y - x  ;

		//-----  index
	int	index[2] ;

		//----- Is  value set?
	int	flag_value_set ;

 public:
	Diamond() ;

	void	set(	double xmin_given , double xmax_given ,
			double ymin_given , double ymax_given ,
			double umin_given , double umax_given ,
			double vmin_given , double vmax_given ,
			int index_given  = -1                    ) ;


	void	getDiamond(	double* xmin_p   ,	double* xmax_p ,
				double* ymin_p   ,	double* ymax_p ,
				double* umin_p   ,	double* umax_p ,
				double* vmin_p   ,	double* vmax_p  ) ;

	void	getIndex( int*  index0_p, int*  index1_p ) ;

	int	isValudSet() const { return flag_value_set ; }

}; // Diamond


	//----- Diamond::Diamond()
inline Diamond::Diamond()
{
	xmin = xmax = 0.0 ;
	ymin = ymax = 0.0 ;
	
	umin = umax = 0.0 ;
	vmin = vmax = 0.0 ;

	index[0] = index[1] = -1 ;

	flag_value_set = 0 ;

} // Diamond::Diamond()


	//----- Diamond::set()
inline void Diamond::set(	double xmin_given , double xmax_given ,
				double ymin_given , double ymax_given ,
				double umin_given , double umax_given ,
				double vmin_given , double vmax_given ,
				int index_given                          ) 
{
		//----- set values
	if( !flag_value_set ) {
		xmin = xmin_given ;	xmax = xmax_given ;
		ymin = ymin_given ;	ymax = ymax_given ;
		umin = umin_given ;	umax = umax_given ;
		vmin = vmin_given ;	vmax = vmax_given ;
		index[0] = index[1] = index_given ;
	} else { 
		if( xmin_given < xmin ) { xmin = xmin_given ; }
		if( xmax_given > xmax ) { xmax = xmax_given ; }
		if( ymin_given < ymin ) { ymin = ymin_given ; }
		if( ymax_given > ymax ) { ymax = ymax_given ; }
		if( umin_given < umin ) { umin = umin_given ; }
		if( umax_given > umax ) { umax = umax_given ; }
		if( vmin_given < vmin ) { vmin = vmin_given ; }
		if( vmax_given > vmax ) { vmax = vmax_given ; }
		index[1] = index_given ;
	}

		//----- Values are already set.
	flag_value_set = 1 ;

} // Diamond::set()


	//----- Diamond::getDiamond()
inline void Diamond::getDiamond(	double* xmin_p   ,	double* xmax_p ,
					double* ymin_p   ,	double* ymax_p ,
					double* umin_p   ,	double* umax_p ,
					double* vmin_p   ,	double* vmax_p  )
{
	*xmin_p = xmin  ;	*xmax_p = xmax ;
	*ymin_p = ymin  ;	*ymax_p = ymax ;
	*umin_p = umin  ;	*umax_p = umax ;
	*vmin_p = vmin  ;	*vmax_p = vmax ;

} // Diamond::update()


	//----- Diamond::getIndex()
inline void Diamond::getIndex( int*  index0_p, int*  index1_p )
{
	*index0_p = index[0];	*index1_p = index[1];

} // Diamond::update()





//////////////// end of diamond.h
#endif
