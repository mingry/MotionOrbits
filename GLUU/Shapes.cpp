

#include "GLUU/Shapes.h"
#include "GLUU/Basic.h"
#include <sstream>
#ifndef WIN32
#include <cstring>
#endif

namespace mg
{
void mgluTriangle(const math::vector& a, const math::vector& b, const math::vector &c)
{
	math::vector n = normalize( (b-a)*(c-a) );
	glBegin(GL_TRIANGLES);
	mg::mgluNormalV(n);
	mg::mgluVertexV(a);
	mg::mgluVertexV(b);
	mg::mgluVertexV(c);
	glEnd();
}

void mgluQuad(const math::vector& a, const math::vector& b, const math::vector &c, const math::vector &d)
{
	math::vector n = normalize( (b-a)*(c-a) );
	glBegin(GL_QUADS);
	mg::mgluNormalV(n);
	mg::mgluVertexV(a);
	mg::mgluVertexV(b);
	mg::mgluVertexV(c);
	mg::mgluVertexV(d);
	glEnd();
}

void mgluBox(double width, double height, double depth)
{
	mgluBox(math::vector(-width/2, -height/2, -depth/2)
		, math::vector(width/2, height/2, depth/2));
}


void mgluBox(const math::vector& a, const math::vector& b)
{
	math::vector leftBackBottom = a;
	math::vector rightFrontTop = b;

	for ( int i = 0; i < 3; i++ )
	{
		if ( leftBackBottom[i] > rightFrontTop[i] )
		{
			double tmpF = leftBackBottom[i];
			leftBackBottom[i] = rightFrontTop[i];
			rightFrontTop[i] = tmpF;
		}
	}

	double xLen = rightFrontTop.x() - leftBackBottom.x();
	double yLen = rightFrontTop.y() - leftBackBottom.y();
	double zLen = rightFrontTop.z() - leftBackBottom.z();


	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glVertex3f(leftBackBottom.x(), leftBackBottom.y(), leftBackBottom.z());
	glVertex3f(leftBackBottom.x(), leftBackBottom.y()+yLen, leftBackBottom.z());
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y()+yLen, leftBackBottom.z());
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y(), leftBackBottom.z());
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3f(leftBackBottom.x(), leftBackBottom.y(), leftBackBottom.z()+zLen);
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y(), leftBackBottom.z()+zLen);
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y()+yLen, leftBackBottom.z()+zLen);
	glVertex3f(leftBackBottom.x(), leftBackBottom.y()+yLen, leftBackBottom.z()+zLen);
	glEnd();


	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3f(leftBackBottom.x(), leftBackBottom.y()+yLen, leftBackBottom.z());
	glVertex3f(leftBackBottom.x(), leftBackBottom.y()+yLen, leftBackBottom.z()+zLen);
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y()+yLen, leftBackBottom.z()+zLen);
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y()+yLen, leftBackBottom.z());
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0, -1, 0);
	glVertex3f(leftBackBottom.x(), leftBackBottom.y(), leftBackBottom.z());
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y(), leftBackBottom.z());
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y(), leftBackBottom.z()+zLen);
	glVertex3f(leftBackBottom.x(), leftBackBottom.y(), leftBackBottom.z()+zLen);
	glEnd();


	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
	glVertex3f(leftBackBottom.x(), leftBackBottom.y(), leftBackBottom.z());
	glVertex3f(leftBackBottom.x(), leftBackBottom.y(), leftBackBottom.z()+zLen);
	glVertex3f(leftBackBottom.x(), leftBackBottom.y()+yLen, leftBackBottom.z()+zLen);
	glVertex3f(leftBackBottom.x(), leftBackBottom.y()+yLen, leftBackBottom.z());
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y(), leftBackBottom.z());
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y()+yLen, leftBackBottom.z());
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y()+yLen, leftBackBottom.z()+zLen);
	glVertex3f(leftBackBottom.x()+xLen, leftBackBottom.y(), leftBackBottom.z()+zLen);
	glEnd();
}

void mgluCylinder(double height, double r, int direction)
{	
	int i;

	static const int slice = 32;

	// top circle of cylinder
	static double cylTop[slice][3];

	// bottom circle of cylinder
	static double cylBot[slice][3];

	for ( int i=0; i<slice; i++ )
	{
		cylTop[i][0] = cylBot[i][0] = sin(2*M_PI * (i/(float)slice)) * r;
		cylTop[i][2] = cylBot[i][2] = cos(2*M_PI * (i/(float)slice)) * r;
		cylTop[i][1] = height/2;
		cylBot[i][1] = -1*height/2;
	}

	glPushMatrix();
	if ( direction == 0 )
		glRotated(-90, 0, 0, 1);
	
	else if ( direction == 2 )
		glRotated(90, 1, 0, 0);
	
	// draw cylinder
	glBegin(GL_QUAD_STRIP);
	for ( i = 0; i < slice; i++ )
	{
		glNormal3d(cylTop[i][0], 0, cylTop[i][2]);
		glVertex3dv(cylTop[i]);
		glVertex3dv(cylBot[i]);
	}
	glNormal3d(cylTop[0][0], 0, cylTop[0][2]);
	glVertex3dv(cylTop[0]);
	glVertex3dv(cylBot[0]);
	glEnd();

	// top side
	glBegin(GL_POLYGON);
	for ( i = 0; i < slice; i++ )
	{
		glNormal3d(0, 1, 1);
		glVertex3dv(cylTop[i]);
	}
	glEnd();

	// bottom  side
	glBegin(GL_POLYGON);
	for ( i = slice-1; i >= 0; i-- )
	{
		glNormal3d(0, -1, 0);
		glVertex3dv(cylBot[i]);
	}
	glEnd();


	glPopMatrix();

}


void mgluCylinder(math::vector pos1, math::vector pos2, double r)
{	
	glPushMatrix();

	
	// translate
	glTranslatef(pos1[0], pos1[1], pos1[2]);

	// rotate
	math::vector dirV = pos2 - pos1;
	double tmpA = dirV % (math::y_axis);
	math::vector crossV = (math::y_axis) * dirV;
	double tmpB = len(crossV);
	glRotatef((GLfloat)(atan2(tmpB, tmpA)*180/M_PI), crossV[0], crossV[1], crossV[2]);
	

	// scale
	glScalef(r, dirV.length(), r);
	glTranslated(0, 0.5, 0);

	mgluCylinder(1, 1);

	glPopMatrix();
}

void mgluOpenedCylinder(double height, double r, int direction)
{	
	int i;

	static const int slice = 32;

	// top circle of cylinder
	static double cylTop[slice][3];

	// bottom circle of cylinder
	static double cylBot[slice][3];

	static bool flag_first = true;

	for ( int i=0; i<slice; i++ )
	{
		cylTop[i][0] = cylBot[i][0] = sin(2*M_PI * (i/(float)slice)) * r;
		cylTop[i][2] = cylBot[i][2] = cos(2*M_PI * (i/(float)slice)) * r;
		cylTop[i][1] = height/2;
		cylBot[i][1] = -1*height/2;
	}

	glPushMatrix();
	if ( direction == 0 )
		glRotated(-90, 0, 0, 1);
	
	else if ( direction == 2 )
		glRotated(90, 1, 0, 0);
	
	// draw unit cylinder
	glBegin(GL_QUAD_STRIP);
	for ( i = 0; i < slice; i++ )
	{
		glNormal3d(cylTop[i][0], 0, cylTop[i][2]);
		glVertex3dv(cylTop[i]);
		glVertex3dv(cylBot[i]);
	}
	glNormal3d(cylTop[0][0], 0, cylTop[0][2]);
	glVertex3dv(cylTop[0]);
	glVertex3dv(cylBot[0]);
	glEnd();

	glPopMatrix();

}


void mgluOpenedCylinder(math::vector pos1, math::vector pos2, double r)
{	
	glPushMatrix();

	
	// translate
	glTranslatef(pos1[0], pos1[1], pos1[2]);

	// rotate
	math::vector dirV = pos2 - pos1;
	double tmpA = dirV % (math::y_axis);
	math::vector crossV = (math::y_axis) * dirV;
	double tmpB = len(crossV);
	glRotatef((GLfloat)(atan2(tmpB, tmpA)*180/M_PI), crossV[0], crossV[1], crossV[2]);
	

	// scale
	glScalef(r, dirV.length(), r);
	glTranslated(0, 0.5, 0);

	mgluOpenedCylinder(1, 1);

	glPopMatrix();
}



void mgluCircleZX(math::vector pos, double r)
{	
	int i;

	static const int slice = 24;

	// circle
	static double circle[slice][2];

	static bool flag_first = true;


	if ( flag_first )
	{
		flag_first = false;
		for ( int i=0; i<slice; i++ )
		{
			circle[i][0] = cos(2*M_PI * (i/(float)slice));
			circle[i][1] = sin(2*M_PI * (i/(float)slice));
		}
	}



	glPushMatrix();

	
	// translate
	glTranslatef(pos[0], pos[1], pos[2]);

	// scale
	glScalef(r, 1, r);


	glBegin(GL_POLYGON);
	for ( i = slice-1; i >= 0; i-- )
	{
		glNormal3f(0, 1, 0);
		glVertex3f(circle[i][0], 0, circle[i][1]);
	}
	glEnd();


	glPopMatrix();
}


void mgluCircleXY(math::vector pos, double r)
{	
	int i;

	static const int slice = 24;

	// circle
	static double circle[slice][2];

	static bool flag_first = true;


	if ( flag_first )
	{
		flag_first = false;
		for ( int i=0; i<slice; i++ )
		{
			circle[i][0] = cos(2*M_PI * (i/(float)slice));
			circle[i][1] = sin(2*M_PI * (i/(float)slice));
		}
	}



	glPushMatrix();

	
	// translate
	glTranslatef(pos[0], pos[1], pos[2]);

	// scale
	glScalef(r, r, 1);


	glBegin(GL_POLYGON);
	for ( i = slice-1; i >= 0; i-- )
	{
		glNormal3f(0, 0, 1);
		glVertex3f(circle[i][0], circle[i][1], 0);
	}
	glEnd();


	glPopMatrix();
}


void mgluArrowZX(math::vector from, math::vector to)
{
	math::vector dir = to - from;
	double length = len(dir);
	dir = normalize(dir);
	math::vector dir2(dir.z(), 0, dir.x());

	math::vector p1 = dir2 * ( length/sqrt(3.0) ) + from;
	math::vector p2 = -1*dir2 * ( length/sqrt(3.0) ) + from;

	glBegin(GL_LINES);
	glVertex3d(p1.x(), 0, p1.z());
	glVertex3d(to.x(), 0, to.z());
	glVertex3d(p2.x(), 0, p2.z());
	glVertex3d(to.x(), 0, to.z());
	glEnd();
}

void mgluArrowXY(math::vector from, math::vector to)
{
	from[2] = to[2] = 0.0;
	math::vector dir = to - from;
	double length = len(dir);
	dir = normalize(dir);
	math::vector dir2(dir.y(), -dir.x(), 0.0);

	math::vector p1 = dir2 * ( length/sqrt(3.0) ) + from;
	math::vector p2 = -1*dir2 * ( length/sqrt(3.0) ) + from;

	glBegin(GL_LINES);
	glVertex3d(p1.x(), p1.y(), 0);
	glVertex3d(to.x(), to.y(), 0);
	glVertex3d(p2.x(), p2.y(), 0);
	glVertex3d(to.x(), to.y(), 0);
	glEnd();
}

void mgluArrowXY(math::vector from, math::vector to, double size)
{
	from[2] = to[2] = 0.0;
	math::vector dir = to - from;
	double length = len(dir);
	dir = normalize(dir);
	math::vector dir2(dir.y(), -dir.x(), 0.0);

	math::vector p1 = dir2 * ( length/sqrt(3.0) ) + from;
	math::vector p2 = -1*dir2 * ( length/sqrt(3.0) ) + from;

	p1 = size * normalize(p1 - to) + to;
	p2 = size * normalize(p2 - to) + to;

	glBegin(GL_LINES);
	glVertex3d(p1.x(), p1.y(), 0);
	glVertex3d(to.x(), to.y(), 0);
	glVertex3d(p2.x(), p2.y(), 0);
	glVertex3d(to.x(), to.y(), 0);
	glEnd();
}

void mgluArrow3D(math::vector from, math::vector to, double size)
{
	math::vector dir = normalize(to-from);
	double length = len(to-from);
	double header_length = 2*size;
	if ( header_length > length ) header_length = length/2;

	mgluCylinder(from, to-dir*header_length, size/2);
	mgluCone(to-dir*header_length, to, size);
	
}


void mgluHalfCircleZX(math::vector pos, double r, bool line)
{	
	int i;

	static const int slice = 12;
	static const int vertices_num = 13;

	// circle
	static double circle[vertices_num][2];

	static bool flag_first = true;


	if ( flag_first )
	{
		flag_first = false;
		for ( int i=0; i<vertices_num; i++ )
		{
			circle[i][1] = cos(M_PI * (i/(float)slice));
			circle[i][0] = sin(M_PI * (i/(float)slice));
		}
	}



	glPushMatrix();

	
	// translate
	glTranslatef(pos[0], pos[1], pos[2]);

	// scale
	glScalef(r, 1, r);

	if ( line ) glBegin(GL_LINE_STRIP);
	else glBegin(GL_POLYGON);
	for ( i = vertices_num-1; i >= 0; i-- )
	{
		glNormal3f(0, 1, 0);
		glVertex3f(circle[i][0], 0, circle[i][1]);
	}
	glEnd();


	glPopMatrix();
}


//void mgluCircleZX(math::vector pos, double r)
//{	
//	int i;
//
//	
//	static const double circle[12][2] = { {1.000f, 0.000f},
//										 {0.809f, 0.588f},
//										 {0.309f, 0.951f},
//										 {-0.309f, 0.951f},
//										 {-0.809f, 0.588f},
//										 {-1.000f, 0.000f},
//										 {-0.809f, -0.588f},
//										 {-0.309f, -0.951f},
//										 {0.309f, -0.951f},
//										 {0.809f, -0.588f},
//										 {1.000f, -0.000f},
//										 {0.809f, 0.588f} };
//
//
//
//	glPushMatrix();
//
//	
//	// translate
//	glTranslatef(pos[0], pos[1], pos[2]);
//
//	// scale
//	glScalef(r, 1, r);
//
//
//	glBegin(GL_POLYGON);
//	for ( i = 11; i >= 0; i-- )
//	{
//		glNormal3f(0, 1, 0);
//		glVertex3f(circle[i][0], 0, circle[i][1]);
//	}
//	glEnd();
//
//
//	glPopMatrix();
//}


void 
mgluSphere(double r)
{
	glutSolidSphere(r, 32, 16);
}


void 
mgluSphere(math::vector pos, double r)
{
	glPushMatrix();
	glTranslated(pos[0],pos[1],pos[2]);
	glutSolidSphere(r, 32, 16);
	glPopMatrix();
}


void
mgluCapsule(double cylinder_height, double r, int direction)
{
	glPushMatrix();
	if ( direction == 0 )
		glRotated(-90, 0, 0, 1);
	
	else if ( direction == 2 )
		glRotated(90, 1, 0, 0);

	glPushMatrix();
	glTranslated(0, cylinder_height/2, 0);
	mgluHemisphere(r);
	glPopMatrix();

	mgluOpenedCylinder(cylinder_height, r);

	glPushMatrix();
	glTranslated(0, -1*cylinder_height/2, 0);
	glRotated(180, 1, 0, 0);
	mgluHemisphere(r);
	glPopMatrix();

	glPopMatrix();
}

void
mgluCapsule(math::vector top_p, math::vector bottom_p, double r)
{
	glPushMatrix();
	
	// translate
	glTranslatef(bottom_p[0], bottom_p[1], bottom_p[2]);

	// rotate
	math::vector dirV = top_p - bottom_p;
	double tmpA = dirV % (math::y_axis);
	math::vector crossV = (math::y_axis) * dirV;
	double tmpB = len(crossV);
	glRotatef((GLfloat)(atan2(tmpB, tmpA)*180/M_PI), crossV[0], crossV[1], crossV[2]);
	

	if ( dirV.length()-2*r > 0 )
	{
		glTranslated(0, dirV.length()/2, 0);
		mgluCapsule(dirV.length()-2*r, r);
	}
	else
	{
		glTranslated(0, dirV.length()/2, 0);
		glScaled(r, dirV.length()/2, r);
		glutSolidSphere(1, 32, 16);
	}

	glPopMatrix();
}

void 
mgluCone(math::vector from, math::vector to, double r)
{
	glPushMatrix();

	// translate
	glTranslatef(from[0], from[1], from[2]);

	// rotate
	math::vector dirV = to - from;
	double tmpA = dirV % (math::z_axis);
	math::vector crossV = (math::z_axis) * dirV;
	double tmpB = len(crossV);
	glRotatef((GLfloat)(atan2(tmpB, tmpA)*180/M_PI), crossV[0], crossV[1], crossV[2]);

	double height = len(to-from);

	glutSolidCone(r, height, 15, 15);
	glPopMatrix();
}


void 
mgluCone(math::vector from, math::vector to)
{
	mgluCone(from, to, len(to-from)/1.732);
}

void
mgluDrawCamera(Camera &c, double width, double height, double depth)
{
	double _w_s, _h_s, _d_s;
	_w_s = width/1;
	_h_s = height/1;
	_d_s = depth/1.5;

	glPushMatrix();
	mgluTranslateV(c.getTranslation());
	mgluRotateQ(c.getRotation());
	glPushMatrix();
	glScaled(_w_s, _h_s, _d_s);
	mgluCone(math::vector(0, 0, 0), math::vector(0, 0, -0.5), 0.5);
	mgluBox(math::vector(-0.5, -0.5, -1.5), math::vector(0.5, 0.5, -0.5));
	glPopMatrix();


	math::vector e_far[4];
	e_far[0][1] = c.getFar() * abs(tan(M_PI*c.getFov()/360.0));
	e_far[0][0] = (e_far[0][1] * c.getAspectRatio());
	e_far[0][2] = c.getFar();
	
	e_far[1][0] = +1*e_far[0][0];
	e_far[1][1] = -1*e_far[0][1];
	e_far[1][2] = +1*e_far[0][2];

	e_far[2][0] = -1*e_far[0][0];
	e_far[2][1] = -1*e_far[0][1];
	e_far[2][2] = +1*e_far[0][2];

	e_far[3][0] = -1*e_far[0][0];
	e_far[3][1] = +1*e_far[0][1];
	e_far[3][2] = +1*e_far[0][2];

	double near_s = c.getNear() / c.getFar();

	glBegin(GL_LINES);
	mgluVertexV(math::vector(0, 0, 0));
	mgluVertexV(e_far[0]);
	mgluVertexV(math::vector(0, 0, 0));
	mgluVertexV(e_far[1]);
	mgluVertexV(math::vector(0, 0, 0));
	mgluVertexV(e_far[2]);
	mgluVertexV(math::vector(0, 0, 0));
	mgluVertexV(e_far[3]);
	glEnd();

	glBegin(GL_LINE_LOOP);
	mgluVertexV(e_far[0]);
	mgluVertexV(e_far[1]);
	mgluVertexV(e_far[2]);
	mgluVertexV(e_far[3]);
	glEnd();

	glBegin(GL_LINE_LOOP);
	mgluVertexV(e_far[0] * near_s);
	mgluVertexV(e_far[1] * near_s);
	mgluVertexV(e_far[2] * near_s);
	mgluVertexV(e_far[3] * near_s);
	glEnd();
	glPopMatrix();

}


void
mgluText(const char* s, void *font)
{
	if (s && strlen(s)) 
	{
		while (*s) 
		{
			glutStrokeCharacter(font, *s);
			s++;
		}
	}
}

void
mgluText(int integer, void *font)
{
	std::stringstream s;
	s << integer;

	mgluText(s.str().c_str(), font);
}

void
mgluText(double real, void *font)
{
	std::stringstream s;
	s << real;


	mgluText(s.str().c_str(), font);
}

void
mgluBitmapText(const char* s, void *font)
{
	if (s && strlen(s)) 
	{
		while (*s) 
		{
			glutBitmapCharacter(font, *s);
			s++;
		}
	}
}


void
mgluBitmapText(int integer, void *font)
{
	std::stringstream s;
	s << integer;

	mgluBitmapText(s.str().c_str(), font);
}

void
mgluBitmapText(double real, void *font)
{
	std::stringstream s;
	s << real;


	mgluBitmapText(s.str().c_str(), font);

}


};
