#include "Human.h"

using std::string;

int Human::human_symmetric_part[ NUM_JOINTS_IN_HUMAN ] =
{
	Human::PELVIS,
	Human::SPINE_1,
	Human::SPINE_2,
	Human::SPINE_3,
	Human::SPINE_4,
	Human::CHEST, 
	Human::NECK, Human::HEAD,
	Human::LEFT_SHOULDER, Human::RIGHT_SHOULDER,
	Human::LEFT_COLLAR,   Human::RIGHT_COLLAR,
	Human::UPPER_LEFT_ARM, Human::UPPER_RIGHT_ARM,
	Human::LOWER_LEFT_ARM, Human::LOWER_RIGHT_ARM,
	Human::UPPER_LEFT_LEG, Human::UPPER_RIGHT_LEG, 
	Human::LOWER_LEFT_LEG, Human::LOWER_RIGHT_LEG,
	Human::LEFT_FOOT, Human::RIGHT_FOOT,
	Human::LEFT_TOE, Human::RIGHT_TOE,
	Human::LEFT_PALM,
	Human::LEFT_FINGER_11, Human::LEFT_FINGER_12, Human::LEFT_FINGER_13,
	Human::LEFT_FINGER_21, Human::LEFT_FINGER_22, Human::LEFT_FINGER_23,
	Human::LEFT_FINGER_31, Human::LEFT_FINGER_32, Human::LEFT_FINGER_33,
	Human::LEFT_FINGER_41, Human::LEFT_FINGER_42, Human::LEFT_FINGER_43,
	Human::LEFT_FINGER_51, Human::LEFT_FINGER_52, Human::LEFT_FINGER_53,
	Human::RIGHT_PALM,
	Human::RIGHT_FINGER_11, Human::RIGHT_FINGER_12, Human::RIGHT_FINGER_13,
	Human::RIGHT_FINGER_21, Human::RIGHT_FINGER_22, Human::RIGHT_FINGER_23,
	Human::RIGHT_FINGER_31, Human::RIGHT_FINGER_32, Human::RIGHT_FINGER_33,
	Human::RIGHT_FINGER_41, Human::RIGHT_FINGER_42, Human::RIGHT_FINGER_43,
	Human::RIGHT_FINGER_51, Human::RIGHT_FINGER_52, Human::RIGHT_FINGER_53,
	Human::LEFT_HEEL, Human::RIGHT_HEEL,

	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,
};

string Human::human_part_name[ NUM_JOINTS_IN_HUMAN ] =
{
	"pelvis",
	"spine_1", "spine_2", "spine_3", "spine_4", "chest",
	"neck", "head",
	"right_shoulder", "left_shoulder",
	"right_collar", "left_collar",
	"upper_right_arm", "upper_left_arm", "lower_right_arm", "lower_left_arm",
	"upper_right_leg", "upper_left_leg", "lower_right_leg", "lower_left_leg",
	"right_foot", "left_foot", "right_toe", "left_toe",
	"right_palm",
	"right_finger_11", "right_finger_12", "right_finger_13",
	"right_finger_21", "right_finger_22", "right_finger_23",
	"right_finger_31", "right_finger_32", "right_finger_33",
	"right_finger_41", "right_finger_42", "right_finger_43",
	"right_finger_51", "right_finger_52", "right_finger_53",
	"left_palm",
	"left_finger_11", "left_finger_12", "left_finger_13",
	"left_finger_21", "left_finger_22", "left_finger_23",
	"left_finger_31", "left_finger_32", "left_finger_33",
	"left_finger_41", "left_finger_42", "left_finger_43",
	"left_finger_51", "left_finger_52", "left_finger_53",
	"right_heel", "left_heel",
	
	"dummy1", "dummy2", "dummy3", "dummy4", "dummy5",
	"dummy6", "dummy7", "dummy8", "dummy9", "dummy10",
	"dummy11", "dummy12", "dummy13", "dummy14", "dummy15",
	"dummy16", "dummy17", "dummy18", "dummy19", "dummy20",
	"dummy21", "dummy22", "dummy23", "dummy24", "dummy25",
	"dummy26", "dummy27", "dummy28", "dummy29", "dummy30",
	"dummy31", "dummy32", "dummy33", "dummy34", "dummy35",
	"dummy36", "dummy37", "dummy38", "dummy39", "dummy40",
	"dummy41", "dummy42", "dummy43", "dummy44", "dummy45",
	"dummy46", "dummy47", "dummy48", "dummy49", "dummy50",
};

int Human::dof_for_each_part[ NUM_JOINTS_IN_HUMAN ] =
{
	6,
	3, 3, 3, 3, 3,		// spine*4, chest
	3, 3,				// neck, head 
	3, 3,				// shoulders
	3, 3,				// clavicles
    3, 3, 3, 3,			// arms
	3, 3, 3, 3,			// legs
	3, 3, 3, 3,			// feet, toes
	3,					// right_palm

	0, 0, 0,			// right fingers
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	/*
	3, 1, 1,			// fingers
	1, 1, 1,
	1, 1, 1,
	1, 1, 1,
	1, 1, 1,
	*/

    3,					// left_palm

	0, 0, 0,			// left fingers
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	/*
	3, 1, 1,			// fingers
	1, 1, 1, 
	1, 1, 1,
	1, 1, 1,
	1, 1, 1,
	*/

	0, 0,				// left and right heel

	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
};
