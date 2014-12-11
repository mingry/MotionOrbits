#include "position.h"
#include "vector.h"

using math::position;
using math::vector;

double math::operator%( position const& a, vector const& b )
{
    return ( a.p[0]*b.p[0] + a.p[1]*b.p[1] + a.p[2]*b.p[2] );
}

double math::operator%( vector const& b, position const& a )
{
    return ( a.p[0]*b.p[0] + a.p[1]*b.p[1] + a.p[2]*b.p[2] );
}

double math::operator%( vector const& a, vector const& b )
{
    return ( a.p[0]*b.p[0] + a.p[1]*b.p[1] + a.p[2]*b.p[2] );
}
