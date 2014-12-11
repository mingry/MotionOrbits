#pragma once

#include <map>

#include "mathclass/vector.h"
#include "mathclass/quater.h"
#include "mathclass/transq.h"

#include "SkeletalMotion.h"
#include "PoseConstraint.h"

class MotionEdit
{
public:
	MotionEdit();
	virtual ~MotionEdit();

	inline void setInputMotion( SkeletalMotion* im )	{ input_motion = im; }
	inline void setOutputMotion( SkeletalMotion* om )	{ output_motion = om; }
	
	inline SkeletalMotion* getInputMotion()			{ return input_motion; }
	inline SkeletalMotion* getOutputMotion()		{ return output_motion; }

	void run();

	bool isConstrained( unsigned int frame );
	bool isConstrained( unsigned int frame, unsigned int joint );

	bool addConstraint( unsigned int frame, unsigned int joint, const math::vector& position );
	bool addConstraint( unsigned int frame, unsigned int joint, const math::quater& orientation );
	bool addConstraint( unsigned int frame, unsigned int joint, const math::transq& transform );

	bool getConstraint( unsigned int frame, PoseConstraint** pose_constraint );
	bool getConstraint( unsigned int frame, unsigned int joint, math::transq* transform );

	unsigned int getNumConstraints();
	void clearConstraints();


protected:
	void hierarchicalEdit();

	SkeletalMotion* input_motion;
	SkeletalMotion* output_motion;

	unsigned int num_levels_of_hierarchy;

	std::map< unsigned int, PoseConstraint* > pose_constraints;
};