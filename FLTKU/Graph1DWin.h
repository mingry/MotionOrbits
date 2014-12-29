

#pragma once

#include "Fl/Fl_Gl_Window.H"
#include "GLUU/Camera.h"
#include <vector>
#include "FLTKU/AnimationFrameListener.h"

namespace mg
{

class Graph1DWin : public Fl_Gl_Window, public AnimationFrameListener
{
public:
	Graph1DWin(int x, int y, int w, int h, const char *s=0);
	Graph1DWin(int w, int h, const char *s=0);

	
	void initGL();
	virtual void resize(int x, int y, int w, int h);

	virtual void draw();
	virtual void drawGraph();

	virtual void setData(std::vector<double> d);
	virtual std::vector<double>& getData() { return data; }

	virtual int handle(int event);
	virtual void ChangeFrame(int frame_id);

	virtual void viewAll();

protected:
	void initCamera();

protected:

	mg::Camera camera;

	int curFrame;
	double vScale;

public:
	std::vector<double> data;
	std::vector<int> marks;

};

};




