#pragma once

#include <vector>
#include "mathclass/transq.h"

class PoseData;

class MotionData
{
public:
	MotionData();
	virtual ~MotionData();

	void initialize( unsigned int num_frames, unsigned int num_joints );
	void finalize();
	void copyFrom( MotionData* source_data );

	void addPoseData( PoseData* pose );
	void setPoseData( unsigned int f, PoseData* pose );
	PoseData* getPoseData( unsigned int f );

	void setJointTransform( unsigned int f, unsigned int i, const math::transq& t );
	math::transq getJointTransform( unsigned int f, unsigned int i );

	inline unsigned int getNumFrames()	{ return num_frames; }
	inline unsigned int getNumJoints()	{ return num_joints; }

protected:
	unsigned int num_frames;
	unsigned int num_joints;

	std::vector< PoseData* >	pose_sequence;
};
