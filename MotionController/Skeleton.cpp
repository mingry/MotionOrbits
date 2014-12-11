#include "Skeleton.h"
#include "Joint.h"
#include "Human.h"

#include <stack>

Skeleton::Skeleton()
{
	root_joint = 0;
}

Skeleton::~Skeleton()
{
	cleanup();
}

void Skeleton::cleanup()
{
	delete root_joint;
	root_joint = 0;

	joint_list.clear();
	joint_map.clear();
	human_joints.clear();
}

void Skeleton::copyFrom( Skeleton* source_skeleton )
{
	Joint* source_root = source_skeleton->getRootJoint();
	if( !source_root )
	{
		return;
	}

	this->cleanup();

	std::vector< Joint* >* source_joints = source_skeleton->getJointList();
	std::vector< Joint* >::iterator itor_j = source_joints->begin();
	while( itor_j != source_joints->end() )
	{
		Joint* source_joint = ( *itor_j ++ );
		Joint* source_parent = source_joint->getParent();

		std::string name = source_joint->getName();
		math::vector offset = source_joint->getOffset();
		unsigned int human_id = source_joint->getHumanID();
		
		Joint* this_parent = 0;
		if( source_parent )
		{
			unsigned int parent_id = source_parent->getIndex();
			this_parent = joint_list[ parent_id ];
		}
		Joint* this_joint = createJoint( this_parent, name );
		this_joint->setOffset( offset );
		setHumanJoint( name, human_id );
	}
}

Joint* Skeleton::createJoint( Joint* parent, const std::string& name )
{
	Joint* new_joint = new Joint;

	if( !parent )
	{
		cleanup();
		root_joint = new_joint;
	}
	else
	{
		Joint* existing_joint = getJointByName( name );
		if( existing_joint )
		{
			delete new_joint;
			return 0;
		}
	}

	unsigned int num_joints = getNumJoints();
	new_joint->setIndex( num_joints );
	new_joint->setName( name );
	new_joint->setParent( parent );

	joint_list.push_back( new_joint );
	joint_map[ name ] = new_joint;

	return new_joint;
}

Joint* Skeleton::getJointByIndex( unsigned int i )
{
	if( i < getNumJoints() )
	{
		return joint_list[ i ];
	}
	else
	{
		return 0;
	}
}

Joint* Skeleton::getJointByName( const std::string& name )
{
	std::map< std::string, Joint* >::iterator itor_n = joint_map.find( name );
	if( itor_n != joint_map.end() )
	{
		return itor_n->second;
	}
	else
	{
		return 0;
	}
}

unsigned int Skeleton::calcDOF()
{
	unsigned int total_dof = 0;

	std::vector< Joint* >::iterator itor_j = joint_list.begin();
	while( itor_j != joint_list.end() )
	{
		Joint* j = ( *itor_j ++ );
		total_dof += j->getDOF();
	}
	return total_dof;
}

void Skeleton::setHumanJoint( const std::string& name, int human_joint_id )
{
	Joint* joint = getHumanJoint( human_joint_id );
	if( joint )
	{
		return;
	}
	else
	{
		joint = getJointByName( name );
		if( joint )
		{
			joint->setHumanID( human_joint_id );
			joint->setDOF( Human::dof_for_each_part[ human_joint_id ] );

			human_joints[ human_joint_id ] = joint;
		}
	}
}

Joint* Skeleton::getHumanJoint( int human_joint_id )
{
	std::map< int, Joint* >::iterator itor_j = human_joints.find( human_joint_id );
	if( itor_j != human_joints.end() )
	{
		return itor_j->second;
	}
	else
	{
		return 0;
	}
}

bool Skeleton::isAncestor( unsigned int j1, unsigned int j2 )
{
	Joint* joint1 = getJointByIndex( j1 );
	Joint* joint2 = getJointByIndex( j2 );
	Joint* joint = joint2->getParent();

	while( joint )
	{
		if( joint == joint1 )
		{
			return true;
		}
		joint = joint->getParent();
	}
	return false;
}

