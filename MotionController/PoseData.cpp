#include "PoseData.h"
#include "Skeleton.h"
#include "Joint.h"
#include "Human.h"

#include "mathclass/QmGeodesic.h"

#include <cassert>

PoseData::PoseData()
{
	num_joints = 0;
	joint_transforms = 0;
}

PoseData::~PoseData()
{
	finalize();
}

void PoseData::initialize( unsigned int num_joints )
{
	if( num_joints > 0 )
	{
		finalize();

		this->num_joints = num_joints;
		joint_transforms = new math::transq[ num_joints ];
		
		unsigned int i;
		for( i=0; i < num_joints; i++ )
		{
			setJointTransform( i, math::identity_transq );
		}
	}
	else
	{
		assert( false );
	}
}

void PoseData::finalize()
{
	delete[] joint_transforms;
	joint_transforms = 0;
	num_joints = 0;
}

void PoseData::setJointTransform( unsigned int i, const math::transq& t )
{
	if( i < num_joints )
	{
		joint_transforms[ i ] = t;
	}
	else
	{
		assert( false );
	}
}

math::transq PoseData::getJointTransform( unsigned int i )
{
	if( i < num_joints )
	{
		return joint_transforms[ i ];
	}
	else
	{
		assert( false );
		return math::identity_transq;
	}
}

void PoseData::transform( const math::transq& t )
{
	setJointTransform( 0, t * getJointTransform(0) );
}

bool PoseData::copy( PoseData* pose )
{
	if( this->num_joints != pose->num_joints )
	{
		return false;
	}

	unsigned int i;
	for( i=0; i < num_joints; i++ )
	{
		this->setJointTransform( i, pose->getJointTransform(i) );
	}
	return true;
}

bool PoseData::blend( PoseData* pose1, PoseData* pose2, double ratio )
{
	if( this->num_joints != pose1->num_joints ||
		this->num_joints != pose2->num_joints )
	{
		return false;
	}

	// 1. alignment
	math::transq disp = PoseData::calcDisplacement( pose1, pose2 );

	// 2. warping
	math::transq src_t = pose1->getJointTransform( 0 );
	math::transq tgt_t = disp * pose2->getJointTransform( 0 );

	if( src_t.rotation % tgt_t.rotation < 0 )		tgt_t.rotation = -tgt_t.rotation;

	math::transq warp_t = math::interpolate( ratio, src_t, tgt_t );
	this->setJointTransform( 0, warp_t );

	for( unsigned int j=1; j < num_joints; j++ )
	{
		math::transq src_t = pose1->getJointTransform( j );
		math::transq tgt_t = pose2->getJointTransform( j );

		if( src_t.rotation % tgt_t.rotation < 0 )		tgt_t.rotation = -tgt_t.rotation;
	
		math::transq warp_t = math::interpolate( ratio, src_t, tgt_t );

		this->setJointTransform( j, warp_t );
	}
	return true;
}

bool PoseData::blend( PoseData* pose1, math::transq& T1, PoseData* pose2, math::transq& T2, double ratio )
{
	if( this->num_joints != pose1->num_joints ||
		this->num_joints != pose2->num_joints )
	{
		return false;
	}

	math::transq src_t = T1 * pose1->getJointTransform( 0 );
	math::transq tgt_t = T2 * pose2->getJointTransform( 0 );

	if( src_t.rotation % tgt_t.rotation < 0 )		tgt_t.rotation = -tgt_t.rotation;

	math::transq warp_t = math::interpolate( ratio, src_t, tgt_t );
	this->setJointTransform( 0, warp_t );

	for( unsigned int j=1; j < num_joints; j++ )
	{
		math::transq src_t = pose1->getJointTransform( j );
		math::transq tgt_t = pose2->getJointTransform( j );

		if( src_t.rotation % tgt_t.rotation < 0 )		tgt_t.rotation = -tgt_t.rotation;
	
		math::transq warp_t = math::interpolate( ratio, src_t, tgt_t );

		this->setJointTransform( j, warp_t );
	}
	return true;
}

bool PoseData::addDisplacement( Skeleton* skeleton, const math::vectorN& d )
{
	unsigned int num_joints = skeleton->getNumJoints();
	if( this->num_joints != num_joints )
	{
		return false;
	}

	unsigned int total_dof = skeleton->calcDOF();
	if( total_dof != d.size() )
	{
		return false;
	}

	int i, j = 0;
	math::vector p, v;

	for( i=0; i < NUM_JOINTS_IN_HUMAN; i++ )
	{
		Joint* joint = skeleton->getHumanJoint( i );
		if( joint )
		{
			unsigned int id = joint->getIndex();
			unsigned int dof = joint->getDOF();

			switch( dof ) {
			case 6:
				{
					p = math::vector( d[j  ], d[j+1], d[j+2] );
					v = math::vector( d[j+3], d[j+4], d[j+5] );					
					math::vector t = joint_transforms[ id ].translation;
					math::quater r = joint_transforms[ id ].rotation;
					joint_transforms[ id ].translation = t + p;
					joint_transforms[ id ].rotation = r * math::exp(v);
				}
				break;

			case 3:
				{
					v = math::vector( d[j], d[j+1], d[j+2] );				
					math::quater r = joint_transforms[ id ].rotation;
					joint_transforms[ id ].rotation = r * math::exp(v);
				}
				break;

			case 1:
				{
					v = math::vector( d[j], 0, 0 );				
					math::quater r = joint_transforms[ id ].rotation;
					joint_transforms[ id ].rotation = r * math::exp(v);
				}
				break;
			}
			j += dof;
		}
	}
	return true;
}

math::transq PoseData::getGlobalTransform( Skeleton* skeleton, unsigned int joint_index, const math::transq& body_transform )
{
	math::transq joint_transform = math::identity_transq;

	Joint* joint = skeleton->getJointByIndex( joint_index );
	while( joint )
	{
		math::vector joint_offset = joint->getOffset();
		math::transq local_translation = math::translate_transq( joint_offset );
		math::transq local_rotation = joint_transforms[ joint_index ];
		math::transq local_transform = local_translation * local_rotation;
		
		joint_transform = local_transform * joint_transform;	// child * parent

		joint = joint->getParent();
		if( joint )
		{
			joint_index = joint->getIndex();
		}
	}
	math::transq global_transform = body_transform * joint_transform;
	return global_transform;
}

math::position PoseData::getPosition( Skeleton* skeleton, unsigned int i )
{
	math::position p(0,0,0);
	math::transq t = getGlobalTransform( skeleton, i );
	p *= t;

	return p;
}

math::quater PoseData::getOrientation( Skeleton* skeleton, unsigned int i )
{
	math::transq t = getGlobalTransform( skeleton, i );
	return t.rotation;
}

math::transq PoseData::calcDisplacement( PoseData* p1, PoseData* p2 )
{
	math::transq t1 = p1->getJointTransform( 0 );
	math::transq t2 = p2->getJointTransform( 0 );

	math::vector v1 = t1.translation;
	math::vector v2 = t2.translation;

	math::quater q1 = t1.rotation;
	math::quater q2 = t2.rotation;

	QmGeodesic g1( math::quater(1,0,0,0), math::y_axis );
	QmGeodesic g2( math::quater(1,0,0,0), math::y_axis );

	math::transq T1( g1.nearest( q1 ), math::vector( v1.x(), 0, v1.z() ) );
	math::transq T2( g2.nearest( q2 ), math::vector( v2.x(), 0, v2.z() ) );

	return math::transq( T1 * T2.inverse() );
}
