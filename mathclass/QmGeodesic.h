#ifndef __QM_GEODESIC_H
#define __QM_GEODESIC_H

#include "unit_vector.h"
#include "vector.h"
#include "quater.h"
#include "transf.h"

class QmGeodesic
{
private:
	math::quater		orientation;
	math::unit_vector	axis;

public:
	//
	// Constructors
	//
	QmGeodesic() {}
	QmGeodesic( math::quater const& q, math::unit_vector const& v )
		{ orientation=q; axis=v; }

	//
	// Evaluate points on geodesic
	//
	math::quater	getValue( double );

	//
	// Set and get parameters
	//
    inline void					setOrientation( math::quater const& q ) { orientation=q; }
    inline void					setAxis( math::unit_vector const& v )   { axis=v; }
    inline math::quater			getOrientation() { return orientation; }
    inline math::unit_vector	getAxis()        { return axis; }

	//
	// Metric functions
	//
	math::quater nearest( math::quater const&, double& );
	math::quater farthest( math::quater const&, double& );
	
	math::quater nearest( math::quater const& );
	math::quater farthest( math::quater const& );
};

math::transf PlaneProject( math::transf const& );

#endif
