
#pragma once

#include "MATHCLASS/mathclass.h"


namespace mg
{

class Camera
{
public:

	enum { IN_ROTATION, IN_ROTATION_Y_UP, IN_ZOOM, IN_TRANS, IN_TRANS_Z, IN_FOV, IN_NONUNIFORM_ZOOM };

	Camera();

	void setGLMatrix(double m[16]);
	void getGLMatrix(double m[16]);

	void setTranslation(const math::vector &v);
	void setRotation(const math::quater &q);
	void setZoom(double z);
	void setZoom(math::vector z);
	void setZoom(double sx, double sy, double sz);

	void lookAt(math::vector eye, math::vector centor, math::vector up);
	void glTranslform() const;
	void glTranslformInverse();
	void glProjection() const;

	void setFov(double f);
	double getFov() const;

	void setAspectRatio(double r);
	double getAspectRatio() const;

	void setOrthVolume(math::vector volume) { orth_viewport_volume_min=-0.5*volume; orth_viewport_volume_max=0.5*volume; }
	void setOrthVolume(math::vector volume_min, math::vector volume_max) { orth_viewport_volume_min=volume_min; orth_viewport_volume_max=volume_max; }
	math::vector getOrthVolumeSize() const { return orth_viewport_volume_max-orth_viewport_volume_min; }
	math::vector getOrthVolumeMin() const { return orth_viewport_volume_min; }
	math::vector getOrthVolumeMax() const { return orth_viewport_volume_max; }
	void setNearFar(double n, double f);
	double getNear() const;
	double getFar() const;

	math::vector getTranslation() const;
	math::quater getRotation() const;
	math::vector	 getZoom() const;

	math::vector getTranslationForGL() const;
	math::quater getRotationForGL() const;

	// x1, y1 => previouse normalized mouse point, (0~1, 0~1).
	// x2, y2 => current normalized mouse point, (0~1, 0~1).
	void inputMouse(int button, double x1, double y1, double x2, double y2, double speedScale = -1.0f);
	void inputMouse(int button, double x1, double y1, double x2, double y2, math::vector center, double speedScale = -1.0f);
	void setPivot(math::vector p) { pivot = p; }


	// dx와 dy는 moust point 의 변화량. (y는 화면에서 높으수록 증가)
	void inputMouse(int button, int dx, int dy, double speedScale = -1.0f);

	void inputMouse(int button, int dx, int dy, int dz, double speedScale = -1.0f);

	void enableOrtho(bool f) { flag_ortho = f; }
	bool isOrtho() const { return flag_ortho; }


protected:
	math::vector projectToTrackBall(double x, double y);

protected:
	math::vector cameraP;

	// camera 의 실제 물리적 orientation
	math::quater cameraQ;			
	
	//double cameraZoom;
	math::vector cameraZoom;
	
	double fov;
	double aspect;
	bool flag_ortho;

	double _near, _far;

	double trackBallR;
	math::vector orth_viewport_volume_min; 
	math::vector orth_viewport_volume_max; 

	math::vector pivot;
};



};



