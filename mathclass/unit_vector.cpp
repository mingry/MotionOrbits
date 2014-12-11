#include <iostream>
#include <cmath>

#include "unit_vector.h"

using std::istream;
using std::ostream;

using math::unit_vector;

unit_vector math::x_axis( 1.0, 0.0, 0.0 );
unit_vector math::y_axis( 0.0, 1.0, 0.0 );
unit_vector math::z_axis( 0.0, 0.0, 1.0 );

unit_vector math::normalize( vector const& a )
{
    double s = len(a);

    if ( s != 0.0 )
        return unit_vector( a.p[0]/s, a.p[1]/s, a.p[2]/s );
    else
        return unit_vector( 0.0, 0.0, 0.0);
}

int math::equal_normal( unit_vector const& u, unit_vector const& v )
{
    if ( len(u-v) < EPS ) return TRUE;
                     else return FALSE;
}

ostream& math::operator<<( ostream& os, unit_vector const& a )
{
    os << "( " << a.p[0] << " , " << a.p[1] << " , " << a.p[2] << " )";
    return os;
}

istream& math::operator>>( istream& is, unit_vector& a )
{
	static char	buf[256];
    //is >> "(" >> a.p[0] >> "," >> a.p[1] >> "," >> a.p[2] >> ")";
	is >> buf >> a.p[0] >> buf >> a.p[1] >> buf >> a.p[2] >> buf;
    return is;
}

