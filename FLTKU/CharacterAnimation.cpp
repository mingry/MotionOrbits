




#include "BaseLib/FLTKU/CharacterAnimation.h"
#include "BaseLib/FLTKU/AnimationViewer.h"
#include "BaseLib/Geometry/AABox.h"
#include "BaseLib/Geometry/GeometryGL.h"
#include "BaseLib/GLUU/gluu.h"
#include <algorithm>

namespace mg
{

//////////////////////////////////////////////////////////////////////////
// Class: CharacterAnimation

CharacterAnimation::CharacterAnimation()
{
	motion_ = 0;
	pm_gl_model_ = 0;
	type_ = "CharacterAnimation";
}

CharacterAnimation::CharacterAnimation(PmLinearMotion *m, std::string n)
{
	motion_ = 0;
	pm_gl_model_ = 0;

	motion(m);
	name(n);

	type_ = "CharacterAnimation";
}

CharacterAnimation::~CharacterAnimation()
{
	if ( pm_gl_model_ != 0 ) delete pm_gl_model_;
	pm_gl_model_ = 0;
}


void
CharacterAnimation::motion(PmLinearMotion *m)
{
	this->motion_ = m;

	if ( pm_gl_model_ != 0 )
	{
		delete pm_gl_model_;
		pm_gl_model_ = 0;
	}

	if ( m != 0 )
	{
		pm_gl_model_= new PmBoneGLList01;
		pm_gl_model_->body(m->getBody());
		pm_gl_model_->CreateDefaultBodyGLList();
	}

	NotifyAnimationUpdated();
}

transf
CharacterAnimation::global_transf() const
{
	if ( motion_==0 || motion_->getSize() == 0 ) return transf(math::quater(1, 0, 0, 0), math::vector(0, 0, 0));

	math::transf t = PlaneProject( motion_->getPosture(0).getTransf(PmHuman::PELVIS) );

	return t;
}

int
CharacterAnimation::CountFrames() const
{
	if ( motion_ == 0 ) return 0;
	return motion_->getSize();
}


void
CharacterAnimation::DrawPosturePolygon(int frame)
{
	int pose_id = frame;

	PmPosture const &cur_pose = motion_->getPosture(pose_id);
	DrawPosturePolygon(cur_pose);
}


void
CharacterAnimation::DrawPosturePolygon(PmPosture const &cur_pose)
{
	pm_gl_model_->Draw(cur_pose);
}

void
CharacterAnimation::DrawPostureLine(int frame)
{
	int pose_id = frame;

	PmPosture const &cur_pose = motion_->getPosture(pose_id);
	PmHuman *human = motion_->getBody();
	
	for ( int i = 1; i < PM_HUMAN_NUM_LINKS; i++ )
	{
		if ( !(human->getMask() & MaskBit(i)) ) continue;

		// link
		math::vector v = cur_pose.getGlobalTranslation( i );
		math::vector v2 = cur_pose.getGlobalTranslation( human->getParent(i) );
		//mg::mgluColorMaterial(1, 1, 1, 0.8);
		glBegin(GL_LINES);
		glVertex3f(v[0], v[1], v[2]);
		glVertex3f(v2[0], v2[1], v2[2]);
		glEnd();

	}
	
}

void
CharacterAnimation::Draw(int frame)
{
	InitStencilForShadow();

	if ( motion_ == 0 ) return;
	DrawPosturePolygon(frame);
	//drawPostureLine(frame);

	BeginShadow();
	DrawPosturePolygon(frame);
	EndShadow();
}

void
CharacterAnimation::Draw(int from, int to, int step)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LINE_BIT);
	glLineWidth(3);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int range = to-from +1;
	for ( int i=0; i<range; i+=5 )
	{
		if ( i+from < 0 ) continue;
		if ( i+from>= motion_->getSize() ) continue;
		//glColor4f((double)i/motions[mID]->getSize(), (double)i/motions[mID]->getSize(), 1-(double)i/motions[mID]->getSize(), 0.7);
		glColor4f(1, (double)i/range, 1-(double)i/range, 0.7);
		DrawPostureLine(i+from);
	}
	glDisable(GL_BLEND);
	glPopAttrib();
}













//////////////////////////////////////////////////////////////////////////
// Class: MuliCharacterAni

MultiCharacterAni::MultiCharacterAni()
{
	type_ = "MultiCharacterAni";
}

MultiCharacterAni::~MultiCharacterAni()
{
	RemoveAllMotion();
}

void
MultiCharacterAni::AddMotion(PmLinearMotion *m, std::string motion_name)
{
	motions_.push_back(m);
	pm_gl_models_.push_back( new mg::PmBoneGLList01 );
	pm_gl_models_.back()->body(motions_.back()->getBody());
	pm_gl_models_.back()->CreateBodyGLList();

	motion_names_.push_back(motion_name);
	NotifyAnimationUpdated();
}


void
MultiCharacterAni::RemoveAllMotion()
{
	for ( int i=0; i<(int)pm_gl_models_.size(); i++ )
	{
		if ( pm_gl_models_[i] != 0 )
			delete pm_gl_models_[i];
	}

	motions_.clear();
	pm_gl_models_.clear();
	NotifyAnimationUpdated();
}

transf
MultiCharacterAni::global_transf() const
{
	if ( (int)motions_.size() == 0 ) return transf(math::quater(1, 0, 0, 0), math::vector(0, 0, 0));
	if ( (int)motions_[0]->getSize() == 0 ) return transf(math::quater(1, 0, 0, 0), math::vector(0, 0, 0));

	math::transf t = PlaneProject( motions_[0]->getPosture(0).getTransf(PmHuman::PELVIS) );

	return t;
}

int
MultiCharacterAni::CountFrames() const
{
	int max_frame = 0;
	for ( int i=0; i<(int)motions_.size(); i++ )
	{
		if ( motions_[i]->getSize() > max_frame )
			max_frame = motions_[i]->getSize();
	}
	return max_frame;
}

void
MultiCharacterAni::DrawPosturePolygon(int motion_id, int frame)
{

	int pose_id = frame;

	if ( motion_id < 0 ) return;
	if ( motion_id >= (int)motions_.size() ) return;
	if ( pose_id < 0 ) return;
	if ( pose_id >= motions_[motion_id]->getSize() ) return;

	pm_gl_models_[motion_id]->Draw( motions_[motion_id]->getPosture(pose_id) );
}

void
MultiCharacterAni::DrawPostureLine(int motion_id, int frame)
{
	int pose_id = frame;

	PmPosture const &cur_pose = motions_[motion_id]->getPosture(pose_id);
	PmHuman *human = motions_[motion_id]->getBody();
	
	for ( int i = 1; i < PM_HUMAN_NUM_LINKS; i++ )
	{
		if ( !(human->getMask() & MaskBit(i)) ) continue;

		// link
		math::vector v = cur_pose.getGlobalTranslation( i );
		math::vector v2 = cur_pose.getGlobalTranslation( human->getParent(i) );
		//mg::mgluColorMaterial(1, 1, 1, 0.8);
		glBegin(GL_LINES);
		glVertex3f(v[0], v[1], v[2]);
		glVertex3f(v2[0], v2[1], v2[2]);
		glEnd();

	}
	
}

void
MultiCharacterAni::Draw(int frame)
{
	for ( int m=0; m<(int)motions_.size(); m++ )
	{
		DrawPosturePolygon(m, frame);

		//beginShadow();
		//drawPosturePolygon(m, frame);
		//endShadow();
	}
}

void
MultiCharacterAni::Draw(int from, int to, int step)
{
	for ( int m=0; m<(int)motions_.size(); m++ )
	{
		glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LINE_BIT);
		glLineWidth(3);
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for ( int i=0; i<motions_[m]->getSize(); i+=5 )
		{
			if ( i < 0 ) continue;
			//glColor4f((double)i/motions[mID]->getSize(), (double)i/motions[mID]->getSize(), 1-(double)i/motions[mID]->getSize(), 0.7);
			glColor4f(1, (double)i/motions_[m]->getSize(), 1-(double)i/motions_[m]->getSize(), 0.7);
			DrawPostureLine(m, i);
		}
		glDisable(GL_BLEND);
		glPopAttrib();
	}
}



};