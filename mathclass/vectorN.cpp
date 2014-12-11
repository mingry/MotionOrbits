#include "vectorN.h"
#include "matrixN.h"
#include "smatrixN.h"

#include <cassert>

using math::vectorN;
using math::matrixN;
using math::smatrixN;

using std::istream;
using std::ostream;

math::vectorN::vectorN()
{
	on = n = 0;
}

math::vectorN::vectorN( int x )
{
    on = n = x;
    if ( on>0 ) v = new double[n];
}

math::vectorN::vectorN( int x, double *a )
{
    on = n = x;
    v = new double[n];
	for( int i=0; i<n; i++ )
		v[i] = a[i];
}

math::vectorN::vectorN( const vectorN& a )
{
	on = n = 0;

    vectorN &c = (*this);
    c.setSize( a.size() );

    for( int i=0; i<c.size(); i++ )
        c[i] = a[i];
}

math::vectorN::~vectorN()
{
    if ( on>0 ) delete[] v;
}

void
math::vectorN::setValue( double *d )
{
	for( int i=0; i<n; i++ )
		v[i] = d[i];
}

void
math::vectorN::setValue( int i, double d )
{ 
	assert(i>=0 && i<n); v[i] = d; 
}

void
math::vectorN::getValue( double *d )
{
	for( int i=0; i<n; i++ )
		d[i] = v[i];
}

double
math::vectorN::getValue( int i ) const
{ 
	assert(i>=0 && i<n); 
	return v[i]; 
}

double&
math::vectorN::operator[]( int i ) const
{ 
	assert(i>=0 && i<n); 
	return v[i]; 
}
 
void
math::vectorN::setSize( int x )
{
	if ( on<x )
	{
		if ( on>0 ) delete[] v;
		v = new double[x];
		on = x;
	}
	n = x;
}

vectorN&
math::vectorN::operator=( vectorN const& a )
{
    vectorN &c = (*this);
    c.setSize( a.size() );

    for( int i=0; i<c.size(); i++ )
        c[i] = a[i];
    return c;
}

vectorN&
math::vectorN::assign( vectorN const& a )
{
    vectorN &c = (*this);
    c.setSize( a.size() );

    for( int i=0; i<c.size(); i++ )
        c[i] = a[i];
    return c;
}

vectorN&
math::vectorN::negate()
{
    vectorN &c = (*this);
    for( int i=0; i<c.size(); i++ )
        c[i] = -c[i];
    return c;
}

vectorN&
math::vectorN::add( vectorN const& a, vectorN const& b )
{
    vectorN &c = (*this);
    assert( a.size()==b.size() );
    c.setSize( a.size() );

    for( int i=0; i<a.size(); i++ )
        c[i] = a[i] + b[i];
    return c;
}

vectorN&
math::vectorN::operator+=( vectorN const& a )
{
    vectorN &c = (*this);
    assert( c.size()==a.size() );

    for( int i=0; i<c.size(); i++ )
        c[i] += a[i];
    return c;
}

vectorN&
math::vectorN::sub( vectorN const& a, vectorN const& b )
{
    vectorN &c = (*this);
    assert( a.size()==b.size() );
    c.setSize( a.size() );

    for( int i=0; i<a.size(); i++ )
        c[i] = a[i] - b[i];
    return c;
}

vectorN&
math::vectorN::operator-=( vectorN const& a )
{
    vectorN &c = (*this);
    assert( c.size()==a.size() );

    for( int i=0; i<a.size(); i++ )
        c[i] -= a[i];
    return c;
}

double
math::operator%( vectorN const& a, vectorN const& b )
{
    assert( a.size()==b.size() );

    double c=0;
    for( int i=0; i<a.size(); i++ )
        c += a[i] * b[i];
    return c;
}

vectorN&
math::vectorN::mult( vectorN const& b, double a )
{
    vectorN &c = (*this);
    c.setSize( b.size() );

    for( int i=0; i<c.size(); i++ )
        c[i] = b[i]*a;
    return c;
}

vectorN&
math::vectorN::operator*=( double a )
{
    vectorN &c = (*this);

    for( int i=0; i<c.size(); i++ )
        c[i] *= a;
    return c;
}

vectorN&
math::vectorN::div( vectorN const& b, double a )
{
    vectorN &c = (*this);
    c.setSize( b.size() );

    for( int i=0; i<c.size(); i++ )
        c[i] = b[i]/a;
    return c;
}

vectorN&
math::vectorN::operator/=( double a )
{
    vectorN &c = (*this);

    for( int i=0; i<c.size(); i++ )
        c[i] /= a;
    return c;
}

vectorN&
math::vectorN::mult( matrixN const& a, vectorN const& b )
{
    vectorN &c = (*this);
    assert( a.column()==b.size() );
    c.setSize( a.row() );

    for( int i=0; i<a.row(); i++ )
    {
        c[i] = 0;
        for( int k=0; k<b.size(); k++ )
            c[i] += a[i][k] * b[k];
    }

    return c;
}

vectorN&
math::vectorN::mult( vectorN const& b, matrixN const& a )
{
    vectorN &c = (*this);
    assert( a.row()==b.size() );
    c.setSize( a.column() );

    for( int i=0; i<a.column(); i++ )
    {
        c[i] = 0;
        for( int k=0; k<b.size(); k++ )
            c[i] += b[k] * a[k][i];
    }

    return c;
}

vectorN&
math::vectorN::mult( smatrixN const& a, vectorN const& b )
{
    vectorN &c = (*this);
    assert( a.size()==b.size() );
    c.setSize( a.size() );

	int i;

    for (i=0; i<c.getSize(); i++) c.setValue(i, 0);

    for (i=0; i<c.getSize(); i++ )
    {
        for (entity* p = a.getRows(i).next; p; p = p->next)
        {
            // c[i] += a[i][j] * b[j]
            // c[j] += a[i][j] * b[i]

            c[i] += p->value * b[p->id];
            if (i != p->id) c[p->id] += p->value * b[i];
        }
    }

	return c;
}

vectorN&
math::vectorN::mult( vectorN const& b, smatrixN const& a )
{
    vectorN &c = (*this);
    assert( a.size()==b.size() );
    c.setSize( a.size() );

	int i;

    for ( i=0; i<c.getSize(); i++ ) c.setValue(i, 0);

    for ( i=0; i<c.getSize(); i++ )
    {
        for (entity* p = ( a.getRows(i) ).next; p; p = p->next)
        {
            // c[j] += b[i] * a[i][j]
            // c[i] += b[j] * a[i][j]

            c[p->id] += b[i] * p->value;
            if (i != p->id) c[i] += b[p->id] * p->value;
        }
    }

	return c;
}

double
math::vectorN::length() const
{
    double c=0;
    for( int i=0; i<n; i++ )
        c += this->v[i]*this->v[i];
    return sqrt(c);
}

double
math::vectorN::len() const
{
	return this->length();
}

vectorN&
math::vectorN::normalize()
{
    vectorN &c = (*this);

    double l = this->len();
    for( int i=0; i<n; i++ )
        c[i] = c[i] / l;
    return c;
}

vectorN&
math::vectorN::solve( matrixN const& a, vectorN const& b, int num, double tolerance, double damp )
{
    vectorN &c = (*this);
    assert( a.row()==a.column() );
    assert( a.row()==b.size() );
    c.setSize( b.size() );

    int flag = TRUE;
    for( int i=0; i<num && flag; i++ )
    {
        flag = FALSE;
        for( int j=0; j<a.row(); j++ )
        {
            double r = b[j] - a[j]%c;
            c[j] += damp*r/a[j][j];
            if ( r>tolerance ) flag = TRUE;
        }
    }

    return c;
}


vectorN&
math::vectorN::solve( matrixN const& a, vectorN const& b )
{
    vectorN &c = (*this);
    assert( a.row()==a.column() );
    assert( a.row()==b.size() );

	int n = b.size();
    c.setSize( n );
	c.assign( b );

	static matrixN mat; mat.setSize( n, n );
	mat.assign( a );

	static int* index;
	static int index_count = 0;
	if ( index_count<n )
	{
		if ( index_count>0 ) delete[] index;
		index_count = n;
		if ( index_count>0 ) index = new int[index_count];
	}

	mat.LUdecompose( index );
	mat.LUsubstitute( index, c );

	return c;
}


vectorN&
math::vectorN::solve( matrixN const& a, vectorN const& b, double tolerance )
{
	int m = a.row();
	int n = a.column();

	assert( m >= n );
	assert( b.size()==m );

    vectorN &c = (*this);
    c.setSize( n );

	static matrixN u; u.setSize( m, n );
	static vectorN w; w.setSize( n );
	static matrixN v; v.setSize( n, n );

	u.assign( a );
	u.SVdecompose( w, v );

	int i, j;
	double s;
	static vectorN tmp; tmp.setSize( n );

	double wmax = 0.0f;
	for( j=0; j<n; j++ )
		if ( w[j] > wmax ) wmax = w[j];

	for( j=0; j<n; j++ )
		if ( w[j] < wmax * tolerance ) w[j] = 0.0f;

	for( j=0; j<n; j++ )
	{
		s = 0.0f;
		if ( w[j] )
		{
			for( i=0; i<m; i++ )
				s += u[i][j] * b[i];
			s /= w[j];
		}
		tmp[j] = s;
	}

	for ( j=0; j<n; j++ )
	{
		s = 0.0;
		for ( i=0; i<n; i++ )
			s += v[j][i] * tmp[i];
		c[j] = s;
	}

	return c;
}

// friend functions
double math::difference( vectorN const& a, vectorN const& b )
{
	assert( a.getSize()==b.getSize() );

	double d = 0.0;
	for( int i=0; i<a.getSize(); i++ )
		d += (a.v[i] - b.v[i]) * (a.v[i] - b.v[i]);

	return d;
}

ostream& math::operator<<( ostream& os, vectorN const& a )
{
    os << "( ";
    for( int i=0; i< a.size()-1; i++ )
        os << a.v[i] << " , ";
    os << a.v[a.size()-1] << " )";
    return os;
}

istream& math::operator>>( istream& is, vectorN& a )
{
	static char	buf[256];
    //is >> "(";
	is >> buf;
    for( int i=0; i< a.size()-1; i++ )
	{
		//is >> a.v[i] >> ",";
		is >> a.v[i] >> buf;
	}
	//is >> a.v[a.size()-1] >> ")";
	is >> a.v[a.size()-1] >> buf;
    return is;
}

// zoi

void 
math::vectorN::zero()
{
	for( int i=0; i < n; i++ )
	{
		v[i] = 0;
	}
}
