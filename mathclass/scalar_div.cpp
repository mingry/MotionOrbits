#include <cassert>

#include "vector.h"

using math::vector;

vector& math::operator/=( vector& a, double b )
{
	assert( b != 0.0 );

    a.p[0] = a.p[0] / b;
    a.p[1] = a.p[1] / b;
    a.p[2] = a.p[2] / b;

    return a;
}

vector math::operator/( vector const& a, double b )
{
	assert( b != 0.0 );

    vector c;

    c.p[0] = a.p[0] / b;
    c.p[1] = a.p[1] / b;
    c.p[2] = a.p[2] / b;

    return c;
}

double math::operator/( vector const& a, vector const& b )
{
    if ( b.p[0] > 0.0 ) return a.p[0]/b.p[0];
    if ( b.p[1] > 0.0 ) return a.p[1]/b.p[1];
    if ( b.p[2] > 0.0 ) return a.p[2]/b.p[2];
    return 0.0;
}

