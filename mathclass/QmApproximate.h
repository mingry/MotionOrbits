#pragma once

#include <vector>
#include "vector.h"

class QmScatteredData
{
public:
	QmScatteredData() {}
	~QmScatteredData() {}

	QmScatteredData( const QmScatteredData& sd ) : position(sd.position), value(sd.value) {}
	QmScatteredData( int p, const math::vector& v ): position(p), value(v) {}

	QmScatteredData& operator=( const QmScatteredData& sd )
	{
		position = sd.position;
		value = sd.value;
		return *this;
	}
	bool operator==( const QmScatteredData& sd )
	{
		if( position==sd.position && value==sd.value )	return true;
		return false;
	}

	int				position;
	math::vector	value;
};

class QmApproximate
{
private:
	int				num;
	math::vector*	cp;

public:
	QmApproximate() { num=0; cp=NULL; }

	int		getSize() const { return num; }
	void	setSize( int n );

	void 			smoothBoundary();
	math::vector	evaluate( double ) const;
	void			discretize( int, math::vector* );
	void			approximate( int, std::vector< QmScatteredData >& data_list );
	void			approximateLeastSquare( int, std::vector< QmScatteredData >& data_list );

	static double B0( double t ) { return (1.0-t)*(1.0-t)*(1.0-t) / 6.0; }
	static double B1( double t ) { return (3.0*t*t*t - 6.0*t*t + 4.0) / 6.0; }
	static double B2( double t ) { return (-3.0*t*t*t + 3.0*t*t + 3.0*t + 1.0) / 6.0; }
	static double B3( double t ) { return t*t*t / 6.0; }

	static double DB0( double t ) { return -(1.0-t)*(1.0-t) / 2.0; }
	static double DB1( double t ) { return (3.0*t*t - 4.0*t) / 2.0; }
	static double DB2( double t ) { return (-3.0*t*t + 2.0*t + 1.0) / 2.0; }
	static double DB3( double t ) { return t*t / 2.0; }
};

