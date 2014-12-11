#ifndef VECTOR_N_H
#define VECTOR_N_H

#include "math_def.h"

namespace math
{
	class matrixN;
	class smatrixN;

	class vectorN
	{
	private:
		int     n,
				on;
		double *v;

		// stream
		friend std::ostream&  operator<<( std::ostream&, vectorN const& );
		friend std::istream&  operator>>( std::istream&, vectorN& );

		// dot product
		friend double    operator%( vectorN const&, vectorN const& );

	public:
		vectorN();
		vectorN( const vectorN& vn );
		vectorN( int );
		vectorN( int, double* );
		~vectorN();

		void	  getValue( double* );
		double	  getValue( int i ) const;
		void	  setValue( double* );
		void	  setValue( int i, double d );

		double&   operator[](int i) const;
		int       size() const { return n; }
		int       getSize() const { return n; }
		void      setSize( int );

		double    len() const ;
		double    length() const ;

		vectorN&  normalize();

		vectorN&  assign( vectorN const& );
		vectorN&  operator=( vectorN const& );

		vectorN&  negate();

		vectorN&  add( vectorN const&, vectorN const& );
		vectorN&  operator+=( vectorN const& );

		vectorN&  sub( vectorN const&, vectorN const& );
		vectorN&  operator-=( vectorN const& );

		vectorN&  mult( vectorN const&, double );
		vectorN&  operator*=( double );

		vectorN&  mult( matrixN const&, vectorN const& );
		vectorN&  mult( vectorN const&, matrixN const& );
		vectorN&  mult( smatrixN const&, vectorN const& );
		vectorN&  mult( vectorN const&, smatrixN const& );

		vectorN&  div( vectorN const&, double );
		vectorN&  operator/=( double );

		friend double	difference( vectorN const&, vectorN const& );

		// SOR (Successive Over Relaxation)
		vectorN&  solve( matrixN const&,  vectorN const&, int, double, double );

		// LU Decomposition
		vectorN&  solve( matrixN const&,  vectorN const& );

		// SVD (Singular Value Decomposition)
		vectorN&  solve( matrixN const&,  vectorN const&, double );

		// Preconditioned Conjugate Gradient
		vectorN&  solve( smatrixN const&, vectorN const& );

		// zoi
		void zero();
	};
};

#endif
