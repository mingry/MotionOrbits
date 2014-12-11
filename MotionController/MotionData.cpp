#include "MotionData.h"
#include "PoseData.h"

#include <cassert>

MotionData::MotionData()
{
	num_frames = 0;
	num_joints = 0;
}

MotionData::~MotionData()
{
	finalize();
}

void MotionData::initialize( unsigned int num_frames, unsigned int num_joints )
{
	finalize();

	unsigned int f;
	for( f=0; f < num_frames; f++ )
	{
		PoseData* pose = new PoseData;
		pose->initialize( num_joints );
		pose_sequence.push_back( pose );
	}

	this->num_frames = num_frames;
	this->num_joints = num_joints;
}

void MotionData::finalize()
{
	std::vector< PoseData* >::iterator itor_p = pose_sequence.begin();
	while( itor_p != pose_sequence.end() )
	{
		PoseData* pose = ( *itor_p ++ );
		delete pose;
	}
	pose_sequence.clear();

	num_frames = 0;
	num_joints = 0;
}

void MotionData::copyFrom( MotionData* source_data )
{
	unsigned int source_num_frames = source_data->getNumFrames();
	unsigned int source_num_joints = source_data->getNumJoints();

	if( source_num_frames == 0 || source_num_joints == 0 )
	{
		return;
	}

	this->initialize( source_num_frames, source_num_joints );
	
	unsigned int f;
	for( f=0; f < source_num_frames; f++ )
	{
		this->setPoseData( f, source_data->getPoseData( f ) );
	}
}

void MotionData::addPoseData( PoseData* pose )
{
	if( num_joints != pose->getNumJoints() )	return;

	PoseData* new_pose = new PoseData;
	new_pose->initialize( num_joints );
	new_pose->copy( pose );
	pose_sequence.push_back( new_pose );

	num_frames ++;
}

void MotionData::setPoseData( unsigned int f, PoseData* pose )
{
	if( f < num_frames )
	{
		pose_sequence[ f ]->copy( pose );
	}
	else
	{
		assert( false );
	}
}

PoseData* MotionData::getPoseData( unsigned int f )
{
	if( f < num_frames )
	{
		return pose_sequence[ f ];
	}
	else
	{
		assert( false );
		return 0;
	}
}

void MotionData::setJointTransform( unsigned int f, unsigned int i, const math::transq& t )
{
	if( f < num_frames )
	{
		pose_sequence[ f ]->setJointTransform( i, t );
	}
	else
	{
		assert( false );
	}
}

math::transq MotionData::getJointTransform( unsigned int f, unsigned int i )
{
	if( f < num_frames )
	{
		return pose_sequence[ f ]->getJointTransform( i );
	}
	else
	{
		assert( false );
		return math::identity_transq;
	}
}
