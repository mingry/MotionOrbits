

#pragma once 

#include <FL/Fl_Gl_Window.H>
#include "GLUU/gluu.h"
#include "MATHCLASS/vector.h"
#include "MATHCLASS/quater.h"
#include <vector>

namespace mg
{

class GL3DViewer : public Fl_Gl_Window
{
public:
	GL3DViewer(int x, int y, int w, int h, const char *s=0);
	GL3DViewer(int w, int h, const char *s=0);
	
	virtual mg::Camera* camera() { return &camera_; }

	void EnableSoftwareAntiAli(bool f) { flag_software_anti_ali_ = f; }
	virtual void ContentsUpdated() { redraw(); }
	
	// from Fl_Gl_Window
	virtual int handle(int event);
	virtual void resize(int x, int y, int w, int h);


	virtual void BeginShadow(double r=0.0, double g=0.0, double b=0.0, double alpha=0.5);
	virtual void EndShadow();

	
	std::pair<int, int> WinCoordToViewportCoord(int win_x, int win_y);
	std::pair<int, int> ViewportCoordToWinCoord(int vp_x, int vp_y);

	/// A Warping function of glUnProject
	/// z is a distance from the viewer.
	/// The result value is always true.
	bool UnProjectWinP(int win_x, int win_y, double z, math::vector &out_p);

	/// Similar to unProjectWinP(int win_x, int win_y, double z, math::vector &out_p).
	/// but z value is decided automatically.
	/// It is the distance from the viewer to the rendered thing at the point (win_x, win_y).
	/// If there is not at there, it will result false, otherwise return true.
	bool UnProjectWinP(int win_x, int win_y, math::vector &out_p);
	
	/// Similar to unProjectWinP(int win_x, int win_y, double z, math::vector &out_p).
	/// z value will be same to the NearPlane of view voluem.
	bool UnProjectNearWinP(int win_x, int win_y, math::vector &out_p);
	

	virtual int Pick(int m_x, int m_y);
	virtual int picking_name_buffer_size() const { return picking_name_buffer_size_; }
	virtual const GLuint* picking_name_buffer() const { return picking_name_buffer_; }
	virtual GLuint closest_picked_name() const { return closest_picked_name_; }

	virtual void flag_lighting(bool f) { flag_lighting_ = f; }
	virtual bool flag_lighting() const { return flag_lighting_; }

protected:
	// from Fl_Gl_Window
	virtual void draw();

	virtual void InitGL();
	virtual void Draw();
	virtual void DrawForPicking(int m_x, int m_y);
	virtual void DrawGround();
	virtual void DrawHeader();
	virtual void DrawLight();
	virtual void DrawContents();
	virtual void DrawContentsForPicking()
	{
		////// Example
		/*glInitNames();

		glPushName(1);
		drawBody();
		glPopName();

		glPushName(2);
		drawHead();
		drawEyes();
		glPopName();*/
	};

	virtual void BeginPicking();
	virtual int EndPicking();
	

protected:
	mg::Camera camera_;
	bool flag_software_anti_ali_;
	bool flag_picking_phase_;
	bool flag_shadow_phase_;
	bool flag_draw_header_;
	bool flag_lighting_;

	static const int picking_name_buffer_size_ = 200;
	GLuint picking_name_buffer_[picking_name_buffer_size_];
	int recent_hits_count_;
	GLuint closest_picked_name_;
};
		 
		
};
