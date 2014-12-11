#include "Human.h"
#include "Skeleton.h"
#include "Joint.h"

void setupBboySkeleton( Skeleton* skeleton )
{
	skeleton->setHumanJoint( "root", Human::PELVIS );
	skeleton->setHumanJoint( "Spine", Human::SPINE_1 );
	skeleton->setHumanJoint( "Spine1", Human::SPINE_2 );
	skeleton->setHumanJoint( "Neck", Human::NECK );
	skeleton->setHumanJoint( "Head", Human::HEAD );
	skeleton->setHumanJoint( "R_Thigh", Human::UPPER_RIGHT_LEG );
	skeleton->setHumanJoint( "R_Calf", Human::LOWER_RIGHT_LEG );
	skeleton->setHumanJoint( "R_Foot", Human::RIGHT_FOOT );
	skeleton->setHumanJoint( "R_Toe0", Human::RIGHT_TOE );
	skeleton->setHumanJoint( "L_Thigh", Human::UPPER_LEFT_LEG );
	skeleton->setHumanJoint( "L_Calf", Human::LOWER_LEFT_LEG );
	skeleton->setHumanJoint( "L_Foot", Human::LEFT_FOOT );
	skeleton->setHumanJoint( "L_Toe0", Human::LEFT_TOE );
	skeleton->setHumanJoint( "R_Clavicle", Human::RIGHT_SHOULDER );
	skeleton->setHumanJoint( "R_UpperArm", Human::UPPER_RIGHT_ARM );
	skeleton->setHumanJoint( "R_Forearm", Human::LOWER_RIGHT_ARM );
	skeleton->setHumanJoint( "R_Hand", Human::RIGHT_PALM );
	skeleton->setHumanJoint( "L_Clavicle", Human::LEFT_SHOULDER );
	skeleton->setHumanJoint( "L_UpperArm", Human::UPPER_LEFT_ARM );
	skeleton->setHumanJoint( "L_Forearm", Human::LOWER_LEFT_ARM );
	skeleton->setHumanJoint( "L_Hand", Human::LEFT_PALM );
}
