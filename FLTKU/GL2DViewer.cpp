

#include "FLTKU/GL2DViewer.h"
#include "FL/Fl.H"
#include <fstream>


namespace mg
{


GL2DViewer::GL2DViewer(int x, int y, int w, int h, const char *s) : Fl_Gl_Window(x, y, w, h, s)
{
	camera_.enableOrtho(true);
	camera_.setFov(45.0f);
	camera_.setRotation(math::quater(cos(M_PI/2), 0, sin(M_PI/2), 0));
	camera_.setOrthVolume(math::vector(500, 500, 500));
}


GL2DViewer::GL2DViewer(int w, int h, const char *s) : Fl_Gl_Window(w, h, s)
{
	camera_.enableOrtho(true);
	camera_.setFov(45.0f);
	math::quater c_q=math::quater(cos(M_PI/2), 0, sin(M_PI/2), 0);
	camera_.setRotation(c_q);
	camera_.setOrthVolume(math::vector(w, h, 500));
}



void GL2DViewer::InitGL()
{
	mode(mode() | FL_DOUBLE );

	glClearColor(1., 1., 1., 0);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_NORMALIZE);

	resize(x(), y(), w(), h());
}


void GL2DViewer::resize(int x, int y, int w, int h)
{
	Fl_Gl_Window::resize(x, y, w, h);

	// camera_.setAspectRatio((double)w/(double)h);
	camera_.setOrthVolume(math::vector(w, h, 500));
}

void GL2DViewer::draw()
{
	Draw();
}

void GL2DViewer::DrawHeader()
{
	glViewport(0, 0, (GLsizei)w(), (GLsizei)h());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-w()/2, w()/2, -h()/2, h()/2, -500, 500);
	camera_.glProjection();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera_.glTranslform();


	if ( flag_software_anti_ali_ )
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	}
	
}



void GL2DViewer::Draw()
{
	if ( !valid() )
		InitGL();
	
	DrawHeader();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT | GL_LINE_BIT | GL_POINT_BIT );

	glPopAttrib();
	glPopMatrix();
}








int GL2DViewer::handle(int event)
{
	static int oldX, oldY;
	static int x, y;

	if ( event == FL_PUSH || event == FL_DRAG || event == FL_RELEASE || event == FL_MOVE || event == FL_MOUSEWHEEL )
	{
		oldX = x;
		oldY = y;

		x = Fl::event_x();
		y = Fl::event_y();
	}


	// manipulate camera

	if ( Fl::event_alt() && !Fl::event_ctrl() )
	{
		if ( event == FL_PUSH && Fl::event_alt() )
		{
			if ( Fl::event_button1() || Fl::event_button3() )
			{
				return 1;
			}
		}
		else if ( event == FL_DRAG && Fl::event_alt() )
		{
			if ( Fl::event_button1() )
			{
				make_current();
				camera_.inputMouse(mg::Camera::IN_TRANS, oldX-x, y-oldY);

				redraw();
				return 1;
			}
			else if ( Fl::event_button3() )
			{
				math::vector t = camera_.getTranslation();
				t[0] /= camera_.getZoom()[0];
				t[1] /= camera_.getZoom()[1];
				t[2] /= camera_.getZoom()[2];
				//camera.inputMouse(mg::Camera::IN_NONUNIFORM_ZOOM, x-oldX, -1*(y-oldY), 0.2f);

				double dx = x - oldX;
				double dy = -1*(y-oldY);
				double ds = dx;
				if ( abs(dx) < abs(dy) ) ds = dy;
				camera_.inputMouse(mg::Camera::IN_ZOOM, 0, ds, 0.2f);
				
				t[0] *= camera_.getZoom()[0];
				t[1] *= camera_.getZoom()[1];
				t[2] *= camera_.getZoom()[2];
				camera_.setTranslation(t);
				
				redraw();
				return 1;
			}
			
			
		}
		else if ( event == FL_MOUSEWHEEL )
		{
			camera_.inputMouse(mg::Camera::IN_ZOOM, 0, Fl::event_dy(), 0.2f);

			redraw();
			return 1;
		}
	}


	return Fl_Gl_Window::handle(event);
}


};





