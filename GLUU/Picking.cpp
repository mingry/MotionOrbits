

#include "GLUU/gluu.h"
#ifndef UINT_MAX
#define UINT_MAX (unsigned int)-1
#endif

namespace mg
{

	
bool mgluGetPlaneCrossPoint(int inputX, int inputY, math::vector const &planeP, math::vector const &planeN, math::vector &outP)
{
	glFlush();
	double mv[16], p[16], a, b, c, d, e, f;
	glGetDoublev(GL_MODELVIEW_MATRIX,mv);
	glGetDoublev(GL_PROJECTION_MATRIX,p);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	gluUnProject(inputX,viewport[3]-inputY,0,mv,p,viewport,&a,&b,&c);
	gluUnProject(inputX,viewport[3]-inputY,1,mv,p,viewport,&d,&e,&f);


	math::vector ray1(a, b, c), ray2(d, e, f);


	math::vector planeP_s = planeP;// * camera.getScale();

	//if ( fabs(ray2[1]-ray1[1]) > 0.000001 )
	if ( fabs(planeN%(ray2-ray1)) > 0.000001 )
	{
		outP = ray1 + ( (planeN%(planeP_s-ray1)) / (planeN%(ray2-ray1)) ) * (ray2-ray1);
		return true;
	}
	return false;
}

bool mgluGetPlaneCrossPoint(int inputX, int inputY, math::vector const &planeP, math::vector const &planeN, Camera const& camera, math::vector &outP)
{
	glFlush();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	camera.glProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.glTranslform();

	double mv[16], p[16], a, b, c, d, e, f;
	glGetDoublev(GL_MODELVIEW_MATRIX,mv);
	glGetDoublev(GL_PROJECTION_MATRIX,p);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	gluUnProject(inputX,viewport[3]-inputY,0,mv,p,viewport,&a,&b,&c);
	gluUnProject(inputX,viewport[3]-inputY,1,mv,p,viewport,&d,&e,&f);


	math::vector ray1(a, b, c), ray2(d, e, f);


	math::vector planeP_s = planeP;// * camera.getScale();

	//if ( fabs(ray2[1]-ray1[1]) > 0.000001 )
	if ( fabs(planeN%(ray2-ray1)) > 0.000001 )
	{
		outP = ray1 + ( (planeN%(planeP_s-ray1)) / (planeN%(ray2-ray1)) ) * (ray2-ray1);
		return true;
	}
	return false;
}



double mgluGetDistMouseAnd3dPoint(int win_x, int win_y, math::vector const &p_3d, Camera const& camera)
{
	double dist;

	// plane, parallal to caamera.
	math::vector plane_n = math::normalize( rotate(camera.getRotation(), -1*math::z_axis) );
	math::vector plane_p;

	mgluGetPlaneCrossPoint(win_x, win_y, p_3d, plane_n, plane_p);

	dist = len(plane_p - p_3d);

	return dist;
}








bool mgluGetPointOnRenderedObject(int win_mouse_x, int win_mouse_y, math::vector &outP)
{
	glFlush();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);


	// Get the Z coordinate for the pixel location
	GLuint lDepth;
	glReadPixels( win_mouse_x, ( viewport[ 3 ] - win_mouse_y ), 
		1, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 
		&lDepth );

	if ( lDepth == UINT_MAX ) return false;

	int zdepth;
	double lDistTermInv;
	glGetIntegerv( GL_DEPTH_BITS, &zdepth );
	switch ( zdepth )
	{
	case 16 :
		lDistTermInv = 1.5259018967e-5; // 65535
		break;
	default :
		lDistTermInv = 2.32830643708e-10; // 4294967295
		break;
	}

	double lDistance = lDepth * lDistTermInv;

	double mv[16], p[16];
	glGetDoublev( GL_MODELVIEW_MATRIX, mv );
	glGetDoublev( GL_PROJECTION_MATRIX, p );

	gluUnProject( win_mouse_x, ( viewport[ 3 ] - win_mouse_y ), 
		lDistance, mv, p, viewport,
		&outP[ 0 ], &outP[ 1 ], &outP[ 2 ] );

	return true;
}


math::vector mgluUnProjectWinP(int inputX, int inputY, int z)
{
	glFlush();

	double mv[16], p[16];
	GLint viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	glGetDoublev(GL_PROJECTION_MATRIX, p);
	glGetIntegerv(GL_VIEWPORT,viewport);

	math::vector out_p;
	gluUnProject(inputX, viewport[3]-inputY, z
				, mv, p, viewport
				, &out_p[0], &out_p[1], &out_p[2]);


	return out_p;
}

math::vector mgluUnProjectWinP(int inputX, int inputY, int z, Camera const& camera)
{
	glFlush();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	camera.glProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.glTranslform();

	double mv[16], p[16];
	GLint viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	glGetDoublev(GL_PROJECTION_MATRIX, p);
	glGetIntegerv(GL_VIEWPORT,viewport);

	math::vector out_p;
	gluUnProject(inputX, viewport[3]-inputY, z
				, mv, p, viewport
				, &out_p[0], &out_p[1], &out_p[2]);


	return out_p;
}


};
