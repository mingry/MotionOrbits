

#include "Graph1DWin.h"
#include "GL/glut.h"
#include "FL/Fl.H"
#include "GLUU/gluu.h"

namespace mg
{

Graph1DWin::Graph1DWin(int x, int y, int w, int h, const char *s) : Fl_Gl_Window(x, y, w, h, s)
{
	vScale = 1;
	curFrame = 0;
	initCamera();
}


Graph1DWin::Graph1DWin(int w, int h, const char *s) : Fl_Gl_Window(w, h, s)
{
	vScale = 1;
	curFrame = 0;
	initCamera();
}


void Graph1DWin::initCamera()
{
	camera.setFov(45.0f);
	math::quater q =math::quater(cos(M_PI/2), 0, sin(M_PI/2), 0);
	camera.setRotation(q);
	math::vector v1(0, 0, 0);
	camera.setTranslation(v1);

	math::vector v2(w()/2, h()/2, 0);
	camera.setTranslation(v2);
}


void Graph1DWin::initGL()
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

void Graph1DWin::resize(int x, int y, int w, int h)
{
	Fl_Gl_Window::resize(x, y, w, h);

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	camera.setAspectRatio((double)w/(double)h);
	//gluPerspective(camera.getFov(), camera.getAspectRatio(), 0.1f, 10000.0f);
	glOrtho(-w/2, w/2, -h/2, h/2, -10, 10);
}




void Graph1DWin::draw()
{
	if ( !valid() && false )
	{
		initGL();
	}

	initGL();

	// pre-set
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	camera.glTranslform();

	glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);


	// vertical lines
	{
		double maxY = h() / 2 + camera.getTranslation().y();
		double minY = -1 * h() / 2 + camera.getTranslation().y();
		maxY /= camera.getZoom().y();
		minY /= camera.getZoom().y();

		glBegin(GL_LINES);
		
		// start frame
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex2f(0, minY);
		glVertex2f(0, maxY);

		// end frame
		if ( (int)data.size()>0 )
		{
			double maxX = (double)data.size();
			glColor3f(0.8f, 0.8f, 0.8f);
			glVertex2f(maxX, minY);
			glVertex2f(maxX, maxY);
		}

		//current frame
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex2f(curFrame, minY);
		glVertex2f(curFrame, maxY);

		// marks
		for ( int i=0; i<(int)marks.size(); i++ )
		{
			glColor3f(0.5f, 0.5f, 1.0f);
			glVertex2f(marks[i], minY);
			glVertex2f(marks[i], maxY);
		}

		glEnd();
	}

	

	
	
	drawGraph();
	glPopAttrib();
	
}


void Graph1DWin::drawGraph()
{
	if ( (int)data.size() == 0 ) return;

	glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);
	mg::mgluColorMaterial(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	for ( int i=0; i<(int)data.size(); i++ )
	{
		glVertex2f(i, data[i]*vScale);
	}
	glEnd();
	glPopAttrib();
}


int Graph1DWin::handle(int event)
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
			camera.inputMouse(mg::Camera::IN_TRANS, oldX-x, y-oldY);

			redraw();
			return 1;
		}
		else if ( Fl::event_button3() )
		{
			
			math::vector t = camera.getTranslation();
			t[0] /= camera.getZoom()[0];
			t[1] /= camera.getZoom()[1];
			t[2] /= camera.getZoom()[2];
			camera.inputMouse(mg::Camera::IN_ZOOM, 0, x-oldX, 0.2f);
			
			t[0] *= camera.getZoom()[0];
			t[1] *= camera.getZoom()[1];
			t[2] *= camera.getZoom()[2];

			
			redraw();
			return 1;
		}
		
		
	}
	else if ( event == FL_MOUSEWHEEL )
	{
		vScale *= exp(-1*(double)Fl::event_dy()/3);
		redraw();
		return 1;
	}

// select a frame
	else if ( ( event == FL_PUSH || event == FL_DRAG ) && Fl::event_button() == 1 && !Fl::event_alt() )
	{
		{
			double xf = (double)Fl::event_x();
			xf -= (double)w()/2;
			xf += camera.getTranslation().x();
			xf /= camera.getZoom().x();
			
			printf("x=%d\n", (int)xf);
		}
		redraw();
		return 1;
	}

	
	else if ( event == FL_RELEASE && Fl::event_alt() )
	{
		if ( Fl::event_button1() || Fl::event_button3() )
		{
			return 1;
		}
	}

	return Fl_Gl_Window::handle(event);
}

void Graph1DWin::setData(std::vector<double> d)
{
	data = d;
}


void Graph1DWin::ChangeFrame(int frameIndex)
{
	curFrame = frameIndex;
	redraw();
}






void Graph1DWin::viewAll()
{
	if ( (int)data.size() == 0 ) return;

	

	double zoom = (w()* 0.8)/data.size();
	
	math::vector c_v = math::vector( zoom*data.size()/2.0, 0.8f*h()/2.0f, 0 );
	camera.setTranslation(c_v);
	camera.setZoom( zoom );


	double minV, maxV;

	minV = maxV = data[0];

	for ( int i=0; i<(int)data.size(); i++ )
	{
		if ( data[i] > maxV ) maxV = data[i];
		if ( data[i] < minV ) minV = data[i];
	}

	minV *= zoom;
	maxV *= zoom;

	if ( maxV - minV < 0.0001 )
	{
		vScale = 1;
	}
	else
	{
		vScale = 0.8f*h() / (maxV-minV);
	}


	redraw();
}


};













