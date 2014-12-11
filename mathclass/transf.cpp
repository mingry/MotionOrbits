#include <iostream>
#include <cmath>

#include "transf.h"
#include "matrix.h"
#include "vector.h"
#include "quater.h"
#include "position.h"
#include "unit_vector.h"

using math::transf;
using math::matrix;
using math::vector;
using math::quater;
using math::position;
using math::unit_vector;

using std::istream;
using std::ostream;

// Identity Transform

transf math::identity_transf( scaling(1), vector(0,0,0) );


// Inquiry Function

transf
math::transf::inverse() const
{
    matrix a = m.inverse();
    return transf( a, (-v)*a );
}

transf
math::inverse( transf const& t )
{
    return t.inverse();
}


// generator

transf math::scale_transf( double s )
{
    matrix m = scaling( s );
    vector v( 0.0, 0.0, 0.0 );
    return transf( m, v );
}

transf math::scale_transf( double sx, double sy, double sz )
{
    matrix m = scaling( sx, sy, sz );
    vector v( 0.0, 0.0, 0.0 );
    return transf( m, v );
}

transf math::rotate_transf( double angle, vector const& axis )
{
    matrix m = rotation( angle, axis );
    vector v( 0.0, 0.0, 0.0 );
    return transf( m, v );
}

transf math::reflect_transf( vector const& axis )
{
    matrix m = reflection( axis );
    vector v( 0.0, 0.0, 0.0 );
    return transf( m, v );
}

transf math::translate_transf( vector const& axis )
{
    return transf( scaling( 1.0 ), axis );
}

transf math::translate_transf( double x, double y, double z )
{
    return transf( scaling( 1.0 ), vector(x,y,z) );
}

transf math::coordinate_transf( position const& new_o,
                          unit_vector const& new_x,
                          unit_vector const& new_y )
{
    position o( 0.0, 0.0, 0.0 );
    vector x_axis( 1.0, 0.0, 0.0 );
    vector y_axis( 0.0, 1.0, 0.0 );
    vector z_axis( 0.0, 0.0, 1.0 );

    transf t1 = translate_transf( o - new_o );

    vector v1 = new_x;
    v1.set_x( 0.0 );
    double theta = ACOS( (v1 % z_axis) / len(v1) );
    transf t2 = rotate_transf( theta, x_axis );

    vector v2 = new_x;
    vector v3 = v2 * t2 * t1;
    theta = ACOS( (v3 % x_axis) / len(v3) );
    transf t3 = rotate_transf( theta, y_axis );

    vector v4 = new_y;
    vector v5 = v4 * t3 * t2 * t1;
    theta = ACOS( (v5 % y_axis) / len(v5) );
    transf t4 = rotate_transf( theta, x_axis );

    return t4 * t3 * t2 * t1;
}


ostream& math::operator<<( ostream& os, transf const& a )
{
    os << a.getAffine() << " + " << a.getTranslation();
    return os;
}

istream& math::operator>>( istream& is, transf& a )
{
	static char	buf[256];
    //is >> a.m >> "+" >> a.v;
	is >> a.m >> buf >> a.v;
    return is;
}

transf math::interpolate( double t, transf const& a, transf const& b )
{
	matrix m = interpolate( t, a.getAffine(), b.getAffine() );
	vector v = interpolate( t, a.getTranslation(), b.getTranslation() );

    return transf( m, v );
}

