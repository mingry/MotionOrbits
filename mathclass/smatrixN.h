//
// Sparse Matrix:	Aug 11, 1998
//

#ifndef	_SPARSE_MATRIX_H_
#define	_SPARSE_MATRIX_H_

#include "math_def.h"

namespace math
{
	#define	HEAD_ENTITY	-1
	#define	MEANINGLESS	0

	struct entity
	{
		int		id;
		double	value;
		entity*	next;
	};

	class	vectorN;

	class smatrixN
	{
	public:
		smatrixN(int n)	{ allocate(n); }
		~smatrixN()	{ deallocate(); }

		int		size() const	{ return n; }
		int		getSize() const	{ return n; }

		void	allocate(int n);
		void	deallocate();

		double	setValue(int row, int col, double value, entity** e = NULL);
		double	getValue(int row, int col) const;

		entity  getRows( int i ) const { return rows[i]; }

		double	add(int row, int col, double increment);
		double	sub(int row, int col, double decrement);
		smatrixN& add( smatrixN& a, smatrixN& b);

		friend std::ostream& operator<<(std::ostream& os, smatrixN& A);
		friend std::istream& operator>>(std::istream& is, smatrixN& A);

	private:
		int		n;		// n x n sparse matrix

		int		nnze;	// number of non-zero entity
		entity*	rows;
	};
};

#endif	// _SPARSE_MATRIX_H_
