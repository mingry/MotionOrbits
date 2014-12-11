#include <iostream>
#include <cmath>

#include "quater.h"
#include "matrix.h"
#include "vector.h"
#include "position.h"
#include "unit_vector.h"

using std::istream;
using std::ostream;

using math::quater;
using math::matrix;
using math::vector;
using math::position;
using math::unit_vector;

static double eps = 0.0001;

// member functions
matrix
math::quater::getMatrix()
{
	matrix m;
    double s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

    s  = 2.0/length();
    xs = x() * s;  ys = y() * s;  zs = z() * s;
    wx = w() * xs; wy = w() * ys; wz = w() * zs;
    xx = x() * xs; xy = x() * ys; xz = x() * zs;
    yy = y() * ys; yz = y() * zs; zz = z() * zs;

    m[0][0] = 1.0 - (yy + zz);
    m[1][0] = xy - wz;
    m[2][0] = xz + wy;
    m[0][1] = xy + wz;
    m[1][1] = 1.0 - (xx + zz);
    m[2][1] = yz - wx;
    m[0][2] = xz - wy;
    m[1][2] = yz + wx;
    m[2][2] = 1.0 - (xx + yy);

    return m;
}

void
math::quater::setMatrix( matrix const& m )
{
    quater q;

    double tr, s;
    int    i, j, k;
    static int next[3] = { 1, 2, 0 };

    tr = m.getValue(0,0) + m.getValue(1,1) + m.getValue(2,2);
    if ( tr > 0.0 )
    {
        s = sqrt( tr + 1.0 );
        q.p[0] = ( s * 0.5 );
        s = 0.5 / s;
        q.p[1] = ( m.getValue(1,2) - m.getValue(2,1) ) * s;
        q.p[2] = ( m.getValue(2,0) - m.getValue(0,2) ) * s;
        q.p[3] = ( m.getValue(0,1) - m.getValue(1,0) ) * s;
    }
    else
    {
        i = 0;
        if ( m.getValue(1,1) > m.getValue(0,0) ) i = 1;
        if ( m.getValue(2,2) > m.getValue(i,i) ) i = 2;

        j = next[i];
        k = next[j];

        s = sqrt( (m.getValue(i,i)
					- (m.getValue(j,j) + m.getValue(k,k))) + 1.0 );
        q.p[i+1] = s * 0.5;
        s = 0.5 / s;
        q.p[0]   = ( m.getValue(j,k) - m.getValue(k,j) ) * s;
        q.p[j+1] = ( m.getValue(i,j) + m.getValue(j,i) ) * s;
        q.p[k+1] = ( m.getValue(i,k) + m.getValue(k,i) ) * s;
    }

    (*this) = q;
}

double
math::quater::length() const
{
    return sqrt( p[0]*p[0] + p[1]*p[1] + p[2]*p[2] + p[3]*p[3] );
}

quater
math::quater::normalize() const
{
	return (*this)/this->length();
}

// friend functions
quater math::operator-(quater const& a)
{
    return quater( -a.p[0], -a.p[1], -a.p[2], -a.p[3] );
}

quater math::inverse(quater const& a)
{
    return quater( a.p[0], -a.p[1], -a.p[2], -a.p[3] );
}

quater math::operator+ (quater const& a, quater const& b)
{
    quater c;
    for(int i = 0; i < 4; i++)
        c.p[i] = a.p[i] + b.p[i];
    return c;
}

quater math::operator- (quater const& a, quater const& b)
{
    quater c;
    for(int i = 0; i < 4; i++)
        c.p[i] = a.p[i] - b.p[i];
    return c;
}

quater math::operator* (double a, quater const& b)
{
    quater c;
    for(int i = 0; i < 4; i++)
        c.p[i] = a * b.p[i];
    return c;
}

quater math::operator* (quater const& a, double b)
{
    quater c;
    for(int i = 0; i < 4; i++)
        c.p[i] = a.p[i] * b;
    return c;
}

quater math::operator/ (quater const& a, double b) {
    quater c;
    for(int i = 0; i < 4; i++)
        c.p[i] = a.p[i] / b;
    return c;
}

double math::operator% (quater const& a, quater const& b)
{
    double c = 0;
    for(int i = 0; i < 4; i++)
        c += a.p[i] * b.p[i];
    return c;
}

quater math::operator* (quater const& a, quater const& b)
{
    quater c;
    c.p[0] = a.p[0]*b.p[0] - a.p[1]*b.p[1] - a.p[2]*b.p[2] - a.p[3]*b.p[3];
    c.p[1] = a.p[0]*b.p[1] + a.p[1]*b.p[0] + a.p[2]*b.p[3] - a.p[3]*b.p[2];
    c.p[2] = a.p[0]*b.p[2] + a.p[2]*b.p[0] + a.p[3]*b.p[1] - a.p[1]*b.p[3];
    c.p[3] = a.p[0]*b.p[3] + a.p[3]*b.p[0] + a.p[1]*b.p[2] - a.p[2]*b.p[1];
    return c;
}

matrix math::Quater2Matrix( quater const& q )
{
    matrix m;
    double s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

    s  = 2.0/len(q);
    xs = q.x() * s;  ys = q.y() * s;  zs = q.z() * s;
    wx = q.w() * xs; wy = q.w() * ys; wz = q.w() * zs;
    xx = q.x() * xs; xy = q.x() * ys; xz = q.x() * zs;
    yy = q.y() * ys; yz = q.y() * zs; zz = q.z() * zs;

    m.p[0][0] = 1.0 - (yy + zz);
    m.p[1][0] = xy - wz;
    m.p[2][0] = xz + wy;
    m.p[0][1] = xy + wz;
    m.p[1][1] = 1.0 - (xx + zz);
    m.p[2][1] = yz - wx;
    m.p[0][2] = xz - wy;
    m.p[1][2] = yz + wx;
    m.p[2][2] = 1.0 - (xx + yy);

    return m;
}

quater math::Matrix2Quater( matrix const& m )
{
    quater q;

    double tr, s;
    int    i, j, k;
    static int next[3] = { 1, 2, 0 };

    tr = m.getValue(0,0) + m.getValue(1,1) + m.getValue(2,2);
    if ( tr > 0.0 )
    {
        s = sqrt( tr + 1.0 );
        q.p[0] = ( s * 0.5 );
        s = 0.5 / s;
        q.p[1] = ( m.getValue(1,2) - m.getValue(2,1) ) * s;
        q.p[2] = ( m.getValue(2,0) - m.getValue(0,2) ) * s;
        q.p[3] = ( m.getValue(0,1) - m.getValue(1,0) ) * s;
    }
    else
    {
        i = 0;
        if ( m.getValue(1,1) > m.getValue(0,0) ) i = 1;
        if ( m.getValue(2,2) > m.getValue(i,i) ) i = 2;

        j = next[i];
        k = next[j];

        s = sqrt( (m.getValue(i,i)
					- (m.getValue(j,j) + m.getValue(k,k))) + 1.0 );
        q.p[i+1] = s * 0.5;
        s = 0.5 / s;
        q.p[0]   = ( m.getValue(j,k) - m.getValue(k,j) ) * s;
        q.p[j+1] = ( m.getValue(i,j) + m.getValue(j,i) ) * s;
        q.p[k+1] = ( m.getValue(i,k) + m.getValue(k,i) ) * s;
    }

    return q;
}

vector math::Quater2EulerAngle( quater const& q )
{
	return Matrix2EulerAngle( Quater2Matrix( q ) );
}

quater math::EulerAngle2Quater( vector const& v )
{
	return Matrix2Quater( EulerAngle2Matrix( v ) );
}

double math::len( quater const& v )
{
    return sqrt( v.p[0]*v.p[0] + v.p[1]*v.p[1] + v.p[2]*v.p[2] + v.p[3]*v.p[3] );
}

ostream& math::operator<<( ostream& os, quater const& a )
{
    os << "( " << a.p[0] << " , " << a.p[1] << " , " << a.p[2] << " , " << a.p[3] << " )";
    return os;
}

istream& math::operator>>( istream& is, quater& a )
{
	static char	buf[256];
	//is >> "(" >> a.p[0] >> "," >> a.p[1] >> "," >> a.p[2] >> "," >> a.p[3] >> ")";
	is >> buf >> a.p[0] >> buf >> a.p[1] >> buf >> a.p[2] >> buf >> a.p[3] >> buf;
    return is;
}

quater math::exp(vector const& w)
{
    double theta = sqrt(w % w);
    double sc;

    if(theta < eps) sc = 1;
    else sc = sin(theta) / theta;

    vector v = sc * w;
    return quater(cos(theta), v.x(), v.y(), v.z());
}

quater math::pow(vector const& w, double a)
{
    return exp(a * w);
}

vector math::ln(quater const& q)
{
    double sc = sqrt(q.p[1] * q.p[1] + q.p[2] * q.p[2] + q.p[3] * q.p[3]);
    double theta = atan2(sc, q.p[0]);
    if(sc > eps)
        sc = theta / sc;
    else  sc = 1.0 ;
    return vector(sc * q.p[1], sc * q.p[2], sc * q.p[3]);
}

position math::rotate(quater const& a, position const& v)
{
    quater c = a * quater(0, v.x(), v.y(), v.z()) * inverse(a);
    return position(c.x(), c.y(), c.z());
}

vector math::rotate(quater const& a, vector const& v)
{
    quater c = a * quater(0, v.x(), v.y(), v.z()) * inverse(a);
    return vector(c.x(), c.y(), c.z());
}

unit_vector math::rotate(quater const& a, unit_vector const& v)
{
    quater c = a * quater(0, v.x(), v.y(), v.z()) * inverse(a);
    return unit_vector(c.x(), c.y(), c.z());
}

quater math::slerp( quater const& a, quater const& b, double t )
{
	double c = a % b;

	if ( 1.0+c > EPS )
	{
		if ( 1.0-c > EPS )
		{
			double theta = acos( c );
			double sinom = sin( theta );
			return ( a*sin((1-t)*theta) + b*sin(t*theta) ) / sinom;
		}
		else
			return (a*(1-t) + b*t).normalize();
	}
	else	return a*sin((0.5-t)*M_PI) + b*sin(t*M_PI);
}

quater math::interpolate( double t, quater const& a, quater const& b )
{
	return slerp( a, b, t );
}

double math::distance( quater const& a, quater const& b )
{
	return MIN( ln( a.inverse()* b).length(),
				ln( a.inverse()*-b).length() );
}

vector math::difference( quater const& a, quater const& b )
{
	return ln( b.inverse() * a );
}

//
bool math::operator==( const quater& a, const quater& b )
{
	if( a.w() == b.w() && a.x() == b.x() && a.y() == b.y() && a.z() == b.z() )	return true;
	return false;
}

