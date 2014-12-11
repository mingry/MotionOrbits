#include "QmGeodesic.h"

using namespace math;

quater
QmGeodesic::getValue( double t )
{
	return exp( t*axis/2.0 ) * orientation;
}

quater
QmGeodesic::nearest( quater const& q, double& t )
{
	double ws = q.real();
	vector vs = q.imaginaries();
	double w0 = orientation.real();
	vector v0 = orientation.imaginaries();

	double a = ws*w0 + vs%v0;
	double b = w0*(axis % vs) - ws*(axis % v0) + vs%(axis * v0);

	double alpha = atan2( a,b );

	double t1 = -2*alpha + M_PI;
	double t2 = -2*alpha - M_PI;

	if ( q % getValue(t1) > q % getValue(t2) )
	{
		t = t1;
		return getValue(t1);
	}

	t = t2;
	return getValue(t2);
}

quater
QmGeodesic::farthest( quater const& q, double& t )
{
	double ws = q.real();
	vector vs = q.imaginaries();
	double w0 = orientation.real();
	vector v0 = orientation.imaginaries();

	double a = ws*w0 + vs%v0;
	double b = w0*(axis % vs) - ws*(axis % v0) + vs%(axis * v0);

	double alpha = atan2( a,b );

	double t1 = -alpha + M_PI/2.0;
	double t2 = -alpha - M_PI/2.0;

	if ( q % getValue(t1) > q % getValue(t2) )
	{
		t = t2;
		return getValue(t2);
	}

	t = t1;
	return getValue(t1);
}

quater
QmGeodesic::nearest( quater const& q )
{
	double t;
	return this->nearest( q, t );
}

quater
QmGeodesic::farthest( quater const& q )
{
	double t;
	return this->farthest( q, t );
}

transf
PlaneProject( transf const& t )
{
	QmGeodesic g( quater(1,0,0,0), y_axis );
	vector v = t.translation();

	return transf( g.nearest( t.getRotation() ),
				   vector(v[0], 0, v[2]) );
}
