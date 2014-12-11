#ifndef MATRIX_N_H
#define MATRIX_N_H

#include "math_def.h"
#include "vectorN.h"

namespace math
{
	class vectorN;

	class matrixN
	{
	private:
		int n, m, on;
		vectorN *v;

	public:
		// constructors
		matrixN();
		matrixN( int, int );
		matrixN( int, int, vectorN* );
		~matrixN();

		matrixN( const matrixN& m );
		matrixN&  operator=( const matrixN& m );

		// inquiry functions
		void      getValue( double** ) const;
		double    getValue( int, int ) const;
		void      setValue( double** );
		void      setValue( int, int, double );

		vectorN&  operator[](int i) const;
		int       row()    const { return n; }
		int       column() const { return m; }
		void      setSize( int, int );
		void      setRow( int, const vectorN& );
		void	  setColumn( int, const vectorN& );

		matrixN&  transpose( matrixN const& );
		double    det() const;

		matrixN&  assign( matrixN const& );
		matrixN&  mult( matrixN const&, matrixN const& );
		matrixN&  operator*=( double );
		matrixN&  operator/=( double );
		matrixN&  operator+=( matrixN const& );
		matrixN&  operator-=( matrixN const& );

		matrixN&  mergeUpDown( matrixN const&, matrixN const& );
		matrixN&  mergeLeftRight( matrixN const&, matrixN const& );
		void      splitUpDown( matrixN&, matrixN& );
		void      splitLeftRight( matrixN&, matrixN& );
		void      splitUpDown( matrixN&, matrixN&, int );
		void      splitLeftRight( matrixN&, matrixN&, int );

		bool      LUdecompose( int* );
		void	  LUsubstitute( int*, vectorN& );
		double    LUinverse( matrixN& );

		void	  SVdecompose( vectorN&, matrixN& );
		void	  SVsubstitute( vectorN const&, matrixN const&,
								vectorN const&, vectorN& );
		void      SVinverse( matrixN& );

		// stream
		friend std::ostream& operator<<( std::ostream&, matrixN const& );
		friend std::istream& operator>>( std::istream&, matrixN& );
	};
};

#endif
