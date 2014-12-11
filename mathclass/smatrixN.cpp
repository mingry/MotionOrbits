// Sparse matrix

#include "smatrixN.h"
#include <cassert>

using math::smatrixN;

using std::istream;
using std::ostream;
using std::endl;

// member functions
void
math::smatrixN::allocate(int N)
{
	n = N;

	nnze = 0;

	rows = new entity[n];
	for (int i = 0; i < n; i++)
	{
		rows[i].id = HEAD_ENTITY;
		rows[i].value = MEANINGLESS;
		rows[i].next = NULL;
	}
}

void
math::smatrixN::deallocate()
{
	// deallocate memories for every non-zero entities
	entity*	curr;
	entity*	next;
	for (int i = 0; i < n; i++)
	{
		next = rows[i].next;
		while (next)
		{
			curr = next;
			next = curr->next;
			delete	curr;
		}
	}
	delete []	rows;

	rows = NULL; n = nnze = 0;
}

double
math::smatrixN::setValue(int row, int col, double value, entity** e)
{
	assert(0 <= row && row < n);
	assert(0 <= col && col < n);

	if (row > col)	{ int tmp = row; row = col; col = tmp; }

	// find A(row,col) entity
	for (entity* p = &rows[row]; p; p = p->next)
	{
		if (p->id == col)
		{
			double	old = p->value;
			p->value = value;
			if (e)	*e = p;

			return	old;
		}
		else if (!p->next || p->next->id > col)
		{
			nnze++;

			entity*	tmp = new entity;
			tmp->id = col;
			tmp->value = value;
			tmp->next = p->next;
			p->next = tmp;
			if (e)	*e = tmp;

			return	0;
		}
	}

	return	0;
}

double
math::smatrixN::getValue(int row, int col) const
{
	assert(0 <= row && row < n);
	assert(0 <= col && col < n);

	if (row > col)	{ int tmp = row; row = col; col = tmp; }

	// find A(row,col) entity
	for (entity* p = rows[row].next; p; p = p->next)
		if (p->id == col)	return	p->value;

	// A(row,col) entity is zero
	return	0;
}

double
math::smatrixN::add(int row, int col, double increment)
{
	entity*	e;
	double	old;
	
	old = setValue(row, col, 0, &e);
	e->value = old + increment;

	return	(old + increment);
}

double
math::smatrixN::sub(int row, int col, double decrement)
{
	entity*	e;
	double	old;

	old = setValue(row, col, 0, &e);
	e->value = old - decrement;

	return	(old - decrement);
}

// friend functions
ostream& math::operator<<(ostream& os, smatrixN& A)
{
	os << A.n << " x " << A.n << " ";
	os << A.nnze << " non-zero sparse matrix " << endl;

	for (int i = 0; i < A.n; i++)
	for (entity* p = A.rows[i].next; p; p = p->next)
		os << " [ " << i << " , " << p->id << " ] = " << p->value << endl;
	os << endl;

	return	os;
}

istream& math::operator>>(istream& is, smatrixN& A)
{
	static char	buf[256];

	int	nrow, ncol, nnze;

//	is >> nrow >> "x" >> ncol;
	is >> nrow >> buf >> ncol;
//	is >> nnze >> "non-zero" >> "sparse" >> "matrix";
//	is >> nnze >> buf >> "sparse" >> buf;

	assert(nrow == ncol);

	int	n = nrow;

	A.deallocate();
	A.allocate(n);

	int		row, col;
	double	value;
	for (int i = 0; i < nnze; i++)
	{
		//is >> "[" >> row >> "," >> col >> "]" >> "=" >> value;
		is >> buf >> row >> buf >> col >> buf >> buf >> value;

		if (fabs(value) > EPS)	A.setValue(row, col, value);
	}

	return	is;
}
