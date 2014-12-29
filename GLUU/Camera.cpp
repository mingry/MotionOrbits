


#include "GLUU/Camera.h"
#include <GL/glut.h>

namespace mg
{

// camera가 초기 상태일때 z축의 양의 방향으로 보고 있게 하기 위해서 사용된다.
static math::quater y180(cos(M_PI/2), 0, sin(M_PI/2), 0);

Camera::Camera()
{
	cameraZoom.setValue(1, 1, 1);
	cameraP.setValue(0, 0, 0);
	math::quater q(1, 0, 0, 0);
	setRotation(q);
	
	
	fov = 45;
	aspect = 1;
	_near = 0.1f;
	_far = 10000.0f;

	trackBallR = 0.8f;

	flag_ortho = false;
	setOrthVolume(math::vector(200, 200, 200));

	pivot = math::vector(0, 0, 0);
}



void Camera::setGLMatrix(double m[16])
{
}

void Camera::getGLMatrix(double m[16])
{

}




void Camera::setTranslation(const math::vector &v)
{
	cameraP = v;
}




void Camera::setRotation(const math::quater &r)
{
	cameraQ = r;
//	cameraQ_y180 = math::quater(cos(M_PI/2), 0, sin(M_PI/2), 0) * r;
}

void Camera::setZoom(double z)
{
	cameraZoom.setValue(z, z, z);
}


void Camera::setZoom(math::vector z)
{
	cameraZoom = z;
}

void Camera::setZoom(double sx, double sy, double sz)
{
	setZoom(math::vector(sx, sy, sz));
}




void Camera::setFov(double f)
{
	fov = f;
}

double Camera::getFov() const
{
	if ( flag_ortho ) return 0;
	return fov;
}




void Camera::setAspectRatio(double r)
{
	aspect = r;
}

double Camera::getAspectRatio() const
{
	return aspect;
}

void Camera::setNearFar(double n, double f)
{
	_near = n;
	_far = f;
}

double Camera::getNear() const
{
	if ( flag_ortho ) return orth_viewport_volume_min.z();
	return _near;
}

double Camera::getFar() const
{
	if ( flag_ortho ) return orth_viewport_volume_max.z();
	return _far;
}



math::vector Camera::getTranslation() const
{
	return cameraP;
}

math::quater Camera::getRotation() const
{
	return cameraQ;
}

math::vector Camera::getZoom() const
{
	return cameraZoom;
}

math::vector Camera::getTranslationForGL() const
{
	return -1*cameraP;
}

math::quater Camera::getRotationForGL() const
{
	return y180 * cameraQ.inverse();
}


math::vector Camera::projectToTrackBall(double x, double y)
{
	double d, t, z;
	double r = trackBallR;

	d = (float) sqrt(x*x + y*y);
	if( d < r * 0.70710678118654752440f ) // Inside sphere 
	{		
		z = (float)sqrt( r*r-d*d );
	} 
	else // On hyperbola
	{
		t = r / 1.41421356237309504880f;
		z = t*t / d;
	}
	return math::vector(x, y, z);
}



void Camera::inputMouse(int button, double x1, double y1, double x2, double y2, double speedScale)
{
	double t_scale = 700;
	//x1 = x2 = 0.5;

	if ( speedScale > 0 )
	{
		t_scale = t_scale * speedScale;
	}

	if ( button == IN_TRANS )
	{
		math::vector t(0, 0, 0);
		t[0] = t_scale * (x2 - x1);
		t[1] = -1 * t_scale * (y2 - y1);

		cameraP += rotate(cameraQ, t);
	}
	else if ( button == IN_ZOOM )
	{
		double s;
		if ( speedScale > 0 ) 
			s = exp((y2 - y1)*speedScale);
		else
			s = exp(y2 - y1);

		cameraP[0] -= pivot[0]*cameraZoom[0] ;
		cameraP[1] -= pivot[1]*cameraZoom[1] ;
		cameraP[2] -= pivot[2]*cameraZoom[2] ;
		cameraZoom *= s;
		cameraP[0] += pivot[0]*cameraZoom[0] ;
		cameraP[1] += pivot[1]*cameraZoom[1] ;
		cameraP[2] += pivot[2]*cameraZoom[2] ;
	}
	else if ( button == IN_TRANS_Z )
	{
		math::vector t(0, 0, 0);
		t[2] = -1 * t_scale * (y2 - y1);

		cameraP += rotate(cameraQ, t);
	}
	else if ( button == IN_ROTATION )
	{
		math::vector p1 = projectToTrackBall( x1*2-1, y1*2-1 );
		math::vector p2 = projectToTrackBall( x2*2-1, y2*2-1 );

		/*
		double tmpF = p1[0];
		p1[0] = p2[0];
		p2[0] = tmpF;

		p1[0] *= -1;
		p2[0] *= -1;
		*/
		
		// Figure out how much to rotate around that axis
		math::vector d = p2 - p1;
		double dLen = len(d) / (2.0f * trackBallR);

		// Avoid problems with out-of-control values
		if ( dLen > 1.0f ) 
		{
			dLen = 1.0f;
		}
		else if ( dLen < -1.0f )
		{
			dLen = -1.0f;
		}

		double phi = 2.0f * (float)asin(dLen);

		math::vector cross = p1 * p2;
		cross[1] *= -1;
		math::quater q = exp( normalize(cross) * phi / 2 );


		math::quater tmpQ = cameraQ*q*cameraQ.inverse();
		tmpQ = tmpQ.normalize();
		math::vector s_pivot = pivot;

		s_pivot[0] *= cameraZoom[0];
		s_pivot[1] *= cameraZoom[1];
		s_pivot[2] *= cameraZoom[2];
		cameraP = rotate(tmpQ, cameraP-s_pivot)+s_pivot;
		cameraQ = cameraQ*q;


		/*math::quater tmpQ = cameraQ*q*cameraQ.inverse();
		tmpQ = tmpQ.normalize();
		cameraP = rotate(tmpQ, cameraP);
		cameraQ = cameraQ*q;*/
	
	}
	else if ( button == IN_ROTATION_Y_UP )
	{
		double dy = y2-y1;
		double dx = x2-x1;

		math::quater rot_x = exp(-1*dy*math::x_axis);
		math::quater rot_y = exp(-1*dx*math::y_axis);
		math::quater q = rot_y * rot_x;

		math::quater tmpQ = rot_y*cameraQ*rot_x*cameraQ.inverse();
		tmpQ = tmpQ.normalize();
		math::vector s_pivot = pivot;
		s_pivot[0] *= cameraZoom[0];
		s_pivot[1] *= cameraZoom[1];
		s_pivot[2] *= cameraZoom[2];
		cameraP = rotate(tmpQ, cameraP-s_pivot)+s_pivot;
		cameraQ = rot_y*cameraQ*rot_x;


		/*math::quater tmpQ = cameraQ*q*cameraQ.inverse();
		tmpQ = tmpQ.normalize();
		cameraP = rotate(tmpQ, cameraP);
		cameraQ = cameraQ*q;*/
	
	}
	else if ( button == IN_FOV )
	{
		setFov(getFov() + (y2 - y1)*10);
	}

	
}

void Camera::inputMouse(int button, double x1, double y1, double x2, double y2, math::vector center, double speedScale)
{
	double t_scale = 700;
	//x1 = x2 = 0.5;

	if ( speedScale > 0 )
	{
		t_scale = t_scale * speedScale;
	}

	if ( button == IN_TRANS )
	{
		math::vector t(0, 0, 0);
		t[0] = t_scale * (x2 - x1);
		t[1] = -1 * t_scale * (y2 - y1);

		cameraP += rotate(cameraQ, t);
	}
	else if ( button == IN_ZOOM )
	{
		double s;
		if ( speedScale > 0 ) 
			s = exp((y2 - y1)*speedScale);
		else
			s = exp(y2 - y1);

		cameraP[0] -= center[0]*cameraZoom[0] ;
		cameraP[1] -= center[1]*cameraZoom[1] ;
		cameraP[2] -= center[2]*cameraZoom[2] ;
		cameraZoom *= s;
		cameraP[0] += center[0]*cameraZoom[0] ;
		cameraP[1] += center[1]*cameraZoom[1] ;
		cameraP[2] += center[2]*cameraZoom[2] ;
	}
	else if ( button == IN_TRANS_Z )
	{
		math::vector t(0, 0, 0);
		t[2] = -1 * t_scale * (y2 - y1);

		cameraP += rotate(cameraQ, t);
	}
	else if ( button == IN_ROTATION )
	{
		math::vector p1 = projectToTrackBall( x1*2-1, y1*2-1 );
		math::vector p2 = projectToTrackBall( x2*2-1, y2*2-1 );

		/*
		double tmpF = p1[0];
		p1[0] = p2[0];
		p2[0] = tmpF;

		p1[0] *= -1;
		p2[0] *= -1;
		*/
		
		// Figure out how much to rotate around that axis
		math::vector d = p2 - p1;
		double dLen = len(d) / (2.0f * trackBallR);

		// Avoid problems with out-of-control values
		if ( dLen > 1.0f ) 
		{
			dLen = 1.0f;
		}
		else if ( dLen < -1.0f )
		{
			dLen = -1.0f;
		}

		double phi = 2.0f * (float)asin(dLen);

		math::vector cross = p1 * p2;
		cross[1] *= -1;
		math::quater q = exp( normalize(cross) * phi / 2 );


		math::quater tmpQ = cameraQ*q*cameraQ.inverse();
		tmpQ = tmpQ.normalize();
		math::vector s_center = center;
		s_center[0] *= cameraZoom[0];
		s_center[1] *= cameraZoom[1];
		s_center[2] *= cameraZoom[2];
		cameraP = rotate(tmpQ, cameraP-s_center)+s_center;
		cameraQ = cameraQ*q;
		//cameraP = len(cameraP) * rotate(cameraQ.inverse(), z_axis);
	
	}
	else if ( button == IN_FOV )
	{
		setFov(getFov() + (y2 - y1)*10);
	}

	
}



void Camera::inputMouse(int button, int dx, int dy, double speedScale)
{
	if ( button == IN_TRANS )
	{
		math::vector t(0, 0, 0);
		t[0] = speedScale * (dx);
		t[1] = -1 * speedScale * (dy);

		cameraP += rotate(cameraQ, t);
	}
	else if ( button == IN_TRANS_Z )
	{
		double moveScale = 50;

		if ( speedScale > 0 )
		{
			moveScale *= speedScale;
		}

		math::vector t(0, 0, 0);
		t[2] = -1 * moveScale * dy;
		cameraP += rotate(cameraQ, t);
	}
	else if ( button == IN_ZOOM )
	{
		double s;
		if ( speedScale > 0 ) 
			s = exp(((float)dy/10)*speedScale);
		else
			s = exp((float)dy/10);

		cameraP[0] -= pivot[0]*cameraZoom[0] ;
		cameraP[1] -= pivot[1]*cameraZoom[1] ;
		cameraP[2] -= pivot[2]*cameraZoom[2] ;
		cameraZoom *= s;
		cameraP[0] += pivot[0]*cameraZoom[0] ;
		cameraP[1] += pivot[1]*cameraZoom[1] ;
		cameraP[2] += pivot[2]*cameraZoom[2] ;
	}
	else if ( button == IN_NONUNIFORM_ZOOM )
	{
		if ( speedScale > 0 ) 
		{
			cameraZoom[0] *= exp(((float)dx/10)*speedScale);
			cameraZoom[1] *= exp(((float)dy/10)*speedScale);
			cameraZoom[2] *= exp(((float)dx/10)*speedScale);
		}
		else
		{
			cameraZoom[0] *= exp((float)dx/10);
			cameraZoom[1] *= exp((float)dy/10);
			cameraZoom[2] *= exp((float)dx/10);
		}
	}
	else if ( button == IN_FOV )
	{
		setFov(getFov() + dy);
	}
}	


void Camera::inputMouse(int button, int dx, int dy, int dz, double speedScale)
{
	if ( button == IN_NONUNIFORM_ZOOM )
	{
		if ( speedScale > 0 ) 
		{
			cameraZoom[0] *= exp(((float)dx/10)*speedScale);
			cameraZoom[1] *= exp(((float)dy/10)*speedScale);
			cameraZoom[2] *= exp(((float)dz/10)*speedScale);
		}
		else
		{
			cameraZoom[0] *= exp((float)dx/10);
			cameraZoom[1] *= exp((float)dy/10);
			cameraZoom[2] *= exp((float)dz/10);
		}
	}
	else 
	{
		inputMouse(button, dx, dy, speedScale);
	}
}	






void Camera::lookAt(math::vector eye, math::vector center, math::vector up)
{
	cameraP = eye;

	math::vector F = center - eye;
	math::vector f = F / len(F);
	math::vector upu = up / len(up);
	math::vector s = f * upu;
	math::vector u  = s * f;

	/*
	matrix rm( s[0], s[1], s[2],
			u[0], u[1], u[2],
			-1*f[0], -1*f[1], -1*f[2]);
			*/
	
	math::matrix rm( s[0], u[0], -1*f[0],
			s[1], u[1], -1*f[1],
			s[2], u[2], -1*f[2]);

	math::quater rq = math::Matrix2Quater(rm);

	cameraQ = rq.inverse() * y180.inverse();
}

void Camera::glProjection() const
{
	if ( flag_ortho )
	{
		glOrtho((orth_viewport_volume_min.x()), (orth_viewport_volume_max.x()), 
			(orth_viewport_volume_min.y()), (orth_viewport_volume_max.y()), 
			(orth_viewport_volume_min.z()), (orth_viewport_volume_max.z()));
	}
	else
	{
		gluPerspective(getFov(), getAspectRatio(), _near, _far);
		
	}
}

void Camera::glTranslform() const
{
	// camera가 초기 상태일때 z축의 양의 방향으로 보고 있게 하기 위해 항성 y180을 먼저 적용한다.
	math::vector qv = ln( y180 * cameraQ.inverse() );
	math::vector axis = normalize(qv);
	glRotatef(len(qv) * 360 / M_PI, axis.x(), axis.y(), axis.z());

	math::vector t = -1 * cameraP;
	glTranslatef(t.x(), t.y(), t.z());

	glScalef(cameraZoom.x(), cameraZoom.y(), cameraZoom.z());
}


void Camera::glTranslformInverse()
{
	glScalef(1/cameraZoom.x(), 1/cameraZoom.y(), 1/cameraZoom.z());

	math::vector t = cameraP;
	glTranslatef(t.x(), t.y(), t.z());

	math::vector qv = ln( cameraQ * y180.inverse() );
	math::vector axis = normalize(qv);
	glRotatef(len(qv) * 360 / M_PI, axis.x(), axis.y(), axis.z());
	
}


}





