

#pragma once


#include "MATHCLASS/vector.h"
#include "GLUU/Camera.h"
#include <vector>

namespace mg
{

	// win_x, win_y are integers.
	bool mgluGetPlaneCrossPoint(int win_x, int win_y, math::vector const &planeP, math::vector const &planeN, math::vector &outP);
	bool mgluGetPlaneCrossPoint(int win_x, int win_y, math::vector const &planeP, math::vector const &planeN, Camera const& camera, math::vector &outP);
	double mgluGetDistMouseAnd3dPoint(int win_x, int win_y, math::vector const &p_3d, Camera const& camera);

	
	bool mgluGetPointOnRenderedObject(int win_mouse_x, int win_mouse_y, math::vector &outP);

	// (win_x, win_z) is a 2D point in the window coordinate.
	// z is an imaginary value, which means a distance from the viewer.
	math::vector mgluUnProjectWinP(int win_x, int win_y, int z);
	math::vector mgluUnProjectWinP(int win_x, int win_y, int z, Camera const& camera);
};

