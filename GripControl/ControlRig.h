#pragma once

#include "mathclass/position.h"
#include "mathclass/vector.h"
#include "mathclass/quater.h"

#include <vector>

class ControlRig
{
public:
	ControlRig();
	virtual ~ControlRig();

	void reset();
	
	void evaluateLinearAccelerationBias();
	void evaluateAngularVelocityBias();

	void applyMotion( math::vector& a, math::vector& w, unsigned int curr_time );

	inline void collectBiasedLinearAcceleration( math::vector& a )	{ biased_la_samples.push_back( a ); }
	inline void collectBiasedAngularVelocity( math::vector& w )		{ biased_av_samples.push_back( w ); }

	inline math::position& getPosition()			{ return pos; }
	inline math::vector& getLinearVelocity()		{ return lv; }
	inline math::vector& getLinearAcceleration()	{ return la; }

	inline math::quater& getOrientation()			{ return ori; }
	inline math::vector& getAngularVelocity()		{ return av; }

	inline void capturePosition()		{ pos_trajectory.push_back( pos ); }
	inline void captureOrientation()	{ ori_trajectory.push_back( ori ); }

	inline std::vector< math::position >* getPositionTrajectory()	{ return &pos_trajectory; }
	inline std::vector< math::quater >* getOrientationTrajectory()	{ return &ori_trajectory; }

	inline void setLinearAccelerationBias( math::vector& b )	{ la_bias = b; }
	inline void setAngularVelocityBias( math::vector& b )		{ av_bias = b; }

	inline math::vector& getLinearAccelerationBias()	{ return la_bias; }
	inline math::vector& getAngularVelocityBias()		{ return av_bias; }

	inline void unbiasLinearAcceleration( math::vector& a )	{ a -= la_bias; }
	inline void unbiasAngularVelocity( math::vector& w )	{ w -= av_bias; }

	inline void filterLinearAcceleration( math::vector& a )	{ a = math::interpolate( filter_weight, la, a ); }
	inline void filterAngularVelocity( math::vector& w )	{ w = math::interpolate( filter_weight, av, w ); }

	inline void setFilterWeight( float t )	{ filter_weight = t; }
	inline float getFilterWeight( float t )	{ return filter_weight; }

	inline unsigned int getLastTime()	{ return last_time; }

protected:
	void globalize( math::vector& v );

	float filter_weight;	// [0, 1]

	math::position	pos;
	math::vector	lv;
	math::vector	la;
	math::vector	la_bias;

	math::quater	ori;
	math::vector	av;
	math::vector	av_bias;

	std::vector< math::vector >		biased_la_samples;
	std::vector< math::vector >		biased_av_samples;

	std::vector< math::position >	pos_trajectory;
	std::vector< math::quater >		ori_trajectory;

	bool			is_first;
	unsigned int	last_time;
};
