#ifndef __OPTIMIZE_H
#define __OPTIMIZE_H

#include "math_def.h"

namespace math 
{
	class vectorN;

	void mnbrak(double&, double&, double&,
				double&, double&, double&, double (*func)(double)) ;

	double brent(double, double, double,
				double (*f)(double), double, double&) ;

	void linmin(vectorN&, vectorN&, int, double&, double (*func)(vectorN const&));

	void gradient_descent(	vectorN&, int, double, int&, double&,
				double (*func)(vectorN const&),
				double (*dfunc)(vectorN const&, vectorN&), int max_iter=200 );

	void frprmn(vectorN&, int, double, int&, double&,
				double (*func)(vectorN const&),
				double (*dfunc)(vectorN const&, vectorN&));

	void error( char* );
};

#endif
