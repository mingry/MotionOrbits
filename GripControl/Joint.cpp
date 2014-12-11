#include "Joint.h"
#include "Human.h"

Joint::Joint()
{
	parent_joint = 0;
	index = 0;
	name = std::string( "" );
	
	dof = 0;
	human_id = Human::UNDEFINED;
}

Joint::~Joint()
{
	std::vector< Joint* >::iterator itor_c = child_joints.begin();
	while( itor_c != child_joints.end() )
	{
		Joint* child = ( *itor_c ++ );
		delete child;
	}
	child_joints.clear();
}

void Joint::setParent( Joint* new_parent )
{
	if( parent_joint )
	{
		parent_joint->removeChild( this );
		parent_joint = 0;
	}

	if( new_parent )
	{
		parent_joint = new_parent; 
		parent_joint->addChild( this );
	}
}

void Joint::removeChild( Joint* existing_child )
{
	std::vector< Joint* >::iterator itor_c = child_joints.begin();
	while( itor_c != child_joints.end() )
	{
		Joint* child = ( *itor_c );
		if( child == existing_child )
		{
			child_joints.erase( itor_c );
			return;
		}
		itor_c ++;
	}
}
