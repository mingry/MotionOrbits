#include "unit_vector.h"
#include "vector.h"
#include "position.h"

using math::vector;
using math::position;
using math::unit_vector;

unit_vector math::operator-( unit_vector const& a )
{
    unit_vector b;

    b.p[0] = - a.p[0];
    b.p[1] = - a.p[1];
    b.p[2] = - a.p[2];

    return b;
}

vector math::operator-( vector const& a )
{
    vector b;

    b.p[0] = - a.p[0];
    b.p[1] = - a.p[1];
    b.p[2] = - a.p[2];

    return b;
}

position& math::operator-=( position& a, vector const& b )
{
    a.p[0] -= b.p[0];
    a.p[1] -= b.p[1];
    a.p[2] -= b.p[2];

    return a;
}

vector& math::operator-=( vector& a, vector const& b )
{
    a.p[0] -= b.p[0];
    a.p[1] -= b.p[1];
    a.p[2] -= b.p[2];

    return a;
}

vector math::operator-( vector const& a, vector const& b )
{
    vector c;

    c.p[0] = a.p[0] - b.p[0];
    c.p[1] = a.p[1] - b.p[1];
    c.p[2] = a.p[2] - b.p[2];

    return c;
}

vector math::operator-( position const& a, position const& b )
{
    return vector( a.p[0] - b.p[0], a.p[1] - b.p[1], a.p[2] - b.p[2] );
}

position math::operator-( position const& a, vector const& b )
{
    position c;

    c.p[0] = a.p[0] - b.p[0];
    c.p[1] = a.p[1] - b.p[1];
    c.p[2] = a.p[2] - b.p[2];

    return c;
}

