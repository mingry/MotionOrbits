

#pragma once 

#include "FL/Fl_Gl_Window.H"
#include "GLUU/gluu.h"
#include <vector>

namespace mg
{

class GL2DViewer : public Fl_Gl_Window
{
public:
	GL2DViewer(int x, int y, int w, int h, const char *s=0);
	GL2DViewer(int w, int h, const char *s=0);


	virtual void InitGL();

	virtual void resize(int x, int y, int w, int h);
	virtual int handle(int event);


	void EnableSoftwareAntiAli(bool f=true) { flag_software_anti_ali_ = f; }
	

protected:
	// from Fl_Gl_Window
	virtual void draw();

	virtual void Draw();
	virtual void DrawHeader();
	

protected:
	mg::Camera camera_;
	bool flag_software_anti_ali_;
	
};
		 
		 
};
