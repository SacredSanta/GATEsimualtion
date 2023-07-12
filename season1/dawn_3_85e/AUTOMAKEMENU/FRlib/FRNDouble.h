///////////////////////
///// FRNDouble.h /////
///////////////////////

#if !defined FR_NORMAL_DOUBLE_H
#define FR_NORMAL_DOUBLE_H

class FRNDouble{
private:
	double	value;
	void	normal( double v )
	{
		value = v < 0.0 ? 0.0 :( v > 1.0 ? 1.0 : v ) ;
	}
public:
	FRNDouble( double v = 0.0 ){ normal( v );}

	operator double() const { return value ; }
	
	FRNDouble&	operator=( double v ){ normal( v ) ; return *this ; }
	FRNDouble&	operator+=( double v ){ normal( value+v ) ; return *this ; }
	FRNDouble&	operator-=( double v ){ normal( value-v ) ; return *this ; }
	FRNDouble&	operator*=( double v ){ normal( value*v ) ; return *this ; }
	FRNDouble&	operator/=( double v ){ normal( value/v ) ; return *this ; }
};

#endif
