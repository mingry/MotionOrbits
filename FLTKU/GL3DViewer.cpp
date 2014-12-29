

#include "FLTKU/GL3DViewer.h"
#include <FL/Fl.H>
#include "GLUU/gluu.h"
#include <fstream>
#ifndef UINT_MAX
#define UINT_MAX (unsigned int)-1
#endif


namespace mg
{

GL3DViewer::GL3DViewer(int x, int y, int w, int h, const char *s) : Fl_Gl_Window(x, y, w, h, s)
{
	camera_.setFov(45.0f);
	camera_.setRotation(math::quater(cos(M_PI/2), 0, sin(M_PI/2), 0));
	camera_.setTranslation(math::vector(0, 110, 590));

	flag_software_anti_ali_ = false;
	flag_picking_phase_ = false;
	flag_shadow_phase_ = false;
	flag_draw_header_ = true;
	flag_lighting_ = true;

	recent_hits_count_ = 0;
}


GL3DViewer::GL3DViewer(int w, int h, const char *s) : Fl_Gl_Window(w, h, s)
{
	camera_.setFov(45.0f);
	camera_.setRotation(math::quater(cos(M_PI/2), 0, sin(M_PI/2), 0));
	camera_.setTranslation(math::vector(0, 110, 590));

	flag_software_anti_ali_ = false;
	flag_picking_phase_ = false;
	flag_shadow_phase_ = false;
	flag_draw_header_ = true;
	flag_lighting_ = true;

	recent_hits_count_ = 0;

}



void GL3DViewer::InitGL()
{
	mode(mode() | FL_DOUBLE );

	//glClearColor(0.8f, 0.8f, 0.8f, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 0);
	glClearColor(1, 1, 1, 0);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_NORMALIZE);
	
	resize(x(), y(), w(), h());
}


void GL3DViewer::resize(int x, int y, int w, int h)
{
	Fl_Gl_Window::resize(x, y, w, h);

	if ( camera_.isOrtho() )
	{
		camera_.setOrthVolume(math::vector(w, h, 10000));
	}
	else
	{
		camera_.setAspectRatio((double)w/(double)h);
	}
}


void GL3DViewer::DrawHeader()
{
	glViewport(0, 0, (GLsizei)w(), (GLsizei)h());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(camera.getFov(), camera.getAspectRatio(), camera.getNear(), camera.getFar());
	camera_.glProjection();




	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera_.glTranslform();

	if ( flag_lighting_ )
	{
		glEnable(GL_LIGHTING);
		DrawLight();
	}
	else
	{
		glDisable(GL_LIGHTING);
	}


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



void GL3DViewer::DrawLight()
{
	glEnable(GL_LIGHT0);
	
	GLfloat light_position[] = { 0, 0, 1, 0 };
	GLfloat light_direction[] = { 0.0, -1., -1. };
	GLfloat light_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_diffuse1[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	/*
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);
	//glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 128);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0f);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	//glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
	//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0);
	//glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);
	*/
}



void GL3DViewer::draw()
{
	Draw();
}


void GL3DViewer::Draw()
{
	if ( !valid() )
	{
		InitGL();
	}
	
	if ( flag_draw_header_ )
		DrawHeader();

	

	DrawGround();

	DrawContents();
}



void GL3DViewer::DrawContents()
{
}


void GL3DViewer::DrawGround()
{
	double i;
	int lineNum = 51;
	double blockWidth = 20.0f;
	double min = -1 * (50/2) * blockWidth;
	double max = (50/2) * blockWidth;
	

	glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT | GL_ENABLE_BIT);

	glDisable(GL_LIGHTING);
	

	glBegin(GL_LINES);

	for(i=0;i<lineNum;i+=1) 
	{

		if ( i==25 )
			mg::mgluColorMaterial(0.2f, 0.2f, 0.2f);
		else
			mg::mgluColorMaterial(0.9f, 0.9f, 0.9f);

		glVertex3f(min+i*blockWidth, 0, min);
		glVertex3f(min+i*blockWidth, 0, max);
		glVertex3f(min, 0, min+i*blockWidth);
		glVertex3f(max, 0, min+i*blockWidth);
	}

	glEnd();

	glPopAttrib();
}






int GL3DViewer::handle(int event)
{
	////////////////////////////////////////////////////
	// camera handling
	static double oldX, oldY;
	static double x, y;
	static bool flag_button1 = false;
	static bool flag_button2 = false;
	static bool flag_button12 = false;
	int winW = w();
	int winH = h();


	if ( event == FL_PUSH )
	{
		math::vector pivot;
		pivot = math::vector(0, 0, 0);
		camera_.setPivot(pivot);

		x = (double)Fl::event_x() / winW;
		y = 1 - (double)Fl::event_y() / winH;

		if ( Fl::event_button() == FL_LEFT_MOUSE )
		{
			if ( flag_button2 == true )
			{
				flag_button1 = false;
				flag_button2 = false;
				flag_button12 = true;
			}
			else
				flag_button1 = true;

			return 1;

		}
		else if ( Fl::event_button() == FL_MIDDLE_MOUSE )
		{
			if ( flag_button1 == true )
			{
				flag_button1 = false;
				flag_button2 = false;
				flag_button12 = true;
			}
			else
				flag_button2 = true;

			return 1;

		}
	}
	else if ( event == FL_DRAG )
	{
		oldX = x;
		oldY = y;

		x = (double)Fl::event_x() / winW;
		y = 1 - (double)Fl::event_y() / winH;

		if ( flag_button1 )
		{
			camera_.inputMouse(mg::Camera::IN_ROTATION_Y_UP, oldX, oldY, x, y);

			redraw();
			return 1;
		}
		else if ( flag_button2 )
		{
			camera_.inputMouse(mg::Camera::IN_TRANS, oldX, oldY, x, y);

			redraw();
			return 1;
		}
		else if ( flag_button12 )
		{
			camera_.inputMouse(mg::Camera::IN_ZOOM, oldX, oldY, x, y);

			redraw();
			return 1;
		}
	}
	else if ( event == FL_RELEASE )
	{
		if ( Fl::event_button() == FL_LEFT_MOUSE )
		{
			flag_button1 = false;
			flag_button12 = false;
			return 1;
		}
		else if ( Fl::event_button() == FL_MIDDLE_MOUSE )
		{
			flag_button2 = false;
			flag_button12 = false;
			return 1;
		}

	}
	else if ( event == FL_MOUSEWHEEL )
	{
		math::vector pivot;
		if ( !UnProjectWinP(Fl::event_x(), Fl::event_y(), pivot) )
		{
			pivot = math::vector(0, 0, 0);
		}

		camera_.setPivot(pivot);

		if ( Fl::event_ctrl() )
			camera_.inputMouse(mg::Camera::IN_FOV, 0, Fl::event_dy());
		if ( Fl::event_alt() )
			camera_.inputMouse(mg::Camera::IN_TRANS_Z, 0, Fl::event_dy());
		else
			camera_.inputMouse(mg::Camera::IN_ZOOM, 0, -1*Fl::event_dy());
		redraw();
		return 1;
	}


	return Fl_Gl_Window::handle(event);
}




void 
GL3DViewer::BeginShadow(double r, double g, double b, double alpha)
{
	flag_shadow_phase_ = true;

	mg::mgluPushAttrib();
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_STENCIL_TEST);
	glClearStencil(0x1);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP,GL_KEEP, GL_ZERO);
	//glStencilMask(0x1);		// only deal with the 1st bit
	
	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	// a matrix that squishes things onto the floor
	//double sm[16] = {1,0,0,0, 0,0,0,0.0, 0,0,1,0, 0,0.0,0,1};
	double light1_x = 10.0;
	double light1_y = -10.0;
	double light1_z = 20.0;
	double sm[16] = {1,0,0,0, -(light1_x/light1_z) ,0,-(light1_y/light1_z),0, 0,0,1,0, 0,0,0,1};
	glMultMatrixd(sm);
	// draw in transparent black (to dim the floor)
	glColor4f(r,g,b,alpha);
}

void 
GL3DViewer::EndShadow()
{
	flag_shadow_phase_ = false;

	glPopMatrix();
	mg::mgluPopAttrib();
}


void GL3DViewer::DrawForPicking(int m_x, int m_y)
{
	if ( !valid() )
	{
		InitGL();
	}
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	gluPickMatrix(m_x,viewport[3]-m_y, 5,5,viewport);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera_.glProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera_.glTranslform();

	DrawContentsForPicking();
}

int GL3DViewer::Pick(int m_x, int m_y)
{
	glFlush();
	BeginPicking();
	DrawForPicking(m_x, m_y);
	recent_hits_count_ = EndPicking();

	if ( recent_hits_count_ > 0 )
	{
		GLuint closest_picked_z = 0;
		closest_picked_name_ = 0;
		int ii=0;
		for ( int i=0; i<(int)recent_hits_count_; i++ )
		{
			GLuint depth_of_the_name_stack = picking_name_buffer_[ii];
			ii++; if ( ii >= picking_name_buffer_size_ ) break;

			GLuint z1 = picking_name_buffer_[ii];	// closest z from the view plane.
			ii++; if ( ii >= picking_name_buffer_size_ ) break;
			
			GLuint z2 = picking_name_buffer_[ii];	// farest z from the view plane.
			ii++; if ( ii >= picking_name_buffer_size_ ) break;
			
			std::vector<GLuint> names_in_stack;
			for ( GLuint j=0; j<depth_of_the_name_stack; j++ )
			{
				names_in_stack.push_back(picking_name_buffer_[ii]);
				ii++; if ( ii >= picking_name_buffer_size_ ) break;
			}

			if ( names_in_stack.size() > 0 )
			{
				if ( closest_picked_name_==0 || z1 < closest_picked_z )
				{
					closest_picked_z = z1;
					closest_picked_name_ = names_in_stack.front();
				}
			}

			if ( ii >= picking_name_buffer_size_ ) break;
		}
	}

	return recent_hits_count_;
}

void 
GL3DViewer::BeginPicking() 
{
	flag_picking_phase_ = true;
	
	
	glSelectBuffer(picking_name_buffer_size_, picking_name_buffer_);
	glRenderMode(GL_SELECT);
	glInitNames();

	
}


int 
GL3DViewer::EndPicking() 
{
	glFlush();

	if ( flag_picking_phase_ == false ) return false;
	int hits;

	// return to normal rendering mode
	hits = glRenderMode(GL_RENDER);
	flag_picking_phase_ = false;

	return hits;
}



std::pair<int, int>
GL3DViewer::WinCoordToViewportCoord(int win_x, int win_y)
{

	int win_w = w();
	int win_h = h();
	
	double x = (double)win_x;
	double y = (double)(win_h-win_y);

	std::pair<int, int> out;
	out.first = (int) x;
	out.second = (int) y;

	return out;
}

std::pair<int, int>
GL3DViewer::ViewportCoordToWinCoord(int vp_x, int vp_y)
{
	int win_w = w();
	int win_h = h();
	
	double x = (double)vp_x;
	double y = (double)(win_h-vp_y);

	std::pair<int, int> out;
	out.first = (int) x;
	out.second = (int) y;

	return out;
}

bool 
GL3DViewer::UnProjectWinP(int win_x, int win_y, double z, math::vector &out_p)
{
	glFlush();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	camera_.glProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera_.glTranslform();

	double mv[16], p[16];
	GLint viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	glGetDoublev(GL_PROJECTION_MATRIX, p);
	glGetIntegerv(GL_VIEWPORT,viewport);

	std::pair<int, int> vp_p = WinCoordToViewportCoord(win_x, win_y);
	gluUnProject(vp_p.first, vp_p.second, z
				, mv, p, viewport
				, &out_p[0], &out_p[1], &out_p[2]);


	return true;
}


bool 
GL3DViewer::UnProjectWinP(int win_x, int win_y, math::vector &out_p)
{
	glFlush();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	std::pair<int, int> vp_p = WinCoordToViewportCoord(win_x, win_y);

	// Get the Z coordinate for the pixel location
	GLuint lDepth;
	glReadPixels( vp_p.first, vp_p.second, 
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

	gluUnProject( vp_p.first, vp_p.second, 
		lDistance, mv, p, viewport,
		&out_p[ 0 ], &out_p[ 1 ], &out_p[ 2 ] );

	return true;
}


bool 
GL3DViewer::UnProjectNearWinP(int win_x, int win_y, math::vector &out_p)
{
	glFlush();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	camera_.glProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera_.glTranslform();

	double mv[16], p[16];
	GLint viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	glGetDoublev(GL_PROJECTION_MATRIX, p);
	glGetIntegerv(GL_VIEWPORT,viewport);

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

	double lDistance = camera_.getNear() * lDistTermInv;
	
	std::pair<int, int> vp_p = WinCoordToViewportCoord(win_x, win_y);
	gluUnProject(vp_p.first, vp_p.second, lDistance
				, mv, p, viewport
				, &out_p[0], &out_p[1], &out_p[2]);


	return true;
}


};
