
#pragma once



#include "MATHCLASS/vector.h"
#include "MATHCLASS/position.h"
#include "MATHCLASS/transf.h"
#include <GL/glut.h>
#include "GLUU/Camera.h"

namespace mg
{
	void mgluTriangle(const math::vector& a, const math::vector& b, const math::vector &c);
	void mgluQuad(const math::vector& a, const math::vector& b, const math::vector &c, const math::vector &d);
	void mgluBox(double width, double height, double depth);
	void mgluBox(const math::vector& a, const math::vector& b);
	void mgluCylinder(double height, double r, int direction=1); // direction 0 -> x axis, 1 -> y axis, 2 -> z axis.
	void mgluCylinder(math::vector pos1, math::vector pos2, double r);
	void mgluOpenedCylinder(double height, double r, int direction=1);
	void mgluOpenedCylinder(math::vector pos1, math::vector pos2, double r);
	void mgluCircleZX(math::vector pos, double r);
	void mgluCircleXY(math::vector pos, double r);
	void mgluArrowZX(math::vector from, math::vector to);
	void mgluArrowXY(math::vector from, math::vector to);
	void mgluArrowXY(math::vector from, math::vector to, double size);
	void mgluArrow3D(math::vector from, math::vector to, double size);
	void mgluHalfCircleZX(math::vector pos, double r, bool line=false);
	void mgluSphere(double r);
	void mgluSphere(math::vector pos, double r);
	void mgluHemisphere(math::vector pos, double r);
	void mgluHemisphere(double r);
	void mgluCone(math::vector from, math::vector to, double r);
	void mgluCone(math::vector from, math::vector to);
	void mgluCapsule(double cylinder_height, double r, int direction=1);
	void mgluCapsule(math::vector top_p, math::vector bottom_p, double r);
	void mgluDrawCamera(Camera &c, double width=1, double height=1, double depth=1.5);

	void mgluText(const char *s, void *font=GLUT_STROKE_ROMAN);
	void mgluText(int integer, void *font=GLUT_STROKE_ROMAN);
	void mgluText(double real, void *font=GLUT_STROKE_ROMAN);

	void mgluBitmapText(const char *s, void *font=GLUT_BITMAP_HELVETICA_18);
	void mgluBitmapText(int integer, void *font=GLUT_BITMAP_HELVETICA_18);
	void mgluBitmapText(double real, void *font=GLUT_BITMAP_HELVETICA_18);
};