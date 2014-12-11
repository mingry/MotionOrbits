#ifndef TRANSF_H
#define TRANSF_H

#include "math_def.h"

#include "matrix.h"
#include "vector.h"
#include "position.h"
#include "quater.h"

namespace math 
{
	class unit_vector;

	class transf
	{
	private:
		matrix m;
		vector v;

		// multiplication
		friend transf&      operator*=( transf &, transf const& );
		friend transf       operator*( transf const&, transf const& );
		friend vector&      operator*=( vector&, transf const& );
		friend vector       operator*( vector const&, transf const& );
		friend position&    operator*=( position&, transf const& );
		friend position     operator*( position const&, transf const& );
		friend unit_vector& operator*=( unit_vector&, transf const& );
		friend unit_vector  operator*( unit_vector const&, transf const& );

		// functions
		friend transf       inverse( transf const& );
		friend transf       interpolate( double, transf const&, transf const& );

		// stream
		friend std::ostream& operator<<( std::ostream&, transf const& );
		friend std::istream& operator>>( std::istream&, transf& );

	public:
		// constructors
		transf() {}
		transf( const transf& t ) {
			m = t.m;
			v = t.v;
		}
		transf( matrix const& a, vector const& b ) { m = a; v = b; }
		transf( quater const& a, vector const& b ) { m = Quater2Matrix(a); v = b; }

		// inquiry functions
		inline const matrix&	getAffine() const { return m; };
		inline const vector&	getTranslation() const { return v; };
		const quater	getRotation() const { return Matrix2Quater(m); }

		inline const matrix&	affine() const { return m; };
		inline const vector&	translation() const { return v; }
		inline const position	getPosition() const { return position(v.x(), v.y(), v.z()); }

		inline void			setAffine( matrix const& a ) { m = a; }
		inline void			setTranslation( vector const& a ) { v = a; }
		void			setRotation( quater const& q ) { m = Quater2Matrix(q); }

		transf			inverse() const;

		transf& operator=( const transf& t ) {
			m = t.m;
			v = t.v;
			return ( *this );
		}
	};

	// identity transform
	extern transf identity_transf;

	// generator
	extern transf scale_transf( double );
	extern transf scale_transf( double, double, double );
	extern transf rotate_transf( double, vector const& );
	extern transf reflect_transf( vector const& );
	extern transf translate_transf( vector const& );
	extern transf translate_transf( double, double, double );
	extern transf coordinate_transf( position const&,
					unit_vector const&, unit_vector const& );
};

#endif
