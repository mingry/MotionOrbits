#ifndef UNIT_VECTOR_H
#define UNIT_VECTOR_H

#include "math_def.h"
#include "vector.h"

namespace math
{
	class matrix;
	class transf;

	class unit_vector : public vector
	{
	private:
		// negation
		friend unit_vector    operator-( unit_vector const& );

		// cross product
		friend position  operator*( position const&, unit_vector const& );
		friend position  operator*( unit_vector const&, position const& );

		friend unit_vector& operator*=( unit_vector&, matrix const& );
		friend unit_vector  operator*( unit_vector const&, matrix const& );

		friend unit_vector& operator*=( unit_vector&, transf const& );
		friend unit_vector  operator*( unit_vector const&, transf const& );

		// functions
		friend int    equal_normal( unit_vector const&,
									unit_vector const& );
		friend transf coordinate_transf( position const&,
										unit_vector const&,
										unit_vector const& );

	public:
		// constructors
		unit_vector() { };
		unit_vector( double x, double y, double z ) : vector( x, y, z ) { };
		unit_vector( double a[3] ) : vector( a ) { };

//		vector vectorize() { return vector( p[0], p[1], p[2] ); }
	};

	extern unit_vector x_axis, y_axis, z_axis;
};

#endif
