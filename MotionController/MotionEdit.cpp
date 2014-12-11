#include "MotionEdit.h"
#include "SkeletalMotion.h"
#include "MotionData.h"
#include "PoseData.h"
#include "PoseIK.h"
#include "Skeleton.h"
#include "Joint.h"

#include <vector>
#include "mathclass/QmApproximate.h"

#define DEFAULT_NUM_LEVELS_OF_HIERARCHY		3
#define ERROR_TOLERANCE						0.01


MotionEdit::MotionEdit()
{
	input_motion = 0;
	output_motion = 0;
	num_levels_of_hierarchy = DEFAULT_NUM_LEVELS_OF_HIERARCHY;
}

MotionEdit::~MotionEdit()
{
	std::map< unsigned int, PoseConstraint* >::iterator itor = pose_constraints.begin();
	while( itor != pose_constraints.end() )
	{
		PoseConstraint* pose_constraint = itor->second;
		delete pose_constraint;

		itor ++;
	}
	pose_constraints.clear();
}

void MotionEdit::run()
{
	if( !input_motion || !output_motion )
	{
		return;
	}

	if( getNumConstraints() == 0 )
	{
		output_motion->copyFrom( input_motion );
	}
	else
	{
		hierarchicalEdit();
	}
}

void MotionEdit::hierarchicalEdit()
{
	unsigned int num_frames = input_motion->getNumFrames();
	unsigned int num_joints = input_motion->getNumJoints();
	Skeleton* skeleton = input_motion->getSkeleton();
	
	std::vector< SkeletalMotion* > motions;
	QmApproximate appr;
	std::vector< QmScatteredData > disp_data;
	math::vector* disp_result = new math::vector[ num_frames ];

	// initial guess
	SkeletalMotion* init_motion = new SkeletalMotion;
	init_motion->copyFrom( input_motion );
	motions.push_back( init_motion );

	unsigned int h = num_levels_of_hierarchy;
	unsigned int num_knots = num_frames / 10;	// <-- ???
	for( unsigned int k=1; k < h; k++, num_knots*=2 )
	{
		// initialize lower-level & current-level motion
		SkeletalMotion* lo_motion = motions[ k-1 ];
		SkeletalMotion* hi_motion = new SkeletalMotion;
		hi_motion->copyFrom( lo_motion );
		motions.push_back( hi_motion );

		// solve IK from lower-level motion to current-level motion using constraints
		std::map< unsigned int, PoseConstraint* >::iterator itor_pc = pose_constraints.begin();
		while( itor_pc != pose_constraints.end() )
		{
			unsigned int frame_index = itor_pc->first;
			PoseConstraint* pose_constraint = itor_pc->second;
			itor_pc ++;

			PoseData* lo_pose = lo_motion->getPoseData( frame_index );
			PoseData* hi_pose = hi_motion->getPoseData( frame_index );

			PoseIK::ik_body( skeleton, lo_pose, hi_pose, pose_constraint, 0 );
		}

		// handle translation
		unsigned int f = 0;

		disp_data.clear();
		for( f=0; f < num_frames; f++ )
		{
			PoseData* hi_pose = hi_motion->getPoseData( f );
			PoseData* lo_pose = lo_motion->getPoseData( f );

			math::vector d = 
				hi_pose->getJointTransform( 0 ).translation - 
				lo_pose->getJointTransform( 0 ).translation;

			if( d.length() > ERROR_TOLERANCE )
			{
				disp_data.push_back( QmScatteredData(f,d) );
			}
		}
		appr.setSize( num_knots );
		appr.approximate( num_frames, disp_data );
		appr.discretize( num_frames, disp_result );
		for( f=0; f < num_frames; f++ )
		{
			PoseData* hi_pose = hi_motion->getPoseData( f );
			PoseData* lo_pose = lo_motion->getPoseData( f );

			math::transq loT = lo_pose->getJointTransform( 0 );
			math::transq hiT( loT.rotation, loT.translation + disp_result[f] );

			hi_pose->setJointTransform( 0, hiT );
		}

		// handle rotation
		for( int j=0; j < NUM_JOINTS_IN_HUMAN; j++ )
		{
			Joint* hi_joint = hi_motion->getHumanJoint( j );
			Joint* lo_joint = lo_motion->getHumanJoint( j );

			if( lo_joint && hi_joint )
			{
				disp_data.clear();
				for( f=0; f < num_frames; f++ )
				{
					PoseData* hi_pose = hi_motion->getPoseData( f );
					PoseData* lo_pose = lo_motion->getPoseData( f );

					math::transq hiT = hi_pose->getJointTransform( hi_joint->getIndex() );
					math::transq loT = lo_pose->getJointTransform( lo_joint->getIndex() );

					math::quater q1 = hiT.rotation;
					math::quater q2 = loT.rotation;

					math::vector d;
					if( q1%q2 < 0 )	d = math::difference( q1, -q2 );
					else			d = math::difference( q1, q2 );

					if( d.length() > ERROR_TOLERANCE )
					{
						disp_data.push_back( QmScatteredData(f,d) );
					}
				}
				appr.setSize( num_knots );
				appr.approximate( num_frames, disp_data );
				appr.discretize( num_frames, disp_result );

				for( f=0; f < num_frames; f++ )
				{
					PoseData* hi_pose = hi_motion->getPoseData( f );
					PoseData* lo_pose = lo_motion->getPoseData( f );

					math::transq loT = lo_pose->getJointTransform( lo_joint->getIndex() );
					math::transq hiT( loT.rotation * math::exp( disp_result[f] ), loT.translation );

					hi_pose->setJointTransform( hi_joint->getIndex(), hiT );
				}
			}
		}
	}
	output_motion->copyFrom( motions[h-1] );

	// deallocate memory
	std::vector< SkeletalMotion* >::iterator itor_m = motions.begin();
	while( itor_m != motions.end() )
	{
		SkeletalMotion* m = ( *itor_m ++ );
		delete m;
	}
	delete[] disp_result;
}

bool MotionEdit::isConstrained( unsigned int frame )
{
	std::map< unsigned int, PoseConstraint* >::iterator itor = pose_constraints.find( frame );
	if( itor != pose_constraints.end() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool MotionEdit::isConstrained( unsigned int frame, unsigned int joint )
{
	std::map< unsigned int, PoseConstraint* >::iterator itor = pose_constraints.find( frame );
	if( itor != pose_constraints.end() )
	{
		PoseConstraint* pose_constraint = itor->second;
		return pose_constraint->isConstrained( joint );
	}
	else
	{
		return false;
	}
}

bool MotionEdit::addConstraint( unsigned int frame, unsigned int joint, const math::vector& position )
{
	std::map< unsigned int, PoseConstraint* >::iterator itor = pose_constraints.find( frame );
	if( itor != pose_constraints.end() )
	{
		PoseConstraint* pos_constraint = itor->second;
		return pos_constraint->addConstraint( joint, position );
	}
	else
	{
		PoseConstraint* pos_constraint = new PoseConstraint;
		pos_constraint->addConstraint( joint, position );
		
		pose_constraints[ frame ] = pos_constraint;

		return true;
	}
}

bool MotionEdit::addConstraint( unsigned int frame, unsigned int joint, const math::quater& orientation )
{
	std::map< unsigned int, PoseConstraint* >::iterator itor = pose_constraints.find( frame );
	if( itor != pose_constraints.end() )
	{
		PoseConstraint* orient_constraint = new PoseConstraint;
		return orient_constraint->addConstraint( joint, orientation );
	}
	else
	{
		PoseConstraint* orient_constraint = new PoseConstraint;
		orient_constraint->addConstraint( joint, orientation );

		pose_constraints[ frame ] = orient_constraint;

		return true;
	}
}

bool MotionEdit::addConstraint( unsigned int frame, unsigned int joint, const math::transq& transform )
{
	std::map< unsigned int, PoseConstraint* >::iterator itor = pose_constraints.find( frame );
	if( itor != pose_constraints.end() )
	{
		PoseConstraint* transq_constraint = itor->second;
		return transq_constraint->addConstraint( joint, transform );
	}
	else
	{
		PoseConstraint* transq_constraint = new PoseConstraint;
		transq_constraint->addConstraint( joint, transform );

		pose_constraints[ frame ] = transq_constraint;

		return true;
	}
}

bool MotionEdit::getConstraint( unsigned int frame, PoseConstraint** pose_constraint )
{
	std::map< unsigned int, PoseConstraint* >::iterator itor = pose_constraints.find( frame );
	if( itor != pose_constraints.end() )
	{
		*pose_constraint = itor->second;

		return true;
	}
	else
	{
		return false;
	}
}

bool MotionEdit::getConstraint( unsigned int frame, unsigned int joint, math::transq* transform )
{
	std::map< unsigned int, PoseConstraint* >::iterator itor = pose_constraints.find( frame );
	if( itor != pose_constraints.end() )
	{
		PoseConstraint* pose_constraint = itor->second;
		return pose_constraint->getConstraint( joint, transform );
	}
	else
	{
		return false;
	}
}

unsigned int MotionEdit::getNumConstraints()
{
	return (unsigned int)pose_constraints.size();
}
