#include <iostream>
#include <cmath>

#include "position.h"
#include "vector.h"

using std::istream;
using std::ostream;

using math::position;
using math::vector;

position math::interpolate( double t, position const& a, position const& b )
{
    position c;

    c.p[0] = (1.0-t)*a.p[0] + t*b.p[0];
    c.p[1] = (1.0-t)*a.p[1] + t*b.p[1];
    c.p[2] = (1.0-t)*a.p[2] + t*b.p[2];

    return c;
}

position math::affineCombination( position const& p1, position const& p2, double t1 )
{
    position c;

    c.p[0] = t1*p1.p[0] + (1-t1)*p2.p[0];
    c.p[1] = t1*p1.p[1] + (1-t1)*p2.p[1];
    c.p[2] = t1*p1.p[2] + (1-t1)*p2.p[2];

    return c;
}

position math::affineCombination( position const& p1, position const& p2, position const& p3,
				   double t1, double t2 )
{
    position c;
	double t3 = 1.0 - t1 - t2;

    c.p[0] = t1*p1.p[0] + t2*p2.p[0] + t3*p3.p[0];
    c.p[1] = t1*p1.p[1] + t2*p2.p[1] + t3*p3.p[1];
    c.p[2] = t1*p1.p[2] + t2*p2.p[2] + t3*p3.p[2];

    return c;
}

position math::affineCombination( position const& p1, position const& p2, position const& p3, position const& p4,
				   double t1, double t2, double t3 )
{
    position c;
	double t4 = 1.0 - t1 - t2 - t3;

    c.p[0] = t1*p1.p[0] + t2*p2.p[0] + t3*p3.p[0] + t4*p4.p[0];
    c.p[1] = t1*p1.p[1] + t2*p2.p[1] + t3*p3.p[1] + t4*p4.p[1];
    c.p[2] = t1*p1.p[2] + t2*p2.p[2] + t3*p3.p[2] + t4*p4.p[2];

    return c;
}

position math::affineCombination( int n, position p[], double t[] )
{
	position c(0,0,0);
	double tn = 1.0;

	for( int i=0; i<n-1; i++ )
	{
		c.p[0] += t[i] * p[i].p[0];
		c.p[1] += t[i] * p[i].p[1];
		c.p[2] += t[i] * p[i].p[2];

		tn -= t[i];
	}

	c.p[0] += tn * p[n-1].p[0];
	c.p[1] += tn * p[n-1].p[1];
	c.p[2] += tn * p[n-1].p[2];

	return c;
}

double math::distance( position const& p1, position const& p2 )
{
    return len(p2-p1);
}

double math::distance( position const& p, position const& p1, position const& p2 )
{
    vector v1 = p2 - p1;
    vector v2 = p - p1;
    double t = ( v1 % v2 ) / ( v1 % v1 );
    position p3 = p1 + t*v1;

    return distance( p, p3 );
}

ostream& math::operator<<( ostream& os, position const& a )
{
    os << "( " << a.p[0] << " , " << a.p[1] << " , " << a.p[2] << " )";
    return os;
}

istream& math::operator>>( istream& is, position& a )
{
	static char buf[256];
	// is >> "(" >> a.p[0] >> "," >> a.p[1] >> "," >> a.p[2] >> ")";
    is >> buf >> a.p[0] >> buf >> a.p[1] >> buf >> a.p[2] >> buf;

    return is;
}

bool math::operator!=( const position& a, const position& b ) 
{
	return ( a.p[0] != b.p[0] || a.p[1] != b.p[1] || a.p[2] != b.p[2] );
}

bool math::operator==( const position& a, const position& b ) 
{
	return ( a.p[0] == b.p[0] && a.p[1] == b.p[1] && a.p[2] == b.p[2] );
}
