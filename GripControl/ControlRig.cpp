#include "ControlRig.h"

ControlRig::ControlRig()
{
	la_bias = math::vector(0,0,0);
	av_bias = math::vector(0,0,0);
	filter_weight = 0.1;

	reset();
}

ControlRig::~ControlRig()
{
}

void ControlRig::reset()
{
	pos = math::position(0,0,0);
	lv = math::vector(0,0,0);
	la = math::vector(0,0,0);

	ori = math::quater(1,0,0,0);
	av = math::vector(0,0,0);

	pos_trajectory.clear();
	ori_trajectory.clear();

	biased_la_samples.clear();
	biased_av_samples.clear();

	last_time = 0;
	is_first = true;
}

void ControlRig::evaluateLinearAccelerationBias()
{
	math::vector sum(0,0,0);

	std::vector< math::vector >::iterator itor_b = biased_la_samples.begin();
	while( itor_b != biased_la_samples.end() )
	{
		sum += ( *itor_b ++ );
	}

	la_bias = sum / (double)biased_la_samples.size();
	biased_la_samples.clear();
}

void ControlRig::evaluateAngularVelocityBias()
{
	math::vector sum(0,0,0);

	std::vector< math::vector >::iterator itor_b = biased_av_samples.begin();
	while( itor_b != biased_av_samples.end() )
	{
		sum += ( *itor_b ++ );
	}

	av_bias = sum / (double)biased_av_samples.size();
	biased_av_samples.clear();
}

void ControlRig::applyMotion( math::vector& a, math::vector& w, unsigned int curr_time )
{
	unbiasLinearAcceleration( a );
	unbiasAngularVelocity( w );

	if( is_first )
	{
		is_first = false;

		la = a;
		av = w;
	}
	else
	{
		float dt = (float)( curr_time - last_time ) / 1000;

		globalize( a );
		globalize( w );

		filterLinearAcceleration( a );
		filterAngularVelocity( w );

		// LINEAR INTEGRATION
		la = a;
		lv = lv + la * dt;
		pos = pos + lv * dt;

		// ANGULAR INTEGRATION
		av = w;

		float scale_factor = 1.5f;
		math::quater q( 0, av.x(), av.y(), av.z() );

		ori = ori + (0.5 * q * ori) * dt * scale_factor;
		ori = ori.normalize();
	}

	last_time = curr_time;
}

void ControlRig::globalize( math::vector& v )
{
	math::quater rot_local_to_global = ori;
	v = math::rotate( rot_local_to_global, v );
}
