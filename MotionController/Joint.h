#pragma once

#include <vector>
#include <string>
#include "mathclass/vector.h"

class Joint
{
public:
	Joint();
	virtual ~Joint();

	inline void setIndex( unsigned int i )	{ index = i; }
	inline unsigned int getIndex()			{ return index; }

	inline void setName( const std::string& n )	{ name = n; }
	inline std::string getName()				{ return name; }

	void setParent( Joint* j );
	void removeChild( Joint* j );

	inline Joint* getParent()			{ return parent_joint; }

	inline void setOffset( const math::vector& v )	{ offset = v; }
	inline math::vector getOffset()					{ return offset; }

	inline void addChild( Joint* j )			{ child_joints.push_back( j ); }
	inline unsigned int getNumChildren()		{ return (unsigned int)child_joints.size(); }
	inline Joint* getChild( unsigned int i )	{ if( i < getNumChildren() ) return child_joints[i]; else return 0; }
	inline std::vector< Joint* >* getChildren()	{ return &child_joints; }

	inline void setDOF( unsigned int d )	{ dof = d; }
	inline unsigned int getDOF()			{ return dof; }

	inline void setHumanID( int i )		{ human_id = i; }
	inline int getHumanID()				{ return human_id; }

protected:
	unsigned int	index;
	std::string		name;
	math::vector	offset;

	unsigned int	dof;
	int				human_id;

	Joint*					parent_joint;
	std::vector< Joint* >	child_joints;
};
