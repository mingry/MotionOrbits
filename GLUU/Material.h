

#pragma once


#include <GL/glut.h>
#include "mathclass/vector.h"


namespace mg
{
	bool mgluSoftColorMaterial();
	void mgluSoftColorMaterial(bool flag);
	bool mgluShinyMaterial();
	void mgluShinyMaterial(bool flag);

	void mgluColor(math::vector c);
	void mgluColor(math::vector c, GLdouble a);
	void mgluColor(GLdouble r, GLdouble g, GLdouble b);
	void mgluColor(GLdouble r, GLdouble g, GLdouble b, GLdouble a);
	void mgluColor(unsigned int i);
	void mgluColor(unsigned int i, GLdouble a);

	void mgluSimpleMaterial(math::vector c);
	void mgluSimpleMaterial(math::vector c, GLdouble a);
	void mgluSimpleMaterial(GLdouble r, GLdouble g, GLdouble b);
	void mgluSimpleMaterial(GLdouble r, GLdouble g, GLdouble b, GLdouble a);
	void mgluSimpleMaterial(unsigned int i);
	void mgluSimpleMaterial(unsigned int i, GLdouble a);

	void mgluColorMaterial(math::vector c);
	void mgluColorMaterial(math::vector c, GLdouble a);
	void mgluColorMaterial(GLdouble r, GLdouble g, GLdouble b);
	void mgluColorMaterial(GLdouble r, GLdouble g, GLdouble b, GLdouble a);
	void mgluColorMaterial(unsigned int i);
	void mgluColorMaterial(unsigned int i, GLdouble a);
	math::vector mgluGetIndexColor(unsigned int i);
	unsigned int mgluGetRecentlyUsedColorIndex();
};


