
#pragma once


#include "MATHCLASS/vector.h"
#include "MATHCLASS/position.h"
#include "MATHCLASS/transf.h"
#include <GL/glut.h>


namespace mg
{
	void mgluVertexV(const math::vector &v);
	void mgluVertexP(const math::position &p);
	void mgluNormalV(const math::vector &v);
	
	void mgluScaleD(GLdouble s);
	void mgluScaleV(const math::vector &s);
	void mgluRotateQ(const math::quater &q);
	void mgluTranslateV(const math::vector &v);
	void mgluTransform(const math::transf &t);

	void mgluPush(GLbitfield mask=0, GLbitfield default_mask=GL_CURRENT_BIT|GL_ENABLE_BIT|GL_LIGHTING_BIT);
	void mgluPop();

	void mgluPushAttrib(GLbitfield mask=0, GLbitfield default_mask=GL_CURRENT_BIT|GL_ENABLE_BIT|GL_LIGHTING_BIT);
	void mgluPopAttrib();

	GLuint mgluGenName();
};