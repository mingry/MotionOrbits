#pragma once

#include <string>
#include <vector>
#include <map>

class Joint;

class Skeleton
{
public:
	Skeleton();
	virtual ~Skeleton();

	void copyFrom( Skeleton* source_skeleton );
	void cleanup();

	Joint* createJoint( Joint* parent, const std::string& name );
	Joint* getJointByIndex( unsigned int i );
	Joint* getJointByName( const std::string& name );

	Joint* getRootJoint()		{ return root_joint; }
	unsigned int getNumJoints()	{ return (unsigned int)joint_list.size(); }

	void setHumanJoint( const std::string& name, int human_joint_id );
	Joint* getHumanJoint( int human_joint_id );

	unsigned int calcDOF();

	// is joint1 an ancestor of joint2 ?
	bool isAncestor( unsigned int joint1, unsigned int joint2 );

	inline std::vector< Joint* >* getJointList()	{ return &joint_list; }

protected:
	Joint*	root_joint;

	std::vector< Joint* >				joint_list;
	std::map< std::string, Joint* >		joint_map;
	std::map< int, Joint* >				human_joints;
};
