#include "Human.h"
#include "Skeleton.h"
#include "Joint.h"

void setupBoxingSkeleton( Skeleton* skeleton )
{
	skeleton->setHumanJoint( "root", Human::PELVIS );
	skeleton->setHumanJoint( "lowerback", Human::SPINE_1 );
	skeleton->setHumanJoint( "upperback", Human::SPINE_2 );
	skeleton->setHumanJoint( "thorax", Human::SPINE_3 );
	skeleton->setHumanJoint( "lowerneck", Human::CHEST );
	skeleton->setHumanJoint( "upperneck", Human::NECK );
	skeleton->setHumanJoint( "head", Human::HEAD );
	skeleton->setHumanJoint( "rfemur", Human::UPPER_RIGHT_LEG );
	skeleton->setHumanJoint( "rtibia", Human::LOWER_RIGHT_LEG );
	skeleton->setHumanJoint( "rfoot", Human::RIGHT_FOOT );
	skeleton->setHumanJoint( "rtoes", Human::RIGHT_TOE );
	skeleton->setHumanJoint( "lfemur", Human::UPPER_LEFT_LEG );
	skeleton->setHumanJoint( "ltibia", Human::LOWER_LEFT_LEG );
	skeleton->setHumanJoint( "lfoot", Human::LEFT_FOOT );
	skeleton->setHumanJoint( "ltoes", Human::LEFT_TOE );
	skeleton->setHumanJoint( "rshoulderjoint", Human::RIGHT_SHOULDER );
	skeleton->setHumanJoint( "rhumerus", Human::UPPER_RIGHT_ARM );
	skeleton->setHumanJoint( "rradius", Human::LOWER_RIGHT_ARM );
	skeleton->setHumanJoint( "rhand", Human::RIGHT_PALM );
	skeleton->setHumanJoint( "lshoulderjoint", Human::LEFT_SHOULDER );
	skeleton->setHumanJoint( "lhumerus", Human::UPPER_LEFT_ARM );
	skeleton->setHumanJoint( "lradius", Human::LOWER_LEFT_ARM );
	skeleton->setHumanJoint( "lhand", Human::LEFT_PALM );
}
