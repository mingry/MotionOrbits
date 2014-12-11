#include <iostream>
#include <cmath>

#include "position.h"
#include "vector.h"

using math::position;
using math::vector;

position& math::operator+=( position& a, vector const& b )
{
    a.p[0] += b.p[0];
    a.p[1] += b.p[1];
    a.p[2] += b.p[2];

    return a;
}

vector& math::operator+=( vector& a, vector const& b )
{
    a.p[0] += b.p[0];
    a.p[1] += b.p[1];
    a.p[2] += b.p[2];

    return a;
}

position math::operator+( position const& a, vector const& b )
{
    position c;

    c.p[0] = a.p[0] + b.p[0];
    c.p[1] = a.p[1] + b.p[1];
    c.p[2] = a.p[2] + b.p[2];

    return c;
}

position math::operator+( vector const& b, position const& a )
{
    position c;

    c.p[0] = a.p[0] + b.p[0];
    c.p[1] = a.p[1] + b.p[1];
    c.p[2] = a.p[2] + b.p[2];

    return c;
}

vector math::operator+( vector const& a, vector const& b )
{
    vector c;

    c.p[0] = a.p[0] + b.p[0];
    c.p[1] = a.p[1] + b.p[1];
    c.p[2] = a.p[2] + b.p[2];

    return c;
}

