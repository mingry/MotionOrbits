




#include "FLTKU/Animation.h"
#include "GLUU/gluu.h"
#include <algorithm>
#include "FLTKU/AnimationViewer.h"
#include "GL/gl.h"

namespace mg
{
		
void 
Animation::InitStencilForShadow()
{
	glClearStencil(0x1);
	glClear(GL_STENCIL_BUFFER_BIT);
}

void 
Animation::BeginShadow(double r, double g, double b, double alpha)
{
	flag_shadow_phase_ = true;

	mg::mgluPushAttrib();
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_STENCIL_TEST);
	//glClearStencil(0x1);
	//glClear(GL_STENCIL_BUFFER_BIT);

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
Animation::EndShadow()
{
	flag_shadow_phase_ = false;

	glPopMatrix();
	mg::mgluPopAttrib();
}


void
Animation::FrameChanged(int f)
{
	SeekFrame(f);
}

void 
Animation::SeekFrame(int f) 
{ 
	if ( f<0 ) cur_frame_=0; 
	else if ( f>=CountFrames() ) cur_frame_ = CountFrames()-1; 
	else cur_frame_ = f;	

	NotifyAnimationUpdated();
}

void
Animation::NotifyAnimationUpdated()
{
	for ( int i=0; i<(int)ani_viewers_.size(); i++ )
	{
		ani_viewers_[i]->ContentsUpdated();
	}

	for ( int i=0; i<(int)ani_listeners_.size(); i++ )
	{
		ani_listeners_[i]->AnimationChanged(this);
	}
}

void
Animation::AddAniViewer(AnimationViewer *v)
{
	if ( std::find(ani_viewers_.begin(), ani_viewers_.end(), v) == ani_viewers_.end() )
	{
		ani_viewers_.push_back(v);
	}
}

void
Animation::RemoveAniViewer(AnimationViewer *v)
{
	std::vector<AnimationViewer*>::iterator iter = std::find(ani_viewers_.begin(), ani_viewers_.end(), v);

	if ( iter != ani_viewers_.end() )
	{
		ani_viewers_.erase(iter);
	}
}

void
Animation::Draw(int from, int to, int step)
{
	for ( int i=from; i<=to; i+=step )
	{
		Draw(i);
	}
}


};







