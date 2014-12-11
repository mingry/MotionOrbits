#include <iostream>
#include <cmath>

#include "vector.h"
#include "matrix.h"
#include "position.h"

using math::vector;
using math::position;
using math::matrix;

using std::istream;
using std::ostream;

// member functions
double 
math::vector::length() const
{
    return sqrt( p[0]*p[0] + p[1]*p[1] + p[2]*p[2] );
}

matrix
math::vector::cross() const
{
    return matrix(	vector(  0 ,-z(), y()),
					vector( z(),  0 ,-x()),
					vector(-y(), x(),  0 ) );
}

// friend functions
vector
math::interpolate( double t, vector const& a, vector const& b )
{
	return (1.0-t)*a + t*b;
}

double 
math::len( vector const& v )
{
    return sqrt( v.x()*v.x() + v.y()*v.y() + v.z()*v.z() );
}

double 
math::angle( vector const& a, vector const& b )
{
	return ACOS( (a%b)/(len(a)*len(b)) );
}

position 
math::vector2position( vector const& v )
{
    return position( v.x(), v.y(), v.z() );
}

vector 
math::position2vector( position const& p )
{
    return vector( p.x(), p.y(), p.z() );
}

ostream& 
math::operator<<( ostream& os, vector const& a )
{
    os << "( " << a.x() << " , " << a.y() << " , " << a.z() << " )";
    return os;
}

istream& 
math::operator>>( istream& is, vector& a )
{
	static char buf[256];

	is >> buf >> a.p[0] >> buf >> a.p[1] >> buf >> a.p[2] >> buf;

	return is;
}

//
bool math::operator==( const vector& a, const vector& b )
{
	if( a.x() == b.x() && a.y() == b.y() && a.z() == b.z() )	return true;
	return false;
}

bool math::operator!=( const vector& a, const vector& b )
{
	if( a.x() != b.x() || a.y() != b.y() || a.z() != b.z() )	return true;
	return false;
}
