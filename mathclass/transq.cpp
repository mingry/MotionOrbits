#include <iostream>
#include <cmath>

#include "transq.h"
#include "position.h"
#include "unit_vector.h"
#include "vector.h"
#include "quater.h"

using math::transq;
using math::position;
using math::unit_vector;
using math::vector;
using math::quater;

using std::istream;
using std::ostream;

transq math::identity_transq( quater(1,0,0,0), vector(0,0,0) );


transq
math::transq::inverse() const
{
    quater a = rotation.inverse();
    return transq( a, -rotate(a,translation) );
}

transq math::interpolate( double t, transq const& a, transq const& b )
{
	quater q = interpolate( t, a.rotation, b.rotation );
	vector v = interpolate( t, a.translation, b.translation );

    return transq( q, v );
}

transq math::rotate_transq( double angle, vector const& axis )
{
    return transq( exp( angle * axis / 2.0 ), vector(0,0,0) );
}

transq math::translate_transq( vector const& axis )
{
    return transq( quater(1,0,0,0), axis );
}

transq math::translate_transq( double x, double y, double z )
{
    return transq( quater(1,0,0,0), vector(x,y,z) );
}


ostream& math::operator<<( ostream& os, transq const& a )
{
    os << a.rotation << " + " << a.translation;
    return os;
}

istream& math::operator>>( istream& is, transq& a )
{
	static char	buf[256];
    is >> a.rotation >> buf >> a.translation;
    return is;
}

//--------------------------------------------------------------------------//

position& math::operator*=( position& a, transq const& b )
{
    a = rotate(b.rotation,a) + b.translation;
    return a;
}

position math::operator*( position const& a, transq const& b )
{
    return ( rotate(b.rotation,a) + b.translation );
}

unit_vector& math::operator*=( unit_vector& a, transq const& b )
{
    a = rotate(b.rotation,a);
    return a;
}

unit_vector math::operator*( unit_vector const& a, transq const& b )
{
    return ( rotate(b.rotation,a) );
}

vector& math::operator*=( vector& a, transq const& b )
{
    a = rotate(b.rotation,a);
    return a;
}

vector math::operator*( vector const& a, transq const& b )
{
    return ( rotate(b.rotation,a) );
}

transq math::operator*( transq const& a, transq const& b )
{
    return transq( a.rotation * b.rotation,
				   rotate(a.rotation, b.translation) + a.translation );
}

transq& math::operator*=( transq& a, transq const& b )
{
	a.rotation = a.rotation * b.rotation;
	a.translation = rotate(b.rotation, a.translation) + b.translation;

    return a;
}

bool math::operator==( const transq& a, const transq& b )
{
	if( a.rotation == b.rotation && a.translation == b.translation )	return true;
	return false;
}

