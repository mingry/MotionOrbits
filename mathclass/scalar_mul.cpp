#include "vector.h"
#include "position.h"

using math::vector;
using math::position;

vector& math::operator*=( vector& a, double b )
{
    a.p[0] = a.p[0] * b;
    a.p[1] = a.p[1] * b;
    a.p[2] = a.p[2] * b;

    return a;
}

vector math::operator*( vector const& a, double b )
{
    vector c;

    c.p[0] = a.p[0] * b;
    c.p[1] = a.p[1] * b;
    c.p[2] = a.p[2] * b;

    return c;
}


vector math::operator*( double b, vector const& a )
{
    vector c;

    c.p[0] = a.p[0] * b;
    c.p[1] = a.p[1] * b;
    c.p[2] = a.p[2] * b;

    return c;
}

position& math::operator*=( position& a, double b )
{
    a.p[0] = a.p[0] * b;
    a.p[1] = a.p[1] * b;
    a.p[2] = a.p[2] * b;

    return a;
}

position math::operator*( position const& a, double b )
{
    position c;

    c.p[0] = a.p[0] * b;
    c.p[1] = a.p[1] * b;
    c.p[2] = a.p[2] * b;

    return c;
}


position math::operator*( double b, position const& a )
{
    position c;

    c.p[0] = a.p[0] * b;
    c.p[1] = a.p[1] * b;
    c.p[2] = a.p[2] * b;

    return c;
}

