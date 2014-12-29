
#include "GLUU/gluu.h"

namespace mg
{


void mgluNormalV(const math::vector &v)
{
	glNormal3f(v.x(), v.y(), v.z());
}

void mgluVertexV(const math::vector &v)
{
	glVertex3f(v.x(), v.y(), v.z());
}

void mgluVertexP(const math::position &p)
{
	glVertex3f(p.x(), p.y(), p.z());
}
	
void mgluScaleD(GLdouble s)
{
	glScaled(s, s, s);
}

void mgluScaleV(const math::vector &s)
{
	glScaled(s.x(), s.y(), s.z());
}

void mgluRotateQ(const math::quater &q)
{
	math::vector v = ln(q);
	double r = len(v) * 2;
	v = normalize(v);

	glRotatef((float)r*180.0f/M_PI, v[0], v[1], v[2]);
}


void mgluTranslateV(const math::vector &v)
{
	glTranslatef(v.x(), v.y(), v.z());
}

void mgluTransform(const math::transf &t)
{
	double m[16];

	math::matrix affine = t.affine();
	math::vector tranls = t.translation();

	m[ 0] = affine[0][0];
	m[ 1] = affine[0][1];
	m[ 2] = affine[0][2];
	m[ 3] = 0;		 
					 
	m[ 4] = affine[1][0];
	m[ 5] = affine[1][1];
	m[ 6] = affine[1][2];
	m[ 7] = 0;		 
					 
	m[ 8] = affine[2][0];
	m[ 9] = affine[2][1];
	m[10] = affine[2][2];
	m[11] = 0;

	m[12] = tranls[0];
	m[13] = tranls[1];
	m[14] = tranls[2];
	m[15] = 1;

	glMultMatrixd(m);
}

void mgluPush(GLbitfield mask, GLbitfield default_mask)
{
	glPushMatrix();
	glPushAttrib(mask|default_mask);
}

void mgluPop()
{
	glPopAttrib();
	glPopMatrix();
}

void mgluPushAttrib(GLbitfield mask, GLbitfield default_mask)
{
	glPushAttrib(mask|default_mask);
}

void mgluPopAttrib()
{
	glPopAttrib();
}


GLuint mgluGenName()
{
	static GLuint next_name = 1;

	GLuint cur_name = next_name;
	next_name++;

	return cur_name;

}



};